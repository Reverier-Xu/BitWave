/**
 * @file lyrics.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "lyrics.h"

#include <QRegularExpression>

#include "interface.h"
#include "local_lyrics.h"
#include "ncm_lyrics.h"
#include "player/player.h"

Lyrics* Lyrics::m_instance = nullptr;

Lyrics::Lyrics(QObject* parent) : QObject(parent) {
    initLyrics();
    m_lyricsModel = new LyricsModel(this);

    connect(Player::instance(parent), &Player::currentTimeChanged, this,
            &Lyrics::updateCurrentIndex);
    connect(Player::instance(parent), &Player::mediaChanged, this,
            &Lyrics::requestFetch);
}

Lyrics::~Lyrics() { destroyLyrics(); }

Lyrics* Lyrics::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Lyrics(parent);
        locker.unlock();
    }
    return m_instance;
}

bool Lyrics::loading() const { return m_loading; }

void Lyrics::setLoading(bool loading) {
    if (m_loading == loading) {
        return;
    }
    m_loading = loading;
    emit loadingChanged(m_loading);
}

int Lyrics::currentIndex() const { return m_currentIndex; }

LyricsModel* Lyrics::lyricsModel() { return m_lyricsModel; }

void splitLyrics(QList<Lyric>& lyrics, const QString& raw) {
    QStringList rawList = raw.split("\n");
    QRegularExpression rx(R"(\[\d+:\d+\.\d+\])");
    for (const auto & line : rawList) {
        auto match = rx.match(line);
        if (match.hasMatch()) {
            QString content = line;
            content.remove(rx);
            for (auto& j : match.capturedTexts()) {
                QString captured = j.trimmed();
                captured = captured.mid(1, captured.length() - 2);
                QStringList capturedList = captured.split(":");
                if (capturedList.size() == 2) {
                    Lyric lyric;
                    lyric.content = content;
                    lyric.time = capturedList.at(0).toDouble() * 60 +
                                 capturedList.at(1).toDouble();
                    lyrics << lyric;

                    // NOTE: do not parse lyrics that have per-character timelines.
                    // when we got the first one, break the loop.
                    break;
                }
            }
        } else {
            Lyric lyric;
            lyric.content = line;
            lyric.time = 0;
            lyrics << lyric;
        }
    }
}

void insertLyricsTranslation(QList<Lyric>& lyrics, const QString& tr) {
    QStringList trList = tr.split("\n");
    QRegularExpression rx(R"(\[\d+:\d+\.\d+\])");
    for (const auto & line : trList) {
        // qDebug() << "Insert translation: " << line;
        auto match = rx.match(line);
        if (match.hasMatch()) {
            QString content = line;
            content.remove(rx);
            for (const auto& j : match.capturedTexts()) {
                QString captured = j.trimmed();
                captured = captured.mid(1, captured.length() - 2);
                QStringList capturedList = captured.split(":");
                if (capturedList.size() == 2) {
                    double time = capturedList.at(0).toDouble() * 60 +
                                  capturedList.at(1).toDouble();
                    int left = 0;
                    int right = lyrics.size() - 1;
                    while (left <= right) {
                        int mid = (left + right) / 2;
                        if (lyrics.at(mid).time > time)
                            right = mid - 1;
                        else if (lyrics.at(mid).time < time)
                            left = mid + 1;
                        else
                            break;
                    }
                    // qDebug() << "Inserted \"" << lyrics.at((left + right) / 2).content << "\" translation \"" << content << "\" at " << (left + right) / 2 << ", lyric time "
                    //          << lyrics.at((left + right) / 2).time << ", tr time " << time;
                    if (lyrics.at((left + right) / 2).time == time) {
                        lyrics[(left + right) / 2].translation = content;
                    }
                }
            }
        }
    }
}

void Lyrics::handleLyrics(const QString& lyrics, const QString& translation) {
    QList<Lyric> result;
    splitLyrics(result, lyrics);

    if (!translation.isEmpty()) insertLyricsTranslation(result, translation);
    std::sort(result.begin(), result.end(),
              [](const Lyric& a, const Lyric& b) { return a.time < b.time; });
    m_lyricsModel->setLyrics(result);
}

void Lyrics::updateCurrentIndex(double time) {
    auto lyricsPtr = m_lyricsModel->lyrics();
    if (lyricsPtr->isEmpty()) {
        return;
    }
    int left = 0;
    int right = lyricsPtr->size() - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        if (lyricsPtr->at(mid).time > time)
            right = mid - 1;
        else if (lyricsPtr->at(mid).time < time)
            left = mid + 1;
        else
            break;
    }
    if (lyricsPtr->at(left).time > time) {
        m_currentIndex = left - 1;
    } else {
        m_currentIndex = left;
    }
    emit currentIndexChanged(m_currentIndex);
}

void Lyrics::initLyrics() {
    // prefer local lrc file first
    m_lyrics.append(new LocalLyrics(this));
    m_lyrics.append(new NcmLyrics(this));
}

void Lyrics::destroyLyrics() {
    for (auto lyric : m_lyrics) {
        lyric->deleteLater();
    }
}

void Lyrics::requestFetch(const Media& media) {
    for (auto lyric : m_lyrics) {
        if (lyric->accepted(media)) {
            setLoading(true);
            auto worker = lyric->clone();
            worker->setTaskId(++m_taskId);
            worker->requestFetch(media);
            connect(worker, &ILyrics::lyricsFetched, this,
                    [=](const QString& lyrics, const QString& translation) {
                        if (worker->taskId() == m_taskId) {
                            handleLyrics(lyrics, translation);
                            setLoading(false);
                        }
                        worker->deleteLater();
                    });
            return;
        }
    }
    if (media.embeddedLyrics().isEmpty())
        handleLyrics(tr("[00:00.00]No lyrics provider"), "");
    else
        handleLyrics(media.embeddedLyrics(), "");
}

LyricsModel::LyricsModel(QObject* parent) : QAbstractListModel(parent) {}

LyricsModel::~LyricsModel() = default;

int LyricsModel::rowCount(const QModelIndex& parent) const {
    return m_lyrics.size();
}

QVariant LyricsModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= m_lyrics.size()) {
        return QVariant();
    }
    if (role == LyricsIdRole) {
        return index.row();
    } else if (role == LyricsContentRole) {
        return m_lyrics[index.row()].content;
    } else if (role == LyricsTranslationRole) {
        return m_lyrics[index.row()].translation;
    } else if (role == LyricsTimeRole) {
        return m_lyrics[index.row()].time;
    }
    return QVariant();
}

QHash<int, QByteArray> LyricsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LyricsIdRole] = "lyricsId";
    roles[LyricsContentRole] = "lyricsContent";
    roles[LyricsTranslationRole] = "lyricsTranslation";
    roles[LyricsTimeRole] = "lyricsTime";
    return roles;
}

void LyricsModel::setLyrics(const QList<Lyric>& lyrics) {
    beginResetModel();
    m_lyrics = lyrics;
    endResetModel();
}

void LyricsModel::clearLyrics() {
    beginResetModel();
    m_lyrics.clear();
    endResetModel();
}

QList<Lyric>* LyricsModel::lyrics() { return &m_lyrics; }

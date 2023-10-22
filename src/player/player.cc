/**
 * @file player.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include "player.h"

#include <QMutex>
#include <QtConcurrent>

#include "codec/codec.h"
#include "parser/parser.h"


Player* Player::m_instance = nullptr;

Player::Player(QObject* parent) : QObject(parent) {
    m_engine = new Engine(this);
    m_queue = new PlayQueue(this);
    chooseRandomCover();

    m_media = Media::null();
    m_media.setTitle(tr("No Media"));
    m_media.setAlbum(tr("Unknown Album"));
    m_media.setArtists({tr("Unknown Artist")});

    m_screensaver = Screensaver::getScreensaver();

    connectSignals();
    loadSettings();
}

Player::~Player() { saveSettings(); }

double Player::totalTime() const { return m_totalTime; }

void Player::setTotalTime(double n) {
    m_totalTime = n;
    emit totalTimeChanged(n);
}

double Player::currentTime() const { return m_currentTime; }

void Player::setCurrentTime(double n) {
    m_currentTime = n;
    emit currentTimeChanged(n);
}

Player* Player::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Player(parent);
        locker.unlock();
    }
    return m_instance;
}

bool Player::playing() const { return m_playing; }

void Player::setPlaying(bool n) {
    m_playing = n;
    emit playingChanged(n);
}

double Player::volume() const { return m_volume; }

void Player::setVolume(double n) {
    m_volume = n;
    //    qDebug() << "volume changed: " << n;
    emit volumeChanged(n);
}

bool Player::muted() const { return m_muted; }

void Player::setMuted(bool n) {
    m_muted = n;
    emit mutedChanged(n);
}

void Player::resume() {
    if (valid()) {
        if (ended()) m_engine->play(media().url());
        m_engine->resume();
    }
}

void Player::pause() {
    if (valid() && !ended()) m_engine->pause();
}

void Player::seek(double n) {
    if (valid()) {
        if (ended()) m_engine->play(media().url());
        else {
            m_engine->seek((double) (n));
            emit userSeeked(n);
        }
    }
}

void Player::toggleMute() {
    m_engine->setMute(!muted());
    setMuted(!muted());
}

void Player::togglePause() {
    if (playing())
        pause();
    else
        resume();
}

void Player::connectSignals() {
    connect(m_engine, &Engine::currentTimeChanged, this,
            [=](double secs) { setCurrentTime(secs); });
    connect(m_engine, &Engine::totalTimeChanged, this, [=](double secs) {
        setTotalTime(secs);
        //        qDebug() << "total time changed: " << secs;
    });
    connect(m_engine, &Engine::volumeChanged, this,
            [=](double vol) { setVolume(vol); });
    connect(m_engine, &Engine::started, this, [=]() {
        resume();
        setPlaying(true);
        setEnded(false);
        emit stateChanged();
        //        qDebug() << "started";
    });
    connect(m_engine, &Engine::ended, this, [=]() {
        setPlaying(false);
        setEnded(true);
        m_queue->next();
        emit stateChanged();
        //        qDebug() << "ended";
    });
    connect(m_engine, &Engine::paused, this, [=]() {
        setPlaying(false);
        emit stateChanged();
        //        qDebug() << "paused";
    });
    connect(m_engine, &Engine::resumed, this, [=]() {
        setPlaying(true);
        emit stateChanged();
        //        qDebug() << "resumed";
    });
    connect(m_engine, &Engine::audioDeviceChanged, this,
            [=](const QString& n) { emit audioDeviceChanged(n); });

    connect(m_queue, &PlayQueue::mediaChanged, this,
            [=](const Media& media) { play(media); });
    if (m_screensaver != nullptr) {
        connect(m_engine, &Engine::started, [=]() {
            // qDebug() << "Started";
            if (media().type() == VIDEO) {
                // qDebug() << "Inhibit";
                m_screensaver->inhibit();
            }
        });
        connect(m_engine, &Engine::ended, [=]() {
            m_screensaver->unInhibit();
            // qDebug() << "UnInhibit";
        });
        connect(this, &Player::playingChanged, [=](bool n) {
            if (media().type() == VIDEO && n) {
                // qDebug() << "Inhibit";
                m_screensaver->inhibit();
            } else {
                // qDebug() << "UnInhibit";
                m_screensaver->unInhibit();
            }
        });
    }
}

void Player::toggleVolume(double n) { m_engine->setVolume(n); }

Engine* Player::engine() const { return m_engine; }

void Player::play(const Media& media) {
    auto taskId = ++m_taskId;
    if (valid()) {
        reset();
    }
    if (media.type() == UNKNOWN) {
        return;
    }
    setMedia(media);

    setCoverLoading(true);
    setLoading(true);
    setCoverPath("");
    auto* coverWatcher = new QFutureWatcher<QImage>(this);
    connect(coverWatcher, &QFutureWatcher<QImage>::finished, this, [=]() {
        if (taskId != m_taskId) {
            return;
        }
        try {
            auto cover = coverWatcher->result();
            if (cover.isNull()) throw std::runtime_error("Cover is null.");
            setCover(cover);
        } catch (...) {
            chooseRandomCover();
        }
        setCoverLoading(false);
        coverWatcher->deleteLater();
    });
    coverWatcher->setFuture(QtConcurrent::run(&Parser::extractCover, media));

    auto decodeWatcher = new QFutureWatcher<QString>(this);
    connect(decodeWatcher, &QFutureWatcher<QString>::finished, this, [=]() {
        if (taskId != m_taskId) {
            return;
        }
        setLoading(false);
        try {
            m_engine->play(decodeWatcher->result());
            setValid(true);
        } catch (...) {
            setValid(false);
            // TODO: warn user here.
        }
        decodeWatcher->deleteLater();
    });
    decodeWatcher->setFuture(QtConcurrent::run(&Codec::decode, media));
}

Media Player::media() const { return m_media; }

void Player::setMedia(const Media& n) {
    m_media = n;
    if (n.type() == UNKNOWN) {
        setValid(false);
        m_media.setTitle(tr("No Media"));
        m_media.setAlbum(tr("Unknown Album"));
        m_media.setArtists({tr("Unknown Artist")});
    } else
        setValid(true);
    emit mediaChanged(n);
}

bool Player::valid() const { return m_valid; }

void Player::setValid(bool n) {
    m_valid = n;
    emit validChanged(n);
}

bool Player::ended() const { return m_ended; }

void Player::setEnded(bool n) {
    m_ended = n;
    emit endedChanged(n);
}

void Player::reset() {
    m_engine->stop();
    setPlaying(false);
    setEnded(false);
    setValid(false);
    setMedia(Media());
    setCurrentTime(0);
    setTotalTime(0);
}

QImage Player::cover() const { return m_cover; }

void Player::setCover(const QImage& n) {
    m_cover = n;
    auto tempCoverPath =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
            "/BitWave/Covers/" + m_media.title().replace("/", "-") + ".jpg";
    QDir().mkpath(
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
            "/BitWave/Covers/");
    n.save(tempCoverPath);
    setCoverPath(QUrl::fromLocalFile(tempCoverPath).toString());

    emit coverChanged(n);
}

bool Player::loading() const { return m_loading; }

void Player::setLoading(bool n) {
    m_loading = n;
    emit loadingChanged(n);
}

bool Player::coverLoading() const { return m_coverLoading; }

void Player::setCoverLoading(bool n) {
    m_coverLoading = n;
    emit coverLoadingChanged(n);
}

QString Player::coverPath() const { return m_coverPath; }

void Player::setCoverPath(const QString& n) {
    m_coverPath = n;
    emit coverPathChanged(n);
}

void Player::playUrl(const QString& url) {
    try {
        auto media = Parser::parse(url);
        play(media);
    } catch (...) {
        // ...
    }
}

PlayQueue* Player::queue() const { return m_queue; }

QList<QVariantMap> Player::audioDeviceList() const {
    auto resp = m_engine->getAudioDeviceList();
    QList<QVariantMap> ret;
    for (const auto& item : resp) {
        QVariantMap map;
        map.insert("name", item["name"]);
        map.insert("description", item["description"]);
        ret.append(map);
    }
    return ret;
}

QString Player::audioDevice() const {
    auto resp = m_engine->getMpvProperty("audio-device");
    return resp.value<QString>();
}

void Player::setAudioDevice(const QString& n) {
    m_engine->setMpvProperty("audio-device", n);
}

void Player::loadSettings() {
    QSettings settings;
    settings.beginGroup("Player");
    setVolume(settings.value("Volume", 1.0).toDouble());
    setMuted(settings.value("Muted", false).toBool());
    settings.endGroup();
}

void Player::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Player");
    settings.setValue("Volume", volume());
    settings.setValue("Muted", muted());
    settings.endGroup();
}

void Player::chooseRandomCover() {
    auto pic = QRandomGenerator::global()->bounded(1, 5);
    setCoverPath("qrc:/assets/media-cover-" + QString::number(pic) + ".svg");
    m_cover.load(coverPath());
}

void Player::saveMediaCover(const QUrl& savedUrl) {
    auto path = savedUrl.toLocalFile();
    if (!valid()) return;
    try {
        auto cover = Parser::extractCover(media());
        cover.save(path);
    } catch (...) {
        // TODO: warn user
    }
}

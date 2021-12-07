/**
 * @file lyrics_list_model.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#include "lyrics_list_model.h"

#include <QDebug>
#include <QRegularExpression>
#include <algorithm>

int LyricsListModel::rowCount(const QModelIndex &parent) const {
    return mLyrics.size();
}

QVariant LyricsListModel::data(const QModelIndex &index, int role) const {
    if (index.isValid() && index.row() < mLyrics.size()) {
        switch (role) {
            case LyricContentRole:
                return mLyrics.at(index.row()).content();
            case LyricStartTimeRole:
                return mLyrics.at(index.row()).startTime();
            case LyricEndTimeRole:
                return mLyrics.at(index.row()).endTime();
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> LyricsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[LyricContentRole] = "lyricContent";
    roles[LyricStartTimeRole] = "lyricStartTime";
    roles[LyricEndTimeRole] = "lyricEndTime";
    return roles;
}

void LyricsListModel::addLyric(const QString &content, double start,
                               double end) {
    beginInsertRows(QModelIndex(), mLyrics.size(), mLyrics.size());
    mLyrics << LyricContent(this, content, start, end);
    endInsertRows();
}

void LyricsListModel::addLyric(const LyricContent &lyric) {
    beginInsertRows(QModelIndex(), mLyrics.size(), mLyrics.size());
    mLyrics << lyric;
    endInsertRows();
}

void LyricsListModel::insertLyric(const QString &content, double start,
                                  double end, int row) {
    beginInsertRows(QModelIndex(), row, row);
    mLyrics.insert(row, LyricContent(this, content, start, end));
    endInsertRows();
}

void LyricsListModel::insertLyric(const LyricContent &content, int row) {
    beginInsertRows(QModelIndex(), row, row);
    mLyrics.insert(row, content);
    endInsertRows();
}

void LyricsListModel::removeLyric(int row) {
    if (row < mLyrics.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        mLyrics.removeAt(row);
        endRemoveRows();
    }
}

typedef struct Lyric {
    QString content;
    double start{};
} Lyric;

void splitLyric(QList<Lyric> &lyrics, const QString &raw) {
    QStringList rawList = raw.split("\n");
    QRegExp rx(R"(\[\d+:\d+\.\d+\])");
    for (int i = 0; i < rawList.size(); i++) {
        const QString &line = rawList.at(i);
        if (rx.indexIn(line) != -1) {
            QString content = line;
            content.remove(rx);
            for (const auto &j : rx.capturedTexts()) {
                QString captured = j.trimmed();
                captured = captured.mid(1, captured.length() - 2);
                QStringList capturedList = captured.split(":");
                if (capturedList.size() == 2) {
                    Lyric lyric;
                    lyric.content = content;
                    lyric.start = capturedList.at(0).toDouble() * 60 +
                                  capturedList.at(1).toDouble();
                    lyrics << lyric;
                }
            }
        }
    }
}

void insertLyricTr(QList<Lyric> &lyrics, const QString &tr) {
    QStringList trList = tr.split("\n");
    QRegExp rx(R"(\[\d+:\d+\.\d+\])");
    for (int i = 0; i < trList.size(); i++) {
        const QString &line = trList.at(i);
        if (rx.indexIn(line) != -1) {
            QString content = line;
            content.remove(rx);
            for (const auto &j : rx.capturedTexts()) {
                QString captured = j.trimmed();
                captured = captured.mid(1, captured.length() - 2);
                QStringList capturedList = captured.split(":");
                if (capturedList.size() == 2) {
                    double start = capturedList.at(0).toDouble() * 60 +
                                   capturedList.at(1).toDouble();
                    // 二分法
                    int left = 0;
                    int right = lyrics.size() - 1;
                    while (left <= right) {
                        int mid = (left + right) / 2;
                        if (lyrics.at(mid).start > start)
                            right = mid - 1;
                        else if (lyrics.at(mid).start < start)
                            left = mid + 1;
                        else
                            break;
                    }
                    if (lyrics.at((left + right) / 2).start == start) {
                        lyrics[(left + right) / 2].content += "\n" + content;
                    }
                }
            }
        }
    }
}

void LyricsListModel::parseLyrics(const QString &raw, const QString &tr) {
    QList<Lyric> lyrics;
    splitLyric(lyrics, raw);
    if (!tr.isEmpty()) insertLyricTr(lyrics, tr);
    beginResetModel();
    mLyrics.clear();
    for (auto i = 0; i < lyrics.size(); i++) {
        QString content = lyrics.at(i).content;
        double start = lyrics.at(i).start;
        double end;
        if (i + 1 < lyrics.size())
            end = lyrics.at(i + 1).start;
        else
            end = 0x7fffffff;
        addLyric(content, start, end);
    }
    std::sort(mLyrics.begin(), mLyrics.end());
    endResetModel();
}

int LyricsListModel::getCurrentLyricIndex(double currentTime) const {
    // 二分法
    int left = 0, right = mLyrics.size() - 1;
    while (left < right) {
        int mid = (left + right) / 2;
        if (mLyrics[mid].endTime() < currentTime) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

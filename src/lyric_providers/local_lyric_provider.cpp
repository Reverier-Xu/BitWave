/**
 * @file local_lyric_provider.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief provide local lyric
 * @version 0.1
 * @date 2021-12-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "local_lyric_provider.h"

#include <QFile>
#include <QUrl>
#include <QDebug>

LocalLyricProvider::LocalLyricProvider(QObject *parent)
    : BaseLyricProvider(parent) {
}

LocalLyricProvider *LocalLyricProvider::clone() {
    return new LocalLyricProvider(this->parent());
}

bool LocalLyricProvider::accepted(const Media &media) {
    const auto fileUrl = QUrl(media.rawUrl());
    if (fileUrl.isLocalFile()) {
        auto file = fileUrl.toLocalFile();
        // replace media suffix with .lrc
        auto suffix = file.split(".").last();
        file.replace(QString("." + suffix), ".lrc");
        // qDebug() << "local lyric file: " << file;
        if (QFile::exists(file)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void LocalLyricProvider::getLyricsRequest(const Media &media) {
    const auto fileUrl = QUrl(media.rawUrl());
    if (fileUrl.isLocalFile()) {
        auto file = fileUrl.toLocalFile();
        auto suffix = file.split(".").last();
        file.replace(QString("." + suffix), ".lrc");
        if (QFile::exists(file)) {
            QFile f(file);
            if (f.open(QFile::ReadOnly)) {
                QString raw = f.readAll().trimmed();
                // qDebug() << "local lyric: " << raw;
                QString source;
                QString translation;
                QString head = "IMPOSSIBLE_TIME_HEAD";
                for (auto &i : raw.split('\n')) {
                    if (i.startsWith(head)) {
                        translation += i + '\n';
                    } else {
                        source += i + '\n';
                        head = i.left(i.indexOf(']') + 1);
                    }
                }
                // qDebug() << source;
                // qDebug() << translation;
                emit lyricsIsReady(true, source, translation);
                return;
            } else {
                emit lyricsIsReady(false, QString(), QString());
                return;
            }
        } else {
            emit lyricsIsReady(false, QString(), QString());
            return;
        }
    }
    emit lyricsIsReady(false, QString(), QString());
}

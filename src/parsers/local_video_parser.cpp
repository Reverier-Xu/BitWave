//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_video_parser.h"
#include <QFile>
#include <QFileInfo>
#include <QUrl>

const QStringList &LocalVideoParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types = QStringList() << "mp4"
                              << "avi"
                              << "mkv"
                              << "flv"
                              << "mov"
                              << "wmv"
                              << "mpg"
                              << "mpeg"
                              << "m4v"
                              << "3gp"
                              << "3g2"
                              << "mts"
                              << "m2ts"
                              << "ts"
                              << "m3u8"
                              << "m3u"
                              << "mpd"
                              << "rm"
                              << "rmvb"
                              << "asf"
                              << "wma"
                              << "wmv"
                              << "flv"
                              << "f4v"
                              << "f4p"
                              << "f4a"
                              << "f4b"
                              << "vob";
    }
    return types;
}

bool LocalVideoParser::accepted(const Media &media) {
    return this->accepted(media.rawUrl());
}

bool LocalVideoParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(), Qt::CaseInsensitive);
}

LocalVideoParser *LocalVideoParser::clone() {
    return new LocalVideoParser(*this);
}

Media LocalVideoParser::getMedia(const QString &path) {
    return Media();
}

Media LocalVideoParser::parseMedia(const Media &media) {
    return Media();
}

QString LocalVideoParser::getMediaCover(const Media &media) {
    return "";
}

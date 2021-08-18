//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_netease_music_parser.h"

#include <QFile>
#include <QFileInfo>
#include <QUrl>

const QStringList &LocalNeteaseMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types << "ncm";
    }
    return types;
}

bool LocalNeteaseMusicParser::accepted(const Media &media) {
    return this->accepted(media.rawUrl());
}

bool LocalNeteaseMusicParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(), Qt::CaseInsensitive);
}

LocalNeteaseMusicParser *LocalNeteaseMusicParser::clone() {
    return new LocalNeteaseMusicParser(*this);
}

Media LocalNeteaseMusicParser::getMedia(const QString &path) {
    return Media();
}

Media LocalNeteaseMusicParser::parseMedia(const Media &media) {
    return Media();
}

QString LocalNeteaseMusicParser::getMediaCover(const Media &media) {
    return "";
}


//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_netease_music_parser.h"

#include <QFile>
#include <QFileInfo>

const QStringList &LocalNeteaseMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types << "*.ncm";
    }
    return types;
}

bool LocalNeteaseMusicParser::accepted(const Media &media) {
    return this->accepted(media.rawUrl());
}

bool LocalNeteaseMusicParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(), Qt::CaseInsensitive);
}

const Media LocalNeteaseMusicParser::parse(const Media &media) {
    return Media();
}

LocalNeteaseMusicParser *LocalNeteaseMusicParser::clone() {
    return new LocalNeteaseMusicParser(*this);
}

bool LocalNeteaseMusicParser::fillMetaData(Media &media) {
    return false;
}

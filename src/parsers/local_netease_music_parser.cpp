//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_netease_music_parser.h"

// #include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "utilities/ncm_helper.h"

const QStringList &
LocalNeteaseMusicParser::acceptTypes() {
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

LocalNeteaseMusicParser *
LocalNeteaseMusicParser::clone() {
    return new LocalNeteaseMusicParser(*this);
}

Media LocalNeteaseMusicParser::getMedia(const QString &path) {
    return NcmHelper::getMediaFromPath(path);
}

Media LocalNeteaseMusicParser::parseMedia(const Media &media) {
    auto newPath = NcmHelper::dump(media);
    auto res = media;
    res.setRawUrl(newPath);
    return res;
}

QString
LocalNeteaseMusicParser::getMediaCover(const Media &media) {
    return NcmHelper::dumpMediaCover(media);
}

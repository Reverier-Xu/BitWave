/**
 * @file local_netease_music_parser.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "local_netease_music_parser.h"

// #include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "utilities/ncm_helper.h"

const QStringList &LocalNeteaseMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types << "ncm";
    }
    return types;
}

bool LocalNeteaseMusicParser::accepted(const Media &media) {
    return accepted(media.rawUrl());
}

bool LocalNeteaseMusicParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(),
                                  Qt::CaseInsensitive);
}

LocalNeteaseMusicParser *LocalNeteaseMusicParser::clone() {
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

QString LocalNeteaseMusicParser::getMediaCover(const Media &media) {
    return NcmHelper::dumpMediaCover(media);
}

LocalNeteaseMusicParser::LocalNeteaseMusicParser(QObject *parent)
    : BaseParser(parent) {}

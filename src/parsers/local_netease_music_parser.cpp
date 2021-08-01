//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_netease_music_parser.h"

#include <QFile>
#include <QFileInfo>

const QStringList& LocalNeteaseMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types << "*.ncm";
    }
    return types;
}

bool LocalNeteaseMusicParser::accepted(const Media& media) {
    return QFileInfo(media.rawUrl()).suffix().toLower() == "ncm";
}

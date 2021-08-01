//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_music_parser.h"
#include <QFile>
#include <QFileInfo>

const QStringList& LocalMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty())
        types = QStringList() << "*.mp3"
                              << "*.m4a"
                              << "*.m4b"
                              << "*.m4p"
                              << "*.m4r"
                              << "*.m4v"
                              << "*.mp4"
                              << "*.aac"
                              << "*.flac"
                              << "*.wma"
                              << "*.wv";
    return types;
}

bool LocalMusicParser::accepted(const Media& media) {
    QFileInfo info(media.rawUrl());
    return acceptTypes().contains(info.suffix(), Qt::CaseInsensitive);
}

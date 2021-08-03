//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_music_parser.h"
#include <QFile>
#include <QFileInfo>

const QStringList &LocalMusicParser::acceptTypes() {
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
                              << "*.ape"
                              << "*.flac"
                              << "*.wma"
                              << "*.wv"
                              << "*.wav"
                              << "*.ogg";
    return types;
}

bool LocalMusicParser::accepted(const Media &media) {
    return this->accepted(media.rawUrl());
}

bool LocalMusicParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(), Qt::CaseInsensitive);
}

bool LocalMusicParser::fillMetaData(Media &media) {
    return false;
}

LocalMusicParser *LocalMusicParser::clone() {
    return new LocalMusicParser(*this);
}

const Media LocalMusicParser::parse(const Media &media) {
    return Media();
}

/**
 * @file local_video_parser.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "local_video_parser.h"

#include <utilities/memory_helper.h>

// #include <QFile>
#include <QFileInfo>
#include <QUrl>

extern "C" {
#include <libavformat/avformat.h>
}

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
                              << "webm"
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
    return accepted(media.rawUrl());
}

bool LocalVideoParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(),
                                  Qt::CaseInsensitive);
}

LocalVideoParser *LocalVideoParser::clone() {
    return new LocalVideoParser(*this);
}

Media LocalVideoParser::getMedia(const QString &path) {
    // MemoryHelper::assertMemory("LocalVideoParser::getMedia Begin");
    Media media;
    media.setRawUrl(path);
    media.setType(VIDEO);
    AVFormatContext *ctx = nullptr;
    AVDictionaryEntry *tag = nullptr;
    AVDictionaryEntry *read_tag;
    auto raw_path = path.toStdString();

    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 1");
    avformat_open_input(&ctx, raw_path.c_str(), nullptr, nullptr);
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 2");
    avformat_find_stream_info(ctx, nullptr);
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 3");

    read_tag = av_dict_get(ctx->metadata, "title", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setTitle(read_tag->value);
    else
        media.setTitle(QFileInfo(path).baseName());
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 4");

    read_tag = av_dict_get(ctx->metadata, "artist", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setArtist(read_tag->value);
    else
        media.setArtist(tr("Unknown Artist"));
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 5");

    read_tag = av_dict_get(ctx->metadata, "album", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setCollection(read_tag->value);
    else
        media.setCollection("Unknown Album");
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 6");

    if (ctx->duration != AV_NOPTS_VALUE) {
        int64_t secs, us;
        int64_t duration = ctx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        media.setDuration(double(secs) + double(us) / AV_TIME_BASE);
        //        qDebug() << secs;
        //        qDebug() << us;
    }
    //    MemoryHelper::assertMemory("LocalVideoParser::getMedia Mid 7");
    //    qDebug() << media.duration();
    avformat_close_input(&ctx);
    // MemoryHelper::assertMemory("LocalVideoParser::getMedia End");
    return media;
}

Media LocalVideoParser::parseMedia(const Media &media) { return media; }

QString LocalVideoParser::getMediaCover(const Media &media) {
    return "qrc:/assets/movie-colorful.svg";
}

LocalVideoParser::LocalVideoParser(QObject *parent) : BaseParser(parent) {}

//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_music_parser.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QImage>
#include <QStandardPaths>

extern "C" {
#include <libavformat/avformat.h>
}

const QStringList &LocalMusicParser::acceptTypes() {
    static QStringList types;
    if (types.isEmpty())
        types = QStringList() << "mp3"
                              << "m4a"
                              << "m4b"
                              << "m4p"
                              << "m4r"
                              << "m4v"
                              << "mp4"
                              << "aac"
                              << "ape"
                              << "flac"
                              << "wma"
                              << "wv"
                              << "wav"
                              << "ogg";
    return types;
}

bool LocalMusicParser::accepted(const Media &media) {
    return this->accepted(media.rawUrl());
}

bool LocalMusicParser::accepted(const QString &path) {
    return acceptTypes().contains(QFileInfo(path).suffix(), Qt::CaseInsensitive);
}

LocalMusicParser *LocalMusicParser::clone() {
    return new LocalMusicParser(*this);
}

Media LocalMusicParser::getMedia(const QString &path) {
    auto media = Media();
    media.setType(AUDIO);
    media.setRawUrl(path);
    AVFormatContext *ctx = nullptr;
    AVDictionaryEntry *tag = nullptr;
    AVDictionaryEntry *read_tag;
    std::string raw_path = path.toStdString();
    avformat_open_input(&ctx, raw_path.c_str(), nullptr, nullptr);
    avformat_find_stream_info(ctx, nullptr);
    read_tag = av_dict_get(ctx->metadata, "title", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag) media.setTitle(read_tag->value);
    else media.setTitle(QFileInfo(path).baseName());
    read_tag = av_dict_get(ctx->metadata, "artist", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag) media.setArtist(read_tag->value);
    else media.setArtist(tr("Unknown Artist"));
    read_tag = av_dict_get(ctx->metadata, "album", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag) media.setCollection(read_tag->value);
    else media.setCollection("Unknown Album");
    if (ctx->duration != AV_NOPTS_VALUE) {
        int64_t secs, us;
        int64_t duration = ctx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        media.setDuration(double(secs) + double(us) / AV_TIME_BASE);
//        qDebug() << secs;
//        qDebug() << us;
    }
//    qDebug() << media.duration();
    avformat_close_input(&ctx);
    return media;
}

Media LocalMusicParser::parseMedia(const Media &media) {
    return media;
}

QString LocalMusicParser::getMediaCover(const Media &media) {
    AVFormatContext *ctx = nullptr;
    AVDictionaryEntry *tag = nullptr;
    avformat_open_input(&ctx, media.rawUrl().toStdString().c_str(), nullptr, nullptr);
    avformat_find_stream_info(ctx, nullptr);
    // read the format headers
    if (ctx->iformat->read_header(ctx) < 0) {
        avformat_close_input(&ctx);
        return "qrc:/assets/music-big.svg";
    }

    for (int i = 0; i < ctx->nb_streams; i++){
        if (ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            AVPacket pkt = ctx->streams[i]->attached_pic;
            QImage img = QImage::fromData((uchar*)pkt.data, pkt.size);
            auto temp_loc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/covers/" + media.title() + ".jpg";
            img.save(temp_loc);
            avformat_close_input(&ctx);
            // qDebug() << temp_loc;
            return "file://" + temp_loc;
        }
    }
    avformat_close_input(&ctx);
    return "qrc:/assets/music-big.svg";
}

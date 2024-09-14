/**
 * @file ffmpeg_parser.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-09
 *
 * @copyright 2023 Woo Tech
 */

#include "ffmpeg_parser.h"

#include <QImage>
#include <QObject>

extern "C" {
#include <libavformat/avformat.h>
}

Media FfmpegParser::parse(const QString& path) {
    auto media = Media();
    media.setUrl(QUrl(path).toString());
    AVFormatContext* ctx = nullptr;
    AVDictionaryEntry* tag = nullptr;
    AVDictionaryEntry* read_tag;
    std::string raw_path = path.toStdString();
    //    qDebug() << "Parsing media: " << raw_path.c_str();
    int ret = avformat_open_input(&ctx, raw_path.c_str(), nullptr, nullptr);
    if (ret < 0) throw std::runtime_error("Failed to parse media.");
    avformat_find_stream_info(ctx, nullptr);
    read_tag = av_dict_get(ctx->metadata, "title", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setTitle(read_tag->value);
    else
        media.setTitle(QFileInfo(path).completeBaseName());
    read_tag = av_dict_get(ctx->metadata, "artist", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setArtists(QString(read_tag->value).split("/"));
    else
        media.setArtists({QObject::tr("Unknown Artist")});
    read_tag = av_dict_get(ctx->metadata, "album", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setAlbum(read_tag->value);
    else
        media.setAlbum(QObject::tr("Unknown Album"));
    read_tag = av_dict_get(ctx->metadata, "comment", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setComment(read_tag->value);
    else
        media.setComment("");
    if ((read_tag = av_dict_get(ctx->metadata, "lyrics", tag, AV_DICT_IGNORE_SUFFIX)) or
        (read_tag = av_dict_get(ctx->metadata, "LYRICS", tag, AV_DICT_IGNORE_SUFFIX))) {
        media.setEmbeddedLyrics(read_tag->value);
    } else {
        media.setEmbeddedLyrics("");
    }
    if (ctx->duration != AV_NOPTS_VALUE) {
        int64_t secs, us;
        int64_t duration = ctx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        media.setTime(double(secs) + double(us) / AV_TIME_BASE);
    }

    // detect file type by iterate ffmpeg streams
    auto fileInfo = QFileInfo(path);
    auto suffix = fileInfo.suffix();
    if (m_supportedVideoFormats.contains(suffix))
        media.setType(VIDEO);
    else if (m_supportedAudioFormats.contains(suffix))
        media.setType(MUSIC);
    else
        media.setType(UNKNOWN);

    avformat_close_input(&ctx);
    return media;
}

QImage FfmpegParser::extractCover(const Media& src) {
    AVFormatContext* ctx = nullptr;
    int ret = avformat_open_input(&ctx, src.url().toStdString().c_str(), nullptr, nullptr);
    if (ret < 0) throw std::runtime_error("Failed to open media file.");
    if (avformat_find_stream_info(ctx, nullptr) < 0) {
        avformat_close_input(&ctx);
        throw std::runtime_error("Failed to find stream info.");
    }
    // read the format headers
    AVPacket pkt;
    av_read_frame(ctx, &pkt);
    if (!pkt.size) {
        throw std::runtime_error("Failed to read frame.");
    }
    auto img = QImage::fromData((uchar*)pkt.data, pkt.size);
    // find the first attached picture, if any
    avformat_close_input(&ctx);
    return img;
}

bool FfmpegParser::accepted(const QString& path) {
    auto fileInfo = QFileInfo(path);
    return m_supportedAudioFormats.contains(fileInfo.suffix().toLower()) ||
           m_supportedVideoFormats.contains(fileInfo.suffix().toLower());
}

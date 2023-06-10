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
    media.setUrl(path);
    AVFormatContext *ctx = nullptr;
    AVDictionaryEntry *tag = nullptr;
    AVDictionaryEntry *read_tag;
    std::string raw_path = path.toStdString();
    int ret = avformat_open_input(&ctx, raw_path.c_str(), nullptr, nullptr);
    if (ret < 0) throw std::exception();
    avformat_find_stream_info(ctx, nullptr);
    read_tag = av_dict_get(ctx->metadata, "title", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setTitle(read_tag->value);
    else
        media.setTitle(QFileInfo(path).baseName());
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
    read_tag =
        av_dict_get(ctx->metadata, "comment", tag, AV_DICT_IGNORE_SUFFIX);
    if (read_tag)
        media.setComment(read_tag->value);
    else
        media.setComment("");
    if (ctx->duration != AV_NOPTS_VALUE) {
        int64_t secs, us;
        int64_t duration = ctx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        media.setTime(double(secs) + double(us) / AV_TIME_BASE);
    }

    // detect file type by iterate ffmpeg streams
    auto type = UNKNOWN;
    for (int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (type == UNKNOWN) {
                type = AUDIO;
            }
        } else if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            type = VIDEO;
            break;
        }
    }
    media.setType(type);

    avformat_close_input(&ctx);
    return media;
}

QImage FfmpegParser::extractCover(const Media& src) {
    AVFormatContext *ctx = nullptr;
    int ret = avformat_open_input(&ctx, src.url().toStdString().c_str(),
                                  nullptr, nullptr);
    if (ret < 0) throw std::exception();
    avformat_find_stream_info(ctx, nullptr);
    // read the format headers
    if (ctx->iformat->read_header(ctx) < 0) {
        avformat_close_input(&ctx);
        throw std::runtime_error("No cover found.");
    }

    for (int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            AVPacket pkt = ctx->streams[i]->attached_pic;
            QImage img = QImage::fromData((uchar *)pkt.data, pkt.size);
            avformat_close_input(&ctx);
            return img;
        }
    }
    avformat_close_input(&ctx);
    throw std::runtime_error("No cover found.");
}

bool FfmpegParser::accepted(const QString& path) {
    auto fileInfo = QFileInfo(path);
    return m_supportedFormats.contains(fileInfo.suffix());
}

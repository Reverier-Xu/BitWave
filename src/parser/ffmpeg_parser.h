/**
 * @file ffmpeg_parser.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-09
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include "interface.h"

class FfmpegParser : public IParser {
   private:
    QStringList m_supportedAudioFormats = {
        "mp3", "flac", "wav",  "ape", "ogg", "aac", "m4a",
        "m4b", "wma",  "opus", "m4p", "m4r", "wv",
    };
    QStringList m_supportedVideoFormats = {
        "mp4", "avi", "mkv",  "flv", "mov",  "wmv", "mpg",  "mpeg",
        "m4v", "3gp", "3g2",  "mts", "m2ts", "ts",  "m3u8", "m3u",
        "mpd", "rm",  "rmvb", "asf", "wma",  "wmv", "flv",  "f4v",
        "f4p", "f4a", "f4b",  "vob", "webm",
    };

   public:
    bool accepted(const QString& path) override;
    Media parse(const QString& path) override;
    QImage extractCover(const Media& src) override;
};

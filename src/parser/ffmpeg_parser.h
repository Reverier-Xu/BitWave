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

class FfmpegParser: public IParser {
   private:
    QStringList m_supportedAudioFormats = {
        "mp3", "flac", "wav", "ape", "ogg", "aac", "m4a", "wma", "opus",
    };
    QStringList m_supportedVideoFormats = {
        "mp4", "mkv", "avi", "wmv", "flv", "mov", "rmvb", "rm", "webm", "mpg", "mpeg", "m4v",
    };
   public:
    bool accepted(const QString& path) override;
    Media parse(const QString& path) override;
    QImage extractCover(const Media& src) override;
};

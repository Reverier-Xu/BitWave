/**
 * @file mpv_codec.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include "interface.h"

class MpvCodec : public ICodec {
  private:
    QStringList m_supportedFormats = {
        // popular audio formats
        "mp3",
        "flac",
        "wav",
        "ape",
        "ogg",
        "aac",
        "m4a",
        "m4b",
        "wma",
        "opus",
        "m4p",
        "m4r",
        "wv",
        // popular video formats
        "mp4",
        "avi",
        "mkv",
        "flv",
        "mov",
        "wmv",
        "mpg",
        "mpeg",
        "m4v",
        "3gp",
        "3g2",
        "mts",
        "m2ts",
        "ts",
        "m3u8",
        "m3u",
        "mpd",
        "rm",
        "rmvb",
        "asf",
        "wma",
        "wmv",
        "flv",
        "f4v",
        "f4p",
        "f4a",
        "f4b",
        "vob",
        "webm",
    };

  public:
    MpvCodec() = default;
    ~MpvCodec() override = default;
    const QStringList& supportedFormats() override;
    bool accepted(const Media& src) override;
    QString decode(const Media& src) override;
};

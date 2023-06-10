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

class MpvCodec: public ICodec {
   private:
    QStringList m_supportedFormats = {
        // popular audio formats
        "mp3", "flac", "wav", "ape", "ogg", "aac", "m4a", "wma", "opus",
        // popular video formats
        "mp4", "mkv", "avi", "wmv", "flv", "mov", "rmvb", "rm", "webm", "mpg", "mpeg", "m4v",
    };
public:
    MpvCodec() = default;
    ~MpvCodec() override = default;
    const QStringList& supportedFormats() override;
    bool accepted(const Media& src) override;
    QString decode(const Media& src) override;
};

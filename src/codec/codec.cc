/**
 * @file codec.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "codec.h"
#include "mpv_codec.h"
#include "ncm_codec.h"


static QList<ICodec*> codecs;

QStringList Codec::supportedFormats() {
    QStringList formats;
    for (auto codec : codecs) {
        formats.append(codec->supportedFormats());
    }
    return formats;
}

void Codec::initCodecs() {
    codecs.append(new MpvCodec());
    codecs.append(new NcmCodec());
}

void Codec::destroyCodecs() {
    for (auto codec : codecs) {
        delete codec;
    }
}

QString Codec::decode(const Media& src) {
    for (auto codec : codecs) {
        if (codec->accepted(src)) {
            return codec->decode(src);
        }
    }
    throw std::runtime_error("No codec accepted the media.");
}

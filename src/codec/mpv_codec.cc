/**
 * @file mpv_codec.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "mpv_codec.h"

#include <QFileInfo>

QString MpvCodec::decode(const Media& src) { return src.url(); }

const QStringList& MpvCodec::supportedFormats() { return m_supportedFormats; }

bool MpvCodec::accepted(const Media& src) {
    auto fileInfo = QFileInfo(QUrl(src.url()).path());
    return m_supportedFormats.contains(fileInfo.suffix());
}

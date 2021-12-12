/**
 * @file ncm_helper.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QObject>
#include <QString>

#include "models/media.h"
#include "qaesencryption.h"

namespace NcmHelper {
QString dump(const Media &media);

Media getMediaFromPath(const QString &path);

void getMetadataFrom163Key(Media &dst);

QString dumpMediaCover(const Media &media);

quint64 getMusicId(const QString &comment163Key);

QJsonObject getMusicJsonInfo(const QString &comment163Key);
};  // namespace NcmHelper

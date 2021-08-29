//
// Created by Reverier-Xu on 2021/2/17.
//

#pragma once

#include <QObject>
#include <QString>

#include "models/media.h"
#include "qaesencryption.h"

namespace NcmHelper {
    QString
    dump(const Media &media);

    Media getMediaFromPath(const QString &path);

    void getMetadataFrom163Key(Media &dst);

    QString
    dumpMediaCover(const Media &media);

    quint64
    getMusicId(const QString &comment163Key);

    QJsonObject
    getMusicJsonInfo(const QString &comment163Key);
}; // namespace NcmHelper

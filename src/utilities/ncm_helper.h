//
// Created by Reverier-Xu on 2021/2/17.
//

#pragma once

#include "models/media.h"
#include "qaesencryption.h"
#include <QString>
#include <QObject>

namespace NcmHelper {
    QString dump(const Media &media);

    Media getMediaFromPath(const QString &path);

    void getMetadataFrom163Key(Media& dst);

    QString dumpMediaCover(const Media &media);
};


#pragma once

#include <QList>
#include <QString>
#include <QStringList>

#include "models/media.h"

class ICodec {
   public:
    virtual ~ICodec() = default;
    virtual const QStringList& supportedFormats() = 0;
    virtual bool accepted(const Media& src) = 0;
    virtual QString decode(const Media& src) = 0;
};

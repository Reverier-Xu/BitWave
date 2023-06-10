#pragma once

#include <QString>
#include <QList>
#include "models/media.h"
#include <QStringList>

class ICodec {
   public:
    virtual ~ICodec() = default;
    virtual const QStringList& supportedFormats() = 0;
    virtual bool accepted(const Media& src) = 0;
    virtual QString decode(const Media& src) = 0;
};

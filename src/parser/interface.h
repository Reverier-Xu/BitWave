#pragma once
#include <models/media.h>

class IParser {
   public:
    virtual ~IParser() = default;

    virtual bool accepted(const QString& path) = 0;

    virtual Media parse(const QString& path) = 0;

    virtual QImage extractCover(const Media& src) = 0;
};

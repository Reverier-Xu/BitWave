//
// Created by Reverier-Xu on 2021/7/29.
//

#ifndef BITWAVE_BASE_PARSER_H
#define BITWAVE_BASE_PARSER_H

#include <QObject>
#include "models/media.h"

class BaseParser : public QObject {
    Q_OBJECT
public:
    [[nodiscard]] virtual const QStringList& acceptTypes() = 0;

    [[nodiscard]] virtual bool accepted(const Media& media) = 0;

    [[nodiscard]] virtual const Media& parse(const Media& media) = 0;

    virtual bool fillMimeData(Media& media) = 0;

};


#endif //BITWAVE_BASE_PARSER_H

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
    explicit BaseParser(QObject *parent = nullptr) : QObject(parent) {}

    BaseParser(const BaseParser &parser) {
        this->setParent(parser.parent());
    }

    ~BaseParser() override = default;

    [[nodiscard]] virtual BaseParser *clone() = 0;

    [[nodiscard]] virtual const QStringList &acceptTypes() = 0;

    [[nodiscard]] virtual bool accepted(const Media &media) = 0;

    [[nodiscard]] virtual bool accepted(const QString &path) = 0;

    [[nodiscard]] virtual const Media parse(const Media &media) = 0;

    virtual bool fillMetaData(Media &media) = 0;

};


#endif //BITWAVE_BASE_PARSER_H

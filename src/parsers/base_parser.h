//
// Created by Reverier-Xu on 2021/7/29.
//

#pragma once

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

    [[nodiscard]] virtual Media getMedia(const QString& path) = 0;

    [[nodiscard]] virtual Media parseMedia(const Media& media) = 0;

    [[nodiscard]] virtual QString getMediaCover(const Media& media) = 0;

};

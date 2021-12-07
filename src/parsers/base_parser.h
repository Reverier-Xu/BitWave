/**
 * @file base_parser.h
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
#include <QUuid>

#include "models/media.h"

class BaseParser : public QObject {
    Q_OBJECT
   private:
    QUuid mParserId = QUuid(nullptr);

   public:
    explicit BaseParser(QObject *parent = nullptr) : QObject(parent) {}

    BaseParser(const BaseParser &parser) { this->setParent(parser.parent()); }

    ~BaseParser() override = default;

    [[nodiscard]] virtual BaseParser *clone() = 0;

    [[nodiscard]] virtual const QStringList &acceptTypes() = 0;

    [[nodiscard]] virtual bool accepted(const Media &media) = 0;

    [[nodiscard]] virtual bool accepted(const QString &path) = 0;

    [[nodiscard]] virtual Media getMedia(const QString &path) = 0;

    [[nodiscard]] virtual Media parseMedia(const Media &media) = 0;

    [[nodiscard]] virtual QString getMediaCover(const Media &media) = 0;

    [[nodiscard]] QUuid parserId() { return this->mParserId; }

    void setParserId(const QUuid &n) {
        this->mParserId = n;
        emit this->parserIdChanged(n);
    }

   signals:

    void parserIdChanged(const QUuid &n);
};

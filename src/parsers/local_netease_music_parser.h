//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <QObject>

#include "base_parser.h"

class LocalNeteaseMusicParser : public BaseParser {
Q_OBJECT
public:
    explicit LocalNeteaseMusicParser(QObject *parent = nullptr)
            : BaseParser(parent) {
    }

    LocalNeteaseMusicParser(const LocalNeteaseMusicParser &parser) = default;

    ~LocalNeteaseMusicParser() override = default;

    [[nodiscard]] const QStringList &acceptTypes() final;

    [[nodiscard]] bool accepted(const Media &media) final;

    [[nodiscard]] bool accepted(const QString &path) final;

    [[nodiscard]] LocalNeteaseMusicParser *clone() final;

    [[nodiscard]] Media getMedia(const QString &path) final;

    [[nodiscard]] Media parseMedia(const Media &media) final;

    [[nodiscard]] QString getMediaCover(const Media &media) final;
};

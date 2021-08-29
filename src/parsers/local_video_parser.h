//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <QObject>

#include "base_parser.h"

class LocalVideoParser : public BaseParser {
Q_OBJECT
public:
    explicit LocalVideoParser(QObject *parent = nullptr)
            : BaseParser(parent) {
    }

    LocalVideoParser(const LocalVideoParser &parser) = default;

    ~LocalVideoParser() override = default;

    [[nodiscard]] const QStringList &acceptTypes() final;

    [[nodiscard]] bool accepted(const Media &media) final;

    [[nodiscard]] bool accepted(const QString &path) final;

    [[nodiscard]] LocalVideoParser *clone() final;

    [[nodiscard]] Media getMedia(const QString &path) final;

    [[nodiscard]] Media parseMedia(const Media &media) final;

    [[nodiscard]] QString getMediaCover(const Media &media) final;
};

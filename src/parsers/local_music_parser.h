//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <QObject>
#include "base_parser.h"

class LocalMusicParser : public BaseParser {
Q_OBJECT
public:
    explicit LocalMusicParser(QObject *parent = nullptr) : BaseParser(parent) {}

    LocalMusicParser(const LocalMusicParser &parser) = default;

    ~LocalMusicParser() override = default;

    [[nodiscard]] LocalMusicParser *clone() final;

    [[nodiscard]] const QStringList &acceptTypes() final;

    [[nodiscard]] bool accepted(const Media &media) final;

    [[nodiscard]] bool accepted(const QString &path) final;

    [[nodiscard]] Media getMedia(const QString& path) final;

    [[nodiscard]] Media parseMedia(const Media& media) final;

    [[nodiscard]] QString getMediaCover(const Media& media) final;

};

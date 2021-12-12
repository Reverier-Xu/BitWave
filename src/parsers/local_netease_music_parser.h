/**
 * @file local_netease_music_parser.h
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

#include "base_parser.h"

class LocalNeteaseMusicParser : public BaseParser {
    Q_OBJECT
   public:
    explicit LocalNeteaseMusicParser(QObject *parent = nullptr);

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

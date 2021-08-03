//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_LOCAL_NETEASE_MUSIC_PARSER_H
#define BITWAVE_LOCAL_NETEASE_MUSIC_PARSER_H

#include <QObject>
#include "base_parser.h"


class LocalNeteaseMusicParser : public BaseParser {
Q_OBJECT
public:
    explicit LocalNeteaseMusicParser(QObject *parent = nullptr) : BaseParser(parent) {}

    LocalNeteaseMusicParser(const LocalNeteaseMusicParser& parser) = default;

    ~LocalNeteaseMusicParser() override = default;

    [[nodiscard]] const QStringList &acceptTypes() final;

    [[nodiscard]] bool accepted(const Media &media) final;

    [[nodiscard]] bool accepted(const QString &path) final;

    [[nodiscard]] const Media parse(const Media &media) final;

    [[nodiscard]] bool fillMetaData(Media& media) final;

    [[nodiscard]] LocalNeteaseMusicParser* clone() final;
};

#endif  // BITWAVE_LOCAL_NETEASE_MUSIC_PARSER_H

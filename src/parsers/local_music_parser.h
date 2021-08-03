//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_LOCAL_MUSIC_PARSER_H
#define BITWAVE_LOCAL_MUSIC_PARSER_H

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

    [[nodiscard]] const Media parse(const Media &media) final;

    [[nodiscard]] bool fillMetaData(Media &media) final;
};

#endif  // BITWAVE_LOCAL_MUSIC_PARSER_H

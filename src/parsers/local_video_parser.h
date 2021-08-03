//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_LOCAL_VIDEO_PARSER_H
#define BITWAVE_LOCAL_VIDEO_PARSER_H


#include <QObject>
#include "base_parser.h"

class LocalVideoParser : public BaseParser {
Q_OBJECT
public:
    explicit LocalVideoParser(QObject *parent = nullptr) : BaseParser(parent) {}

    LocalVideoParser(const LocalVideoParser& parser) = default;

    ~LocalVideoParser() override = default;

    [[nodiscard]] const QStringList &acceptTypes() final;

    [[nodiscard]] bool accepted(const Media &media) final;

    [[nodiscard]] bool accepted(const QString &path) final;

    [[nodiscard]] const Media parse(const Media &media) final;

    bool fillMetaData(Media &media) final;

    [[nodiscard]] LocalVideoParser* clone() final;
};

#endif  // BITWAVE_LOCAL_VIDEO_PARSER_H

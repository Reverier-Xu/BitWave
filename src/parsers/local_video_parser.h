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
    [[nodiscard]] const QStringList& acceptTypes() final;

    [[nodiscard]] bool accepted(const Media& media) final;
};

#endif  // BITWAVE_LOCAL_VIDEO_PARSER_H

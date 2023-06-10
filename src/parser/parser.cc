/**
 * @file parser.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */
#include "parser.h"
#include "interface.h"
#include "ncm_parser.h"
#include "ffmpeg_parser.h"
#include <QImage>

static QList<IParser*> parsers;

Media Parser::parse(const QString& path) {
    for (auto parser : parsers) {
        if (parser->accepted(path)) {
            return parser->parse(path);
        }
    }
    throw std::runtime_error("No parser accepted the media.");
}

QImage Parser::extractCover(const Media& src) {
    for (auto parser : parsers) {
        if (parser->accepted(src.url())) {
            return parser->extractCover(src);
        }
    }
    throw std::runtime_error("No parser accepted the media.");
}

void Parser::initParsers() {
    parsers.append(new NcmParser());
    parsers.append(new FfmpegParser());
}

void Parser::destroyParsers() {
    for (auto parser : parsers) {
        delete parser;
    }
}

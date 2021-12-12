/**
 * @file parser_factory.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "parser_factory.h"

#include <QDebug>
#include <QFileInfo>

ParserFactory *ParserFactory::mInstance = nullptr;

ParserFactory::ParserFactory(QObject *parent) : QObject(parent) {}

ParserFactory *ParserFactory::instance(QObject *parent) {
    if (!mInstance) {
        mInstance = new ParserFactory(parent);
    }
    return mInstance;
}

void ParserFactory::registerParser(BaseParser *parser) {
    for (auto &i : parser->acceptTypes())
        ParserFactory::instance()->mParsersMap[i] = parser;
}

BaseParser *ParserFactory::getParser(const Media &media) {
    const auto &path = media.rawUrl();
    return ParserFactory::getParser(path);
}

BaseParser *ParserFactory::getParser(const QString &media_path) {
    if (!ParserFactory::instance()->mParsersMap.contains(
            QFileInfo(media_path).suffix().toLower()))
        return nullptr;
    return ParserFactory::instance()
        ->mParsersMap.value(QFileInfo(media_path).suffix().toLower(), nullptr)
        ->clone();
}

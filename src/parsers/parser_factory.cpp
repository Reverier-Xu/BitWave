//
// Created by Reverier-Xu on 2021/8/1.
//

#include <QFileInfo>
#include "parser_factory.h"

ParserFactory *ParserFactory::mInstance = nullptr;

ParserFactory::ParserFactory(QObject *parent) : QObject(parent) {

}

ParserFactory *ParserFactory::getInstance(QObject *parent) {
    if (!mInstance) {
        mInstance = new ParserFactory(parent);
    }
    return mInstance;
}

void ParserFactory::registerParser(BaseParser *parser) {
    for (auto &i : parser->acceptTypes())
        ParserFactory::getInstance()->mParsersMap[i] = parser;
}

BaseParser *ParserFactory::getParser(const Media &media) {
    const auto &path = media.rawUrl();
    return ParserFactory::getParser(path);
}

BaseParser *ParserFactory::getParser(const QString &media_path) {
    return ParserFactory::getInstance()->mParsersMap.value(QFileInfo(media_path).suffix().toLower(),
                                                           nullptr)->clone();
}

//
// Created by Reverier-Xu on 2021/8/1.
//

#include "parser_factory.h"

ParserFactory* ParserFactory::mInstance = nullptr;

ParserFactory::ParserFactory(QObject *parent) : QObject(parent) {}

ParserFactory* ParserFactory::getInstance(QObject* parent) {
    if (!mInstance) {
        mInstance = new ParserFactory(parent);
    }
    return mInstance;
}

/**
 * @file base_parser.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "base_parser.h"

QUuid BaseParser::parserId() { return mParserId; }

void BaseParser::setParserId(const QUuid &n) {
    mParserId = n;
    emit parserIdChanged(n);
}

BaseParser::BaseParser(QObject *parent) : QObject(parent) {}

BaseParser::BaseParser(const BaseParser &parser) { setParent(parser.parent()); }

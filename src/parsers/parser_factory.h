/**
 * @file parser_factory.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QMap>
#include <QObject>

#include "base_parser.h"

class ParserFactory : public QObject {
    Q_OBJECT
   private:
    QMap<QString, BaseParser *> mParsersMap;

   protected:
    static ParserFactory *mInstance;

    explicit ParserFactory(QObject *parent);

   public:
    static ParserFactory *instance(QObject *parent = nullptr);

    static void registerParser(BaseParser *parser);

    static BaseParser *getParser(const Media &media);

    static BaseParser *getParser(const QString &media_path);
};

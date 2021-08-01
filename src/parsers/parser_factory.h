//
// Created by Reverier-Xu on 2021/8/1.
//

#ifndef BITWAVE_PARSER_FACTORY_H
#define BITWAVE_PARSER_FACTORY_H

#include <QMap>
#include <QObject>

#include "base_parser.h"

class ParserFactory : public QObject {
    Q_OBJECT
   private:
    QMap<QString, BaseParser *> mParsers;

   protected:
    static ParserFactory *mInstance;

    ParserFactory(QObject *parent);

   public:
    static ParserFactory *getInstance(QObject *parent = nullptr);

    static void registerParser(BaseParser *parser);

    static BaseParser *getParser(const Media &media);
};

#endif  // BITWAVE_PARSER_FACTORY_H

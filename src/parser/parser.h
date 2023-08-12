/**
 * @file parser.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QString>

#include "models/media.h"

class Parser {
   public:
    static void initParsers();

    static void destroyParsers();

    static Media parse(const QString& path);

    static bool accept(const QString& path);
    
    static QImage extractCover(const Media& src);
};

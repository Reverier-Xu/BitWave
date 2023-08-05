/**
 * @file codec.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once
#include "interface.h"

class Codec {
   public:
    static void initCodecs();

    static QStringList supportedFormats();

    static QString decode(const Media& src);

    static void destroyCodecs();
};

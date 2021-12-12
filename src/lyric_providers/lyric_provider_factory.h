/**
 * @file lyric_provider_factory.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QList>
#include <QMap>
#include <QObject>

#include "base_lyric_provider.h"

class LyricProviderFactory : public QObject {
    Q_OBJECT
   private:
    QList<BaseLyricProvider *> mLyricProviderList;

   protected:
    static LyricProviderFactory *mInstance;

    explicit LyricProviderFactory(QObject *parent);

   public:
    static LyricProviderFactory *instance(QObject *parent = nullptr);

    static void registerProvider(BaseLyricProvider *provider);

    static BaseLyricProvider *getProvider(const Media &media);
};

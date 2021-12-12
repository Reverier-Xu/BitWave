/**
 * @file lyric_provider_factory.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "lyric_provider_factory.h"

LyricProviderFactory *LyricProviderFactory::mInstance = nullptr;

LyricProviderFactory::LyricProviderFactory(QObject *parent) : QObject(parent) {}

LyricProviderFactory *LyricProviderFactory::instance(QObject *parent) {
    if (mInstance == nullptr) {
        mInstance = new LyricProviderFactory(parent);
    }
    return mInstance;
}

void LyricProviderFactory::registerProvider(BaseLyricProvider *provider) {
    LyricProviderFactory::instance()->mLyricProviderList.append(provider);
}

BaseLyricProvider *LyricProviderFactory::getProvider(const Media &media) {
    for (auto provider : LyricProviderFactory::instance()->mLyricProviderList) {
        if (provider->accepted(media)) {
            return provider->clone();
        }
    }
    return nullptr;
}

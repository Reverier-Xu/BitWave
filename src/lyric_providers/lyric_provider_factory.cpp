//
// Created by reverier on 2021/8/29.
//

#include "lyric_provider_factory.h"

LyricProviderFactory *LyricProviderFactory::mInstance = nullptr;

LyricProviderFactory::LyricProviderFactory(QObject *parent)
        : QObject(parent) {
}

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

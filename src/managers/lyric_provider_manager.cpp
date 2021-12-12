/**
 * @file lyric_provider_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "lyric_provider_manager.h"

LyricProviderManager *LyricProviderManager::mInstance = nullptr;

LyricProviderManager::LyricProviderManager(QObject *parent) : QObject(parent) {
    registerProvidersInFactory();
}

LyricProviderManager *LyricProviderManager::instance(QObject *parent) {
    if (mInstance == nullptr) {
        mInstance = new LyricProviderManager(parent);
    }
    return mInstance;
}

void LyricProviderManager::registerProvidersInFactory() {
    auto factory = LyricProviderFactory::instance(this);
    LyricProviderFactory::registerProvider(new NeteaseLyricProvider(factory));
}

void LyricProviderManager::setNeededLyricsUuid(QUuid uuid) {
    mNeededLyricsUuid = uuid;
}

QUuid LyricProviderManager::neededLyricsUuid() { return mNeededLyricsUuid; }

void LyricProviderManager::handleGetLyricsRequest(const Media &media) {
    auto provider = LyricProviderFactory::getProvider(media);
    if (provider == nullptr) {
        emit lyricsIsReady(false, QString(), QString());
        return;
    }
    provider->setProviderId(QUuid::createUuid());
    setNeededLyricsUuid(provider->providerId());
    provider->getLyricsRequest(media);
    connect(provider, &BaseLyricProvider::lyricsIsReady, this,
            [=](bool ok, const QString &raw, const QString &tr) {
                if (neededLyricsUuid() == provider->providerId()) {
                    emit lyricsIsReady(ok, raw, tr);
                }
                provider->deleteLater();
            });
}

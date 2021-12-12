/**
 * @file base_lyric_provider.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "base_lyric_provider.h"

QUuid BaseLyricProvider::providerId() const { return mProviderId; }

void BaseLyricProvider::setProviderId(const QUuid &id) {
    mProviderId = id;
    emit providerIdChanged(id);
}

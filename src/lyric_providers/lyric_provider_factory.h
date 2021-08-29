//
// Created by reverier on 2021/8/29.
//

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

//
// Created by reverier on 2021/8/29.
//

#pragma once

#include <QFuture>
#include <QObject>
#include <QUuid>
#include <QtConcurrent/QtConcurrent>

#include "lyric_providers/base_lyric_provider.h"
#include "lyric_providers/lyric_provider_factory.h"
#include "lyric_providers/netease_lyric_provider.h"
#include "models/media.h"

class LyricProviderManager : public QObject {
Q_OBJECT
private:
    QUuid mNeededLyricsUuid;

protected:
    static LyricProviderManager *mInstance;

    explicit LyricProviderManager(QObject *parent);

public:
    static LyricProviderManager *instance(QObject *parent = nullptr);

    void registerProvidersInFactory();

    void setNeededLyricsUuid(QUuid uuid);

    QUuid neededLyricsUuid();

public slots:

    void handleGetLyricsRequest(const Media &media);

signals:

    void lyricsIsReady(bool ok, const QString &raw, const QString &tr);
};

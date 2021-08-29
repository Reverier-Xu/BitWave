//
// Created by Reverier-Xu in 2021/8/29
//

#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "base_lyric_provider.h"
#include "models/media.h"

class NeteaseLyricProvider : public BaseLyricProvider {
Q_OBJECT
private:
    QNetworkAccessManager *mManager = nullptr;

public:
    explicit NeteaseLyricProvider(QObject *parent = nullptr);

    ~NeteaseLyricProvider() override = default;

    NeteaseLyricProvider *clone() override;

    bool accepted(const Media &media) override;

public slots:

    void getLyricsRequest(const Media &media) override;

    void onRequestFinished(QNetworkReply *reply);
};

/**
 * @file netease_lyric_provider.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

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

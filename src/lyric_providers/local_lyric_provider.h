/**
 * @file local_lyric_provider.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief provide local lyric
 * @version 0.1
 * @date 2021-12-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "base_lyric_provider.h"
#include "models/media.h"

class LocalLyricProvider : public BaseLyricProvider {
    Q_OBJECT
   private:
    QNetworkAccessManager *mManager = nullptr;

   public:
    explicit LocalLyricProvider(QObject *parent = nullptr);

    ~LocalLyricProvider() override = default;

    LocalLyricProvider *clone() override;

    bool accepted(const Media &media) override;

   public slots:
    void getLyricsRequest(const Media &media) override;

};

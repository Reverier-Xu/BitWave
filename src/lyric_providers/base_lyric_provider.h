/**
 * @file base_lyric_provider.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QObject>
#include <QUuid>

#include "models/media.h"

class BaseLyricProvider : public QObject {
    Q_OBJECT
   private:
    QUuid mProviderId{};

   public:
    explicit BaseLyricProvider(QObject *parent = nullptr) : QObject(parent) {}

    ~BaseLyricProvider() override = default;

    [[nodiscard]] virtual BaseLyricProvider *clone() = 0;

    virtual bool accepted(const Media &media) = 0;

    [[nodiscard]] QUuid providerId() const { return this->mProviderId; }

    void setProviderId(const QUuid &id) {
        this->mProviderId = id;
        emit this->providerIdChanged(id);
    }

   public slots:

    virtual void getLyricsRequest(const Media &media) = 0;

   signals:

    void providerIdChanged(QUuid id);

    void lyricsIsReady(bool ok, QString raw, QString tr);
};

//
// Created by Reverier-Xu in 2021/8/29
//

#pragma once

#include <QObject>
#include <QUuid>

#include "models/media.h"

class BaseLyricProvider : public QObject {
Q_OBJECT
private:
    QUuid mProviderId{ };

public:
    explicit BaseLyricProvider(QObject *parent = nullptr)
            : QObject(parent) {
    }

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

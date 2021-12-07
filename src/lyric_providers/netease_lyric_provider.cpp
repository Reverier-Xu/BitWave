/**
 * @file netease_lyric_provider.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#include "netease_lyric_provider.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "app_defs.h"
#include "utilities/ncm_helper.h"

NeteaseLyricProvider::NeteaseLyricProvider(QObject *parent)
    : BaseLyricProvider(parent) {
    mManager = new QNetworkAccessManager(this);
    connect(mManager, &QNetworkAccessManager::finished, this,
            &NeteaseLyricProvider::onRequestFinished);
}

NeteaseLyricProvider *NeteaseLyricProvider::clone() {
    return new NeteaseLyricProvider(this->parent());
}

bool NeteaseLyricProvider::accepted(const Media &media) {
    const auto &cm = media.comment();
    if (cm.startsWith("163 key(Don't modify):")) return true;
    return false;
}

void NeteaseLyricProvider::getLyricsRequest(const Media &media) {
    auto musicId = NcmHelper::getMusicId(media.comment());
    auto url = BitWaveConstants::getNeteaseLyricsApiAddress(
        QString("%1").arg(musicId));
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  BitWaveConstants::cliApiUa());
    this->mManager->get(req);
}

void NeteaseLyricProvider::onRequestFinished(QNetworkReply *reply) {
    // qDebug() << reply;
    if (reply->error() != QNetworkReply::NoError) {
        // qDebug() << reply->errorString();
        emit this->lyricsIsReady(false, QString(), QString());
        return;
    }
    QString result = QString::fromUtf8(reply->readAll());
    // qDebug() << result;
    QJsonParseError jsonError{};
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(result.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        // qDebug() << jsonError.errorString();
        emit this->lyricsIsReady(false, QString(), QString());
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.value("nolyric").toBool()) {
        emit this->lyricsIsReady(true, tr("[00:00.00]Pure Music"),
                                 tr("[00:00.00]Enjoy."));
        return;
    }
    if (jsonObj.value("uncollected").toBool()) {
        emit this->lyricsIsReady(true, tr("[00:00.00]Lyrics not found"),
                                 QString());
        return;
    }
    auto rawLyrics = jsonObj.value("lrc").toObject().value("lyric").toString();
    auto trLyrics =
        jsonObj.value("tlyric").toObject().value("lyric").toString();
    emit this->lyricsIsReady(true, rawLyrics, trLyrics);
    reply->deleteLater();
}

#include "ncm_lyrics.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

#include "utils/ncm_helper.h"

NcmLyrics::NcmLyrics(QObject* parent) : ILyrics(parent) {
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this,
            &NcmLyrics::processReply);
}

NcmLyrics::~NcmLyrics() { m_manager->deleteLater(); }

ILyrics* NcmLyrics::clone() {
    return new NcmLyrics(this->parent());
}

void NcmLyrics::processReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit lyricsFetched(tr("[00:00.00]Lyrics not found"));
        return;
    }
    QString result = QString::fromUtf8(reply->readAll());
    // qDebug() << result;
    QJsonParseError jsonError{};
    QJsonDocument jsonDoc =
        QJsonDocument::fromJson(result.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        emit lyricsFetched(tr("[00:00.00]Json parse error"));
        return;
    }
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.value("uncollected").toBool()) {
        emit lyricsFetched(tr("[00:00.00]Lyrics not found"));
        return;
    }
    auto rawLyrics =
        jsonObj.value("lrc").toObject().value("lyric").toString().trimmed();
    if (rawLyrics.isEmpty()) {
        emit lyricsFetched(tr("[00:00.00]Pure Music\n[00:03.00]Enjoy."));
        return;
    }
    auto trLyrics =
        jsonObj.value("tlyric").toObject().value("lyric").toString().trimmed();
    emit lyricsFetched(rawLyrics, trLyrics);
    reply->deleteLater();
}

bool NcmLyrics::accepted(const Media& media) {
    if (media.comment().startsWith("163 key(Don't modify):")) {
        return true;
    }
    return false;
}

void NcmLyrics::requestFetch(const Media& media) {
    auto metadata = getDecryptedMetadata(media.comment());
    auto musicId = metadata["musicId"].toVariant().toLongLong();
    auto url = QUrl(QString(LYRICS_API).arg(musicId));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, CLI_API_UA);
    auto reply = m_manager->get(request);
}

/**
 * @file media.h
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
#include <QString>
#include <QtQmlIntegration>


enum MediaType { AUDIO = 0, VIDEO, UNKNOWN };

//enum MediaFormat {
//    // popular audio formats
//    MP3, FLAC, WAV, APE, OGG, AAC, M4A, WMA, OPUS,
//    // popular video formats
//    MP4, MKV, AVI, WMV, FLV, MOV, RMVB, RM, WEBM, MPG, MPEG, M4V,
//    // Netease Cloud Music
//    NCM,
//};

class Media : public QObject {
   Q_GADGET
    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(double time READ time WRITE setTime)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QStringList artists READ artists WRITE setArtists)
    Q_PROPERTY(MediaType type READ type WRITE setType)
    Q_PROPERTY(QString album READ album WRITE setAlbum)
    Q_PROPERTY(QString comment READ comment WRITE setComment)

   private:
    QString m_url;

    double m_time{};

    QString m_title;

    QStringList m_artists;

    QString m_album;

    MediaType m_type;

    QString m_comment;

   public:
    explicit Media(const QString& rawUrl = "",
                   const QString& title = "No media", const QStringList& artists = {"Unknown Artist"},
                   const QString& album = "Unknown Album", MediaType type = UNKNOWN,
                   double duration = 0.0, const QString& comment = "");

    Media(const Media& media);

    Media& operator=(const Media& media);

    [[nodiscard]] QString url() const;

    void setUrl(const QString& n);

    [[nodiscard]] double time() const;

    void setTime(double n);

    [[nodiscard]] QString title() const;

    void setTitle(const QString& n);

    [[nodiscard]] QStringList artists() const;

    void setArtists(const QStringList& n);

    [[nodiscard]] MediaType type() const;

    void setType(MediaType n);

    [[nodiscard]] const QString& album() const;

    void setAlbum(const QString& n);

    [[nodiscard]] const QString& comment() const;

    void setComment(const QString& n);
};

Q_DECLARE_METATYPE(Media)

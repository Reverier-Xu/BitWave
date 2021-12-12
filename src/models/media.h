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

enum MediaType { AUDIO = 0, VIDEO, UNKNOWN };

enum MediaSourceType {
    LOCAL_MUSIC,
    LOCAL_VIDEO,
    LOCAL_NETEASE_MUSIC,
    ONLINE_NETEASE_MUSIC,
    ONLINE_BILIBILI_VIDEO
};

class Media : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString rawUrl MEMBER mRawUrl READ rawUrl WRITE setRawUrl NOTIFY
                   rawUrlChanged)
    Q_PROPERTY(double duration MEMBER mDuration READ duration WRITE setDuration
                   NOTIFY durationChanged)
    Q_PROPERTY(QString title MEMBER mTitle READ title WRITE setTitle NOTIFY
                   titleChanged)
    Q_PROPERTY(QString artist MEMBER mArtist READ artist WRITE setArtist NOTIFY
                   artistChanged)
    Q_PROPERTY(
        MediaType type MEMBER mType READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString collection MEMBER mCollection READ collection WRITE
                   setCollection NOTIFY collectionChanged)
    Q_PROPERTY(QString comment MEMBER mComment READ comment WRITE setComment
                   NOTIFY commentChanged)

   private:      // data
    uint mId{};  // for database use only
    QString mRawUrl;
    double mDuration{};
    QString mTitle;
    QString mArtist;
    QString mCollection;
    MediaType mType;
    QString mComment;

   public:
    explicit Media(QObject *parent = nullptr, const QString &rawUrl = "",
                   const QString &title = "", const QString &artist = "",
                   const QString &collection = "", MediaType type = UNKNOWN,
                   double duration = 0.0, const QString &comment = "")
        : QObject(parent) {
        mRawUrl = rawUrl;
        mTitle = title;
        mArtist = artist;
        mCollection = collection;
        mType = type;
        mDuration = duration;
        mComment = comment;
    }

    Media(const Media &media);

    Media &operator=(const Media &media);

    [[nodiscard]] QString rawUrl() const;

    void setRawUrl(const QString &n);

    [[nodiscard]] double duration() const;

    void setDuration(double n);

    [[nodiscard]] QString title() const;

    void setTitle(const QString &n);

    [[nodiscard]] QString artist() const;

    void setArtist(const QString &n);

    [[nodiscard]] MediaType type() const;

    void setType(MediaType n);

    [[nodiscard]] const QString &collection() const;

    void setCollection(const QString &n);

    [[nodiscard]] const QString &comment() const;

    void setComment(const QString &n);

   signals:

    void rawUrlChanged(const QString &n);

    void durationChanged(double n);

    void titleChanged(const QString &n);

    void artistChanged(const QString &n);

    void typeChanged(MediaType n);

    void collectionChanged(const QString &n);

    void commentChanged(const QString &n);
};

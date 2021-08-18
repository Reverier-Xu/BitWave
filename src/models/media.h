//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <QObject>
#include <QString>

enum MediaType {
    AUDIO, VIDEO, UNKNOWN
};

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
    Q_PROPERTY(QString coverUrl MEMBER mCoverUrl READ coverUrl WRITE setCoverUrl
                       NOTIFY coverUrlChanged)
    Q_PROPERTY(
            MediaType type MEMBER mType READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString collection MEMBER mCollection READ collection WRITE setCollection NOTIFY collectionChanged)

private:
    QString mRawUrl;
    double mDuration;
    QString mTitle;
    QString mArtist;
    QString mCollection;
    QString mCoverUrl;
    MediaType mType;

public:
    explicit Media(QObject *parent = nullptr,
                   const QString &rawUrl = "",
                   const QString &title = "",
                   const QString &artist = "",
                   const QString &collection="",
                   MediaType type = UNKNOWN,
                   double duration=0.0) : QObject(parent) {
        mRawUrl = rawUrl;
        mTitle = title;
        mArtist = artist;
        mCollection = collection;
        mType = type;
        mDuration = duration;
    }

    Media(const Media &media) : QObject(media.parent()) {
        mRawUrl = media.mRawUrl;
        mDuration = media.mDuration;
        mTitle = media.mTitle;
        mArtist = media.mArtist;
        mCollection = media.mCollection;
        mCoverUrl = media.mCoverUrl;
        mType = media.mType;
    }

    Media &operator=(const Media &media) {
        this->setParent(media.parent());
        mRawUrl = media.mRawUrl;
        mDuration = media.mDuration;
        mTitle = media.mTitle;
        mArtist = media.mArtist;
        mCoverUrl = media.mCoverUrl;
        mType = media.mType;
        return *this;
    }

    [[nodiscard]] QString rawUrl() const { return this->mRawUrl; }

    void setRawUrl(const QString &n) {
        this->mRawUrl = n;
        emit rawUrlChanged(n);
    }

    [[nodiscard]] double duration() const { return this->mDuration; }

    void setDuration(double n) {
        this->mDuration = n;
        emit durationChanged(n);
    }

    [[nodiscard]] QString title() const { return this->mTitle; }

    void setTitle(const QString &n) {
        this->mTitle = n;
        emit titleChanged(n);
    }

    [[nodiscard]] QString artist() const { return this->mArtist; }

    void setArtist(const QString &n) {
        this->mArtist = n;
        emit artistChanged(n);
    }

    [[nodiscard]] QString coverUrl() const { return this->mCoverUrl; }

    void setCoverUrl(const QString &n) {
        this->mCoverUrl = n;
        emit coverUrlChanged(n);
    }

    [[nodiscard]] MediaType type() const { return this->mType; }

    void setType(MediaType n) {
        this->mType = n;
        emit typeChanged(n);
    }

    [[nodiscard]] const QString &collection() const { return this->mCollection; }

    void setCollection(const QString &n) {
        this->mCollection = n;
        emit collectionChanged(n);
    }

signals:

    void rawUrlChanged(const QString &n);

    void durationChanged(double n);

    void titleChanged(const QString &n);

    void artistChanged(const QString &n);

    void coverUrlChanged(const QString &n);

    void typeChanged(MediaType n);

    void collectionChanged(const QString &n);
};

/**
 * @file media.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "media.h"

Media::Media(const Media &media) : QObject(media.parent()) {
    mRawUrl = media.mRawUrl;
    mDuration = media.mDuration;
    mTitle = media.mTitle;
    mArtist = media.mArtist;
    mCollection = media.mCollection;
    mType = media.mType;
    mComment = media.mComment;
}

Media &Media::operator=(const Media &media) {
    setParent(media.parent());
    mRawUrl = media.mRawUrl;
    mDuration = media.mDuration;
    mTitle = media.mTitle;
    mArtist = media.mArtist;
    mType = media.mType;
    mComment = media.mComment;
    return *this;
}

QString Media::rawUrl() const { return mRawUrl; }

void Media::setRawUrl(const QString &n) {
    mRawUrl = n;
    emit rawUrlChanged(n);
}

double Media::duration() const { return mDuration; }

void Media::setDuration(double n) {
    mDuration = n;
    emit durationChanged(n);
}

QString Media::title() const { return mTitle; }

void Media::setTitle(const QString &n) {
    mTitle = n;
    emit titleChanged(n);
}

QString Media::artist() const { return mArtist; }

void Media::setArtist(const QString &n) {
    mArtist = n;
    emit artistChanged(n);
}

MediaType Media::type() const { return mType; }

void Media::setType(MediaType n) {
    mType = n;
    emit typeChanged(n);
}

const QString &Media::collection() const { return mCollection; }

void Media::setCollection(const QString &n) {
    mCollection = n;
    emit collectionChanged(n);
}

const QString &Media::comment() const { return mComment; }

void Media::setComment(const QString &n) {
    mComment = n;
    emit commentChanged(n);
}

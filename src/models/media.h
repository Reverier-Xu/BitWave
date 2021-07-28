//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_MEDIA_H
#define BITWAVE_MEDIA_H

#include <QString>

class Media {
   private:
    QString mRawUrl;
    double mDuration;
    QString mTitle;
    QString mArtist;
    QString mCoverUrl;

   public:
    void setRawUrl(const QString &n) { this->mRawUrl = n; }

    [[nodiscard]] QString rawUrl() const { return this->mRawUrl; }

    void setDuration(double n) { this->mDuration = n; }

    [[nodiscard]] double duration() const { return this->mDuration; }

    void setTitle(const QString &n) { this->mTitle = n; }

    [[nodiscard]] QString title() const { return this->mTitle; }

    void setArtist(const QString &n) { this->mArtist = n; }

    [[nodiscard]] QString artist() const { return this->mArtist; }

    void setCoverUrl(const QString &n) { this->mCoverUrl = n; }

    [[nodiscard]] QString coverUrl() const { return this->mCoverUrl; }
};

#endif  // BITWAVE_MEDIA_H

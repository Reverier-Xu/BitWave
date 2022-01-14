/**
 * @file playlist_model.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-01-14
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QAbstractListModel>

#include "models/media.h"

class PlaylistModel : public QAbstractListModel {
    Q_OBJECT
   public:
    enum PlaylistRoles {
        TitleRole = Qt::UserRole + 1,
        AlbumRole,
        ArtistRole,
        DurationRole,
        UriRole,
    };

    explicit PlaylistModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setMediaList(QList<Media> *mediaList);

    void setUriMap(QMap<QString, QString> *uriMap);

    void reload();

    void setIsEndpoint(bool isEndpoint);

   private:
    QList<Media> *mMediaList{};
    QMap<QString, QString> *mUriMap{};
    bool mIsEndpoint = false;
};

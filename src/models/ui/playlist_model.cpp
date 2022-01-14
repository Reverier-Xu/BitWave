/**
 * @file playlist_model.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-01-14
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "playlist_model.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent) {}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    if (mIsEndpoint) {
        return mMediaList->size();
    } else {
        return mUriMap->size();
    }
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (mIsEndpoint) {
        if (index.row() < mMediaList->size()) {
            switch (role) {
                case TitleRole:
                    return mMediaList->at(index.row()).title();
                case AlbumRole:
                    return mMediaList->at(index.row()).collection();
                case ArtistRole:
                    return mMediaList->at(index.row()).artist();
                case DurationRole:
                    return mMediaList->at(index.row()).duration();
                case UriRole:
                    return "";
                default:
                    return QVariant();
            }
        }
    } else {
        if (index.row() < mUriMap->size()) {
            switch (role) {
                case TitleRole:
                    return mUriMap->keys().at(index.row());
                case AlbumRole:
                    return "";
                case ArtistRole:
                    return "";
                case DurationRole:
                    return 0;
                case UriRole:
                    return mUriMap->value(mUriMap->keys().at(index.row()));
                default:
                    return QVariant();
            }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[AlbumRole] = "album";
    roles[ArtistRole] = "artist";
    roles[DurationRole] = "duration";
    roles[UriRole] = "uri";
    return roles;
}

void PlaylistModel::setMediaList(QList<Media> *mediaList) {
    mMediaList = mediaList;
}

void PlaylistModel::setUriMap(QMap<QString, QString> *uriMap) {
    mUriMap = uriMap;
}

void PlaylistModel::reload() {
    beginResetModel();
    endResetModel();
}

void PlaylistModel::setIsEndpoint(bool isEndpoint) {
    mIsEndpoint = isEndpoint;
    reload();
}

/**
 * @file media_list.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-08-02
 *
 * @copyright 2023 Woo Tech
 */

#include "media_list.h"


MediaList::MediaList(QObject* parent) : QAbstractListModel(parent) {}

MediaList::~MediaList() = default;

int MediaList::rowCount(const QModelIndex& parent) const {
    return (int)m_list->size();
}

QVariant MediaList::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= m_list->size()) {
        return {};
    }

    switch (role) {
        case MediaTitleRole:
            return m_list->at(index.row()).title();
        case MediaTypeRole:
            return m_list->at(index.row()).type();
        case MediaArtistsRole:
            return m_list->at(index.row()).artists();
        case MediaAlbumRole:
            return m_list->at(index.row()).album();
        case MediaTimeRole:
            return m_list->at(index.row()).time();
        default:
            return {};
    }
}

QHash<int, QByteArray> MediaList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MediaTitleRole] = "mediaTitle";
    roles[MediaTypeRole] = "mediaType";
    roles[MediaArtistsRole] = "mediaArtists";
    roles[MediaAlbumRole] = "mediaAlbum";
    roles[MediaTimeRole] = "mediaTime";
    return roles;
}

void MediaList::setList(QVector<Media>* list) {
    beginResetModel();
    m_list = list;
    endResetModel();
}

void MediaList::reload() {
    beginResetModel();
    endResetModel();
}

void MediaList::removeMedia(int pos) {
    beginRemoveRows(QModelIndex(), pos, pos);
    endRemoveRows();
}

void MediaList::insertMedia(int pos) {
    beginInsertRows(QModelIndex(), pos, pos);
    endInsertRows();
}

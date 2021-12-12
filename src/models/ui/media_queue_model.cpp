/**
 * @file media_queue_model.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "media_queue_model.h"

#include <QDebug>
#include <QQueue>

MediaQueueModel::MediaQueueModel(QObject *parent)
    : QAbstractListModel(parent) {}

int MediaQueueModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    // qDebug() << "rowCount:" << mMediaQueue->count();
    return mMediaQueue->count();
}

QVariant MediaQueueModel::data(const QModelIndex &index, int role) const {
    //    qDebug() << index << role;
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mMediaQueue->size()) {
        return QVariant();
    }

    switch (role) {
        case MediaQueueModel::MediaTitleRole:
            return mMediaQueue->at(index.row()).title();
        case MediaQueueModel::MediaDurationRole:
            return mMediaQueue->at(index.row()).duration();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> MediaQueueModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MediaTitleRole] = "mediaTitle";
    roles[MediaDurationRole] = "mediaDuration";
    // qDebug() << roles;
    return roles;
}

void MediaQueueModel::setMediaQueue(QQueue<Media> *mediaQueue) {
    beginResetModel();
    mMediaQueue = mediaQueue;
    endResetModel();
}

void MediaQueueModel::reloadQueue(QQueue<Media> *mediaQueue) {
    beginResetModel();
    endResetModel();
}

void MediaQueueModel::beginRemoveMedia(int pos) {
    beginRemoveRows(QModelIndex(), pos, pos);
}

void MediaQueueModel::endRemoveMedia() { endRemoveRows(); }

void MediaQueueModel::beginInsertMedia(int pos) {
    beginInsertRows(QModelIndex(), pos, pos);
}

void MediaQueueModel::endInsertMedia() { endInsertRows(); }

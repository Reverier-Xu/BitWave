#include "media_queue_model.h"

#include <QDebug>
#include <QQueue>

MediaQueueModel::MediaQueueModel(QObject *parent)
        : QAbstractListModel(parent) {
}

int MediaQueueModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    // qDebug() << "rowCount:" << mMediaQueue->count();
    return mMediaQueue->count();
}

QVariant MediaQueueModel::data(const QModelIndex &index, int role) const {
    // qDebug() << index << role;
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mMediaQueue->size()) {
        return QVariant();
    }

    switch (role) {
        case MediaQueueModel::MediaIDRole:
            return index.row();
        case MediaQueueModel::MediaTitleRole:
            return mMediaQueue->at(index.row()).title();
        case MediaQueueModel::MediaDurationRole:
            return mMediaQueue->at(index.row()).duration();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray>
MediaQueueModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MediaIDRole] = "mediaId";
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

void MediaQueueModel::reloadQueue() {
    beginResetModel();
    endResetModel();
}

//
// Created by: Reverier-Xu on 2021.08.26
//

#pragma once
#include <QAbstractListModel>
#include "models/media.h"

class MediaQueueModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum MediaQueueRoles {
        MediaIDRole = Qt::UserRole + 1,
        MediaTitleRole,
        MediaDurationRole,
    };

    explicit MediaQueueModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setMediaQueue(QQueue<Media> *mediaQueue);

    void reloadQueue();

private:
    QQueue<Media> *mMediaQueue{};

    int currentPlayingIndex = 0;
};

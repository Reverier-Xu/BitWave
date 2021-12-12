/**
 * @file media_queue_model.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QAbstractListModel>

#include "models/media.h"

class MediaQueueModel : public QAbstractListModel {
    Q_OBJECT
   public:
    enum MediaQueueRoles {
        MediaTitleRole = Qt::UserRole + 1,
        MediaDurationRole,
    };

    explicit MediaQueueModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setMediaQueue(QQueue<Media> *mediaQueue);

    void reloadQueue(QQueue<Media> *mediaQueue);

    void beginRemoveMedia(int pos);  // [begin, end]

    void endRemoveMedia();

    void beginInsertMedia(int pos);

    void endInsertMedia();

   private:
    QQueue<Media> *mMediaQueue{};
};

/**
 * @file media_list.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-08-02
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include "media.h"
#include <QAbstractListModel>

class MediaList : public QAbstractListModel {
    Q_OBJECT

  private:
    QVector<Media>* m_list{};

  public:
    explicit MediaList(QObject* parent = nullptr);

    ~MediaList() override;

    enum MediaListRoles {
        MediaTitleRole = Qt::UserRole + 1,
        MediaTypeRole,
        MediaArtistsRole,
        MediaAlbumRole,
        MediaTimeRole,
        MediaUrlRole,
    };

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setList(QVector<Media>* queue);

    void reload();

    void removeMedia(int pos);    // [begin, end]

    void insertMedia(int pos);
};

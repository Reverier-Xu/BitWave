/**
 * @file playlist.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "models/media.h"
#include "models/media_list.h"

class Playlist : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(SortStatus sortStatus READ sortStatus WRITE setSortStatus NOTIFY sortStatusChanged)
    Q_PROPERTY(QString current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(qsizetype size READ size NOTIFY sizeChanged)

  private:
    QString m_title{};
    QVector<Media> m_playlist{};
    MediaList* m_model;
    QStringList m_playlists;
    SortStatus m_sortStatus = TitleAsc;
    QString m_current{};

  protected:
    static Playlist* m_instance;
    explicit Playlist(QObject* parent = nullptr);
    ~Playlist() override;

    void loadStorage();

  public:
    enum PlaylistRoles {
        NameRole = Qt::UserRole + 1,
    };

    static Playlist* instance(QObject* parent = nullptr);

    [[nodiscard]] MediaList* model() const;

    [[nodiscard]] SortStatus sortStatus() const;

    void setSortStatus(SortStatus sortStatus);

    [[nodiscard]] QString current() const;

    void setCurrent(const QString& current);

    [[nodiscard]] qsizetype size() const;

    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

  public slots:
    Q_INVOKABLE void switchPlaylist(const QString& playlist);

    Q_INVOKABLE void addPlaylist(const QString& playlist);

    Q_INVOKABLE void removePlaylist(const QString& playlist);

    Q_INVOKABLE void addMediaToPlaylist(const Media& media, const QString& playlist);
    Q_INVOKABLE void addMediaIndexToPlaylist(const int index, const QString& playlist);

    Q_INVOKABLE void removeMediaFromPlaylist(const Media& media, const QString& playlist);
    Q_INVOKABLE void removeMediaIndex(const int index);

    Q_INVOKABLE void sortByTitle();

    Q_INVOKABLE void sortByArtists();

    Q_INVOKABLE void sortByAlbum();

    Q_INVOKABLE const QVector<Media>& currentMedias();

    Q_INVOKABLE void reload();

  signals:
    void sortStatusChanged(SortStatus sortStatus);

    void currentChanged(QString current);

    void sizeChanged(qsizetype size);
};

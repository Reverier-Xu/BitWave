/**
 * @file storage.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-08-05
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include "models/media.h"


class Storage : public QObject {
   Q_OBJECT

   private:
    QSqlDatabase m_db;

    bool m_isValid = false;

   protected:
    static Storage* m_instance;

    explicit Storage(QObject* parent = nullptr);

    ~Storage() override;

    void createLibraryStorage();

    void createPlaylistStorage();

    void createPlayQueue();

   public:
    static Storage* instance(QObject* parent = nullptr);

    QVector<Media> loadLibrary();

    QVector<Media> loadPlayQueue();

    QStringList loadPlaylists();

    QVector<Media> loadPlaylist(const QString& playlist);

   public slots:
    Q_INVOKABLE void addMedia(const Media& media);

    Q_INVOKABLE void addPlaylist(const QString& title, MediaType type);

    Q_INVOKABLE void linkMediaToPlaylist(const Media& media, const QString& playlist);

    Q_INVOKABLE void removeMedia(const Media& media);

    Q_INVOKABLE void removePlaylist(const QString& playlist);

    Q_INVOKABLE void removeMediaFromPlaylist(const Media& media, const QString& playlist);

    Q_INVOKABLE void renamePlaylist(const QString& oldName, const QString& newName);

    Q_INVOKABLE void storePlayQueue(const QVector<Media>& playQueue);
};

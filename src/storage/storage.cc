/**
 * @file storage.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-08-05
 *
 * @copyright 2023 Woo Tech
 */

#include <QMutexLocker>
#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include "storage.h"


Storage* Storage::m_instance = nullptr;

Storage::Storage(QObject* parent) : QObject(parent) {
    QDir dir;
    dir.mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    auto libraryDbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/library.db";
    m_libraryDb = QSqlDatabase::addDatabase("QSQLITE", libraryDbPath);
    m_libraryDb.setDatabaseName(libraryDbPath);
    auto queueDbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/queue.db";
    m_queueDb = QSqlDatabase::addDatabase("QSQLITE", queueDbPath);
    m_queueDb.setDatabaseName(queueDbPath);
    m_isValid = m_libraryDb.open() && m_queueDb.open();
    try {
        createLibraryStorage();
        createPlaylistStorage();
        createPlayQueue();
    } catch (std::runtime_error& e) {
        m_isValid = false;
        qWarning() << e.what();
    }
}

Storage::~Storage() = default;

Storage* Storage::instance(QObject* parent) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (m_instance == nullptr) {
        m_instance = new Storage(parent);
    }
    return m_instance;
}

void Storage::createLibraryStorage() {
    if (!m_isValid) {
        return;
    }
    QStringList sql;
    sql << "CREATE TABLE IF NOT EXISTS _LIBRARY ("
        << "id              INTEGER PRIMARY KEY AUTOINCREMENT,"
        << "title           TEXT,"
        << "artist          TEXT,"
        << "album           TEXT,"
        << "time            REAL,"
        << "url             VARCHAR(511) NOT NULL UNIQUE,"
        << "type            INTEGER,"
        << "comment         TEXT,"
        << "embedded_lyrics TEXT,"
        << "playlist_id     INTEGER"
        << ");";
    QSqlQuery query(sql.join(" "), m_libraryDb);
    if (!query.exec()) {
        throw std::runtime_error("Failed to create table _LIBRARY");
    }
    m_libraryDb.commit();
}

void Storage::createPlaylistStorage() {
    if (!m_isValid) {
        return;
    }
    QStringList sql;
    sql << "CREATE TABLE IF NOT EXISTS _PLAYLIST ("
        << "id              INTEGER PRIMARY KEY AUTOINCREMENT,"
        << "title           TEXT NOT NULL UNIQUE,"
        << "type            INTEGER"
        << ");";
    QSqlQuery query(sql.join(" "), m_libraryDb);
    if (!query.exec()) {
        throw std::runtime_error("Failed to create table _PLAYLIST");
    }
    m_libraryDb.commit();
}

void Storage::createPlayQueue() {
    QStringList sql;
    sql << "CREATE TABLE IF NOT EXISTS _PLAY_QUEUE ("
        << "id              INTEGER PRIMARY KEY AUTOINCREMENT,"
        << "title           TEXT,"
        << "artist          TEXT,"
        << "album           TEXT,"
        << "time            REAL,"
        << "url             VARCHAR(511) NOT NULL UNIQUE,"
        << "type            INTEGER,"
        << "comment         TEXT,"
        << "embedded_lyrics TEXT,"
        << "playlist_id     INTEGER"
        << ");";
    QSqlQuery query(sql.join(" "), m_queueDb);
    if (!query.exec()) {
        throw std::runtime_error("Failed to create table _PLAY_QUEUE");
    }
    m_queueDb.commit();
}

void Storage::addMedia(const Media& media) {
    QStringList sql;
    sql << "INSERT INTO _LIBRARY ("
        << "title          ,"
        << "artist         ,"
        << "album          ,"
        << "time           ,"
        << "url            ,"
        << "type           ,"
        << "comment        ,"
        << "embedded_lyrics,"
        << "playlist_id) VALUES ("
        << ":title          ,"
        << ":artist         ,"
        << ":album          ,"
        << ":time           ,"
        << ":url            ,"
        << ":type           ,"
        << ":comment        ,"
        << ":embedded_lyrics,"
        << ":playlist_id );";

    QSqlQuery query(m_libraryDb);
//    qDebug() << sql.join(" ");
    query.prepare(sql.join(" "));
    query.bindValue(":title", media.title());
    query.bindValue(":artist", media.artists().join("/"));
    query.bindValue(":album", media.album());
    query.bindValue(":time", media.time());
    query.bindValue(":url", media.url());
    query.bindValue(":type", media.type());
    query.bindValue(":comment", media.comment());
    query.bindValue(":embedded_lyrics", media.embeddedLyrics());
    query.bindValue(":playlist_id", -1);
    if (!query.exec()) {
        throw std::runtime_error("Failed to add media");
    }
//    qDebug() << "insert finished:" << query.lastError();
    m_libraryDb.commit();
}

void Storage::addPlaylist(const QString& title, MediaType type) {
    QStringList sql;
    sql << "INSERT INTO _PLAYLIST ("
        << "title,"
        << "type) VALUES ("
        << ":title,"
        << ":type );";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":title", title);
    query.bindValue(":type", type);
    if (!query.exec()) {
        throw std::runtime_error("Failed to add playlist");
    }
    m_libraryDb.commit();
}

void Storage::linkMediaToPlaylist(const Media& media, const QString& playlist) {
    QStringList sql;
    sql << "UPDATE _LIBRARY SET playlist_id = ("
        << "SELECT id FROM _PLAYLIST WHERE title = :playlist"
        << ") WHERE url = :url;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":playlist", playlist);
    query.bindValue(":url", media.url());
    if (!query.exec()) {
        throw std::runtime_error("Failed to link media to playlist");
    }
    m_libraryDb.commit();
}

QVector<Media> Storage::loadLibrary() {
    QStringList sql;
    sql << "SELECT * FROM _LIBRARY;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    if (!query.exec()) {
        throw std::runtime_error("Failed to load library");
    }
    QVector<Media> library;
    while (query.next()) {
        Media media;
        media.setTitle(query.value("title").toString());
        media.setArtists(query.value("artist").toString().split("/"));
        media.setAlbum(query.value("album").toString());
        media.setTime(query.value("time").toReal());
        media.setUrl(query.value("url").toString());
        media.setType(static_cast<MediaType>(query.value("type").toInt()));
        media.setComment(query.value("comment").toString());
        media.setEmbeddedLyrics(query.value("embedded_lyrics").toString());
        library.append(media);
    }
    return library;
}

QVector<Media> Storage::loadPlayQueue() {
    QStringList sql;
    sql << "SELECT * FROM _PLAY_QUEUE;";
    QSqlQuery query(m_queueDb);
    query.prepare(sql.join(" "));
    if (!query.exec()) {
        throw std::runtime_error("Failed to load play queue");
    }
    QVector<Media> playQueue;
    while (query.next()) {
        Media media;
        media.setTitle(query.value("title").toString());
        media.setArtists(query.value("artist").toString().split("/"));
        media.setAlbum(query.value("album").toString());
        media.setTime(query.value("time").toReal());
        media.setUrl(query.value("url").toString());
        media.setType(static_cast<MediaType>(query.value("type").toInt()));
        media.setComment(query.value("comment").toString());
        media.setEmbeddedLyrics(query.value("embedded_lyrics").toString());
        playQueue.append(media);
    }
    return playQueue;
}

QStringList Storage::loadPlaylists() {
    QStringList sql;
    sql << "SELECT title FROM _PLAYLIST;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    if (!query.exec()) {
        throw std::runtime_error("Failed to load playlists");
    }
    QStringList playlists;
    while (query.next()) {
        playlists.append(query.value("title").toString());
    }
    return playlists;
}

QVector<Media> Storage::loadPlaylist(const QString& playlist) {
    QStringList sql;
    sql << "SELECT * FROM _LIBRARY WHERE playlist_id = ("
        << "SELECT id FROM _PLAYLIST WHERE title = :playlist"
        << ");";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":playlist", playlist);
    if (!query.exec()) {
        throw std::runtime_error("Failed to load playlist");
    }
    QVector<Media> playlistMedias;
    while (query.next()) {
        Media media;
        media.setTitle(query.value("title").toString());
        media.setArtists(query.value("artist").toString().split("/"));
        media.setAlbum(query.value("album").toString());
        media.setTime(query.value("time").toReal());
        media.setUrl(query.value("url").toString());
        media.setType(static_cast<MediaType>(query.value("type").toInt()));
        media.setComment(query.value("comment").toString());
        media.setEmbeddedLyrics(query.value("embedded_lyrics").toString());
        playlistMedias.append(media);
    }
    return playlistMedias;
}

void Storage::removeMedia(const Media& media) {
    QStringList sql;
    sql << "DELETE FROM _LIBRARY WHERE url = :url;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":url", media.url());
    if (!query.exec()) {
        throw std::runtime_error("Failed to remove media");
    }
    m_libraryDb.commit();
}

void Storage::removePlaylist(const QString& playlist) {
    QStringList sql;
    sql << "DELETE FROM _PLAYLIST WHERE title = :title;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":title", playlist);
    if (!query.exec()) {
        throw std::runtime_error("Failed to remove playlist");
    }
    m_libraryDb.commit();
}

void Storage::removeMediaFromPlaylist(const Media& media, const QString& playlist) {
    QStringList sql;
    sql << "UPDATE _LIBRARY SET playlist_id = NULL WHERE url = :url AND playlist_id = ("
        << "SELECT id FROM _PLAYLIST WHERE title = :playlist"
        << ");";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":url", media.url());
    query.bindValue(":playlist", playlist);
    if (!query.exec()) {
        throw std::runtime_error("Failed to remove media from playlist");
    }
    m_libraryDb.commit();
}

void Storage::renamePlaylist(const QString& oldName, const QString& newName) {
    QStringList sql;
    sql << "UPDATE _PLAYLIST SET title = :newName WHERE title = :oldName;";
    QSqlQuery query(m_libraryDb);
    query.prepare(sql.join(" "));
    query.bindValue(":newName", newName);
    query.bindValue(":oldName", oldName);
    if (!query.exec()) {
        throw std::runtime_error("Failed to rename playlist");
    }
    m_libraryDb.commit();
}

void Storage::storePlayQueue(const QVector<Media>& playQueue) {
    QStringList sql;
    sql << "DELETE FROM _PLAY_QUEUE;";
    QSqlQuery query(m_queueDb);
    query.prepare(sql.join(" "));
    if (!query.exec()) {
        throw std::runtime_error("Failed to store play queue");
    }
    sql.clear();
    sql << "INSERT INTO _PLAY_QUEUE (title, artist, album, time, url, type, comment, embedded_lyrics) VALUES ("
        << ":title, :artist, :album, :time, :url, :type, :comment, :embedded_lyrics"
        << ");";
        query.prepare(sql.join(" "));
    for (const Media& media : playQueue) {
        query.bindValue(":title", media.title());
        query.bindValue(":artist", media.artists().join("/"));
        query.bindValue(":album", media.album());
        query.bindValue(":time", media.time());
        query.bindValue(":url", media.url());
        query.bindValue(":type", static_cast<int>(media.type()));
        query.bindValue(":comment", media.comment());
        query.bindValue(":embedded_lyrics", media.embeddedLyrics());
        if (!query.exec()) {
            throw std::runtime_error("Failed to store play queue");
        }
    }
    m_queueDb.commit();
}

/**
 * @file sqlite_engine.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "sqlite_engine.h"

#include <QDebug>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

SQLiteEngine *SQLiteEngine::mInstance = nullptr;

SQLiteEngine::SQLiteEngine(QObject *parent) { mDatabaseFile = ""; }

SQLiteEngine *SQLiteEngine::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new SQLiteEngine(parent);
    return mInstance;
}

bool SQLiteEngine::open(const QString &db) {
    mDatabaseFile = db;
    if (mDatabaseFile.isEmpty()) {
        qDebug() << "database file is empty";
        setIsOpen(false);
        setIsValid(false);
        return false;
    }
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.setDatabaseName(mDatabaseFile);
    if (!mDatabase.open()) {
        qDebug() << "database open failed";
        setIsOpen(false);
        setIsValid(false);
        return false;
    }
    // qDebug() << "database open succeeded";
    return true;
}

void SQLiteEngine::close() { mDatabase.close(); }

void SQLiteEngine::createMediaList(const QString &tableName,
                                   const QList<Media> &mediaList) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "CREATE TABLE IF NOT EXISTS " + tableName + " ("
            << "id         INTEGER PRIMARY KEY AUTOINCREMENT,"
            << "title      TEXT,"
            << "artist     TEXT,"
            << "collection TEXT,"
            << "duration   REAL,"
            << "url        TEXT,"
            << "type       INTEGER,"
            << "comment    TEXT,";
        QSqlQuery query(sql.join(" "), database());
        query.exec();
        query.clear();
        sql.clear();
        sql << "INSERT INTO :tableName ("
            << "title,"
            << "artist,"
            << "collection,"
            << "duration,"
            << "url,"
            << "type,"
            << "comment) VALUES ("
            << ":title,"
            << ":artist,"
            << ":collection,"
            << ":duration,"
            << ":url,"
            << ":type,"
            << ":comment);";

        for (const auto &media : mediaList) {
            query.clear();
            query.prepare(sql.join(" "));
            query.bindValue(":title", media.title());
            query.bindValue(":artist", media.artist());
            query.bindValue(":collection", media.collection());
            query.bindValue(":duration", media.duration());
            query.bindValue(":url", media.rawUrl());
            query.bindValue(":type", media.type());
            query.bindValue(":comment", media.comment());
            query.exec();
        }
    }
}

bool SQLiteEngine::dropMediaList(const QString &tableName) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "DROP TABLE IF EXISTS :tableName";
        QSqlQuery query(sql.join(" "), database());
        return query.exec();
    } else {
        return false;
    }
}

bool SQLiteEngine::addMedia(const QString &tableName, const Media &media) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "INSERT INTO :tableName ("
            << "title,"
            << "artist,"
            << "collection,"
            << "duration,"
            << "url,"
            << "type,"
            << "comment) VALUES ("
            << ":title,"
            << ":artist,"
            << ":collection,"
            << ":duration,"
            << ":url,"
            << ":type,"
            << ":comment);";
        QSqlQuery query(sql.join(" "), database());
        return query.exec();
    } else {
        return false;
    }
}

bool SQLiteEngine::deleteMedia(const QString &tableName, const Media &media) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "DELETE FROM :tableName WHERE "
            << "url=:url;";
        QSqlQuery query(sql.join(" "), database());
        query.bindValue(":tableName", tableName);
        query.bindValue(":url", media.rawUrl());
        return query.exec();
    } else {
        return false;
    }
}

QList<Media> SQLiteEngine::getMediaList(const QString &tableName) {
    QList<Media> mediaList;
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "SELECT * FROM :tableName";
        QSqlQuery query(sql.join(" "), database());
        query.bindValue(":tableName", tableName);
        query.exec();
        while (query.next()) {
            Media media;
            media.setTitle(query.value("title").toString());
            media.setArtist(query.value("artist").toString());
            media.setCollection(query.value("collection").toString());
            media.setDuration(query.value("duration").toDouble());
            media.setRawUrl(query.value("url").toString());
            media.setType((MediaType)(query.value("type").toInt()));
            media.setComment(query.value("comment").toString());
            mediaList.append(media);
        }
    }
    return mediaList;
}

bool SQLiteEngine::isMediaExist(const QString &tableName, const Media &media) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "SELECT * FROM :tableName WHERE "
            << "url=:url;";
        QSqlQuery query(sql.join(" "), database());
        query.bindValue(":tableName", tableName);
        query.bindValue(":url", media.rawUrl());
        query.exec();
        if (query.next()) {
            return true;
        } else {
            return false;
        }
    } else {
        throw std::runtime_error("DataBase is not available");
    }
}

bool SQLiteEngine::updateMedia(const QString &tableName, const Media &media) {
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "UPDATE       :tableName SET "
            << "title      = :title,"
            << "artist     = :artist,"
            << "collection = :collection,"
            << "duration   = :duration,"
            << "url        = :url,"
            << "type       = :type,"
            << "comment    = :comment"
            << " WHERE url = :srcUrl;";
        QSqlQuery query(sql.join(" "), database());
        query.bindValue(":tableName", tableName);
        query.bindValue(":title", media.title());
        query.bindValue(":artist", media.artist());
        query.bindValue(":collection", media.collection());
        query.bindValue(":duration", media.duration());
        query.bindValue(":url", media.rawUrl());
        query.bindValue(":type", media.type());
        query.bindValue(":comment", media.comment());
        query.bindValue(":srcUrl", media.rawUrl());
        return query.exec();
    } else {
        return false;
    }
}

bool SQLiteEngine::updateMediaList(const QString &tableName,
                                   const QList<Media> &mediaList) {
    bool res = true;
    if (isOpen() && isValid()) {
        for (const auto &media : mediaList) {
            res &= updateMedia(tableName, media);
        }
    }
    return res;
}

QStringList SQLiteEngine::getMediaLists() {
    QStringList mediaLists;
    if (isOpen() && isValid()) {
        QStringList sql;
        sql << "SELECT name FROM sqlite_master WHERE type='table' AND name NOT "
               "LIKE 'sqlite_%';";
        QSqlQuery query(mDatabase);
        query.exec(sql.join(" "));
        while (query.next()) {
            mediaLists.append(query.value("name").toString());
        }
    }
    return mediaLists;
}

QString SQLiteEngine::databaseFile() { return mDatabaseFile; }

void SQLiteEngine::setDatabaseFile(const QString &databaseFile) {
    mDatabaseFile = databaseFile;
}

QString SQLiteEngine::databaseName() { return mDatabaseName; }

void SQLiteEngine::setDatabaseName(const QString &databaseName) {
    mDatabaseName = databaseName;
}

QSqlDatabase SQLiteEngine::database() { return mDatabase; }

void SQLiteEngine::setDatabase(const QSqlDatabase &database) {
    mDatabase = database;
}

bool SQLiteEngine::isOpen() const { return mIsOpen; }

void SQLiteEngine::setIsOpen(bool isOpen) { mIsOpen = isOpen; }

bool SQLiteEngine::isReadOnly() const { return mIsReadOnly; }

void SQLiteEngine::setIsReadOnly(bool isReadOnly) { mIsReadOnly = isReadOnly; }

bool SQLiteEngine::isValid() const { return mIsValid; }

void SQLiteEngine::setIsValid(bool n) { mIsValid = n; }

/*
 * sqlite_engine.cpp
 *
 * Summary: wrapper of sqlite.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-07-23
 * Last Modified: 2021-08-11
 *
 */

#include "sqlite_engine.h"

#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SQLiteEngine *SQLiteEngine::mInstance = nullptr;

SQLiteEngine::SQLiteEngine(QObject *parent) {
    this->mDatabaseFile = "";
}

SQLiteEngine *SQLiteEngine::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new SQLiteEngine(parent);
    return mInstance;
}

bool SQLiteEngine::open(const QString &db) {
    this->mDatabaseFile = db;
    if (this->mDatabaseFile.isEmpty()) {
        qDebug() << "database file is empty";
        this->setIsOpen(false);
        this->setIsValid(false);
        return false;
    }
    this->mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    this->mDatabase.setDatabaseName(this->mDatabaseFile);
    if (!this->mDatabase.open()) {
        qDebug() << "database open failed";
        this->setIsOpen(false);
        this->setIsValid(false);
        return false;
    }
    // qDebug() << "database open succeeded";
    return true;
}

void SQLiteEngine::close() {
    this->mDatabase.close();
}

void SQLiteEngine::createMediaList(const QString &tableName, const QList<Media> &mediaList) {
    if (this->isOpen() && this->isValid()) {
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
        QSqlQuery query(sql.join(" "), this->database());
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
    if (this->isOpen() && this->isValid()) {
        QStringList sql;
        sql << "DROP TABLE IF EXISTS :tableName";
        QSqlQuery query(sql.join(" "), this->database());
        return query.exec();
    } else {
        return false;
    }
}

bool SQLiteEngine::addMedia(const QString &tableName, const Media &media) {
    if (this->isOpen() && this->isValid()) {
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
        QSqlQuery query(sql.join(" "), this->database());
        return query.exec();
    } else {
        return false;
    }
}

bool SQLiteEngine::deleteMedia(const QString &tableName, const Media &media) {
    if (this->isOpen() && this->isValid()) {
        QStringList sql;
        sql << "DELETE FROM :tableName WHERE "
            << "url=:url;";
        QSqlQuery query(sql.join(" "), this->database());
        query.bindValue(":tableName", tableName);
        query.bindValue(":url", media.rawUrl());
        return query.exec();
    } else {
        return false;
    }
}

QList<Media> SQLiteEngine::getMediaList(const QString &tableName) {
    QList<Media> mediaList;
    if (this->isOpen() && this->isValid()) {
        QStringList sql;
        sql << "SELECT * FROM :tableName";
        QSqlQuery query(sql.join(" "), this->database());
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
    if (this->isOpen() && this->isValid()) {
        QStringList sql;
        sql << "SELECT * FROM :tableName WHERE "
            << "url=:url;";
        QSqlQuery query(sql.join(" "),this->database());
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
    if (this->isOpen() && this->isValid()) {
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
        QSqlQuery query(sql.join(" "), this->database());
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

bool SQLiteEngine::updateMediaList(const QString &tableName, const QList<Media> &mediaList) {
    bool res = true;
    if (this->isOpen() && this->isValid()) {
        for (const auto &media : mediaList) {
            res &= this->updateMedia(tableName, media);
        }
    }
    return res;
}

QStringList SQLiteEngine::getMediaLists() {
    QStringList mediaLists;
    if (this->isOpen() && this->isValid()) {
        QStringList sql;
        sql << "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
        QSqlQuery query(this->mDatabase);
        query.exec(sql.join(" "));
        while (query.next()) {
            mediaLists.append(query.value("name").toString());
        }
    }
    return mediaLists;
}

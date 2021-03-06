/**
 * @file sqlite_engine.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

#include "models/media.h"

class SQLiteEngine : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString databaseFile MEMBER mDatabaseFile READ databaseFile WRITE
                   setDatabaseFile)
    Q_PROPERTY(QString databaseName MEMBER mDatabaseName READ databaseName WRITE
                   setDatabaseName)
    Q_PROPERTY(
        QSqlDatabase database MEMBER mDatabase READ database WRITE setDatabase)
    Q_PROPERTY(bool isOpen MEMBER mIsOpen READ isOpen WRITE setIsOpen)
    Q_PROPERTY(
        bool isReadOnly MEMBER mIsReadOnly READ isReadOnly WRITE setIsReadOnly)
    Q_PROPERTY(bool isValid MEMBER mIsValid READ isValid WRITE setIsValid)

   private:
    QString mDatabaseFile;
    QString mDatabaseName;
    QSqlDatabase mDatabase;
    bool mIsOpen{};
    bool mIsReadOnly{};
    bool mIsValid{};

   public:
    explicit SQLiteEngine(QObject *parent);

    [[nodiscard]] QString databaseFile();

    void setDatabaseFile(const QString &n);

    [[nodiscard]] QString databaseName();

    void setDatabaseName(const QString &n);

    [[nodiscard]] QSqlDatabase database();

    void setDatabase(const QSqlDatabase &n);

    [[nodiscard]] bool isOpen() const;

    void setIsOpen(bool n);

    [[nodiscard]] bool isReadOnly() const;

    void setIsReadOnly(bool n);

    [[nodiscard]] bool isValid() const;

    void setIsValid(bool n);

   public slots:
    bool open(const QString &db);

    void close();

    void createMediaList(const QString &tableName,
                         const QList<Media> &mediaList = QList<Media>());

    bool dropMediaList(const QString &tableName);

    bool addMedia(const QString &tableName, const Media &media);

    bool deleteMedia(const QString &tableName, const Media &media);

    QList<Media> getMediaList(const QString &tableName);

    bool isMediaExist(const QString &tableName, const Media &media);

    bool updateMedia(const QString &tableName, const Media &media);

    bool updateMediaList(const QString &tableName,
                         const QList<Media> &mediaList);

    QStringList getMediaLists();
};

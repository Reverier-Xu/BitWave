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

   protected:
    explicit SQLiteEngine(QObject *parent);

    static SQLiteEngine *mInstance;

   public:
    static SQLiteEngine *instance(QObject *parent = nullptr);

    [[nodiscard]] QString databaseFile() { return this->mDatabaseFile; }

    void setDatabaseFile(const QString &n) { this->mDatabaseFile = n; }

    [[nodiscard]] QString databaseName() { return this->mDatabaseName; }

    void setDatabaseName(const QString &n) { this->mDatabaseName = n; }

    [[nodiscard]] QSqlDatabase database() { return this->mDatabase; }

    void setDatabase(const QSqlDatabase &n) { this->mDatabase = n; }

    [[nodiscard]] bool isOpen() const { return this->mIsOpen; }

    void setIsOpen(bool n) { this->mIsOpen = n; }

    [[nodiscard]] bool isReadOnly() const { return this->mIsReadOnly; }

    void setIsReadOnly(bool n) { this->mIsReadOnly = n; }

    [[nodiscard]] bool isValid() const { return this->mIsValid; }

    void setIsValid(bool n) { this->mIsValid = n; }

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

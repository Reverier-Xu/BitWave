//
// Created by Reverier-Xu on 2021/7/23.
//

#ifndef BITWAVE_SQLITE_ENGINE_H
#define BITWAVE_SQLITE_ENGINE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

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
    bool mIsOpen;
    bool mIsReadOnly;
    bool mIsValid;

   protected:
    explicit SQLiteEngine(QObject* parent);

    static SQLiteEngine* mInstance;

   public:
    static SQLiteEngine* getInstance(QObject* parent=nullptr);

    [[nodiscard]] QString databaseFile() { return this->mDatabaseFile; }
    void setDatabaseFile(const QString& n) { this->mDatabaseFile = n; }

    [[nodiscard]] QString databaseName() { return this->mDatabaseName; }
    void setDatabaseName(const QString& n) { this->mDatabaseName = n; }

    [[nodiscard]] QSqlDatabase database() { return this->mDatabase; }
    void setDatabase(QSqlDatabase n) { this->mDatabase = n; }

    [[nodiscard]] bool isOpen() { return this->mIsOpen; }
    void setIsOpen(bool n) { this->mIsOpen = n; }

    [[nodiscard]] bool isReadOnly() { return this->mIsReadOnly; }
    void setIsReadOnly(bool n) { this->mIsReadOnly = n; }

    [[nodiscard]] bool isValid() { return this->mIsValid; }
    void setIsValid(bool n) { this->mIsValid = n; }
};

#endif  // BITWAVE_SQLITE_ENGINE_H

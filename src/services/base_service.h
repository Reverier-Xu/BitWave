/**
 * @file base_service.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QMap>
#include <QObject>

#include "models/media.h"

class BaseService : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QString name MEMBER mName READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(
        QString icon MEMBER mIcon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(
        QStringList router READ router WRITE setRouter NOTIFY routerChanged)
    Q_PROPERTY(bool isEndpoint MEMBER mIsEndpoint WRITE setIsEndpoint NOTIFY
                   isEndpointChanged)
    Q_PROPERTY(bool hasStorage MEMBER mHasStorage READ hasStorage WRITE
                   hasStorageChanged)
    Q_PROPERTY(
        bool readOnly MEMBER mReadOnly READ readOnly WRITE readOnlyChanged)

   private:
    QString mName;
    QString mIcon;
    bool mIsEndpoint{};
    bool mHasStorage{};
    bool mReadOnly{};

    QMap<QString, BaseService *> mSubServices;

   public:
    explicit BaseService(BaseService *parent = nullptr) : QObject(parent) {}

    ~BaseService() override = default;

    bool registerSubService(BaseService *service);

    bool removeSubService(BaseService *service);

    BaseService *getSubService(const QStringList &router);

    [[nodiscard]] QString name() const { return this->mName; }

    void setName(const QString &name) {
        if (this->mName != name) {
            this->mName = name;
            emit this->nameChanged(this->mName);
        }
    }

    [[nodiscard]] QString icon() const { return this->mIcon; }

    void setIcon(const QString &icon) {
        if (this->mIcon != icon) {
            this->mIcon = icon;
            emit this->iconChanged(this->mIcon);
        }
    }

    [[nodiscard]] QStringList router() const {
        if (this->parent() != nullptr) {
            return QStringList()
                   << qobject_cast<BaseService *>(parent())->name()
                   << this->name();
        }
        return QStringList() << this->name();
    }

    void setRouter(const QStringList &router) {
        emit this->routerChanged(this->router());
    }

    [[nodiscard]] bool isEndpoint() const { return this->mIsEndpoint; }

    void setIsEndpoint(bool isEndpoint) {
        if (this->mIsEndpoint != isEndpoint) {
            this->mIsEndpoint = isEndpoint;
            emit this->isEndpointChanged(this->mIsEndpoint);
        }
    }

    [[nodiscard]] bool hasStorage() const { return this->mHasStorage; }

    void hasStorage(bool hasStorage) {
        if (this->mHasStorage != hasStorage) {
            this->mHasStorage = hasStorage;
            emit this->hasStorageChanged(this->mHasStorage);
        }
    }

    [[nodiscard]] bool readOnly() const { return this->mReadOnly; }

    void readOnly(bool readOnly) {
        if (this->mReadOnly != readOnly) {
            this->mReadOnly = readOnly;
            emit this->readOnlyChanged(this->mReadOnly);
        }
    }

   public slots:

    virtual void handleSearchRequest(const QString &keyword) = 0;

    virtual void handleCreateMediaRequest(const Media &data) = 0;

    virtual void handleUpdateMediaRequest(const Media &data) = 0;

    virtual void handleDeleteMediaRequest(const Media &data) = 0;

    virtual void handleGetContentRequest() = 0;

   signals:

    void nameChanged(const QString &name);

    void iconChanged(const QString &icon);

    void routerChanged(const QStringList &router);

    void isEndpointChanged(bool isEndpoint);

    void hasStorageChanged(bool hasStorage);

    void readOnlyChanged(bool readOnly);

    void contentChanged();

    void searchIsCompleted(bool ok, QList<Media> data);

    void createIsCompleted(bool ok);

    void updateIsCompleted(bool ok);

    void deleteIsCompleted(bool ok);

    void getContentMediaIsCompleted(bool ok, QList<Media> data);

    void getSubServicesIsCompleted(bool ok, QStringList routerNames);
};

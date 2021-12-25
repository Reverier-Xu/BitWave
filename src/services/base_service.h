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
    explicit BaseService(BaseService *parent = nullptr);

    ~BaseService() override = default;

    bool registerSubService(BaseService *service);

    bool removeSubService(BaseService *service);

    BaseService *getSubService(const QStringList &router);

    [[nodiscard]] QString name() const;

    void setName(const QString &name);

    [[nodiscard]] QString icon() const;

    void setIcon(const QString &icon);

    [[nodiscard]] QStringList router() const;

    void setRouter(const QStringList &router_);

    [[nodiscard]] bool isEndpoint() const;

    void setIsEndpoint(bool isEndpoint);

    [[nodiscard]] bool hasStorage() const;

    void hasStorage(bool hasStorage);

    [[nodiscard]] bool readOnly() const;

    void readOnly(bool readOnly);

    virtual void loadSettings() = 0;

    virtual void saveSettings() const = 0;

   public slots:

    virtual void handleQueryMediaRequest(const QString &keyword) = 0;

    virtual void handleCreateMediaRequest(const Media &data) = 0;

    virtual void handleUpdateMediaRequest(const Media &data) = 0;

    virtual void handleDeleteMediaRequest(const Media &data) = 0;

    virtual void handleSyncMediaListRequest() = 0;

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

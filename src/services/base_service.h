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

   private:
    QString mName;
    QString mIcon;

   public:
    explicit BaseService(BaseService *parent = nullptr);

    ~BaseService() override = default;

    [[nodiscard]] QString name() const;

    void setName(const QString &name);

    [[nodiscard]] QString icon() const;

    void setIcon(const QString &icon);

    virtual void loadSettings() = 0;

    virtual void saveSettings() const = 0;

   public slots:
    virtual void handleRefreshContentRequest(const QString &uri) = 0;

    virtual void handleCreatePageRequest(const QString &uri,
                                         const QString &page) = 0;

    virtual void handleDeletePageRequest(const QString &uri,
                                         const QString &page) = 0;

    virtual void handleSearchMediaRequest(const QString &keyword) = 0;

    virtual void handleCreateMediaRequest(const QString &uri,
                                          const Media &data) = 0;

    virtual void handleUpdateMediaRequest(const QString &uri,
                                          const Media &data) = 0;

    virtual void handleDeleteMediaRequest(const QString &uri,
                                          const Media &data) = 0;

    virtual void handleGetUriRequest(const QString &uri) = 0;

    virtual void getUriType(const QString &uri) = 0;

   signals:
    void nameChanged(const QString &name);

    void iconChanged(const QString &icon);

    void contentChanged();

    void searchIsCompleted(bool ok, QList<Media> data);

    // only for tips, user changes should emit contentChanged at the same time.
    void createIsCompleted(bool ok);

    void updateIsCompleted(bool ok);  // only for tips

    void deleteIsCompleted(bool ok);  // only for tips

    void getMediaIsCompleted(bool ok, QList<Media> data);

    void getSubPageIsComplated(bool ok, QList<QString> data);
};

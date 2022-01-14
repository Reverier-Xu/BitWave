/**
 * @file service_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QList>
#include <QMap>
#include <QObject>
#include <QStringList>

#include "models/media.h"
#include "models/ui/playlist_model.h"
#include "models/ui/service_list_model.h"
#include "services/base_service.h"

class ServiceManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool localSearchEnabled READ localSearchEnabled WRITE
                   setLocalSearchEnabled NOTIFY localSearchEnabledChanged)
    Q_PROPERTY(bool onlineSearchEnabled READ onlineSearchEnabled WRITE
                   setOnlineSearchEnabled NOTIFY onlineSearchEnabledChanged)
    Q_PROPERTY(bool musicSearchEnabled READ musicSearchEnabled WRITE
                   setMusicSearchEnabled NOTIFY musicSearchEnabledChanged)
    Q_PROPERTY(bool videoSearchEnabled READ videoSearchEnabled WRITE
                   setVideoSearchEnabled NOTIFY videoSearchEnabledChanged)
    Q_PROPERTY(QStringList currentUri READ currentUri WRITE setCurrentUri NOTIFY
                   currentUriChanged)
    Q_PROPERTY(bool currentUriIsEndpoint READ currentUriIsEndpoint WRITE
                   setCurrentUriIsEndpoint NOTIFY currentUriChanged)
    Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle NOTIFY
                   pageTitleChanged)
    Q_PROPERTY(
        QString pageIcon READ pageIcon WRITE setPageIcon NOTIFY pageIconChanged)
   private:
    bool mLocalSearchEnabled{true};
    bool mOnlineSearchEnabled{false};
    bool mMusicSearchEnabled{true};
    bool mVideoSearchEnabled{false};
    QStringList mCurrentUri{};
    bool mCurrentUriIsEndpoint{false};

    QList<Media> mMediaList{};
    QMap<QString, QString> mUriMap{};
    QMap<QString, BaseService *> mServiceMap{};
    PlaylistModel *mPlaylistModel{};
    ServiceListModel *mServiceListModel{};

   protected:
    explicit ServiceManager(QObject *parent);

    static ServiceManager *mInstance;

    void registerService(BaseService *service);

   public:
    [[nodiscard]] static ServiceManager *instance(QObject *parent);

    ~ServiceManager() override;

    void loadSettings();

    void saveSettings() const;

    [[nodiscard]] bool localSearchEnabled() const;

    void setLocalSearchEnabled(bool enabled);

    [[nodiscard]] bool onlineSearchEnabled() const;

    void setOnlineSearchEnabled(bool enabled);

    [[nodiscard]] bool musicSearchEnabled() const;

    void setMusicSearchEnabled(bool enabled);

    [[nodiscard]] bool videoSearchEnabled() const;

    void setVideoSearchEnabled(bool enabled);

    [[nodiscard]] QStringList currentUri() const;

    void setCurrentUri(const QStringList &uri);

    [[nodiscard]] bool currentUriIsEndpoint() const;

    void setCurrentUriIsEndpoint(bool isEndpoint);

    [[nodiscard]] QString pageTitle() const;

    void setPageTitle(const QString &title);

    [[nodiscard]] QString pageIcon() const;

    void setPageIcon(const QString &icon);

    [[nodiscard]] PlaylistModel *playlistModel() const;

    [[nodiscard]] ServiceListModel *serviceListModel() const;

   public slots:
    Q_INVOKABLE void search(const QString &input);

    Q_INVOKABLE void visit(const QStringList &uri);

    Q_INVOKABLE void enter(const QString &name);

    Q_INVOKABLE void refresh();

    Q_INVOKABLE void back();

    Q_INVOKABLE void play(int id);

   signals:
    void localSearchEnabledChanged(bool enabled);

    void onlineSearchEnabledChanged(bool enabled);

    void musicSearchEnabledChanged(bool enabled);

    void videoSearchEnabledChanged(bool enabled);

    void currentUriChanged(const QStringList &uri);

    void currentUriIsEndpointChanged(bool isEndpoint);

    void pageTitleChanged(const QString &title);

    void pageIconChanged(const QString &icon);

    void contentChanged();
};

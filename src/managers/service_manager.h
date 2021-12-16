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

#include <QMap>
#include <QObject>

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
   private:
    BaseService *mRootService{};
    bool mLocalSearchEnabled{true};
    bool mOnlineSearchEnabled{false};
    bool mMusicSearchEnabled{true};
    bool mVideoSearchEnabled{false};

   protected:
    explicit ServiceManager(QObject *parent);

    static ServiceManager *mInstance;

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

    [[nodiscard]] BaseService *rootService() const;

   public slots:
    Q_INVOKABLE void search(const QString &input);

    signals:
    void localSearchEnabledChanged(bool enabled);

    void onlineSearchEnabledChanged(bool enabled);

    void musicSearchEnabledChanged(bool enabled);

    void videoSearchEnabledChanged(bool enabled);
};

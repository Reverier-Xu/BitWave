/**
 * @file service_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "service_manager.h"

ServiceManager *ServiceManager::mInstance = nullptr;

ServiceManager *ServiceManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new ServiceManager(parent);
    return mInstance;
}

ServiceManager::ServiceManager(QObject *parent) : QObject(parent) {
    loadSettings();
}

void ServiceManager::loadSettings() {}

void ServiceManager::saveSettings() const {}

ServiceManager::~ServiceManager() { saveSettings(); }

void ServiceManager::search(const QString &input) {

}

void ServiceManager::visit(const QString &uri) {

}

bool ServiceManager::localSearchEnabled() const { return mLocalSearchEnabled; }

void ServiceManager::setLocalSearchEnabled(bool localSearchEnabled) {
    mLocalSearchEnabled = localSearchEnabled;
    emit localSearchEnabledChanged(mLocalSearchEnabled);
}

bool ServiceManager::onlineSearchEnabled() const { return mOnlineSearchEnabled; }

void ServiceManager::setOnlineSearchEnabled(bool onlineSearchEnabled) {
    mOnlineSearchEnabled = onlineSearchEnabled;
    emit onlineSearchEnabledChanged(mOnlineSearchEnabled);
}

bool ServiceManager::musicSearchEnabled() const { return mMusicSearchEnabled; }

void ServiceManager::setMusicSearchEnabled(bool musicSearchEnabled) {
    mMusicSearchEnabled = musicSearchEnabled;
    emit musicSearchEnabledChanged(mMusicSearchEnabled);
}

bool ServiceManager::videoSearchEnabled() const { return mVideoSearchEnabled; }

void ServiceManager::setVideoSearchEnabled(bool videoSearchEnabled) {
    mVideoSearchEnabled = videoSearchEnabled;
    emit videoSearchEnabledChanged(mVideoSearchEnabled);
}

QStringList ServiceManager::currentUri() const { return mCurrentUri; }

void ServiceManager::setCurrentUri(const QStringList &currentUri) {
    mCurrentUri = currentUri;
    emit currentUriChanged(mCurrentUri);
}

bool ServiceManager::currentUriIsEndpoint() const { return mCurrentUriIsEndpoint; }

void ServiceManager::setCurrentUriIsEndpoint(bool currentUriIsEndpoint) {
    mCurrentUriIsEndpoint = currentUriIsEndpoint;
    emit currentUriIsEndpointChanged(mCurrentUriIsEndpoint);
}

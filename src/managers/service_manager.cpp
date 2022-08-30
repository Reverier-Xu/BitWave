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

#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "queue_manager.h"
#include "services/local_music_service.h"

ServiceManager *ServiceManager::mInstance = nullptr;

ServiceManager *ServiceManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new ServiceManager(parent);
    return mInstance;
}

ServiceManager::ServiceManager(QObject *parent) : QObject(parent) {
    mPlaylistModel = new PlaylistModel(this);
    mPlaylistModel->setMediaList(&mMediaList);
    mPlaylistModel->setUriMap(&mUriMap);

    auto localMusicService = new LocalMusicService(this);
    registerService(localMusicService);

    mServiceListModel = new ServiceListModel(this);
    mServiceListModel->setServiceMap(&mServiceMap);

    loadSettings();
}

void ServiceManager::registerService(BaseService *service) {
    mServiceMap.insert(service->name(), service);
    connect(service, &BaseService::nameChanged, this,
            [=](const QString &name) {});
    connect(service, &BaseService::iconChanged, this,
            [=](const QString &icon) {});
    connect(service, &BaseService::contentChanged, this, [=]() {
        visit(currentUri());
        auto res = QtConcurrent::run([=]() { QThread::sleep(1); });
        auto resWatcher = new QFutureWatcher<void>();
        resWatcher->setFuture(res);
        connect(resWatcher, &QFutureWatcher<void>::finished, this, [=]() {
            emit contentChanged();
            resWatcher->deleteLater();
        });
    });
    connect(service, &BaseService::getMediaIsCompleted, this,
            [=](bool ok, const QList<Media> &mediaList) {
                mMediaList = mediaList;
                // qDebug() << "media list: " << mMediaList.size();
                playlistModel()->reload();
            });
    connect(service, &BaseService::getSubPageIsCompleted, this,
            [=](bool ok, const QMap<QString, QString> &uri) {
                mUriMap = uri;
                // qDebug() << "uri: " << uri;
                playlistModel()->reload();
            });
}

void ServiceManager::loadSettings() {
    for (auto service : mServiceMap) {
        service->loadSettings();
    }
}

void ServiceManager::saveSettings() const {
    for (auto service : mServiceMap) {
        service->saveSettings();
    }
}

ServiceManager::~ServiceManager() { saveSettings(); }

void ServiceManager::search(const QString &input) {}

void ServiceManager::visit(const QStringList &uri) {
    setCurrentUri(uri);
    auto service = mServiceMap.value(uri.first());
    setCurrentUriIsEndpoint(service->getUriType(uri) ==
                            ServiceUriType::Playlist);
    if (service) service->handleGetUriRequest(uri);
}

bool ServiceManager::localSearchEnabled() const { return mLocalSearchEnabled; }

void ServiceManager::setLocalSearchEnabled(bool localSearchEnabled) {
    mLocalSearchEnabled = localSearchEnabled;
    emit localSearchEnabledChanged(mLocalSearchEnabled);
}

bool ServiceManager::onlineSearchEnabled() const {
    return mOnlineSearchEnabled;
}

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
    emit pageTitleChanged(pageTitle());
    emit pageIconChanged(pageIcon());
    emit currentUriChanged(mCurrentUri);
}

bool ServiceManager::currentUriIsEndpoint() const {
    return mCurrentUriIsEndpoint;
}

void ServiceManager::setCurrentUriIsEndpoint(bool currentUriIsEndpoint) {
    mCurrentUriIsEndpoint = currentUriIsEndpoint;
    playlistModel()->setIsEndpoint(currentUriIsEndpoint);
    emit currentUriIsEndpointChanged(mCurrentUriIsEndpoint);
}

QString ServiceManager::pageTitle() const {
    if (currentUri().isEmpty()) return tr("Playlist");
    return currentUri().first();
}

void ServiceManager::setPageTitle(const QString &title) {
    emit pageTitleChanged(pageTitle());
}

QString ServiceManager::pageIcon() const {
    if (currentUri().isEmpty()) return "qrc:/assets/favorites.svg";
    return mServiceMap.value(currentUri().first())->icon();
}

void ServiceManager::setPageIcon(const QString &icon) {
    emit pageIconChanged(pageIcon());
}

void ServiceManager::enter(const QString &name) {
    QStringList uri = currentUri();
    uri.append(name);
    visit(uri);
}

PlaylistModel *ServiceManager::playlistModel() const {
    return mPlaylistModel;
}

ServiceListModel *ServiceManager::serviceListModel() const {
    return mServiceListModel;
}

void ServiceManager::refresh() {
    auto service = mServiceMap.value(currentUri().first());
    service->handleRefreshContentRequest(currentUri());
}

void ServiceManager::back() {
    QStringList uri = currentUri();
    uri.removeLast();
    visit(uri);
}

void ServiceManager::play(int id) {
    QueueManager::instance()->playPlaylist(mMediaList, id);
}

QString ServiceManager::filterString() const {
    return mFilterString;
}

void ServiceManager::setFilterString(const QString &filterString) {
    mFilterString = filterString;
    emit filterStringChanged(mFilterString);
}

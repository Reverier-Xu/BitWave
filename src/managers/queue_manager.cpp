/**
 * @file queue_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "queue_manager.h"

#include <qglobal.h>

#ifdef Q_OS_LINUX

#include <malloc.h>

#endif

// #include <QDebug>
#include <QRandomGenerator>
#include <QSettings>

#include "engines/sqlite_engine.h"
#include "parser_manager.h"
#include "player_manager.h"

QueueManager *QueueManager::mInstance = nullptr;

QueueManager::QueueManager(QObject *parent) : QObject(parent) {
    mQueueModel = new MediaQueueModel(this);
    mQueueModel->setMediaQueue(&mMainQueue);
    // qDebug() << mQueueModel->rowCount(QModelIndex());

    mSqliteEngine = new SQLiteEngine(this->parent());
    mSqliteEngine->open(
        QStandardPaths::writableLocation(QStandardPaths::DataLocation) +
        "/Database/playqueue.db");
    loadSettings();
    connectSignals();
}

QueueManager::~QueueManager() {
    saveSettings();
    mSqliteEngine->close();
    mSqliteEngine->deleteLater();
}

QueueManager *QueueManager::instance(QObject *parent) {
    if (mInstance == nullptr) {
        mInstance = new QueueManager(parent);
    }
    return mInstance;
}

void QueueManager::changeMode() { setPlayMode((playMode() + 1) % 5); }

void QueueManager::addMedia(const Media &media) {
    switch (addMediaMode()) {
        case 0:
            addMediaAtHead(media);
            break;
        case 1:
            addMediaAtNext(media);
            break;
        case 2:
            addMediaAtTail(media);
            break;
        default:
            addMediaAtHead(media);
    }
}

void QueueManager::addMediaAtHead(const Media &media) {
    if (queuePos() == -1) {
        mQueuePos = 0;
    }
    mQueueModel->beginInsertMedia(queuePos());
    mMainQueue.insert(queuePos(), media);
    mUserSwitch = true;
    setQueuePos(queuePos());  // playing the new media now.
    mQueueModel->endInsertMedia();
    emit mediaQueueChanged();
}

void QueueManager::addMediaAtNext(const Media &media) {
    // not support at random mode.
    mQueueModel->beginInsertMedia(queuePos() + 1);
    mMainQueue.insert(queuePos() + 1, media);
    mQueueModel->endInsertMedia();
    emit mediaQueueChanged();
}

void QueueManager::addMediaAtTail(const Media &media) {
    mQueueModel->beginInsertMedia(mainQueue().count());
    mMainQueue.append(media);
    mQueueModel->endInsertMedia();
    emit mediaQueueChanged();
}

void QueueManager::moveMedia(int index, int offset) {
    // TODO: model has not impl move actions.
    if (index < queuePos() && index + offset >= queuePos())
        mQueuePos--;
    else if (index > queuePos() && index + offset <= queuePos())
        mQueuePos++;
    else if (index == queuePos())
        mQueuePos = index + offset;
    mMainQueue.move(index, index + offset);
}

void QueueManager::removeMedia(int removed) {
    mQueueModel->beginRemoveMedia(removed);

    if (removed < queuePos()) mQueuePos--;

    mMainQueue.removeAt(removed);

    mQueueModel->endRemoveMedia();
    emit mediaQueueChanged();

    if (!mMainQueue.count()) {  // media is the last one in the queue.
        mQueueEnded = true;
        emit playQueueEnded();
        PlayerManager::instance(this->parent())->resetPlayer();
        emit showTips("qrc:/assets/current.svg", tr("Finished"));
    } else if (removed == queuePos()) {
        userNextRequested();
    }
}

void QueueManager::next() {
#ifdef Q_OS_LINUX
    malloc_trim(0);  // free memories.
#endif
    if (mMainQueue.empty()) {
        PlayerManager::instance(this->parent())->resetPlayer();
        return;
    }
    if (queuePos() <= mMainQueue.size() - 1 && queuePos() > -1)
        mHistoryStack.push(queuePos());
    int next_pos;
    switch (playMode()) {
        case 0:
            next_pos = queuePos() + 1;  // repeat all
            next_pos = next_pos % mMainQueue.count();
            break;
        case 1:
            next_pos = queuePos();  // repeat one
            break;
        case 2:
            do {
                next_pos = QRandomGenerator::global()->bounded(
                    0, mMainQueue.count());  // random
            } while (next_pos == queuePos() && mMainQueue.count() > 1);
            break;
        case 3:
            next_pos = queuePos() + 1;  // order
            if (next_pos >= mMainQueue.count()) {
                next_pos = -1;
                mQueueEnded = true;
                emit playQueueEnded();
                emit showTips("qrc:/assets/current.svg", tr("Finished"));
            }
            break;
        case 4:
            next_pos = queuePos() - 1;  // reverse
            if (next_pos < 0) {
                next_pos = -1;
                mQueueEnded = true;
                emit playQueueEnded();
                emit showTips("qrc:/assets/current.svg", tr("Finished"));
            }
            break;
        default:
            next_pos = -1;
            break;
    }
    setQueuePos(next_pos);
}

void QueueManager::previous() {
    if (mHistoryStack.count() > 0 && mHistoryStack.top() < mMainQueue.count() &&
        mHistoryStack.top() >= 0) {
        setQueuePos(mHistoryStack.top());
        mHistoryStack.pop();
    } else {
        next();
    }
}

void QueueManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("PlayQueue");
    setAddMediaMode(settings.value("AddMediaMode", 0).toInt());
    setPlayMode(settings.value("PlayMode", 0).toInt());
    settings.endGroup();

    auto media_list = mSqliteEngine->getMediaLists();
    // qDebug() << media_list;
    if (media_list.contains("cached")) {
        auto media_list_cached = mSqliteEngine->getMediaList("cached");
        // qDebug() << media_list_cached.count();
        for (auto media : media_list_cached) {
            addMediaAtTail(media);
        }
    }
}

void QueueManager::saveSettings() const {
    QSettings settings;
    settings.beginGroup("PlayQueue");
    settings.setValue("AddMediaMode", addMediaMode());
    settings.setValue("PlayMode", playMode());
    settings.endGroup();
    settings.sync();
    mSqliteEngine->dropMediaList("cached");
    mSqliteEngine->createMediaList("cached", mMainQueue);
}

void QueueManager::connectSignals() {
    connect(PlayerManager::instance(this->parent()), &PlayerManager::playEnded,
            this, [=]() {
                if (!checkUserSwitch()) {
                    next();
                }
            });
    connect(ParserManager::instance(this->parent()),
            &ParserManager::mediaInfoIsReady, this,
            [=](bool ok, const Media &media) {
                if (ok) {
                    addMediaAtHead(media);
                } else {
                    emit showTips("qrc:/assets/warning.svg", tr("Play Failed"));
                }
            });
    connect(this, &QueueManager::externMediaInfoRequested,
            ParserManager::instance(this->parent()),
            &ParserManager::handleGetMediaInfoRequest);
}

void QueueManager::clearQueue() {
    mMainQueue.clear();
    clearHistory();
    setQueuePos(-1);
    mQueueEnded = false;
    emit playQueueEnded();
    emit mediaQueueChanged();
    mQueueModel->reloadQueue(&mMainQueue);
    PlayerManager::instance(this->parent())->resetPlayer();
}

void QueueManager::clearHistory() { mHistoryStack.clear(); }

void QueueManager::userNextRequested() {
    if (PlayerManager::instance()->isMediaLoaded()) {
        mUserSwitch = true;
    }
    next();
}

void QueueManager::userPreviousRequested() {
    if (PlayerManager::instance()->isMediaLoaded()) {
        mUserSwitch = true;
    }
    previous();
}

void QueueManager::userSwitchRequested(int id) {
    if (PlayerManager::instance()->isMediaLoaded()) {
        mUserSwitch = true;
    }
    setQueuePos(id);
}

int QueueManager::playMode() const { return mPlayMode; }

void QueueManager::setPlayMode(int mode) {
    mPlayMode = mode;
    emit playModeChanged(mode);
    emit playModeIconChanged(playModeIcon());
}

QUrl QueueManager::playModeIcon() const {
    switch (playMode()) {
        case 1:
            return {"qrc:/assets/play-repeat-one.svg"};  // repeat one
        case 2:
            return {"qrc:/assets/play-random.svg"};  // random
        case 3:
            return {"qrc:/assets/play-order.svg"};  // order
        case 4:
            return {"qrc:/assets/play-reverse.svg"};  // reverse
        case 0:
        default:
            return {"qrc:/assets/play-repeat-all.svg"};
    }
}

const QList<Media> &QueueManager::mainQueue() const { return mMainQueue; }

MediaQueueModel *QueueManager::getQueueModel() const { return mQueueModel; }

void QueueManager::setPlayModeIcon(const QUrl &icon) {
    emit playModeIconChanged(playModeIcon());
}

QString QueueManager::playModeName() const {
    switch (playMode()) {
        case 0:
            return tr("Repeat All");
        case 1:
            return tr("Repeat One");
        case 2:
            return tr("Random");
        case 3:
            return tr("In Order");
        case 4:
            return tr("Reverse");
        default:
            return tr("Repeat All");
    }
}

void QueueManager::setPlayModeName(const QString &name) {
    emit playModeNameChanged(playModeName());
}

int QueueManager::addMediaMode() const { return mAddMediaMode; }

void QueueManager::setAddMediaMode(int n) {
    mAddMediaMode = n;
    emit addMediaModeChanged(n);
}

int QueueManager::queuePos() const { return mQueuePos; }

void QueueManager::setQueuePos(int n) {
    mQueuePos = n;
    emit queuePosChanged(n);  // notify the view
    if (!mMainQueue.empty() && queuePos() > -1) {
        auto media = mMainQueue.at(n);
        setCurrentMedia(media);
    } else if (queuePos() == -1) {
        PlayerManager::instance(this->parent())->resetPlayer();
    }
}

bool QueueManager::queueEnded() const { return mQueueEnded; }

Media QueueManager::currentMedia() const { return mCurrentMedia; }

bool QueueManager::checkUserSwitch() {
    bool res = mUserSwitch;
    mUserSwitch = false;
    return res;
}

void QueueManager::setCurrentMedia(const Media &m) {
    mCurrentMedia = m;
    PlayerManager::instance(this->parent())->play(m);
    emit currentMediaChanged(m);
}

void QueueManager::addExternMedia(const QString &path) {
    emit externMediaInfoRequested(path);
}

void QueueManager::loadPlaylist(const QList<Media> &mediaList) {
    clearQueue();
    mMainQueue = mediaList;
    mQueueModel->reloadQueue(&mMainQueue);
}

void QueueManager::playPlaylist(const QList<Media> &mediaList, int id) {
    loadPlaylist(mediaList);
    userSwitchRequested(id);
}

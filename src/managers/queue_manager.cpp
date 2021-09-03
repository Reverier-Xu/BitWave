#include "queue_manager.h"
#include <qglobal.h>

#ifdef Q_OS_LINUX

#include <malloc.h>

#endif

// #include <QDebug>
#include <QRandomGenerator>
#include <QSettings>

#include "parser_manager.h"

QueueManager *QueueManager::mInstance = nullptr;

QueueManager::QueueManager(QObject *parent)
        : QObject(parent) {
    this->mQueueModel = new MediaQueueModel(this);
    this->mQueueModel->setMediaQueue(&this->mMainQueue);
    // qDebug() << this->mQueueModel->rowCount(QModelIndex());
    this->loadSettings();
    this->connectSignals();
}

QueueManager::~QueueManager() {
    this->saveSettings();
}

QueueManager *QueueManager::instance(QObject *parent) {
    if (mInstance == nullptr) {
        mInstance = new QueueManager(parent);
    }
    return mInstance;
}

void QueueManager::changeMode() {
    this->setPlayMode((this->playMode() + 1) % 5);
}

void QueueManager::addMedia(const Media &media) {
    switch (this->addMediaMode()) {
        case 0:
            this->addMediaAtHead(media);
            break;
        case 1:
            this->addMediaAtNext(media);
            break;
        case 2:
            this->addMediaAtTail(media);
            break;
        default:
            this->addMediaAtHead(media);
    }
}

void QueueManager::addMediaAtHead(const Media &media) {
    this->mQueueModel->beginInsertMedia(this->queuePos());
    this->mMainQueue.insert(this->queuePos(), media);
    this->setQueuePos(this->queuePos()); // playing the new media now.
    this->mQueueModel->endInsertMedia();
    emit this->mediaQueueChanged();
}

void QueueManager::addMediaAtNext(const Media &media) { // not support at random mode.
    this->mQueueModel->beginInsertMedia(this->queuePos()+1);
    this->mMainQueue.insert(this->queuePos() + 1, media);
    this->mQueueModel->endInsertMedia();
    emit this->mediaQueueChanged();
}

void QueueManager::addMediaAtTail(const Media &media) {
    this->mQueueModel->beginInsertMedia(this->mainQueue().count());
    this->mMainQueue.enqueue(media);
    this->mQueueModel->endInsertMedia();
    emit this->mediaQueueChanged();
}

void QueueManager::moveMedia(int index, int offset) {
    // TODO: model has not impl move actions.
    if (index < this->queuePos() and index + offset >= this->queuePos())
        this->mQueuePos--;
    else if (index > this->queuePos() and index + offset <= this->queuePos())
        this->mQueuePos++;
    else if (index == this->queuePos())
        this->mQueuePos = index + offset;
    this->mMainQueue.move(index, index + offset);
}

void QueueManager::playExternMedia(const QString &path) {
    // qDebug() << "Extern Media Requested: " << path;
    // MemoryHelper::assertMemory("QueueManager::playExternMedia");
    // this->setPlayMode(3);
#ifdef Q_OS_LINUX
    malloc_trim(0); // free memories.
#endif
    emit this->playExternMediaRequested(path);
}

void QueueManager::removeMedia(int index) {
    this->mQueueModel->beginRemoveMedia(index);
    this->mMainQueue.removeAt(index);

    if (index < this->queuePos()) {
        this->mQueuePos -= 1; // do not disturb playing.
    } else if (index == this->queuePos() and index < this->mMainQueue.count()) {
        this->setQueuePos(index); // stop current media and playing next one.
    } else if (index > this->mMainQueue.count()) { // media is the last one in the queue.
        this->mQueueEnded = true;
        emit this->playQueueEnded();
        emit this->showTips("qrc:/assets/current.svg", tr("Finished"));
    }

    this->mQueueModel->endRemoveMedia();
    emit this->mediaQueueChanged();
}

void QueueManager::next() {
    // qDebug() << "Next Called.";
    // MemoryHelper::assertMemory("QueueManager::next Begin");
#ifdef Q_OS_LINUX
    malloc_trim(0); // free memories.
#endif
    if (this->mMainQueue.empty())
        return;
    if (this->mQueueEnded) {
        // qDebug() << "Queue is ended, reboot.";
        this->mQueueEnded = false;
        this->setQueuePos(0);
        return;
    }
    if (this->queuePos() <= this->mMainQueue.size() - 1 && this->queuePos() > -1)
        this->mHistoryStack.push(this->queuePos());
    if (this->queuePos() < -1)
        return;

    int next_pos;
    switch (this->playMode()) {
        case 0:
            next_pos = this->queuePos() + 1; // repeat all
            next_pos = next_pos % this->mMainQueue.count();
            break;
        case 1:
            next_pos = this->queuePos(); // repeat one
            if (next_pos == -1)
                next_pos = 0;
            break;
        case 2:
            do {
                next_pos = QRandomGenerator::global()->bounded(
                        0, this->mMainQueue.count() - 1); // random
            } while (next_pos == this->queuePos() and this->mMainQueue.count() > 1);
            break;
        case 3:
            next_pos = this->queuePos() + 1; // order
            break;
        case 4:
            next_pos = this->queuePos() - 1; // reverse
            break;
        default:
            next_pos = -1;
            break;
    }
    if (next_pos >= 0 and next_pos < this->mMainQueue.count()) {
        // qDebug() << "Next: " << next_pos;
        this->setQueuePos(next_pos);
    } else {
        // qDebug() << "Next failed: " << this->queuePos() << " " <<
        // this->mMainQueue.count(); this->setQueuePos(-1); this->clearQueue();
        this->mQueueEnded = true;
        emit this->playQueueEnded();
        emit this->showTips("qrc:/assets/current.svg", tr("Finished"));
    }
    // MemoryHelper::assertMemory("QueueManager::next End");
}

void QueueManager::previous() {
    if (this->mHistoryStack.count() > 0 and this->mHistoryStack.top() < this->mMainQueue.count() and
        this->mHistoryStack.top() >= 0) {
        this->setQueuePos(this->mHistoryStack.top());
        this->mHistoryStack.pop();
    } else {
        this->next();
    }
}

void QueueManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("PlayQueue");
    this->setAddMediaMode(settings.value("AddMediaMode", 0).toInt());
    this->setPlayMode(settings.value("PlayMode", 0).toInt());
    settings.endGroup();
}

void QueueManager::saveSettings() const {
    QSettings settings;
    settings.beginGroup("PlayQueue");
    settings.setValue("AddMediaMode", this->addMediaMode());
    settings.setValue("PlayMode", this->playMode());
    settings.endGroup();
    settings.sync();
}

void QueueManager::handleExternMediaInfoIsReady(bool ok, const Media &media) {
    // MemoryHelper::assertMemory("QueueManager::handleExternMediaInfoIsReady
    // Begin"); this->clearQueue();
    if (!ok) {
        emit this->showTips("qrc:/assets/warning.svg", tr("Open Failed"));
        return;
    }
    this->mQueueModel->beginInsertMedia(this->queuePos());
    this->mMainQueue.insert(this->queuePos(), media);
    this->mQueueModel->endInsertMedia();
    this->setQueuePos(this->queuePos());
    // MemoryHelper::assertMemory("QueueManager::handleExternMediaInfoIsReady
    // End");
}

void QueueManager::connectSignals() const {
    connect(ParserManager::instance(),
            &ParserManager::externMediaInfoIsReady,
            this,
            &QueueManager::handleExternMediaInfoIsReady);
    connect(this,
            &QueueManager::playExternMediaRequested,
            ParserManager::instance(),
            &ParserManager::handleGetExternMediaInfoRequest);
}

void QueueManager::clearQueue() {
    this->mMainQueue.clear();
    this->clearHistory();
    this->setQueuePos(-1);
    emit this->mediaQueueChanged();
    this->mQueueModel->reloadQueue();
}

void QueueManager::clearHistory() {
    this->mHistoryStack.clear();
}

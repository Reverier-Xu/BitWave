//
// Created by Reverier-Xu on 2021/8/7.
//

#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QQueue>
#include <QStack>

#include "models/media.h"
#include "base_manager.h"
#include "utilities/memory_helper.h"

class QueueManager : public BaseManager {
Q_OBJECT
    Q_PROPERTY(int playMode MEMBER mPlayMode READ playMode WRITE setPlayMode
                       NOTIFY playModeChanged)
    Q_PROPERTY(QUrl playModeIcon READ playModeIcon WRITE setPlayModeIcon NOTIFY
                       playModeIconChanged)
    Q_PROPERTY(int addMediaMode MEMBER mAddMediaMode READ addMediaMode WRITE setAddMediaMode NOTIFY
                       addMediaModeChanged)
    Q_PROPERTY(unsigned int queuePos MEMBER mQueuePos READ queuePos WRITE setQueuePos NOTIFY queuePosChanged)
    Q_PROPERTY(QString playModeName READ playModeName WRITE setPlayModeName NOTIFY playModeNameChanged)
    Q_PROPERTY(Media currentMedia READ currentMedia WRITE setCurrentMedia NOTIFY currentMediaChanged)
private:
    Media mCurrentMedia{};
    int mPlayMode = 0;
    int mQueuePos = 0;
    QQueue<Media> mMainQueue;
    QStack<int> mHistoryStack;
    int mAddMediaMode = 0;
    bool mQueueEnded = false;

protected:
    static QueueManager *mInstance;

    explicit QueueManager(QObject *parent);

    void connectSignals() const;

    ~QueueManager() override;

public:
    static QueueManager *instance(QObject *parent = nullptr);

    void loadSettings() override;

    void saveSettings() override;

    [[nodiscard]] int playMode() const { return this->mPlayMode; };

    void setPlayMode(int mode) {
        this->mPlayMode = mode;
        emit this->playModeChanged(mode);
        emit this->playModeIconChanged(this->playModeIcon());
    }

    [[nodiscard]] QUrl playModeIcon() const {
        switch (this->playMode()) {
            case 0:
                return QUrl("qrc:/assets/play-repeat-all.svg");  // repeat all
            case 1:
                return QUrl("qrc:/assets/play-repeat-one.svg");  // repeat one
            case 2:
                return QUrl("qrc:/assets/play-random.svg");  // random
            case 3:
                return QUrl("qrc:/assets/play-order.svg");  // order
            case 4:
                return QUrl("qrc:/assets/play-reverse.svg");  // reverse
            default:
                return QUrl("qrc:/assets/play-repeat-all.svg");
        }
    }

    [[nodiscard]] const QQueue<Media> &mainQueue() const { return this->mMainQueue; }

    void setPlayModeIcon(const QUrl &icon) {
        emit this->playModeIconChanged(this->playModeIcon());
    }

    [[nodiscard]] QString playModeName() const {
        switch (this->playMode()) {
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

    void setPlayModeName(const QString &name) {
        emit this->playModeNameChanged(this->playModeName());
    }

    [[nodiscard]] int addMediaMode() const { return this->mAddMediaMode; }

    void setAddMediaMode(int n) {
        this->mAddMediaMode = n;
        emit this->addMediaModeChanged(n);
    }

    [[nodiscard]] int queuePos() const { return this->mQueuePos; }

    void setQueuePos(int n) {
        this->mQueuePos = n;
        emit this->queuePosChanged(n);
        if (!this->mMainQueue.empty() and this->queuePos() > -1) {
            auto media = this->mMainQueue.at(n);
            this->setCurrentMedia(media);
        }
    }

    [[nodiscard]] bool queueEnded() const { return this->mQueueEnded; }

    [[nodiscard]] Media currentMedia() const { return this->mCurrentMedia; }

    void setCurrentMedia(const Media& m) {
        this->mCurrentMedia = m;
        emit currentMediaChanged(m);
    }

public slots:
    Q_INVOKABLE void changeMode();

    Q_INVOKABLE void addMedia(const Media &media);

    Q_INVOKABLE void addMediaAtHead(const Media &media);

    Q_INVOKABLE void addMediaAtNext(const Media &media);

    Q_INVOKABLE void addMediaAtTail(const Media &media);

    Q_INVOKABLE void playExternMedia(const QString &path);

    Q_INVOKABLE void handleExternMediaInfoIsReady(bool ok, const Media &media);

    Q_INVOKABLE void removeMedia(int index);

    Q_INVOKABLE void next();

    Q_INVOKABLE void previous();

    Q_INVOKABLE void clearQueue();

    Q_INVOKABLE void clearHistory();

signals:

    void playModeChanged(int mode);

    void playModeIconChanged(QUrl icon);

    void currentMediaChanged(const Media &media);

    void addMediaModeChanged(int n);

    void mediaQueueChanged();

    void queuePosChanged(int n);

    void playModeNameChanged(const QString& name);

    void playExternMediaRequested(const QString& path);

    void playQueueEnded();
};

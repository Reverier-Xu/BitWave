/**
 * @file queue_manager.h
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
#include <QObject>
#include <QQueue>
#include <QStack>
#include <QString>
#include <QUrl>

#include "models/media.h"
#include "models/ui/media_queue_model.h"
#include "utilities/memory_helper.h"

class QueueManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int playMode MEMBER mPlayMode READ playMode WRITE setPlayMode
                   NOTIFY playModeChanged)
    Q_PROPERTY(QUrl playModeIcon READ playModeIcon WRITE setPlayModeIcon NOTIFY
                   playModeIconChanged)
    Q_PROPERTY(int addMediaMode MEMBER mAddMediaMode READ addMediaMode WRITE
                   setAddMediaMode NOTIFY addMediaModeChanged)
    Q_PROPERTY(int queuePos MEMBER mQueuePos READ queuePos WRITE setQueuePos
                   NOTIFY queuePosChanged)
    Q_PROPERTY(QString playModeName READ playModeName WRITE setPlayModeName
                   NOTIFY playModeNameChanged)
    Q_PROPERTY(Media currentMedia READ currentMedia WRITE setCurrentMedia NOTIFY
                   currentMediaChanged)
   private:
    Media mCurrentMedia{};
    int mPlayMode = 0;
    int mQueuePos = 0;
    QQueue<Media> mMainQueue;
    QStack<int> mHistoryStack;
    int mAddMediaMode = 0;
    bool mQueueEnded = false;
    bool mUserSwitch = false;
    MediaQueueModel *mQueueModel;

   protected:
    static QueueManager *mInstance;

    explicit QueueManager(QObject *parent);

    void connectSignals();

    ~QueueManager() override;

   public:
    static QueueManager *instance(QObject *parent = nullptr);

    void loadSettings();

    void saveSettings() const;

    [[nodiscard]] int playMode() const;
    ;

    void setPlayMode(int mode);

    [[nodiscard]] QUrl playModeIcon() const;

    [[nodiscard]] const QQueue<Media> &mainQueue() const;

    [[nodiscard]] MediaQueueModel *getQueueModel() const;

    void setPlayModeIcon(const QUrl &icon);

    [[nodiscard]] QString playModeName() const;

    void setPlayModeName(const QString &name);

    [[nodiscard]] int addMediaMode() const;

    void setAddMediaMode(int n);

    [[nodiscard]] int queuePos() const;

    void setQueuePos(int n);

    [[nodiscard]] bool queueEnded() const;

    [[nodiscard]] Media currentMedia() const;

    [[nodiscard]] bool checkUserSwitch();

    void setCurrentMedia(const Media &m);

   public slots:
    Q_INVOKABLE void changeMode();

    Q_INVOKABLE void addMedia(const Media &media);

    Q_INVOKABLE void addMediaAtHead(const Media &media);

    Q_INVOKABLE void addMediaAtNext(const Media &media);

    Q_INVOKABLE void addMediaAtTail(const Media &media);

    Q_INVOKABLE void addExternMedia(const QString &path);

    Q_INVOKABLE void removeMedia(int removed);

    Q_INVOKABLE void moveMedia(int index, int offset);

    Q_INVOKABLE void next();

    Q_INVOKABLE void previous();

    Q_INVOKABLE void userNextRequested();

    Q_INVOKABLE void userPreviousRequested();

    Q_INVOKABLE void userSwitchRequested(int id);

    Q_INVOKABLE void clearQueue();

    Q_INVOKABLE void clearHistory();

   signals:

    void playModeChanged(int mode);

    void playModeIconChanged(QUrl icon);

    void currentMediaChanged(const Media &media);

    void addMediaModeChanged(int n);

    void mediaQueueChanged();

    void queuePosChanged(int n);

    void playModeNameChanged(const QString &name);

    void playQueueEnded();

    void externMediaInfoRequested(const QString& path);

    void showTips(const QString &icon, const QString &info);
};

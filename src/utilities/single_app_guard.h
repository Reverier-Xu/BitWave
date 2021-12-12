/**
 * @file single_app_guard.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard {
   public:
    explicit RunGuard(const QString &key);

    ~RunGuard();

    bool isAnotherRunning();

    bool tryToRun();

    void release();

   private:
    const QString key;
    const QString memLockKey;
    const QString sharedMemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY(RunGuard)
};

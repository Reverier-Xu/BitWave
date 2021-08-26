//
// Created by reverier on 2021/8/26.
//

#pragma once

#include <QObject>

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

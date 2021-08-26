//
// Created by reverier on 2021/8/26.
//

#include "single_app_guard.h"
#include <QCryptographicHash>
#include <QDebug>

namespace {

    QString generateKeyHash(const QString &key, const QString &salt) {
        QByteArray data;

        data.append(key.toUtf8());
        data.append(salt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
    }

}


RunGuard::RunGuard(const QString &key)
        : key(key), memLockKey(generateKeyHash(key, "_@_Wo0t3c_e78ac157")),
          sharedMemKey(generateKeyHash(key, "_@_Wo0t3c_1483ab68")), sharedMem(sharedMemKey), memLock(memLockKey, 1) {
    memLock.acquire();
    {
        QSharedMemory fix(sharedMemKey);
        fix.attach();
    }
    memLock.release();
}

RunGuard::~RunGuard() {
    release();
}

bool RunGuard::isAnotherRunning() {
    if (sharedMem.isAttached())
        return false;

    memLock.acquire();
    const bool isRunning = sharedMem.attach();
    if (isRunning)
        sharedMem.detach();
    memLock.release();

    return isRunning;
}

bool RunGuard::tryToRun() {
    if (isAnotherRunning())   // Extra check
        return false;
    memLock.acquire();
    const bool result = sharedMem.create(sizeof(quint64));
    memLock.release();
    if (!result) {
        release();
        return false;
    }

    return true;
}

void RunGuard::release() {
    memLock.acquire();
    if (sharedMem.isAttached())
        sharedMem.detach();
    memLock.release();
}

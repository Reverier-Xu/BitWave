/**
 * @file app_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include <QDir>
#include <QStandardPaths>
#include <QThread>

#ifdef QT_DBUS_LIB

#include "dbus/mpris2.h"

#endif

#include "app_manager.h"
#include "gui_manager.h"
#include "lyric_provider_manager.h"
#include "parser_manager.h"
#include "player_manager.h"

void detectPaths();

AppManager::AppManager(QObject *parent) : QObject(parent) {}

void AppManager::initialize(const QString &file) {
    registerTypes();
    detectPaths();
    auto guiManager = GuiManager::instance(this);
    GuiManager::exportComponents();
    guiManager->exportManagers();
    guiManager->createUI();

    mParserThread = new QThread(this);
    ParserManager::instance()->moveToThread(mParserThread);
    mParserThread->start();

    mLyricProviderThread = new QThread(this);
    LyricProviderManager::instance()->moveToThread(mLyricProviderThread);
    mLyricProviderThread->start();

#ifdef QT_DBUS_LIB
    using mpris::Mpris2;
    new Mpris2(this);
//    qDebug() << "MPRIS2 loaded.";
#endif
    if (!file.isEmpty()) {
        QueueManager::instance()->addExternMedia(file);
    }
}

void AppManager::registerTypes() { qRegisterMetaType<Media>("Media"); }

AppManager::~AppManager() {
    mParserThread->exit(0);
    mParserThread->wait();
    mLyricProviderThread->exit(0);
    mLyricProviderThread->wait();
    ParserManager::instance()->deleteLater();
    LyricProviderManager::instance()->deleteLater();
}

void detectPaths() {
    auto dataPath =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto cachePath =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
        "/BitWave";
    QDir dir;
    QStringList dataPaths = {"/Database"};
    QStringList cachePaths = {"/Covers", "/CachedSongs"};
    for (auto &i : dataPaths)
        if (!dir.exists(dataPath + i)) dir.mkpath(dataPath + i);
    for (auto &i : cachePaths)
        if (!dir.exists(cachePath + i)) dir.mkpath(cachePath + i);
}

void AppManager::onSecondaryInstanceMessageReceived(quint32 instanceId,
                                                    QByteArray message) {
    if (!message.isEmpty()) {
        char flags = message[0];
        if (flags & 0x01) {
            PlayerManager::instance(this)->resume();
        } else if (flags & 0x02) {
            PlayerManager::instance(this)->pause();
        } else if (flags & 0x04) {
            QueueManager::instance(this)->userNextRequested();
        } else if (flags & 0x08) {
            QueueManager::instance(this)->userPreviousRequested();
        } else {
            QString file = QString::fromUtf8(message.mid(1));
            if (!file.isEmpty()) {
                QueueManager::instance(this)->addExternMedia(file);
            }
        }
    }
}

void AppManager::onSecondaryInstanceStarted() {
    // qDebug() << "Secondary instance started.";
    GuiManager::instance(this)->onSecondaryInstanceStarted();
}

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

#ifdef Qt5DBus_FOUND

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

#ifdef Qt5DBus_FOUND
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

void AppManager::onSecondaryInstanceStarted(quint32 instanceId, QByteArray message) {
    GuiManager::instance(this)->onSecondaryInstanceStarted(instanceId, message);
    if (!message.isEmpty()) {
        auto file = QString::fromUtf8(message);
        QueueManager::instance()->addExternMedia(file);
    }
}

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

void AppManager::initialize() {
    registerTypes();
    detectPaths();
    auto guiManager = GuiManager::instance(this);
    GuiManager::exportComponents();
    guiManager->exportManagers();
    guiManager->createUI();

    this->mParserThread = new QThread(this);
    ParserManager::instance()->moveToThread(this->mParserThread);
    this->mParserThread->start();

    this->mLyricProviderThread = new QThread(this);
    LyricProviderManager::instance()->moveToThread(this->mLyricProviderThread);
    this->mLyricProviderThread->start();

#ifdef Qt5DBus_FOUND
    using mpris::Mpris2;
    new Mpris2(this);
//    qDebug() << "MPRIS2 loaded.";
#endif
}

void AppManager::registerTypes() { qRegisterMetaType<Media>("Media"); }

AppManager::~AppManager() {
    this->mParserThread->exit(0);
    this->mParserThread->wait();
    this->mLyricProviderThread->exit(0);
    this->mLyricProviderThread->wait();
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

/* 
 * app_manager.cpp
 *
 * Summary: the whole app manager.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
 * 
 */

#include <QDir>
#include <QStandardPaths>
#include <QThread>
#include "app_manager.h"

#include "gui_manager.h"
#include "player_manager.h"
#include "parser_manager.h"

void detectPaths();

AppManager::AppManager(QObject *parent) : QObject(parent) {}

void AppManager::initialize() {
    registerTypes();
    detectPaths();
    auto guiManager = GuiManager::instance(this);
    GuiManager::exportComponents();
    guiManager->exportManagers();
    guiManager->createUI();

    this->parser_thread = new QThread(this);
    ParserManager::instance()->moveToThread(this->parser_thread);
    this->parser_thread->start();
}

void AppManager::registerTypes() {
    qRegisterMetaType<Media>("Media");
}

AppManager::~AppManager() {
    this->parser_thread->exit(0);
}

void detectPaths() {
    auto dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    auto cachePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/BitWave";
    QDir dir;
    QStringList dataPaths = {"/Database"};
    QStringList cachePaths = {"/Covers", "/CachedSongs"};
    for (auto &i:dataPaths)
        if (!dir.exists(dataPath + i))
            dir.mkpath(dataPath + i);
    for (auto &i:cachePaths)
        if (!dir.exists(cachePath + i))
            dir.mkpath(cachePath + i);
}



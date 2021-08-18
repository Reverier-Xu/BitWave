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

AppManager::AppManager(QObject *parent) : QObject(parent) {}

void AppManager::initialize() {
    registerTypes();
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



//
// Created by Reverier-Xu on 2021/6/25.
//

#include "app_manager.h"

#include "gui_manager.h"
#include "player_manager.h"

AppManager::AppManager(QObject *parent) : QObject(parent) {}

void AppManager::initialize() {
    auto guiManager = GuiManager::getInstance(this);
    GuiManager::exportComponents();
    guiManager->exportManagers();
    guiManager->createUI();
}

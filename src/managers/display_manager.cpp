/* 
 * display_manager.cpp
 *
 * Summary: misc variables for QML frontend.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
 * 
 */

#include "display_manager.h"
#include <QSettings>

DisplayManager* DisplayManager::mInstance = nullptr;

DisplayManager *DisplayManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new DisplayManager(parent);
    return mInstance;
}

DisplayManager::DisplayManager(QObject *parent) : BaseManager(parent) {
    this->loadSettings();
}

DisplayManager::~DisplayManager() {
    this->saveSettings();
}

void DisplayManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    this->setThemeColor(settings.value("ThemeColor", "#0078d6").toString());
    this->setAlertColor(settings.value("AlertColor", "#ff6033").toString());
    this->setColorStyle(settings.value("ColorStyle", true).toBool());
    settings.endGroup();
}

void DisplayManager::saveSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ThemeColor", this->themeColor().name());
    settings.setValue("AlertColor", this->alertColor().name());
    settings.setValue("ColorStyle", this->colorStyle());
    settings.endGroup();

    settings.sync();
}

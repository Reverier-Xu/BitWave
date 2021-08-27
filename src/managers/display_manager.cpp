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

#include <QSettings>
#include "display_manager.h"
#include "player_manager.h"

DisplayManager* DisplayManager::mInstance = nullptr;

DisplayManager *DisplayManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new DisplayManager(parent);
    return mInstance;
}

DisplayManager::DisplayManager(QObject *parent) : BaseManager(parent) {
    this->hideTimer = new QTimer(this);
    this->hideTimer->setInterval(1000);

    connect(this->hideTimer, &QTimer::timeout, [=]() {
        if(PlayerManager::instance()->currentMediaIsVideo() && this->pageIndex() == 0)
            this->setMouseIsActive(false);
    });

    connect(this, &DisplayManager::pageIndexChanged, [=](int index) {
        if(index == 0) {
            this->delayedHide();
        } else {
            this->blockDelayedHide();
        }
    });

    connect(QueueManager::instance(this->parent()), &QueueManager::playQueueEnded, [=]() {
        this->setMouseIsActive(true);
        this->setSideBarExpanded(true);
    });

    connect(PlayerManager::instance(), &PlayerManager::showTips, [=](const QString &icon, const QString &info) {
        emit this->showTips(icon, info);
    });

    connect(QueueManager::instance(), &QueueManager::showTips, [=](const QString &icon, const QString &info) {
        emit this->showTips(icon, info);
    });

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

void DisplayManager::delayedHide() {
    this->setMouseIsActive(true);
    if (this->hideTimer->isActive()) this->hideTimer->stop();
    this->hideTimer->start();
}

void DisplayManager::blockDelayedHide() {
    this->setMouseIsActive(true);
    if (this->hideTimer->isActive()) this->hideTimer->stop();
}

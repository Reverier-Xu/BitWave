/**
 * @file display_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#include "display_manager.h"

#include <QObject>
#include <QSettings>

#include "player_manager.h"

DisplayManager *DisplayManager::mInstance = nullptr;

DisplayManager *DisplayManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new DisplayManager(parent);
    return mInstance;
}

DisplayManager::DisplayManager(QObject *parent) : QObject(parent) {
    this->mHideTimer = new QTimer(this);
    this->mHideTimer->setInterval(1000);

    connect(this->mHideTimer, &QTimer::timeout, [=]() {
        if (PlayerManager::instance()->currentMediaIsVideo() &&
            this->pageIndex() == 0)
            this->setMouseIsActive(false);
    });

    connect(this, &DisplayManager::pageIndexChanged, [=](int index) {
        if (index == 0) {
            this->delayedHide();
        } else {
            this->blockDelayedHide();
        }
    });

    connect(QueueManager::instance(this->parent()),
            &QueueManager::playQueueEnded, [=]() {
                this->setMouseIsActive(true);
                this->setSideBarExpanded(true);
            });

    connect(PlayerManager::instance(),
            &PlayerManager::currentMediaIsVideoChanged, this, [=](bool n) {
                if (!n) this->setMouseIsActive(true);
            });

    connect(PlayerManager::instance(), &PlayerManager::showTips,
            [=](const QString &icon, const QString &info) {
                emit this->showTips(icon, info);
            });

    connect(QueueManager::instance(), &QueueManager::showTips,
            [=](const QString &icon, const QString &info) {
                emit this->showTips(icon, info);
            });

    this->loadSettings();
}

DisplayManager::~DisplayManager() { this->saveSettings(); }

void DisplayManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    this->setThemeColor(settings.value("ThemeColor", "#0078d6").toString());
    this->setAlertColor(settings.value("AlertColor", "#ff6033").toString());
    this->setColorStyle(settings.value("ColorStyle", true).toBool());
    settings.endGroup();
}

void DisplayManager::saveSettings() const {
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
    if (this->mHideTimer->isActive()) this->mHideTimer->stop();
    this->mHideTimer->start();
}

void DisplayManager::blockDelayedHide() {
    this->setMouseIsActive(true);
    if (this->mHideTimer->isActive()) this->mHideTimer->stop();
}

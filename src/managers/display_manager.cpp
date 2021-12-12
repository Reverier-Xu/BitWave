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
    mHideTimer = new QTimer(this);
    mHideTimer->setInterval(1000);

    connect(mHideTimer, &QTimer::timeout, [=]() {
        if (PlayerManager::instance()->currentMediaIsVideo() &&
            pageIndex() == 0)
            setMouseIsActive(false);
    });

    connect(this, &DisplayManager::pageIndexChanged, [=](int index) {
        if (index == 0) {
            delayedHide();
        } else {
            blockDelayedHide();
        }
    });

    connect(QueueManager::instance(this->parent()),
            &QueueManager::playQueueEnded, [=]() {
                setMouseIsActive(true);
                setSideBarExpanded(true);
            });

    connect(PlayerManager::instance(),
            &PlayerManager::currentMediaIsVideoChanged, this, [=](bool n) {
                if (!n) setMouseIsActive(true);
            });

    connect(PlayerManager::instance(), &PlayerManager::showTips,
            [=](const QString &icon, const QString &info) {
                emit showTips(icon, info);
            });

    connect(QueueManager::instance(), &QueueManager::showTips,
            [=](const QString &icon, const QString &info) {
                emit showTips(icon, info);
            });

    loadSettings();
}

DisplayManager::~DisplayManager() { saveSettings(); }

void DisplayManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    setThemeColor(settings.value("ThemeColor", "#0078d6").toString());
    setAlertColor(settings.value("AlertColor", "#ff6033").toString());
    setColorStyle(settings.value("ColorStyle", true).toBool());
    settings.endGroup();
}

void DisplayManager::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ThemeColor", themeColor().name());
    settings.setValue("AlertColor", alertColor().name());
    settings.setValue("ColorStyle", colorStyle());
    settings.endGroup();

    settings.sync();
}

void DisplayManager::delayedHide() {
    setMouseIsActive(true);
    if (mHideTimer->isActive()) mHideTimer->stop();
    mHideTimer->start();
}

void DisplayManager::blockDelayedHide() {
    setMouseIsActive(true);
    if (mHideTimer->isActive()) mHideTimer->stop();
}

int DisplayManager::activeTabIndex() const { return mActiveTabIndex; }

void DisplayManager::setActiveTabIndex(int n) {
    mActiveTabIndex = n;
    emit activeTabIndexChanged(n);
    emit pageIndexChanged(pageIndex());
}

int DisplayManager::pageIndex() const {
    if (activeTabIndex() == -1)
        return 0;
    else if (activeTabIndex() >= 0)
        return 1;
    else if (activeTabIndex() == -2)
        return 3;
    else if (activeTabIndex() == -3)
        return 2;
    else {
        throw std::exception();
    }
}

void DisplayManager::setPageIndex(int n) { emit pageIndexChanged(n); }

int DisplayManager::queueBarIndex() const { return mQueueBarIndex; }

void DisplayManager::setQueueBarIndex(int n) {
    mQueueBarIndex = n;
    emit queueBarIndexChanged(n);
}

bool DisplayManager::colorStyle() const { return mColorStyle; }

void DisplayManager::setColorStyle(bool value) {
    mColorStyle = value;
    emit colorStyleChanged(value);
    emit contentColorChanged(contentColor());
}

bool DisplayManager::sideBarExpanded() const { return mSideBarExpanded; }

void DisplayManager::setSideBarExpanded(bool value) {
    mSideBarExpanded = value;
    emit sideBarExpandedChanged(value);
}

bool DisplayManager::queueBarExpanded() const { return mQueueBarExpanded; }

void DisplayManager::setQueueBarExpanded(bool value) {
    mQueueBarExpanded = value;
    emit queueBarExpandedChanged(value);
}

bool DisplayManager::mouseIsActive() const { return mMouseIsActive; }

void DisplayManager::setMouseIsActive(bool value) {
    mMouseIsActive = value;
    emit mouseIsActiveChanged(value);
}

QColor DisplayManager::themeColor() const { return mThemeColor; }

void DisplayManager::setThemeColor(const QColor &value) {
    mThemeColor = value;
    emit themeColorChanged(value);
}

void DisplayManager::setThemeColor(const QString &value) {
    mThemeColor.setNamedColor(value);
    emit themeColorChanged(value);
}

QColor DisplayManager::alertColor() const { return mAlertColor; }

void DisplayManager::setAlertColor(const QColor &value) {
    mAlertColor = value;
    emit alertColorChanged(value);
}

void DisplayManager::setAlertColor(const QString &value) {
    mAlertColor.setNamedColor(value);
    emit alertColorChanged(value);
}

QColor DisplayManager::contentColor() const {
    return colorStyle() ? QColor(0x222222) : QColor(0xdddddd);
}

void DisplayManager::setContentColor(const QColor &value) {
    emit contentColorChanged(contentColor());
}

bool DisplayManager::isFullScreen() const { return mIsFullScreen; }

void DisplayManager::setFullScreen(bool n) {
    mIsFullScreen = n;
    emit isFullScreenChanged(n);
}

bool DisplayManager::showVideoTime() const { return mShowVideoTime; }

void DisplayManager::setShowVideoTime(bool n) {
    mShowVideoTime = n;
    emit showVideoTimeChanged(n);
}

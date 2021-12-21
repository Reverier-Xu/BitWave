/**
 * @file gui_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "gui_manager.h"

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QQmlContext>
#include <SingleApplication>

#include "display_manager.h"
#include "player_manager.h"
#include "queue_manager.h"
#include "resources/components/video_player.h"
#include "service_manager.h"

GuiManager *GuiManager::mInstance = nullptr;

GuiManager::GuiManager(QObject *parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    mUiEngine = new QQmlApplicationEngine(this);
}

void GuiManager::createUI() {
    const QUrl url(QStringLiteral("qrc:/views/MainWindow.qml"));
    mUiEngine->load(url);
}

void GuiManager::exportManagers() {
    mUiEngine->rootContext()->setContextProperty(
        "player", PlayerManager::instance(this->parent()));
    mUiEngine->rootContext()->setContextProperty(
        "display", DisplayManager::instance(this->parent()));
    mUiEngine->rootContext()->setContextProperty(
        "queue", QueueManager::instance(this->parent()));
    mUiEngine->rootContext()->setContextProperty(
        "service", ServiceManager::instance(this->parent()));
    mUiEngine->rootContext()->setContextProperty(
        "lyricsModel", PlayerManager::instance()->getLyricsModel());
    mUiEngine->rootContext()->setContextProperty(
        "mainQueueModel", QueueManager::instance()->getQueueModel());
}

GuiManager *GuiManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new GuiManager(parent);
    return mInstance;
}

void GuiManager::exportComponents() {
    qmlRegisterType<VideoPlayer>("Reverier.MediaWidgets", 1, 0, "VideoPlayer");
}

void GuiManager::onSecondaryInstanceStarted(quint32 instanceId,
                                            QByteArray message) {
    // qDebug() << "Secondary instance started with id: " << instanceId;
    DisplayManager::instance(this)->raiseWindow();
}

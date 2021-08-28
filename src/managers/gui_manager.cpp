/* 
 * gui_manager.cpp
 *
 * Summary: manager of QML Frontend and engines.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
 * 
 */

#include "gui_manager.h"

#include <QQmlContext>

#include "player_manager.h"
#include "resources/components/video_player.h"
#include "display_manager.h"
#include "queue_manager.h"

GuiManager *GuiManager::mInstance = nullptr;

GuiManager::GuiManager(QObject *parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    this->mUiEngine = new QQmlApplicationEngine(this);


}

void GuiManager::createUI() {
    const QUrl url(QStringLiteral("qrc:/views/MainWindow.qml"));
    this->mUiEngine->load(url);
}

void GuiManager::exportManagers() {
    this->mUiEngine->rootContext()->setContextProperty(
            "player", PlayerManager::instance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "display", DisplayManager::instance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "queue", QueueManager::instance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "lyricsModel", PlayerManager::instance()->getLyricsModel());
    this->mUiEngine->rootContext()->setContextProperty(
            "mainQueueModel", QueueManager::instance()->getQueueModel());
}

GuiManager *GuiManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new GuiManager(parent);
    return mInstance;
}

void GuiManager::exportComponents() {
    qmlRegisterType<VideoPlayer>("Reverier.MediaWidgets", 1, 0, "VideoPlayer");
}

//
// Created by Reverier-Xu on 2021/6/25.
//

#include "gui_manager.h"

#include <QQmlContext>

#include "player_manager.h"
#include "resources/components/video_player.h"
#include "settings_manager.h"
#include "display_manager.h"

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
            "settings", SettingsManager::getInstance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "player", PlayerManager::getInstance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "display", DisplayManager::getInstance(this->parent()));
    this->mUiEngine->rootContext()->setContextProperty(
            "lyricsModel", PlayerManager::getInstance()->getLyricsModel());
}

GuiManager *GuiManager::getInstance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new GuiManager(parent);
    return mInstance;
}

void GuiManager::exportComponents() {
    qmlRegisterType<VideoPlayer>("Reverier.MediaWidgets", 1, 0, "VideoPlayer");
}

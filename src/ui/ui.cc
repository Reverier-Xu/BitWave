/**
 * @file ui.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include "ui.h"
#include "player/video_player.h"

#include <config/config.h>
#include <player/player.h>

#include <QQmlContext>


Ui::Ui(QObject* parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    m_engine = new QQmlApplicationEngine(this);
    m_colorize = new Colorize(this);
    m_router = new Router(this);
}

Ui::~Ui() = default;

void Ui::initialize() {
    exportProperties();
    connectSignals();
    createUi();
}

void Ui::onSecondaryInstanceStarted() {
    emit raiseWindowRequested();
}

void Ui::exportProperties() {
    m_engine->rootContext()->setContextProperty("player", Player::instance(this->parent()));
    m_engine->rootContext()->setContextProperty("displayConfig", Config::instance(this->parent())->displayConfig());
    m_engine->rootContext()->setContextProperty("ui", this);
    m_engine->rootContext()->setContextProperty("router", m_router);
    m_engine->rootContext()->setContextProperty("colorize", m_colorize);
    qmlRegisterType<VideoPlayer>("RxUI.MediaWidgets", 1, 0, "VideoPlayer");
//    qmlRegisterType<Media>("RxUI.Models", 1, 0, "Media");
}

void Ui::createUi() {
    const QUrl url(u"qrc:/BitWave/ui/Main.qml"_qs);
    m_engine->addImportPath(u"qrc:/BitWave"_qs);
    m_engine->load(url);
}

void Ui::connectSignals() {
    connect(Player::instance(this->parent()), &Player::coverChanged, this, [=](const QImage& image){
        m_colorize->requestColorize(image);
    });
}

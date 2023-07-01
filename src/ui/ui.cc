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

#include <player/player.h>

#include <QApplication>
#include <QQmlContext>

#include "player/video_player.h"

Ui::Ui(QObject* parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    m_engine = new QQmlApplicationEngine(this);
    m_colorize = new Colorize(this);
    m_router = new Router(this);
    m_uiConfig = new UiConfig(this);
}

Ui::~Ui() { m_engine->deleteLater(); }

void Ui::initialize() {
    exportProperties();
    connectSignals();
    createUi();
}

void Ui::onSecondaryInstanceStarted() {
    emit m_uiConfig->raiseWindowRequested();
}

void Ui::exportProperties() {
    m_engine->rootContext()->setContextProperty(
        "player", Player::instance(this->parent()));
    m_engine->rootContext()->setContextProperty(
        "queue", Player::instance(this->parent())->queue());
    m_engine->rootContext()->setContextProperty(
        "queueModel", Player::instance(this->parent())->queue()->model());
    m_engine->rootContext()->setContextProperty("ui", m_uiConfig);
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
    connect(Player::instance(this->parent()), &Player::coverChanged, this,
            [=](const QImage& image) { m_colorize->requestColorize(image); });
    connect(m_uiConfig, &UiConfig::languageChanged, this,
            [=]() { m_engine->retranslate(); });
}

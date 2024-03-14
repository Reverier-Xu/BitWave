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

#include "lyrics/lyrics.h"
#include "player/video_player.h"
#include "library/library.h"


Ui* Ui::m_instance = nullptr;

Ui::Ui(QObject* parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    m_engine = new QQmlApplicationEngine(this);
    m_colorize = new Colorize(this);
    m_router = new Router(this);
    m_uiConfig = new UiConfig(this, m_router);
}

Ui::~Ui() { m_engine->deleteLater(); }

void Ui::initialize() {
    exportProperties();
    connectSignals();
    createUi();
}

Ui* Ui::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Ui(parent);
        locker.unlock();
    }
    return m_instance;
}

Q_INVOKABLE void Ui::requestQuit() {
    Player::instance(this->parent())->saveSettings();
    Player::instance(this->parent())->queue()->saveSettings();
    Library::instance(this->parent())->saveSettings();

    QApplication::quit();
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
    
    m_engine->rootContext()->setContextProperty("app", this);
    m_engine->rootContext()->setContextProperty("ui", m_uiConfig);
    m_engine->rootContext()->setContextProperty("router", m_router);
    m_engine->rootContext()->setContextProperty("colorize", m_colorize);
    m_engine->rootContext()->setContextProperty("library", Library::instance(this->parent()));
    m_engine->rootContext()->setContextProperty("libraryModel", Library::instance(this->parent())->model());
    m_engine->rootContext()->setContextProperty(
        "lyrics", Lyrics::instance(this->parent()));
    m_engine->rootContext()->setContextProperty(
        "lyricsModel", Lyrics::instance(this->parent())->lyricsModel());
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

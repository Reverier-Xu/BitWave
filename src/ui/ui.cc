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
#include <player/player.h>

#include <QApplication>
#include <QQmlContext>


Ui::Ui(QObject* parent) : QObject(parent) {
    std::setlocale(LC_NUMERIC, "C");
    m_engine = new QQmlApplicationEngine(this);
    m_colorize = new Colorize(this);
    m_router = new Router(this);
    loadSettings();
}

Ui::~Ui() {
    saveSettings();
}

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
    m_engine->rootContext()->setContextProperty("queue", Player::instance(this->parent())->queue());
    m_engine->rootContext()->setContextProperty("queueModel", Player::instance(this->parent())->queue()->model());
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

void Ui::setColorStyle(bool n) {
    m_colorStyle = n;
    emit colorStyleChanged(n);
}

bool Ui::colorStyle() const {
    return m_colorStyle;
}

void Ui::setLanguage(const QString& n) {
    m_language = n;
    QApplication::removeTranslator(&m_translator);
    auto ok = m_translator.load(QString(":/%1/%2.qm").arg("i18n", n));
    if (!ok) {
        // TODO: warn user here.
    }
    QApplication::installTranslator(&m_translator);
    m_engine->retranslate();
    emit languageChanged(n);
}

QString Ui::language() const {
    return m_language;
}

void Ui::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    setColorStyle(settings.value("ColorStyle", true).toBool());
    setLanguage(settings.value("Language", "en_US").toString());
    settings.endGroup();
}

void Ui::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ColorStyle", colorStyle());
    settings.setValue("Language", language());
    settings.endGroup();
}

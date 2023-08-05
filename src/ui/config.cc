/**
 * @file ui_config.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-25
 *
 * @copyright 2023 Woo Tech
 */

#include "config.h"
#include "player/player.h"

#include <QApplication>
#include <QSettings>


UiConfig::UiConfig(QObject* parent, Router* router) : QObject(parent), m_router(router) {
    loadSettings();
    m_hideTimer = new QTimer(this);
    m_hideTimer->setInterval(1000);

    connect(m_hideTimer, &QTimer::timeout, [=]() {
        if (!this->controlWidgetExpanded() && Player::instance()->playing() && Player::instance()->media().type() ==
        VIDEO && router &&
            router->currentRoute()
                .startsWith("player"))
            setHideControls(true);
    });

    connect(router, &Router::currentRouteChanged, this, [=](const QString& route) {
        setHideControls(false);
        if (route.startsWith("player")) {
            autoHideControls();
        } else {
            blockHideControls();
        }
    });

    connect(Player::instance(), &Player::mediaChanged, this, [=](const Media& media) {
        setHideControls(false);
        if (media.type() == VIDEO && router->currentRoute().startsWith("player")) {
            autoHideControls();
        } else {
            blockHideControls();
        }
    });
}

UiConfig::~UiConfig() { saveSettings(); }

void UiConfig::setColorStyle(bool n) {
    m_colorStyle = n;
    emit colorStyleChanged(n);
}

bool UiConfig::colorStyle() const { return m_colorStyle; }

void UiConfig::setLanguage(const QString& n) {
    m_language = n;
    QApplication::removeTranslator(&m_translator);
    auto ok = m_translator.load(QString(":/%1/%2.qm").arg("i18n", n));
    if (!ok) {
        // TODO: warn user here.
    }
    QApplication::installTranslator(&m_translator);
    emit languageChanged(n);
}

QString UiConfig::language() const { return m_language; }

void UiConfig::setFullscreen(bool n) {
    m_fullscreen = n;
    emit fullscreenChanged(n);
}

bool UiConfig::fullscreen() const {
    return m_fullscreen;
}

void UiConfig::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    setColorStyle(settings.value("ColorStyle", true).toBool());
    setLanguage(settings.value("Language", "en_US").toString());
    setFullscreen(settings.value("Fullscreen", false).toBool());
    setFlatSystemTray(settings.value("FlatSystemTray", false).toBool());
    settings.endGroup();
}

void UiConfig::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ColorStyle", colorStyle());
    settings.setValue("Language", language());
    settings.setValue("Fullscreen", fullscreen());
    settings.setValue("FlatSystemTray", flatSystemTray());
    settings.endGroup();
}

void UiConfig::setFlatSystemTray(bool n) {
    m_flatSystemTray = n;
    emit flatSystemTrayChanged(n);
}

bool UiConfig::flatSystemTray() const {
    return m_flatSystemTray;
}

void UiConfig::setHideControls(bool n) {
    m_hideControls = n;
    emit hideControlsChanged(n);
}

bool UiConfig::hideControls() const {
    return m_hideControls;
}

void UiConfig::autoHideControls() {
    if (m_hideTimer->isActive()) m_hideTimer->stop();
    m_hideTimer->start();
}

void UiConfig::blockHideControls() {
    setHideControls(false);
    m_hideTimer->stop();
}

void UiConfig::setSideBarExpanded(bool n) {
    m_sideBarExpanded = n;
    emit sideBarExpandedChanged(n);
}

bool UiConfig::sideBarExpanded() const {
    return m_sideBarExpanded;
}

void UiConfig::setControlWidgetExpanded(bool n) {
    m_controlWidgetExpanded = n;
    emit controlWidgetExpandedChanged(n);
}

bool UiConfig::controlWidgetExpanded() const {
    return m_controlWidgetExpanded;
}

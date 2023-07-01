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

#include <QApplication>
#include <QSettings>

UiConfig::UiConfig(QObject* parent) : QObject(parent) { loadSettings(); }

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

void UiConfig::loadSettings() {
    QSettings settings;
    settings.beginGroup("Display");
    setColorStyle(settings.value("ColorStyle", true).toBool());
    setLanguage(settings.value("Language", "en_US").toString());
    settings.endGroup();
}

void UiConfig::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ColorStyle", colorStyle());
    settings.setValue("Language", language());
    settings.endGroup();
}
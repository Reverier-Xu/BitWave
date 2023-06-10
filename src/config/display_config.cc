/**
 * @file display_config.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include <QSettings>
#include "display_config.h"


DisplayConfig::DisplayConfig(QObject* parent) : QObject(parent) { }

DisplayConfig::~DisplayConfig() = default;

bool DisplayConfig::colorStyle() const {
    return m_colorStyle;
}

void DisplayConfig::setColorStyle(bool colorStyle) {
    m_colorStyle = colorStyle;
    emit colorStyleChanged(colorStyle);
}

void DisplayConfig::loadConfig() {
    QSettings settings;
    settings.beginGroup("Display");
    setColorStyle(settings.value("ColorStyle", false).toBool());
    settings.endGroup();
}

void DisplayConfig::saveConfig() const {
    QSettings settings;
    settings.beginGroup("Display");
    settings.setValue("ColorStyle", colorStyle());
    settings.endGroup();
}

int DisplayConfig::tabIndex() const {
    return m_tabIndex;
}

void DisplayConfig::setTabIndex(int tabIndex) {
    m_tabIndex = tabIndex;
    emit tabIndexChanged(tabIndex);
}

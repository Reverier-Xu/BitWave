/**
 * @file config.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include <QSettings>
#include <QMutex>
#include "config.h"


Config::Config(QObject* parent) : QObject(parent) {
    m_displayConfig = new DisplayConfig(this);
    loadConfig();
}

Config::~Config() {
    saveConfig();
}

void Config::saveConfig() {
    m_displayConfig->saveConfig();
}

void Config::loadConfig() {
    m_displayConfig->loadConfig();
}

Config* Config::m_instance = nullptr;

Config* Config::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr)
            m_instance = new Config(parent);
        locker.unlock();
    }
    return m_instance;
}

DisplayConfig* Config::displayConfig() {
    return m_displayConfig;
}

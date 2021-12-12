/**
 * @file service_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "service_manager.h"

ServiceManager *ServiceManager::mInstance = nullptr;

ServiceManager *ServiceManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new ServiceManager(parent);
    return mInstance;
}

ServiceManager::ServiceManager(QObject *parent) : QObject(parent) {
    loadSettings();
}

void ServiceManager::loadSettings() {}

void ServiceManager::saveSettings() const {}

ServiceManager::~ServiceManager() { saveSettings(); }

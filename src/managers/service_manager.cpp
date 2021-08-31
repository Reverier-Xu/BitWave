//
// Created by reverier on 2021/8/29.
//

#include "service_manager.h"

ServiceManager *ServiceManager::mInstance = nullptr;

ServiceManager *ServiceManager::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new ServiceManager(parent);
    return mInstance;
}

ServiceManager::ServiceManager(QObject *parent)
        : BaseManager(parent) {
}

void ServiceManager::loadSettings() {
}

void ServiceManager::saveSettings() {
}

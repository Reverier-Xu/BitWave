//
// Created by Reverier-Xu on 2021/6/25.
//

#include "provider_manager.h"
#include <QObject>
#include <QSettings>

ProviderManager* ProviderManager::mInstance = nullptr;

ProviderManager* ProviderManager::instance(QObject* parent) {
    if (mInstance == nullptr) {
        mInstance = new ProviderManager(parent);
    }
    return mInstance;
}

ProviderManager::ProviderManager(QObject *parent) : BaseManager(parent) {
    this->loadSettings();
}

ProviderManager::~ProviderManager() {
    this->saveSettings();
}

void ProviderManager::loadSettings() {}

void ProviderManager::saveSettings() {}

//
// Created by Reverier-Xu on 2021/6/11.
//

#include "settings_manager.h"

SettingsManager* SettingsManager::mInstance = nullptr;

SettingsManager::SettingsManager(QObject* parent) : QObject(parent) {}

SettingsManager* SettingsManager::getInstance(QObject* parent) {
    if (mInstance == nullptr) mInstance = new SettingsManager(parent);
    return mInstance;
}

//
// Created by reverier on 2021/8/29.
//

#include "router_manager.h"

RouterManager* RouterManager::mInstance = nullptr;

RouterManager *RouterManager::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new RouterManager(parent);
    return mInstance;
}

RouterManager::RouterManager(QObject *parent) : BaseManager(parent) {

}

void RouterManager::loadSettings() {

}

void RouterManager::saveSettings() {

}

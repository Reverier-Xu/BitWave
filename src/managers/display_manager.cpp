//
// Created by Reverier-Xu on 2021/7/29.
//

#include "display_manager.h"

DisplayManager* DisplayManager::mInstance = nullptr;

DisplayManager *DisplayManager::getInstance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new DisplayManager(parent);
    return mInstance;
}

DisplayManager::DisplayManager(QObject *parent) : QObject(parent) {

}

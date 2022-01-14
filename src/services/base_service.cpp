/**
 * @file base_service.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "base_service.h"


BaseService::BaseService(QObject *parent) : QObject(parent) {}

QString BaseService::name() const { return mName; }

void BaseService::setName(const QString &name) {
    if (mName != name) {
        mName = name;
        emit nameChanged(mName);
    }
}

QString BaseService::icon() const { return mIcon; }

void BaseService::setIcon(const QString &icon) {
    if (mIcon != icon) {
        mIcon = icon;
        emit iconChanged(mIcon);
    }
}

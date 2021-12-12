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

bool BaseService::registerSubService(BaseService *service) {
    if (isEndpoint()) return false;
    mSubServices.insert(service->name(), service);
    emit contentChanged();
    return true;
}

bool BaseService::removeSubService(BaseService *service) {
    if (isEndpoint()) return false;
    mSubServices.remove(service->name());
    emit contentChanged();
    return true;
}

BaseService *BaseService::getSubService(const QStringList &router) {
    if (isEndpoint() or router.isEmpty()) {
        return this;
    }
    if (!mSubServices.contains(router.first())) {
        throw std::runtime_error("Service not found");
    } else {
        return mSubServices[router.first()]->getSubService(router.mid(1));
    }
}

BaseService::BaseService(BaseService *parent) : QObject(parent) {}

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

QStringList BaseService::router() const {
    if (this->parent() != nullptr) {
        return QStringList()
               << qobject_cast<BaseService *>(parent())->name() << name();
    }
    return QStringList() << name();
}

void BaseService::setRouter(const QStringList &router_) {
    emit routerChanged(router());
}

bool BaseService::isEndpoint() const { return mIsEndpoint; }

void BaseService::setIsEndpoint(bool isEndpoint) {
    if (mIsEndpoint != isEndpoint) {
        mIsEndpoint = isEndpoint;
        emit isEndpointChanged(mIsEndpoint);
    }
}

bool BaseService::hasStorage() const { return mHasStorage; }

void BaseService::hasStorage(bool hasStorage) {
    if (mHasStorage != hasStorage) {
        mHasStorage = hasStorage;
        emit hasStorageChanged(mHasStorage);
    }
}

bool BaseService::readOnly() const { return mReadOnly; }

void BaseService::readOnly(bool readOnly) {
    if (mReadOnly != readOnly) {
        mReadOnly = readOnly;
        emit readOnlyChanged(mReadOnly);
    }
}

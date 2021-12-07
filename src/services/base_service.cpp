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

bool BaseService::registerSubService(BaseService* service) {
    if (this->isEndpoint()) return false;
    mSubServices.insert(service->name(), service);
    emit this->contentChanged();
    return true;
}

bool BaseService::removeSubService(BaseService* service) {
    if (this->isEndpoint()) return false;
    mSubServices.remove(service->name());
    emit this->contentChanged();
    return true;
}

BaseService* BaseService::getSubService(const QStringList& router) {
    if (this->isEndpoint() or router.isEmpty()) {
        return this;
    }
    if (!this->mSubServices.contains(router.first())) {
        throw std::runtime_error("Service not found");
    } else {
        return this->mSubServices[router.first()]->getSubService(router.mid(1));
    }
}

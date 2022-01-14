/**
 * @file service_list_model.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-01-01
 *
 * @copyright Copyright (c) 2022 Wootec
 *
 */

#include "service_list_model.h"

ServiceListModel::ServiceListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ServiceListModel::rowCount(const QModelIndex &parent) const {
    return mServiceMap->size();
}

QVariant ServiceListModel::data(const QModelIndex &index, int role) const {
    if (index.row() < mServiceMap->size()) {
        switch (role) {
            case ServiceName:
                return mServiceMap->keys().at(index.row());
            case ServiceIcon:
                return mServiceMap->value(mServiceMap->keys().at(index.row()))->icon();
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> ServiceListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ServiceName] = "serviceName";
    roles[ServiceIcon] = "serviceIcon";
    return roles;
}

void ServiceListModel::setServiceMap(
    QMap<QString, BaseService *> *serviceMap) {
    mServiceMap = serviceMap;
    reload();
}

void ServiceListModel::reload() {
    beginResetModel();
    endResetModel();
}

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
    return 0;
}

QVariant ServiceListModel::data(const QModelIndex &index, int role) const {
    return QVariant();
}

QHash<int, QByteArray> ServiceListModel::roleNames() const {
    return QHash<int, QByteArray>();
}

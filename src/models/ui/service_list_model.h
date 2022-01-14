/**
 * @file service_list_model.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-01-01
 *
 * @copyright Copyright (c) 2022 Wootec
 *
 */

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include "services/base_service.h"

class ServiceListModel : public QAbstractListModel {
    Q_OBJECT
   public:
    enum ServiceListRoles {
        ServiceName = Qt::UserRole + 1,
        ServiceIcon,
    };

    explicit ServiceListModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setServiceMap(QMap<QString, BaseService *> *serviceList);

    void reload();

   private:
    QMap<QString, BaseService *> *mServiceMap{};
};

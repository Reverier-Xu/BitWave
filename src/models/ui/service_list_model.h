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

class ServiceListModel : public QAbstractListModel {
    Q_OBJECT
   public:
    enum ServiceListRoles {
        Category = Qt::UserRole + 1,
        ServiceName,
    };

    explicit ServiceListModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
};

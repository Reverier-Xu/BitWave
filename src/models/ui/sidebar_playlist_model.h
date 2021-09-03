//
// Created by Reverier-Xu on 2021/9/4.
//

#pragma once

#include <QAbstractListModel>

#include "services/base_service.h"

class SideBarPlaylistModel : public QAbstractListModel {
Q_OBJECT
    enum SideBarPlaylistRoles {
        PlaylistIconRole = Qt::UserRole + 1,
        PlaylistNameRole,
    };
private:
    BaseService *mRootService{ };
public:
    explicit SideBarPlaylistModel(QObject *parent = nullptr, BaseService *rootService = nullptr);
};


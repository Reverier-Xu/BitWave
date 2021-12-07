/**
 * @file sidebar_playlist_model.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

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
    BaseService *mRootService{};

   public:
    explicit SideBarPlaylistModel(QObject *parent = nullptr,
                                  BaseService *rootService = nullptr);
};

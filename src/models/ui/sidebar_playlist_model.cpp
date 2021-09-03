//
// Created by Reverier-Xu on 2021/9/4.
//

#include "sidebar_playlist_model.h"

SideBarPlaylistModel::SideBarPlaylistModel(QObject *parent, BaseService *rootService)
        : QAbstractListModel(parent) {
    this->mRootService = rootService;
}

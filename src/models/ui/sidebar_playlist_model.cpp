/**
 * @file sidebar_playlist_model.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "sidebar_playlist_model.h"

SideBarPlaylistModel::SideBarPlaylistModel(QObject *parent,
                                           BaseService *rootService)
    : QAbstractListModel(parent) {
    mRootService = rootService;
}

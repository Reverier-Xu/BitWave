/**
 * @file local_music_service.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "local_music_service.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QtConcurrent/QtConcurrent>

#include "engines/sqlite_engine.h"
#include "parsers/parser_factory.h"
#include "utilities/string_helper.h"

LocalMusicService::LocalMusicService(QObject *parent) : BaseService(parent) {
    setName(tr("Local Musics"));
    setIcon("qrc:/assets/music.svg");
    mEngine = new SQLiteEngine(this);
    mEngine->open(
        QStandardPaths::writableLocation(QStandardPaths::DataLocation) +
        "/Database/localmusic.db");
}

LocalMusicService::~LocalMusicService() {
    mEngine->close();
    delete mEngine;
}

void LocalMusicService::loadSettings() {
    QSettings settings;
    settings.beginGroup("LocalMusicService");
    auto folderPaths = settings
                           .value("Folders", QStandardPaths::writableLocation(
                                                 QStandardPaths::MusicLocation))
                           .toString();
    settings.endGroup();
    // qDebug() << "folder paths: " << folderPaths;
    auto folderPathsList = folderPaths.split("::");
    for (auto &path : folderPathsList) {
        auto playlistName = (StringHelper::getHashedIdFromPath(path));
        mPlaylistMap.insert(playlistName, path);
        mServiceMap.insert(path, playlistName);
    }
}

void LocalMusicService::saveSettings() const {
    QSettings settings;
    settings.beginGroup("LocalMusicService");
    settings.setValue("Folders",
                      QVariant::fromValue(mPlaylistMap.values().join("::")));
    settings.endGroup();
}

void LocalMusicService::handleRefreshContentRequest(const QStringList &uri) {
    if (uri.size() == 1) {
        emit contentChanged();
    } else {
        auto res = QtConcurrent::run([this, uri]() {
            auto playlist = uri.last();
            auto playlistPath = mPlaylistMap.value(playlist);
            // qDebug() << playlist << " " << playlistPath;
            if (!playlistPath.isEmpty()) {
                auto files = QDir(playlistPath).entryList(QDir::Files);
                mEngine->dropMediaList(playlist);
                mEngine->createMediaList(playlist);
                for (auto &file : files) {
                    auto filePath = playlistPath + "/" + file;
                    auto parser = ParserFactory::getParser(filePath);
                    if (parser) {
                        auto media = parser->getMedia(filePath);
                        mEngine->addMedia(playlist, media);
                        // qDebug() << "add media: " << filePath;
                        parser->deleteLater();
                    }
                }
            }
        });
        auto *resWatcher = new QFutureWatcher<void>(this);
        resWatcher->setFuture(res);
        connect(resWatcher, &QFutureWatcher<void>::finished, [=]() {
            emit contentChanged();
            // qDebug() << "refresh content finished";
            resWatcher->deleteLater();
        });
    }
}

void LocalMusicService::handleCreatePageRequest(const QStringList &uri,
                                                const QString &page) {
    emit createIsCompleted(false);
}

void LocalMusicService::handleDeletePageRequest(const QStringList &uri,
                                                const QString &page) {
    emit deleteIsCompleted(false);
}

void LocalMusicService::handleSearchMediaRequest(const QString &keyword) {
    return;
}

void LocalMusicService::handleCreateMediaRequest(const QStringList &uri,
                                                 const Media &data) {
    emit createIsCompleted(false);
}

void LocalMusicService::handleUpdateMediaRequest(const QStringList &uri,
                                                 const Media &data) {
    emit updateIsCompleted(false);
}

void LocalMusicService::handleDeleteMediaRequest(const QStringList &uri,
                                                 const Media &data) {
    emit deleteIsCompleted(false);
}

void LocalMusicService::handleGetUriRequest(const QStringList &uri) {
    if (uri.size() == 1) {
        emit getSubPageIsCompleted(true, mServiceMap);
    } else {
        auto playlist_name = uri.last();
        auto playlist = mEngine->getMediaList(playlist_name);
        emit getMediaIsCompleted(true, playlist);
        // qDebug() << "get media list: " << playlist_name << " "
        //          << playlist.size();
    }
}

ServiceUriType LocalMusicService::getUriType(const QStringList &uri) {
    if (uri.size() == 1 && uri.first() == tr("Local Musics"))
        return ServiceUriType::Page;
    return ServiceUriType::Playlist;
}

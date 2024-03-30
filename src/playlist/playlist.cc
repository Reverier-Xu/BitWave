/**
 * @file playlist.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "playlist.h"

#include <QDebug>
#include <QMutex>

#include "storage/storage.h"
#include "utils/language_helper.h"

Playlist* Playlist::m_instance = nullptr;

Playlist::Playlist(QObject* parent) : QAbstractListModel(parent) {
    m_model = new MediaList(this);
    m_model->setList(&m_playlist);
    loadStorage();
}

Playlist::~Playlist() = default;

void Playlist::loadStorage() {
    auto playlists = Storage::instance()->loadPlaylists();
    beginResetModel();
    m_playlists = playlists;
    endResetModel();
    emit sizeChanged(m_playlists.size());
}

Playlist* Playlist::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Playlist(parent);
        locker.unlock();
    }
    return m_instance;
}

MediaList* Playlist::model() const { return m_model; }

SortStatus Playlist::sortStatus() const { return m_sortStatus; }

QString Playlist::current() const { return m_current; }

void Playlist::setCurrent(const QString& current) {
    if (m_current == current) {
        return;
    }
    m_current = current;
    emit currentChanged(m_current);
}

qsizetype Playlist::size() const { return m_playlists.size(); }

int Playlist::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    // qDebug() << "playlist size: " << m_playlists.size();
    return m_playlists.size();
}

QVariant Playlist::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_playlists.size()) {
        return QVariant();
    }

    if (role == NameRole) {
        return QVariant::fromValue(m_playlists[index.row()]);
    }

    return QVariant();
}

QHash<int, QByteArray> Playlist::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

void Playlist::addPlaylist(const QString& playlist) {
    beginInsertRows(QModelIndex(), m_playlists.size(), m_playlists.size());
    m_playlists.append(playlist);
    endInsertRows();
    emit sizeChanged(m_playlists.size());
    Storage::instance()->addPlaylist(playlist);
}

void Playlist::removePlaylist(const QString& playlist) {
    auto pos = m_playlists.indexOf(playlist);
    if (pos == -1) {
        return;
    }
    beginRemoveRows(QModelIndex(), pos, pos);
    m_playlists.removeAt(pos);
    endRemoveRows();
    emit sizeChanged(m_playlists.size());
    Storage::instance()->removePlaylist(playlist);
}

void Playlist::addMediaToPlaylist(const Media& media, const QString& playlist) {
    if (!m_playlists.contains(playlist)) {
        return;
    }
    Storage::instance()->linkMediaToPlaylist(media, playlist);
    if (m_current == playlist) {
        m_playlist.append(media);
        m_model->insertMedia(m_playlist.size() - 1);
    }
}

Q_INVOKABLE void Playlist::addMediaIndexToPlaylist(const int index,
                                                   const QString& playlist) {
    auto media = m_playlist.at(index);
    addMediaToPlaylist(media, playlist);
}

void Playlist::removeMediaFromPlaylist(const Media& media,
                                       const QString& playlist) {
    if (!m_playlists.contains(playlist)) {
        return;
    }
    Storage::instance()->removeMediaFromPlaylist(media, playlist);
    if (m_current == playlist) {
        qsizetype index;
        while ((index = m_playlist.indexOf(media)) != -1) {
            m_playlist.remove(index);
            m_model->removeMedia(index);
        }
    }
}

Q_INVOKABLE void Playlist::removeMediaIndex(const int index) {
    auto media = m_playlist.at(index);
    removeMediaFromPlaylist(media, m_current);
}

Q_INVOKABLE void Playlist::sortByTitle() {
    if (sortStatus() == TitleAsc)
        setSortStatus(TitleDesc);
    else
        setSortStatus(TitleAsc);
}

Q_INVOKABLE void Playlist::sortByArtists() {
    if (sortStatus() == ArtistsAsc)
        setSortStatus(ArtistsDesc);
    else
        setSortStatus(ArtistsAsc);
}

Q_INVOKABLE void Playlist::sortByAlbum() {
    if (sortStatus() == AlbumAsc)
        setSortStatus(AlbumDesc);
    else
        setSortStatus(AlbumAsc);
}

Q_INVOKABLE const QVector<Media>& Playlist::currentMedias() {
    return m_playlist;
}

Q_INVOKABLE void Playlist::reload() {
    beginResetModel();
    m_playlist.clear();
    auto medias = Storage::instance()->loadPlaylist(m_current);
    m_playlist.append(medias);
    endResetModel();
}

void Playlist::switchPlaylist(const QString& playlist) {
    m_current = playlist;
    m_playlist.clear();
    auto medias = Storage::instance()->loadPlaylist(playlist);
    m_playlist.append(medias);
    emit currentChanged(m_current);
}

void Playlist::setSortStatus(SortStatus sortStatus) {
    if (m_sortStatus == sortStatus) {
        return;
    }
    m_sortStatus = sortStatus;

    QVector<Media>& medias = m_playlist;
    switch (m_sortStatus) {
        case TitleAsc:
            std::sort(medias.begin(), medias.end(), cmpTitleAsc);
            break;
        case TitleDesc:
            std::sort(medias.begin(), medias.end(), cmpTitleDesc);
            break;
        case ArtistsAsc:
            std::sort(medias.begin(), medias.end(), cmpArtistsAsc);
            break;
        case ArtistsDesc:
            std::sort(medias.begin(), medias.end(), cmpArtistsDesc);
            break;
        case AlbumAsc:
            std::sort(medias.begin(), medias.end(), cmpAlbumAsc);
            break;
        case AlbumDesc:
            std::sort(medias.begin(), medias.end(), cmpAlbumDesc);
            break;
    }

    m_model->reload();

    emit sortStatusChanged(m_sortStatus);
}

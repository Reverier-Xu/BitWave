/**
 * @file library.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "library.h"
#include "parser/parser.h"
#include "storage/storage.h"
#include <QDebug>
#include <QMutex>
#include <QtConcurrent>


Library* Library::m_instance = nullptr;

Library::Library(QObject* parent) : QObject(parent) {
    m_model = new MediaList(this);
    m_model->setList(&m_musics);
    loadSettings();
    loadStorage();
}

Library::~Library() {
    saveSettings();
}

Library* Library::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Library(parent);
        locker.unlock();
    }
    return m_instance;
}

QString Library::filter() const {
    return m_filter;
}

void Library::setFilter(const QString& filter) {
    if (m_filter == filter) {
        return;
    }
    m_filter = filter;
    m_model->setList(filter == "music" ? &m_musics : &m_videos);
    emit filterChanged(m_filter);
}

void Library::load(const QString& route) {
    auto filter = route;
    filter.replace("libraries/", "");
    if (filter.isEmpty()) {
        filter = "music";
    }
    setFilter(filter);
}

void Library::scan() {
    setScanning(true);
    auto taskWatcher = new QFutureWatcher<void>(this);
    auto taskFuture = QtConcurrent::run([=]() {
        QStringList medias;
        for (auto& path : m_folders) {
            QDir dir(path);
            if (!dir.exists()) {
                continue;
            }
            auto files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            for (auto& file : files) {
                if (Parser::accept(file.absoluteFilePath()))
                    medias.append(file.absoluteFilePath());
            }
        }
        auto mediaWatcher = new QFutureWatcher<Media>();
        auto mediaFuture = QtConcurrent::mapped(medias, [=](const QString& path) {
            try {
                auto media = Parser::parse(path);
                return media;
            } catch (...) {
                return Media::null();
            }
        });
        mediaWatcher->setFuture(mediaFuture);
        mediaWatcher->waitForFinished();
        mediaWatcher->deleteLater();
        auto mediaResults = mediaFuture.results();
        auto mediaOrigins = m_musics;
        mediaOrigins << m_videos;
        QVector<Media> mediaNew;

        for (auto& media : mediaResults) {
            if (!mediaOrigins.contains(media)) {
                mediaNew.append(media);
            } else {
                mediaOrigins.removeAll(media);
            }
        }
        for (auto& media : mediaOrigins) {
            Storage::instance()->removeMedia(media);
            m_musics.removeAll(media);
            m_videos.removeAll(media);
        }
        for (auto& media : mediaNew) {
            if (media.type() == MUSIC) {
                m_musics.append(media);
            } else {
                m_videos.append(media);
            }
            Storage::instance()->addMedia(media);
        }
        std::sort(m_musics.begin(), m_musics.end());
    });
    taskWatcher->setFuture(taskFuture);
    connect(taskWatcher, &QFutureWatcher<void>::finished, this, [=]() {
        setScanning(false);
        m_model->reload();
//        qDebug() << "Library scanning finished";
        taskWatcher->deleteLater();
    });
}

void Library::loadSettings() {
    QSettings settings;
    settings.beginGroup("Library");
    m_folders =
        settings.value("Folders", QStandardPaths::standardLocations(QStandardPaths::MusicLocation)).toStringList();
    settings.endGroup();
}

void Library::saveSettings() {
    QSettings settings;
    settings.beginGroup("Library");
    settings.setValue("Folders", m_folders);
    settings.endGroup();
}

void Library::addFolderPath(const QString& path) {
    QUrl url(path);
    QDir dir(url.toLocalFile());
    if (!dir.exists()) {
        return;
    }
    auto cPath = dir.canonicalPath();
    if (m_folders.contains(cPath)) {
        return;
    }
    m_folders.append(cPath);
    emit foldersChanged(m_folders);
}

void Library::removeFolderPath(const QString& path) {
    m_folders.removeAll(path);
    emit foldersChanged(m_folders);
}

bool Library::scanning() const {
    return m_scanning;
}

void Library::setScanning(bool scanning) {
    if (m_scanning == scanning) {
        return;
    }
    m_scanning = scanning;
    emit scanningChanged(m_scanning);
}

QStringList Library::folders() const {
    return m_folders;
}

void Library::setFolders(const QStringList& folders) {
    m_folders = folders;
    emit foldersChanged(folders);
}

void Library::loadStorage() {
    auto medias = Storage::instance()->loadLibrary();
    for (auto& media : medias) {
        if (media.type() == MUSIC) {
            m_musics.append(media);
        } else {
            m_videos.append(media);
        }
    }
    std::sort(m_musics.begin(), m_musics.end());
    std::sort(m_videos.begin(), m_videos.end());
}

MediaList* Library::model() {
    return m_model;
}

void Library::sortByTitle() {
    if (sortStatus() == TitleAsc)
        setSortStatus(TitleDesc);
    else
        setSortStatus(TitleAsc);
}

void Library::sortByArtists() {
    if (sortStatus() == ArtistsAsc)
        setSortStatus(ArtistsDesc);
    else
        setSortStatus(ArtistsAsc);
}

void Library::sortByAlbum() {
    if (sortStatus() == AlbumAsc)
        setSortStatus(AlbumDesc);
    else
        setSortStatus(AlbumAsc);
}

SortStatus Library::sortStatus() const {
    return m_sortStatus;
}

bool cmpTitleAsc(const Media& m1, const Media& m2) {
    return m1.title() < m2.title();
}

bool cmpTitleDesc(const Media& m1, const Media& m2) {
    return m1.title() > m2.title();
}

bool cmpArtistsAsc(const Media& m1, const Media& m2) {
    return m1.artists() < m2.artists();
}

bool cmpArtistsDesc(const Media& m1, const Media& m2) {
    return m1.artists() > m2.artists();
}

bool cmpAlbumAsc(const Media& m1, const Media& m2) {
    return m1.album() < m2.album();
}

bool cmpAlbumDesc(const Media& m1, const Media& m2) {
    return m1.album() > m2.album();
}

void Library::setSortStatus(SortStatus sortStatus) {
    if (m_sortStatus == sortStatus) {
        return;
    }
    m_sortStatus = sortStatus;

    QVector<Media>& medias = filter() == "music" ? m_musics : m_videos;
    switch (m_sortStatus) {
        case TitleAsc:std::sort(medias.begin(), medias.end(), cmpTitleAsc);
            break;
        case TitleDesc:std::sort(medias.begin(), medias.end(), cmpTitleDesc);
            break;
        case ArtistsAsc:std::sort(medias.begin(), medias.end(), cmpArtistsAsc);
            break;
        case ArtistsDesc:std::sort(medias.begin(), medias.end(), cmpArtistsDesc);
            break;
        case AlbumAsc:std::sort(medias.begin(), medias.end(), cmpAlbumAsc);
            break;
        case AlbumDesc:std::sort(medias.begin(), medias.end(), cmpAlbumDesc);
            break;
    }

    m_model->reload();

    emit sortStatusChanged(m_sortStatus);
}

const QVector<Media>& Library::currentMedias() {
    return filter() == "music" ? m_musics : m_videos;
}

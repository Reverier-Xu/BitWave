/**
 * @file library.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include "models/media.h"
#include "models/media_list.h"


enum SortStatus {
    TitleAsc = 0,
    TitleDesc,
    ArtistsAsc,
    ArtistsDesc,
    AlbumAsc,
    AlbumDesc,
};

class Library : public QObject {
   Q_OBJECT

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(QStringList folders READ folders WRITE setFolders NOTIFY foldersChanged)
    Q_PROPERTY(SortStatus sortStatus READ sortStatus WRITE setSortStatus NOTIFY sortStatusChanged)

   private:
    QVector<Media> m_musics{};

    QVector<Media> m_videos{};

    MediaList* m_model;

    QStringList m_folders{};

    QString m_filter{};

    bool m_scanning = false;

    SortStatus m_sortStatus = TitleAsc;

   protected:
    static Library* m_instance;

    explicit Library(QObject* parent = nullptr);

    ~Library() override;

   public:

    static Library* instance(QObject* parent = nullptr);

    void loadSettings();

    void loadStorage();

    void saveSettings();

    [[nodiscard]] MediaList* model();

    [[nodiscard]] QString filter() const;

    void setFilter(const QString& filter);

    [[nodiscard]] bool scanning() const;

    void setScanning(bool scanning);

    [[nodiscard]] QStringList folders() const;

    void setFolders(const QStringList& folders);

    [[nodiscard]] SortStatus sortStatus() const;

    void setSortStatus(SortStatus sortStatus);

   public slots:

    Q_INVOKABLE void load(const QString& route);

    Q_INVOKABLE void scan();

    Q_INVOKABLE void addFolderPath(const QString& path);

    Q_INVOKABLE void removeFolderPath(const QString& path);

    Q_INVOKABLE void sortByTitle();

    Q_INVOKABLE void sortByArtists();

    Q_INVOKABLE void sortByAlbum();

    Q_INVOKABLE const QVector<Media>& currentMedias();

   signals:

    void filterChanged(const QString& filter);

    void scanningChanged(bool scanning);

    void foldersChanged(const QStringList& folders);

    void sortStatusChanged(SortStatus sortStatus);
};

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
#include <qtmetamacros.h>

#include "models/media.h"
#include "models/media_list.h"

class Library : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(bool recursiveScanning READ recursiveScanning WRITE setRecursiveScanning NOTIFY recursiveScanningChanged)
    Q_PROPERTY(QStringList folders READ folders WRITE setFolders NOTIFY foldersChanged)
    Q_PROPERTY(SortStatus sortStatus READ sortStatus WRITE setSortStatus NOTIFY sortStatusChanged)
    Q_PROPERTY(QString searchKeyword READ searchKeyword WRITE setSearchKeyword NOTIFY searchKeywordChanged)

  private:
    QVector<Media> m_musics{};

    QVector<Media> m_videos{};

    QVector<Media> m_searchResult{};

    MediaList* m_model;

    MediaList* m_searchModel;

    QStringList m_folders{};

    QString m_filter{};

    bool m_scanning = false;

    bool m_recursiveScanning = false;

    SortStatus m_sortStatus = TitleAsc;

    QString m_searchKeyword{};

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

    [[nodiscard]] MediaList* searchModel();

    [[nodiscard]] QString filter() const;

    void setFilter(const QString& filter);

    [[nodiscard]] bool scanning() const;

    void setScanning(bool scanning);

    [[nodiscard]] QStringList folders() const;

    void setFolders(const QStringList& folders);

    [[nodiscard]] SortStatus sortStatus() const;

    void setSortStatus(SortStatus sortStatus);

    [[nodiscard]] bool recursiveScanning() const;

    void setRecursiveScanning(bool n);

    [[nodiscard]] QString searchKeyword() const;

    void setSearchKeyword(const QString& keyword);

  public slots:

    Q_INVOKABLE void load(const QString& route);

    Q_INVOKABLE void scan();

    Q_INVOKABLE void addFolderPath(const QString& path);

    Q_INVOKABLE void removeFolderPath(const QString& path);

    Q_INVOKABLE void sortByTitle();

    Q_INVOKABLE void sortByArtists();

    Q_INVOKABLE void sortByAlbum();

    Q_INVOKABLE const QVector<Media>& currentMedias();

    Q_INVOKABLE void addMediaIndexToPlaylist(const int index, const QString& playlist);

  signals:

    void filterChanged(const QString& filter);

    void scanningChanged(bool scanning);

    void foldersChanged(const QStringList& folders);

    void sortStatusChanged(SortStatus sortStatus);

    void recursiveScanningChanged(bool recursiveScanning);

    void searchKeywordChanged(const QString& keyword);
};

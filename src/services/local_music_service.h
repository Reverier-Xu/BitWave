/**
 * @file local_music_service.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "base_service.h"
#include "engines/sqlite_engine.h"

class LocalMusicService : public BaseService {
    Q_OBJECT
   private:
    QMap<QString, QString> mPlaylistMap{};
    QMap<QString, QString> mServiceMap{};
    SQLiteEngine *mEngine;

   public:
    explicit LocalMusicService(QObject *parent = nullptr);

    ~LocalMusicService() override;

    void loadSettings() override;

    void saveSettings() const override;

   public slots:
    void handleRefreshContentRequest(const QStringList &uri) override;

    void handleCreatePageRequest(const QStringList &uri,
                                 const QString &page) override;

    void handleDeletePageRequest(const QStringList &uri,
                                 const QString &page) override;

    void handleSearchMediaRequest(const QString &keyword) override;

    void handleCreateMediaRequest(const QStringList &uri,
                                  const Media &data) override;

    void handleUpdateMediaRequest(const QStringList &uri,
                                  const Media &data) override;

    void handleDeleteMediaRequest(const QStringList &uri,
                                  const Media &data) override;

    void handleGetUriRequest(const QStringList &uri) override;

    ServiceUriType getUriType(const QStringList &uri) override;
};

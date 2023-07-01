/**
 * @file router.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-10
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QStack>

/**
 * @brief The Router class
 *
 * PlayerView: `player`
 * LibraryView: `libraries/${LIBRARY_ID}/${LIBRARY_VIEW_TYPE}`
 * PlaylistView: `playlists` `playlists/${PLAYLIST_ID}`
 * SettingsView: `settings`
 * SearchView: `search` `search/${KEYWORD}`
 * ExploreView: `explore` `explore/${EXPLORE_TYPE}`
 */

class Router : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currentRoute READ currentRoute WRITE setCurrentRoute
                   NOTIFY currentRouteChanged)
    Q_PROPERTY(bool hasPrevious READ hasPrevious WRITE setHasPrevious NOTIFY
                   hasPreviousChanged)
   private:
    QStack<QString> m_history;

    QString m_currentRoute;

   public:
    explicit Router(QObject* parent = nullptr);

    ~Router() override;

    [[nodiscard]] QString currentRoute() const;

    void setCurrentRoute(const QString& path);

    [[nodiscard]] bool hasPrevious() const;

    void setHasPrevious(bool hasPrevious);

   public slots:
    Q_INVOKABLE void push(const QString& path);

    Q_INVOKABLE void pop();

    Q_INVOKABLE void clear();

   signals:

    void currentRouteChanged(const QString& path);

    void hasPreviousChanged(bool hasPrevious);
};

/**
 * @file play_queue.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-17
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QtQmlIntegration>

#include "models/media.h"
#include "models/media_list.h"


enum QueueMode {
    IN_ORDER = 0,
    REVERSED = 1,
    LOOP_ALL = 2,
    LOOP_ONE = 3,
    RANDOM = 4,
};

class PlayQueue : public QObject {
    Q_OBJECT

    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(int cursor READ cursor WRITE setCursor NOTIFY cursorChanged)
    Q_PROPERTY(QString modeIcon READ modeIcon NOTIFY modeIconChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
   private:
    QueueMode m_mode{IN_ORDER};

    QVector<Media>* m_playlist{};

    MediaList* m_model{};

    int m_cursor{0};

    int m_cursorPos{0};

    QVector<int> m_queuedPlayOrder{};

    bool m_loading{false};

    bool m_saving{false};

    void loadPlaylist_(const QVector<Media>& playlist);

   public:
    explicit PlayQueue(QObject* parent = nullptr);

    ~PlayQueue() override;

    void loadSettings();

    void saveSettings() const;

    void loadStorage();

    void saveStorage();

    [[nodiscard]] int mode() const;

    [[nodiscard]] QString modeIcon() const;

    void setMode(int mode);

    [[nodiscard]] bool loading() const;

    void setLoading(bool n);

    [[nodiscard]] QVector<Media>* queue();

    [[nodiscard]] MediaList* model();

    [[nodiscard]] int cursor() const;

    void setCursor(int n);

    [[nodiscard]] const Media& current();

   public slots:
    Q_INVOKABLE void addMedia(const Media& media);

    Q_INVOKABLE void addMediaByUrl(const QString& url);

    Q_INVOKABLE void addMediasByUrlsThenPlay(const QList<QUrl>& urls);

    Q_INVOKABLE void removeMedia(int pos);

    Q_INVOKABLE void clearMedia();

    Q_INVOKABLE void moveMedia(int from, int to);

    Q_INVOKABLE void loadPlaylist(const QVector<Media>& playlist);

    Q_INVOKABLE void toggleChangeMode();

    Q_INVOKABLE void next();

    Q_INVOKABLE void prev();

    Q_INVOKABLE void play(int pos);

    Q_INVOKABLE void loadFromLibrary(int pos);

   signals:

    void modeChanged(int mode);

    void cursorChanged(int n);

    void mediaChanged(Media media);

    void modeIconChanged(QString icon);

    void loadingChanged(bool n);
};

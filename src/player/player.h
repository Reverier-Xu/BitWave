/**
 * @file player.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QImage>
#include <QObject>

#include "engine.h"
#include "models/media.h"
#include "play_queue.h"
#include "dbus/screensaver.h"

class Player : public QObject {
    Q_OBJECT

    Q_PROPERTY(Media media READ media WRITE setMedia NOTIFY mediaChanged)
    Q_PROPERTY(QImage cover READ cover WRITE setCover NOTIFY coverChanged)
    Q_PROPERTY(QString coverPath READ coverPath WRITE setCoverPath NOTIFY
                   coverPathChanged)

    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(bool coverLoading READ coverLoading WRITE setCoverLoading NOTIFY
                   coverLoadingChanged)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(bool ended READ ended WRITE setEnded NOTIFY endedChanged)
    Q_PROPERTY(double totalTime READ totalTime WRITE setTotalTime NOTIFY
                   totalTimeChanged)
    Q_PROPERTY(double currentTime READ currentTime WRITE setCurrentTime NOTIFY
                   currentTimeChanged)

    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
   private:
    Media m_media;

    Screensaver* m_screensaver{};

    QImage m_cover{};

    QString m_coverPath{};

    bool m_playing{false};

    bool m_loading{false};

    bool m_coverLoading{false};

    bool m_valid{false};

    bool m_ended{false};

    double m_totalTime{};

    double m_currentTime{};

    double m_volume{100};

    bool m_muted{false};

    Engine* m_engine{};

    PlayQueue* m_queue{};

    int m_taskId{0};

   protected:
    static Player* m_instance;

    explicit Player(QObject* parent = nullptr);

    void connectSignals();

    ~Player() override;

    void chooseRandomCover();

   public:
    [[nodiscard]] static Player* instance(QObject* parent = nullptr);

    [[nodiscard]] Engine* engine() const;

    void loadSettings();

    void saveSettings() const;

    [[nodiscard]] Media media() const;

    void setMedia(const Media& n);

    [[nodiscard]] QImage cover() const;

    void setCover(const QImage& n);

    [[nodiscard]] QString coverPath() const;

    void setCoverPath(const QString& n);

    [[nodiscard]] bool playing() const;

    void setPlaying(bool n);

    [[nodiscard]] bool loading() const;

    void setLoading(bool n);

    [[nodiscard]] bool coverLoading() const;

    void setCoverLoading(bool n);

    [[nodiscard]] bool valid() const;

    void setValid(bool n);

    [[nodiscard]] bool ended() const;

    void setEnded(bool n);

    [[nodiscard]] double totalTime() const;

    void setTotalTime(double n);

    [[nodiscard]] double currentTime() const;

    void setCurrentTime(double n);

    [[nodiscard]] double volume() const;

    void setVolume(double n);

    [[nodiscard]] bool muted() const;

    void setMuted(bool n);

    [[nodiscard]] PlayQueue* queue() const;

   public slots:
    Q_INVOKABLE void play(const Media& media);

    Q_INVOKABLE void playUrl(const QString& url);

    Q_INVOKABLE void resume();

    Q_INVOKABLE void pause();

    Q_INVOKABLE void togglePause();

    Q_INVOKABLE void seek(double n);

    Q_INVOKABLE void toggleMute();

    Q_INVOKABLE void toggleVolume(double n);

    Q_INVOKABLE void reset();

    Q_INVOKABLE void saveMediaCover(const QUrl& url);

   signals:

    void mediaChanged(const Media& n);

    void playingChanged(bool n);

    void loadingChanged(bool n);

    void coverLoadingChanged(bool n);

    void validChanged(bool n);

    void endedChanged(bool n);

    void totalTimeChanged(double n);

    void currentTimeChanged(double n);

    void volumeChanged(double n);

    void mutedChanged(bool n);

    void coverChanged(QImage n);

    void coverPathChanged(QString n);

    void userSeeked(double n);

    void stateChanged();
};

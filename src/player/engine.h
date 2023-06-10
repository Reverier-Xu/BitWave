/**
 * @file engine.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>

#include <mpv/client.h>

#include <QMimeData>
#include <QMimeDatabase>
#include <QObject>
#include <QtQuick/QQuickFramebufferObject>

class Engine : public QObject {
   Q_OBJECT

   private:
    mpv_handle* m_mpvHandle;

    void handleMpvEvent(mpv_event* event);

    QMimeDatabase mMimeDatabase;

    void loadFile(const QString& path);

   public:
    explicit Engine(QObject* parent = nullptr);

    Engine(Engine& other) = delete;

    [[nodiscard]] mpv_handle* getMpvHandle() { return m_mpvHandle; }

    Q_INVOKABLE QVariant command(const QVariant& params);

    Q_INVOKABLE void setMpvProperty(const QString& name, const QVariant& value);

    [[nodiscard]] Q_INVOKABLE QVariant getMpvProperty(const QString& name) const;

    Q_INVOKABLE void play(const QString& path);

    Q_INVOKABLE void resume();

    Q_INVOKABLE void pause();

    Q_INVOKABLE void stop();

    Q_INVOKABLE void seek(double secs);

    Q_INVOKABLE void setMute(bool ok);

    Q_INVOKABLE void setVolume(double vol);

   public slots:

    void onMpvEvents();

   signals:

    void totalTimeChanged(double secs);

    void currentTimeChanged(double secs);

    void volumeChanged(double vol);

    void started();

    void paused();

    void resumed();

    void ended();
};

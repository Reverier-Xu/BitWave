/* 
 * mpv_engine.h
 *
 * Summary: wrapper of mpv engine.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-01-06
 * Last Modified: 2021-03-14
 * 
 */


#pragma once

#include <mpv/client.h>

#include <QMimeData>
#include <QMimeDatabase>
#include <QObject>
#include <QtQuick/QQuickFramebufferObject>

#include "utilities/mpv_helper.h"

class MpvEngine : public QObject {
    Q_OBJECT

   private:
    mpv_handle *mpv;

    void handleMpvEvent(mpv_event *event);

    explicit MpvEngine(QObject *parent = nullptr);

    QMimeDatabase type_db;

   protected:
    static MpvEngine *mMpvEngine;

   public:
    MpvEngine(MpvEngine &other) = delete;

    void operator=(const MpvEngine &) = delete;

    [[nodiscard]] static MpvEngine *instance(QObject *parent = nullptr);

    [[nodiscard]] mpv_handle *getMpvHandle() { return this->mpv; }

    Q_INVOKABLE QVariant command(const QVariant &params);

    Q_INVOKABLE void setProperty(const QString &name, const QVariant &value);

    [[nodiscard]] Q_INVOKABLE QVariant getProperty(const QString &name) const;

    Q_INVOKABLE void playMedia(const QString &path);

    Q_INVOKABLE void resume();

    Q_INVOKABLE void pause();

    Q_INVOKABLE void stop();

    Q_INVOKABLE void setTimePos(double msecs);

    Q_INVOKABLE void setMute(bool ok);

   public slots:

    void onMpvEvents();

   signals:
    void durationChanged(double msecs);
    void positionChanged(double msecs);
    void volumeChanged(double vol);
    void started();
    void paused();
    void ended();
    void resumed();
    void newMusicOpened();
    void newVideoOpened();
};

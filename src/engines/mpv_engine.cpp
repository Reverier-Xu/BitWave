/**
 * @file mpv_engine.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "mpv_engine.h"

#include <QDebug>

#include "utilities/mpv_helper.h"

MpvEngine *MpvEngine::mMpvEngine = nullptr;

static void wakeup(void *ctx) {
    // qDebug() << "[BitWave] wakeup!!!";
    QMetaObject::invokeMethod((MpvEngine *)ctx, "onMpvEvents",
                              Qt::QueuedConnection);
}

MpvEngine::MpvEngine(QObject *parent) : QObject(parent) {
    mMpv = mpv_create();
    if (!mMpv) throw std::runtime_error("could not create mpv context");

    mpv_set_option_string(mMpv, "terminal", "yes");
    mpv_set_option_string(mMpv, "msg-level", "all=warn");
    mpv_set_option_string(mMpv, "audio-display", "no");
    mpv_set_option_string(mMpv, "keep-open", "no");
    mpv_set_option_string(mMpv, "idle", "yes");
    // mpv_set_option_string(mpv, "demuxer-max-back-bytes", "32MiB");
    // mpv_set_option_string(mpv, "demuxer-max-bytes", "32MiB");
    // mpv_set_option_string(mpv, "cache", "no");
    mpv_set_property_string(mMpv, "audio-client-name", "BitWave");

    if (mpv_initialize(mMpv) < 0)
        throw std::runtime_error("could not initialize mpv context");

    // Request hw decoding, just for testing.
    mpv::qt::set_option_variant(mMpv, "hwdec", "auto");

    mpv_observe_property(mMpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mMpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mMpv, 0, "media-title", MPV_FORMAT_STRING);
    mpv_set_wakeup_callback(mMpv, wakeup, this);
}

void MpvEngine::handleMpvEvent(mpv_event *event) {
    switch (event->event_id) {
        // qDebug() << event->event_id;
        case MPV_EVENT_PROPERTY_CHANGE: {
            auto *prop = (mpv_event_property *)event->data;
            if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double *)prop->data;
                    emit positionChanged(time);
                    // qDebug() << "cursor: " << time;
                }
            } else if (strcmp(prop->name, "duration") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double *)prop->data;
                    emit durationChanged(time);
                    // qDebug() << "total: " << time;
                }
            } else if (strcmp(prop->name, "volume") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double volume = *(double *)prop->data;
                    emit volumeChanged(volume);
                    // qDebug() << "total: " << time;
                }
            }
            break;
        }
        /**
         * Notification before playback start of a file (before the file is
         * loaded). See also mpv_event and mpv_event_start_file.
         */
        case MPV_EVENT_START_FILE:
            emit resumed();
            emit started();
            break;
        case MPV_EVENT_END_FILE:
            emit paused();
            emit ended();
            break;
        case MPV_EVENT_UNPAUSE:
            emit resumed();
            break;
        case MPV_EVENT_PAUSE:
            emit paused();
            break;

        default:;
            // Ignore uninteresting or unknown events.
    }
}

void MpvEngine::onMpvEvents() {
    // qDebug() << "[BitWave] handle events!!!";
    // Process all events, until the event queue is empty.
    while (mMpv) {
        mpv_event *event = mpv_wait_event(mMpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        handleMpvEvent(event);
    }
}

QVariant MpvEngine::command(const QVariant &params) {
    return mpv::qt::command_variant(mMpv, params);
}

void MpvEngine::setProperty(const QString &name, const QVariant &value) {
    mpv::qt::set_property_variant(mMpv, name, value);
}

void MpvEngine::setOption(const QString &name, const QVariant &value) {
    mpv::qt::set_option_variant(mMpv, name, value);
}

QVariant MpvEngine::getProperty(const QString &name) const {
    return mpv::qt::get_property_variant(mMpv, name);
}

MpvEngine *MpvEngine::instance(QObject *parent) {
    if (mMpvEngine == nullptr) {
        mMpvEngine = new MpvEngine(parent);
    }
    if (parent != nullptr) mMpvEngine->setParent(parent);
    return mMpvEngine;
}

void MpvEngine::playMedia(const QString &path) {
    auto mediaType = mMimeDatabase.mimeTypeForFile(path);
    if (mediaType.name().startsWith("audio/")) {
        emit newMusicOpened();
    } else if (mediaType.name().startsWith("video/")) {
        emit newVideoOpened();
    }
    command(QStringList() << "loadfile" << path);
}

void MpvEngine::resume() { setProperty("pause", false); }

void MpvEngine::pause() { setProperty("pause", true); }

void MpvEngine::stop() {
    command(QStringList() << "stop");
    // command(QStringList() << "drop-buffers");
}

void MpvEngine::setTimePos(double secs) { setProperty("time-pos", secs); }

void MpvEngine::setMute(bool ok) { setProperty("mute", ok ? "yes" : "no"); }

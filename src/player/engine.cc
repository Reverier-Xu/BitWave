/**
 * @file engine.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "engine.h"

#include <utils/mpv_helper.h>

static void wakeup(void* ctx) {
    QMetaObject::invokeMethod((Engine*)ctx, &Engine::onMpvEvents,
                              Qt::QueuedConnection);
}

Engine::Engine(QObject* parent) : QObject(parent) {
    m_mpvHandle = mpv_create();
    if (!m_mpvHandle) throw std::runtime_error("could not create mpv context");

    setMpvProperty("terminal", "yes");
    setMpvProperty("msg-level", "all=warn");
    setMpvProperty("audio-display", "no");
    setMpvProperty("keep-open", "no");
    setMpvProperty("idle", "yes");
    setMpvProperty("audio-client-name", "Bit Wave");
    setMpvProperty("pause", true);

    if (mpv_initialize(m_mpvHandle) < 0)
        throw std::runtime_error("could not initialize mpv context");

    // Request hardware decoding, maybe useful on some laptops.
    setMpvProperty("hwdec", "auto");

    mpv_observe_property(m_mpvHandle, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpvHandle, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpvHandle, 0, "pause", MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpvHandle, 0, "volume", MPV_FORMAT_DOUBLE);
    mpv_set_wakeup_callback(m_mpvHandle, wakeup, this);
}

void Engine::handleMpvEvent(mpv_event* event) {
    switch (event->event_id) {
        // qDebug() << event->event_id;
        case MPV_EVENT_PROPERTY_CHANGE: {
            auto* prop = (mpv_event_property*)event->data;
            if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double*)prop->data;
                    emit currentTimeChanged(time);
                    //                     qDebug() << "cursor: " << time;
                }
            } else if (strcmp(prop->name, "duration") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double time = *(double*)prop->data;
                    emit totalTimeChanged(time);
                    //                     qDebug() << "total: " << time;
                }
            } else if (strcmp(prop->name, "volume") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    double volume = *(double*)prop->data;
                    emit volumeChanged(volume);
                    //                    qDebug() << "volume changed: " <<
                    //                    volume;
                }
            } else if (strcmp(prop->name, "pause") == 0) {
                if (prop->format == MPV_FORMAT_FLAG) {
                    bool isPaused = *(bool*)prop->data;
                    if (isPaused) {
                        emit paused();
                    } else {
                        emit resumed();
                    }
                }
            }
            break;
        }
        case MPV_EVENT_FILE_LOADED:
            emit started();
            break;
        case MPV_EVENT_END_FILE: {
            auto* eventPtr = (mpv_event_end_file*)event->data;
            if (eventPtr->reason != MPV_END_FILE_REASON_STOP &&
                eventPtr->reason != MPV_END_FILE_REASON_REDIRECT) {
                emit ended();
            }
        } break;
        default:;
            // Ignore uninteresting or unknown events.
            break;
    }
}

void Engine::setMute(bool ok) { setMpvProperty("mute", ok ? "yes" : "no"); }

void Engine::setVolume(double volume) { setMpvProperty("volume", volume); }

void Engine::onMpvEvents() {
    // qDebug() << "[BitWave] handle events!!!";
    // Process all events, until the event queue is empty.
    while (m_mpvHandle) {
        mpv_event* event = mpv_wait_event(m_mpvHandle, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        handleMpvEvent(event);
    }
}

QVariant Engine::command(const QVariant& params) {
    return mpv::qt::command(m_mpvHandle, params);
}

void Engine::setMpvProperty(const QString& name, const QVariant& value) {
    mpv::qt::set_property(m_mpvHandle, name, value);
}

QVariant Engine::getMpvProperty(const QString& name) const {
    return mpv::qt::get_property(m_mpvHandle, name);
}

void Engine::play(const QString& path) {
    loadFile(path);
    resume();
}

void Engine::resume() { setMpvProperty("pause", false); }

void Engine::pause() { setMpvProperty("pause", true); }

void Engine::seek(double secs) { setMpvProperty("time-pos", secs); }

void Engine::loadFile(const QString& path) {
    command(QStringList() << "loadfile" << path);
}

void Engine::stop() { command(QStringList() << "stop"); }

/**
 * @file mpris2.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "mpris2.h"

#include <QApplication>
#include <QDBusConnection>
#include <QtMath>

#include "dbus/mpris2_player.h"
#include "dbus/mpris2_root.h"
#include "mpris_common.h"
#include "player/player.h"
#include "ui/ui.h"

namespace mpris {

const char *Mpris2::kMprisObjectPath = "/org/mpris/MediaPlayer2";
const char *Mpris2::kServiceName = "org.mpris.MediaPlayer2.BitWave";
const char *Mpris2::kFreedesktopPath = "org.freedesktop.DBus.Properties";

Mpris2::Mpris2(QObject *parent) : QObject(parent) {
    new Mpris2Root(this);
    new Mpris2Player(this);

    if (!QDBusConnection::sessionBus().registerService(kServiceName)) {
        return;
    }

    QDBusConnection::sessionBus().registerObject(kMprisObjectPath, this);

    connect(Player::instance(), &Player::coverPathChanged,
            [=](const QString &n) {
                MetadataLoaded(Player::instance()->media(), n);
            });
    connect(Player::instance(), &Player::volumeChanged, this,
            &Mpris2::VolumeChanged);
    connect(Player::instance(), &Player::userSeeked, this,
            [=](double n) { emit Seeked((qint64)(floor(n)) * 1000 * 1000); });
    connect(Player::instance(), &Player::mediaChanged, this,
            &Mpris2::CurrentSongChanged);
    connect(Player::instance(), &Player::stateChanged, this,
            &Mpris2::EngineStateChanged);
}

Mpris2::~Mpris2() {
    EmitNotification("PlaybackStatus", "Stopped");
    EmitNotification("Metadata", QVariantMap());
    QDBusConnection::sessionBus().unregisterService(kServiceName);
}

void Mpris2::VolumeChanged() { EmitNotification("Volume"); }

void Mpris2::EmitNotification(const QString &name, const QVariant &val) {
    EmitNotification(name, val, "org.mpris.MediaPlayer2.Player");
}

void Mpris2::EmitNotification(const QString &name, const QVariant &val,
                              const QString &mprisEntity) {
    QDBusMessage msg = QDBusMessage::createSignal(
        kMprisObjectPath, kFreedesktopPath, "PropertiesChanged");
    QVariantMap map;
    map.insert(name, val);
    //     qDebug() << "Name and Val to MPRIS2: " << name << val;
    //     qDebug() << "Map to MPRIS2: " << map;
    msg << mprisEntity << map << QStringList();
    //     qDebug() << "FINAL msg to MPRIS2: " << msg.arguments();
    QDBusConnection::sessionBus().send(msg);
}

void Mpris2::EmitNotification(const QString &name) {
    QVariant value;
    if (name == "Metadata")
        value = Metadata();
    else if (name == "PlaybackStatus")
        value = PlaybackStatus();
    else if (name == "LoopStatus")
        value = LoopStatus();
    else if (name == "Volume")
        value = Volume();
    else if (name == "Position")
        value = Position();
    else if (name == "CanGoNext")
        value = CanGoNext();
    else if (name == "CanGoPrevious")
        value = CanGoPrevious();
    else if (name == "CanSeek")
        value = CanSeek();
    else if (name == "CanPlay")
        value = CanPlay();
    else if (name == "CanPause")
        value = CanPause();

    if (value.isValid()) EmitNotification(name, value);
}

bool Mpris2::CanQuit() { return true; }

bool Mpris2::CanRaise() { return true; }

bool Mpris2::HasTrackList() { return false; }

QString Mpris2::Identity() { return QApplication::applicationName(); }

QString Mpris2::DesktopEntryAbsolutePath() {
    QStringList xdg_data_dirs = QString(getenv("XDG_DATA_DIRS")).split(":");
    xdg_data_dirs.append("/usr/local/share/");
    xdg_data_dirs.append("/usr/share/");

    for (const QString &directory : xdg_data_dirs) {
        QString path =
            QString("%1/applications/%2.desktop")
                .arg(directory, QApplication::applicationName().toLower());
        if (QFile::exists(path)) return path;
    }
    return QString();
}

QString Mpris2::DesktopEntry() {
    return QApplication::applicationName().toLower();
}

QStringList Mpris2::SupportedUriSchemes() {
    static QStringList res = QStringList() << "file"
                                           << "http";
    return res;
}

QStringList Mpris2::SupportedMimeTypes() {
    static QStringList res = QStringList() << "application/ogg"
                                           << "application/x-ogg"
                                           << "application/x-ogm-audio"
                                           << "audio/aac"
                                           << "audio/mp4"
                                           << "audio/mpeg"
                                           << "audio/mpegurl"
                                           << "audio/ogg"
                                           << "audio/vnd.rn-realaudio"
                                           << "audio/vorbis"
                                           << "audio/x-ape"
                                           << "audio/x-flac"
                                           << "audio/x-mp3"
                                           << "audio/x-mpeg"
                                           << "audio/x-mpegurl"
                                           << "audio/x-ms-wma"
                                           << "audio/x-musepack"
                                           << "audio/x-oggflac"
                                           << "audio/x-pn-realaudio"
                                           << "audio/x-scpls"
                                           << "audio/x-speex"
                                           << "audio/x-vorbis"
                                           << "audio/x-vorbis+ogg"
                                           << "audio/x-wav"
                                           << "video/x-ms-asf"
                                           << "x-content/audio-player";
    return res;
}

void Mpris2::Raise() { Ui::instance()->onSecondaryInstanceStarted(); }

void Mpris2::Quit() { QApplication::quit(); }

double Mpris2::Rate() { return 1.0; }

void Mpris2::SetRate(double rate) {
    if (rate == 0) {
        Player::instance()->pause();
    }
}

QVariantMap Mpris2::Metadata() const { return m_lastMetadata; }

// We send Metadata change notification as soon as the process of
// changing song starts...
void Mpris2::CurrentSongChanged(const Media &song) {
    MetadataLoaded(song, "");
    EmitNotification("CanGoNext", CanGoNext());
    EmitNotification("CanGoPrevious", CanGoPrevious());
    EmitNotification("CanSeek", CanSeek());
    EmitNotification("Rate", 1.0);
    EmitNotification("Volume", 1.0);
}

// ... and we add the cover information later, when it's available.
void Mpris2::MetadataLoaded(const Media &song, const QString &art_uri) {
    m_lastMetadata = QVariantMap();

    using mpris::AddMetadata;

    AddMetadata("xesam:artist", song.artists(), &m_lastMetadata);
    AddMetadata("xesam:title", song.title(), &m_lastMetadata);
    AddMetadata("xesam:album", song.album(), &m_lastMetadata);
    AddMetadata("xesam:url", song.url(), &m_lastMetadata);
    AddMetadata("mpris:length", (qint64)floor(song.time() * 1000000.0), &m_lastMetadata);
    AddMetadata("mpris:trackid", QDBusObjectPath("/org/mpris/MediaPlayer2/TrackList/NoTrack"),
                &m_lastMetadata);

    if (!art_uri.isEmpty()) {
        AddMetadata("mpris:artUrl", art_uri, &m_lastMetadata);
    }

    EmitNotification("Metadata", m_lastMetadata);
}

double Mpris2::Volume() const { return Player::instance()->volume(); }

void Mpris2::SetVolume(double value) { Player::instance()->setVolume(value); }

qint64 Mpris2::Position() const {
    return qint64(Player::instance()->currentTime() * 1000 * 1000);
}

double Mpris2::MaximumRate() { return 1.0; }

double Mpris2::MinimumRate() { return 1.0; }

bool Mpris2::CanGoNext() { return true; }

bool Mpris2::CanGoPrevious() { return true; }

bool Mpris2::CanPlay() const {
    // qDebug() << "CanPlay: " << (Player::instance()->valid() && !Player::instance()->playing());
    return Player::instance()->valid();
}

// This one's a bit different from MPRIS 1 - we want this to be true even when
// the song is already paused or stopped.
bool Mpris2::CanPause() const {
    // qDebug() << "CanPause: " << (Player::instance()->valid() && Player::instance()->playing());
    return Player::instance()->valid();
}

bool Mpris2::CanSeek() const {
    // qDebug() << "CanSeek";
    return true;
}

bool Mpris2::CanControl() const { return true; }

void Mpris2::Next() {
    // qDebug() << "Next";
    Player::instance()->queue()->next();
}

void Mpris2::Previous() {
    // qDebug() << "Previous";
    Player::instance()->queue()->prev();
}

void Mpris2::Pause() {
    // qDebug() << "Pause";
    if (CanPause()) {
        Player::instance()->pause();
    }
}

void Mpris2::PlayPause() {
    // qDebug() << "PlayPause";
    if (CanPause()) {
        Player::instance()->pause();
    } else if (CanPlay()) {
        Player::instance()->resume();
    }
}

void Mpris2::Stop() {
    // qDebug() << "Stop";
    Player::instance()->reset();
}

void Mpris2::Play() {
    // qDebug() << "Play";
    if (CanPlay()) {
        Player::instance()->resume();
    }
}

void Mpris2::OpenUri(const QString &uri) {
    Player::instance()->queue()->addMediaByUrl(uri);
}

bool Mpris2::CanSetFullscreen() const { return false; }

bool Mpris2::Fullscreen() const {
    return Ui::instance()->uiConfig()->fullscreen();
}

QString Mpris2::PlaybackStatus() const {
    if (Player::instance()->valid()) {
        if (Player::instance()->playing())
            return "Playing";
        else
            return "Paused";
    } else {
        return "Stopped";
    }
}

QString Mpris2::LoopStatus() const { return "None"; }

void Mpris2::EngineStateChanged() {
    //         qDebug() << "engine state changed";
    if (!Player::instance()->valid()) {
        m_lastMetadata = QVariantMap();
        EmitNotification("Metadata", m_lastMetadata);
    }
    if (m_lastMetadata.isEmpty() && Player::instance()->valid()) {
        MetadataLoaded(Player::instance()->media(),
                       Player::instance()->coverPath());
    }
    EmitNotification("PlaybackStatus", PlaybackStatus());
    EmitNotification("LoopStatus", LoopStatus());

    EmitNotification("CanPlay", CanPlay());
    EmitNotification("CanPause", CanPause());
    EmitNotification("CanGoNext", CanGoNext());
    EmitNotification("CanGoPrevious", CanGoPrevious());
    EmitNotification("CanSeek", CanSeek());
}

void Mpris2::Seek(qint64 offset) {
    Player::instance()->seek((double)offset / 1000000.0);
}
void Mpris2::SetPosition(const QDBusObjectPath &trackId, qint64 position) {
    Player::instance()->seek((double)position / 1000000.0);
}
}  // namespace mpris

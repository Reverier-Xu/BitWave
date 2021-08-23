/* This file is part of Clementine.
   Copyright 2010-2012, David Sansome <me@davidsansome.com>
   Copyright 2010-2011, Paweł Bara <keirangtp@gmail.com>
   Copyright 2012, 2014, John Maguire <john.maguire@gmail.com>
   Copyright 2013, Arnaud Bienner <arnaud.bienner@gmail.com>
   Copyright 2013, TTSDA <ttsda@ttsda.cc>
   Copyright 2013, Aggelos Biboudis <biboudis@gmail.com>
   Copyright 2014, Krzysztof Sobiecki <sobkas@gmail.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mpris2.h"

#include <QApplication>
#include <QDBusConnection>
#include <algorithm>
#include <QtMath>

#include "dbus/mpris2_player.h"
#include "dbus/mpris2_root.h"
#include "mpris_common.h"
#include "managers/display_manager.h"
#include "managers/queue_manager.h"
#include "managers/player_manager.h"

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

        connect(PlayerManager::instance(), &PlayerManager::currentMediaCoverChanged, [=](const QString &n) {
            this->MetadataLoaded(QueueManager::instance()->currentMedia(), n);
        });
//        connect(PlayerManager::instance(), &PlayerManager::totalTimeChanged, [=](double n) {
//            this->MetadataLoaded(QueueManager::instance()->currentMedia());
//        });

        connect(PlayerManager::instance(), &PlayerManager::volumeChanged, this, &Mpris2::VolumeChanged);
        connect(PlayerManager::instance(), &PlayerManager::currentTimeChanged, this, [=](double n) {
            emit this->Seeked((qlonglong)(floor(n)) * 1000 * 1000);
        });

        connect(QueueManager::instance(), &QueueManager::currentMediaChanged, this, &Mpris2::CurrentSongChanged);
        connect(PlayerManager::instance(), &PlayerManager::stateChanged, this, &Mpris2::EngineStateChanged);
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
        QVariantList args = QVariantList() << mprisEntity << map << QStringList();
        // qDebug() << "Name and Val to MPRIS2: " << name << val;
        // qDebug() << "Map to MPRIS2: " << map;
        // qDebug() << "Args to MPRIS2: " << args;
        msg.setArguments(args);
        // qDebug() << "FINAL msg to MPRIS2: " << msg.arguments();
        QDBusConnection::sessionBus().send(msg);
    }

    void Mpris2::EmitNotification(const QString &name) {
        QVariant value;
        if (name == "Metadata")
            value = Metadata();
        else if (name == "PlaybackStatus")
            value = PlaybackStatus();
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

// ------------------Root Interface--------------- //

    bool Mpris2::CanQuit() const { return true; }

    bool Mpris2::CanRaise() const { return true; }

    QString Mpris2::Identity() const { return QApplication::applicationName(); }

    QString Mpris2::DesktopEntryAbsolutePath() const {
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

    QString Mpris2::DesktopEntry() const {
        return QApplication::applicationName().toLower();
    }

    QStringList Mpris2::SupportedUriSchemes() const {
        static QStringList res = QStringList() << "file"
                                               << "http";
        return res;
    }

    QStringList Mpris2::SupportedMimeTypes() const {
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

    void Mpris2::Raise() { emit RaiseMainWindow(); }

    void Mpris2::Quit() { QApplication::quit(); }

    double Mpris2::Rate() const { return 1.0; }

    void Mpris2::SetRate(double rate) {
        if (rate == 0) {
            PlayerManager::instance()->pause();
        }
    }

    QVariantMap Mpris2::Metadata() const { return last_metadata_; }

// We send Metadata change notification as soon as the process of
// changing song starts...
    void Mpris2::CurrentSongChanged(const Media &song) {
        MetadataLoaded(song, "");
        EmitNotification("CanPlay");
        EmitNotification("CanPause");
        EmitNotification("CanGoNext", CanGoNext());
        EmitNotification("CanGoPrevious", CanGoPrevious());
        EmitNotification("CanSeek", CanSeek());
    }

// ... and we add the cover information later, when it's available.
    void Mpris2::MetadataLoaded(const Media &song, const QString &art_uri) {
        last_metadata_ = QVariantMap();

        using mpris::AddMetadata;

        AddMetadata("xesam:artist", song.artist().split("/"), &last_metadata_);
        AddMetadata("xesam:title", song.title(), &last_metadata_);
        AddMetadata("xesam:album", song.collection(), &last_metadata_);
        AddMetadata("xesam:url", song.rawUrl(), &last_metadata_);
        AddMetadata("mpris:length", song.duration() * 1000000,
                    &last_metadata_);

        if (!art_uri.isEmpty()) {
            AddMetadata("mpris:artUrl", art_uri, &last_metadata_);
        }

        EmitNotification("Metadata", last_metadata_);
    }

    double Mpris2::Volume() const { return PlayerManager::instance()->volume(); }

    void Mpris2::SetVolume(double value) { PlayerManager::instance()->setVolume(value); }

    qlonglong Mpris2::Position() const {
        return qlonglong(PlayerManager::instance()->currentTime() * 1000 * 1000);
    }

    double Mpris2::MaximumRate() const { return 1.0; }

    double Mpris2::MinimumRate() const { return 1.0; }

    bool Mpris2::CanGoNext() const {
        return true;
    }

    bool Mpris2::CanGoPrevious() const {
        return true;
    }

    bool Mpris2::CanPlay() const {
        return PlayerManager::instance()->isMediaLoaded() && !PlayerManager::instance()->isPlaying();
    }

// This one's a bit different than MPRIS 1 - we want this to be true even when
// the song is already paused or stopped.
    bool Mpris2::CanPause() const {
        return PlayerManager::instance()->isMediaLoaded() && PlayerManager::instance()->isPlaying();
    }

    bool Mpris2::CanSeek() const { return PlayerManager::instance()->isMediaLoaded(); }

    bool Mpris2::CanControl() const { return true; }

    void Mpris2::Next() {
        if (CanGoNext()) {
            QueueManager::instance()->next();
        }
    }

    void Mpris2::Previous() {
        if (CanGoPrevious()) {
            QueueManager::instance()->previous();
        }
    }

    void Mpris2::Pause() {
        if (CanPause()) {
            PlayerManager::instance()->pause();
        }
    }

    void Mpris2::PlayPause() {
        if (CanPause()) {
            PlayerManager::instance()->pause();
        } else if (CanPlay()) {
            PlayerManager::instance()->resume();
        }

    }

    void Mpris2::Stop() { PlayerManager::instance()->stop(); }

    void Mpris2::Play() {
        if (CanPlay()) {
            PlayerManager::instance()->resume();
        }
    }

    void Mpris2::Seek(qlonglong offset) {
        if (CanSeek()) {
            qDebug() << (double) (offset) / 1000000.0;
            PlayerManager::instance()->userDragHandler((double) (offset) / 1000000.0);
        }
    }

    void Mpris2::OpenUri(const QString &uri) {
        QueueManager::instance()->playExternMedia(uri);
    }

    bool Mpris2::CanSetFullscreen() const {
        return true;
    }

    bool Mpris2::Fullscreen() const {
        return DisplayManager::instance()->isFullScreen();
    }

    void Mpris2::SetFullscreen(bool n) {
        DisplayManager::instance()->setFullScreen(n);
    }

    QString Mpris2::PlaybackStatus() const {
        if (PlayerManager::instance()->isMediaLoaded()) {
            if (PlayerManager::instance()->isPlaying())
                return "Playing";
            else return "Paused";
        } else {
            return "Stopped";
        }
    }

    void Mpris2::EngineStateChanged() {
        if (PlayerManager::instance()->isMediaLoaded() == false) {
            // qDebug() << "media is not loaded.";
            last_metadata_ = QVariantMap();
            EmitNotification("Metadata");
        }
        if (last_metadata_.isEmpty() and PlayerManager::instance()->isMediaLoaded()) {
            MetadataLoaded(QueueManager::instance()->currentMedia(),
                           PlayerManager::instance()->currentMediaCover());
        }
        EmitNotification("CanPlay");
        EmitNotification("CanPause");
        EmitNotification("PlaybackStatus");
        EmitNotification("CanSeek");
        EmitNotification("CanGoNext");
        EmitNotification("CanGoPrevious");
    }
}  // namespace mpris

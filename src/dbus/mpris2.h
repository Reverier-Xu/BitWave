/* This file is part of Clementine.
   Copyright 2010-2011, Paweł Bara <keirangtp@gmail.com>
   Copyright 2010-2012, David Sansome <me@davidsansome.com>
   Copyright 2011-2012, 2014, John Maguire <john.maguire@gmail.com>
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

#pragma once

#include <QMetaObject>
#include <QObject>
#include <QtDBus>
#include <models/media.h>
#include <managers/app_manager.h>

typedef QList<QVariantMap> TrackMetadata;
Q_DECLARE_METATYPE(TrackMetadata)

namespace mpris {

class Mpris2 : public QObject {
  Q_OBJECT

 public:
  explicit Mpris2(QObject* parent = nullptr);

  // org.mpris.MediaPlayer2 MPRIS 2.0 Root interface
  Q_PROPERTY(bool CanQuit READ CanQuit)
  Q_PROPERTY(bool CanRaise READ CanRaise)
  Q_PROPERTY(QString Identity READ Identity)
  Q_PROPERTY(QString DesktopEntry READ DesktopEntry)
  Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)
  Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)

  // org.mpris.MediaPlayer2 MPRIS 2.2 Root interface
  Q_PROPERTY(bool CanSetFullscreen READ CanSetFullscreen)
  Q_PROPERTY(bool Fullscreen READ Fullscreen WRITE SetFullscreen)

  // org.mpris.MediaPlayer2.Player MPRIS 2.0 Player interface
  Q_PROPERTY(double Rate READ Rate WRITE SetRate)
  Q_PROPERTY(QVariantMap Metadata READ Metadata)
  Q_PROPERTY(double Volume READ Volume WRITE SetVolume)
  Q_PROPERTY(qlonglong Position READ Position)
  Q_PROPERTY(double MinimumRate READ MinimumRate)
  Q_PROPERTY(double MaximumRate READ MaximumRate)
  Q_PROPERTY(bool CanGoNext READ CanGoNext)
  Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
  Q_PROPERTY(bool CanPlay READ CanPlay)
  Q_PROPERTY(bool CanPause READ CanPause)
  Q_PROPERTY(bool CanSeek READ CanSeek)
  Q_PROPERTY(bool CanControl READ CanControl)

  // Root Properties
  [[nodiscard]] bool CanQuit() const;
  [[nodiscard]] bool CanRaise() const;
  [[nodiscard]] QString Identity() const;
  [[nodiscard]] QString DesktopEntry() const;
  [[nodiscard]] QStringList SupportedUriSchemes() const;
  [[nodiscard]] QStringList SupportedMimeTypes() const;

  // Root Properties added in MPRIS 2.2
  [[nodiscard]] bool CanSetFullscreen() const;
  [[nodiscard]] bool Fullscreen() const;
  void SetFullscreen(bool);

  // Methods
  void Raise();
  void Quit();

  // Player Properties
  [[nodiscard]] double Rate() const;
  void SetRate(double value);
  [[nodiscard]] QVariantMap Metadata() const;
  [[nodiscard]] double Volume() const;
  void SetVolume(double value);
  [[nodiscard]] qlonglong Position() const;
  [[nodiscard]] double MaximumRate() const;
  [[nodiscard]] double MinimumRate() const;
  [[nodiscard]] bool CanGoNext() const;
  [[nodiscard]] bool CanGoPrevious() const;
  [[nodiscard]] bool CanPlay() const;
  [[nodiscard]] bool CanPause() const;
  [[nodiscard]] bool CanSeek() const;
  [[nodiscard]] bool CanControl() const;

  // Methods
  void Next();
  void Previous();
  void Pause();
  void PlayPause();
  void Stop();
  void Play();
  void Seek(qlonglong offset);
  void OpenUri(const QString& uri);

 signals:
  // Player
  void Seeked(qlonglong position);

  void RaiseMainWindow();

 private slots:
  void MetadataLoaded(const Media& song, const QString& art_uri="");
  void VolumeChanged();
  void EngineStateChanged();

  void CurrentSongChanged(const Media& song);

 private:
  void EmitNotification(const QString& name);
  void EmitNotification(const QString& name, const QVariant& val);
  void EmitNotification(const QString& name, const QVariant& val,
                        const QString& mprisEntity);

  QString PlaybackStatus() const;

  [[nodiscard]] QString DesktopEntryAbsolutePath() const;

 private:
  static const char* kMprisObjectPath;
  static const char* kServiceName;
  static const char* kFreedesktopPath;

  QVariantMap last_metadata_;
};

}  // namespace mpris

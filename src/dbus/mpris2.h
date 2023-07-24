/**
 * @file mpris2.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include "models/media.h"

#include <QMetaObject>
#include <QObject>
#include <QtDBus>

typedef QList<QVariantMap> TrackMetadata;

Q_DECLARE_METATYPE(TrackMetadata)

namespace mpris {

class Mpris2 : public QObject {
    Q_OBJECT

   public:
    explicit Mpris2(QObject *parent = nullptr);

    ~Mpris2() override;

    // org.mpris.MediaPlayer2 MPRIS 2.0 Root interface
    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QString DesktopEntry READ DesktopEntry)
    Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)

    // org.mpris.MediaPlayer2 MPRIS 2.2 Root interface
    Q_PROPERTY(bool CanSetFullscreen READ CanSetFullscreen)
    Q_PROPERTY(bool Fullscreen READ Fullscreen)

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
    [[nodiscard]] static bool CanQuit();

    [[nodiscard]] static bool CanRaise();

    [[nodiscard]] static QString Identity();

    [[nodiscard]] static QString DesktopEntry();

    [[nodiscard]] static QStringList SupportedUriSchemes();

    [[nodiscard]] static QStringList SupportedMimeTypes();

    // Root Properties added in MPRIS 2.2
    [[nodiscard]] bool CanSetFullscreen() const;

    [[nodiscard]] bool Fullscreen() const;

    // Methods
    void Raise();

    static void Quit();

    // Player Properties
    [[nodiscard]] static double Rate();

    void SetRate(double value);

    [[nodiscard]] QVariantMap Metadata() const;

    [[nodiscard]] double Volume() const;

    void SetVolume(double value);

    [[nodiscard]] qlonglong Position() const;

    [[nodiscard]] static double MaximumRate();

    [[nodiscard]] static double MinimumRate();

    [[nodiscard]] static bool CanGoNext();

    [[nodiscard]] static bool CanGoPrevious();

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

    void SetPosition(const QVariant& trackId, qlonglong position);

    void OpenUri(const QString &uri);

   signals:

    // Player
    void Seeked(qlonglong offset);

    void RaiseMainWindow();

   private slots:

    void MetadataLoaded(const Media &song, const QString &art_uri = "");

    void VolumeChanged();

    void EngineStateChanged();

    void CurrentSongChanged(const Media &song);

   private:
    void EmitNotification(const QString &name);

    static void EmitNotification(const QString &name, const QVariant &val);

    static void EmitNotification(const QString &name, const QVariant &val,
                                 const QString &mprisEntity);

    [[nodiscard]] QString PlaybackStatus() const;

    [[nodiscard]] static QString DesktopEntryAbsolutePath();

   private:
    static const char *kMprisObjectPath;
    static const char *kServiceName;
    static const char *kFreedesktopPath;

    QVariantMap m_lastMetadata;
};

}  // namespace mpris

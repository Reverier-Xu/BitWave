#pragma once

#include <managers/app_manager.h>
#include <managers/display_manager.h>
#include <managers/player_manager.h>
#include <managers/queue_manager.h>
#include <models/media.h>

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
        [[nodiscard]] bool CanQuit();

        [[nodiscard]] bool CanRaise();

        [[nodiscard]] QString Identity();

        [[nodiscard]] QString DesktopEntry() const;

        [[nodiscard]] QStringList SupportedUriSchemes() const;

        [[nodiscard]] QStringList SupportedMimeTypes() const;

        // Root Properties added in MPRIS 2.2
        [[nodiscard]] bool CanSetFullscreen() const;

        [[nodiscard]] bool Fullscreen() const;

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

        void OpenUri(const QString &uri);

    signals:

        // Player
        void Seeked(qlonglong position);

        void RaiseMainWindow();

    private slots:

        void MetadataLoaded(const Media &song, const QString &art_uri = "");

        void VolumeChanged();

        void EngineStateChanged();

        void CurrentSongChanged(const Media &song);

    private:
        void EmitNotification(const QString &name);

        void EmitNotification(const QString &name, const QVariant &val);

        void EmitNotification(const QString &name,
                              const QVariant &val,
                              const QString &mprisEntity);

        [[nodiscard]] QString PlaybackStatus() const;

        [[nodiscard]] QString DesktopEntryAbsolutePath();

    private:
        static const char *kMprisObjectPath;
        static const char *kServiceName;
        static const char *kFreedesktopPath;

        QVariantMap last_metadata_;

        PlayerManager *player_;
        QueueManager *queue_;
        DisplayManager *display_;
    };

} // namespace mpris

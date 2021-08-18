//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <models/media.h>

#include <QObject>
#include "base_manager.h"
#include "engines/mpv_engine.h"
#include "models/ui/lyrics_list_model.h"

class PlayerManager : public BaseManager {
Q_OBJECT
    // this property just show current media is video, do not sig that the media
    // is not paused.
    Q_PROPERTY(bool currentMediaIsVideo MEMBER mCurrentMediaIsVideo READ
                       currentMediaIsVideo WRITE setCurrentMediaIsVideo NOTIFY
                       currentMediaIsVideoChanged)
    Q_PROPERTY(bool isPlaying MEMBER mIsPlaying READ isPlaying WRITE setPlaying
                       NOTIFY isPlayingChanged)
    Q_PROPERTY(double totalTime MEMBER mTotalTime READ totalTime WRITE
                       setTotalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(double currentTime MEMBER mCurrentTime READ currentTime WRITE
                       setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(double volume MEMBER mVolume READ volume WRITE setVolume NOTIFY
                       volumeChanged)
    Q_PROPERTY(bool isMuted MEMBER mIsMuted READ isMuted WRITE setIsMuted NOTIFY
                       isMutedChanged)
    Q_PROPERTY(bool isFullScreen MEMBER mIsFullScreen READ isFullScreen WRITE
                       setIsFullScreen NOTIFY isFullScreenChanged)
    Q_PROPERTY(QString currentMediaUrl MEMBER mCurrentMediaUrl READ currentMediaUrl
                       WRITE setCurrentMediaUrl NOTIFY currentMediaUrlChanged)
    Q_PROPERTY(QString currentMediaTitle MEMBER mCurrentMediaTitle READ
                       currentMediaTitle WRITE setCurrentMediaTitle NOTIFY
                       currentMediaTitleChanged)
    Q_PROPERTY(QString currentMediaArtist MEMBER mCurrentMediaArtist READ
                       currentMediaArtist WRITE setCurrentMediaArtist NOTIFY
                       currentMediaArtistChanged)
    Q_PROPERTY(QString currentMediaAlbum MEMBER mCurrentMediaAlbum READ
                       currentMediaAlbum WRITE setCurrentMediaAlbum NOTIFY
                       currentMediaAlbumChanged)
    Q_PROPERTY(QString currentMediaCover MEMBER mCurrentMediaCover READ
                       currentMediaCover WRITE setCurrentMediaCover NOTIFY
                       currentMediaCoverChanged)
    Q_PROPERTY(bool isMediaLoaded MEMBER mIsMediaLoaded READ isMediaLoaded WRITE
                       setIsMediaLoaded NOTIFY isMediaLoadedChanged)
    Q_PROPERTY(int isLyricLoaded MEMBER mIsLyricLoaded READ isLyricLoaded WRITE
                       setIsLyricLoaded NOTIFY isLyricLoadedChanged)
    Q_PROPERTY(int currentLyricIndex MEMBER mCurrentLyricIndex READ currentLyricIndex
                       WRITE setCurrentLyricIndex NOTIFY currentLyricIndexChanged)
    Q_PROPERTY(QColor coverColor MEMBER mCoverColor READ coverColor
                       WRITE setCoverColor NOTIFY coverColorChanged)
private:
    bool mCurrentMediaIsVideo = false;
    bool mIsPlaying = false;
    double mTotalTime = 0.0;
    double mCurrentTime = 0.0;
    double mVolume = 1.0;
    bool mIsMuted = false;
    bool mIsFullScreen = false;
    QString mCurrentMediaUrl = "";
    QString mCurrentMediaTitle = tr("No media");
    QString mCurrentMediaArtist = tr("No artist");
    QString mCurrentMediaAlbum = tr("No album");
    QString mCurrentMediaCover = "qrc:/assets/music-big.svg";
    bool mIsMediaLoaded = false;
    int mIsLyricLoaded = 0;
    int mCurrentLyricIndex = 0;

    QColor mCoverColor = QColor(0x00, 0x78, 0xd6);

    LyricsListModel mLyricsModel{};

    MpvEngine *mEngine = nullptr;

protected:
    explicit PlayerManager(QObject *parent);

    ~PlayerManager() override;

    static PlayerManager *mInstance;

    void connectSignals();

public:
    static PlayerManager *instance(QObject *parent = nullptr);

    void loadSettings() override;

    void saveSettings() override;

    [[nodiscard]] bool currentMediaIsVideo() const {
        return this->mCurrentMediaIsVideo;
    }

    void setCurrentMediaIsVideo(bool n) {
        this->mCurrentMediaIsVideo = n;
        emit this->currentMediaIsVideoChanged(n);
    }

    [[nodiscard]] bool isPlaying() const { return this->mIsPlaying; }

    void setPlaying(bool n) {
        this->mIsPlaying = n;
        emit this->isPlayingChanged(n);
    }

    [[nodiscard]] double totalTime() const { return this->mTotalTime; }

    void setTotalTime(double n) {
        if (n < 0.0) {
            n = 0.0;
        }
        this->mTotalTime = n;
        emit this->totalTimeChanged(n);
    }

    [[nodiscard]] double currentTime() const { return this->mCurrentTime; }

    void setCurrentTime(double n) {
        if (n < 0.0) {
            n = 0.0;
        }
        this->mCurrentTime = n;
        emit this->currentTimeChanged(n);
    }

    [[nodiscard]] double volume() const { return this->mVolume; }

    void setVolume(double n) {
        if (n < 0.0) {
            n = 0.0;
        }
        this->mVolume = n;
        this->mEngine->setProperty("volume", mVolume * 100);
        emit this->volumeChanged(n);
    }

    [[nodiscard]] bool isMuted() const { return this->mIsMuted; }

    void setIsMuted(bool n) {
        this->mIsMuted = n;
        this->mEngine->setMute(n);
        emit this->isMutedChanged(n);
    }

    [[nodiscard]] bool isFullScreen() const { return this->mIsFullScreen; }

    void setIsFullScreen(bool n) {
        this->mIsFullScreen = n;
        emit this->isFullScreenChanged(n);
    }

    [[nodiscard]] QString currentMediaUrl() const {
        return this->mCurrentMediaUrl;
    }

    void setCurrentMediaUrl(const QString &n) {
        this->mCurrentMediaUrl = n;
        emit this->currentMediaUrlChanged(n);
    }

    [[nodiscard]] QString currentMediaTitle() const {
        return this->mCurrentMediaTitle;
    }

    void setCurrentMediaTitle(const QString &n) {
        this->mCurrentMediaTitle = n;
        emit this->currentMediaTitleChanged(n);
    }

    [[nodiscard]] QString currentMediaArtist() const {
        return this->mCurrentMediaArtist;
    }

    void setCurrentMediaArtist(const QString &n) {
        this->mCurrentMediaArtist = n;
        emit this->currentMediaArtistChanged(n);
    }

    [[nodiscard]] QString currentMediaAlbum() const {
        return this->mCurrentMediaAlbum;
    }

    void setCurrentMediaAlbum(const QString &n) {
        this->mCurrentMediaAlbum = n;
        emit this->currentMediaAlbumChanged(n);
    }

    [[nodiscard]] QString currentMediaCover() const {
        return this->mCurrentMediaCover;
    }

    void setCurrentMediaCover(const QString &n) {
        this->mCurrentMediaCover = n;
        emit this->currentMediaCoverChanged(n);
    }

    [[nodiscard]] bool isMediaLoaded() const { return this->mIsMediaLoaded; }

    void setIsMediaLoaded(bool n) {
        this->mIsMediaLoaded = n;
        emit this->isMediaLoadedChanged(n);
    }

    [[nodiscard]] int isLyricLoaded() const { return this->mIsLyricLoaded; }

    void setIsLyricLoaded(int n) {
        this->mIsLyricLoaded = n;
        emit this->isLyricLoadedChanged(n);
    }

    [[nodiscard]] const QColor &coverColor() const { return this->mCoverColor; }

    void setCoverColor(const QColor &n) {
        this->mCoverColor = n;
        emit this->coverColorChanged(n);
    }

public slots:
    Q_INVOKABLE void userDragHandler(double t);

    Q_INVOKABLE void play(const Media &m);

    Q_INVOKABLE void playUrl(const QString &m);

    Q_INVOKABLE void pause();

    Q_INVOKABLE void resume();

    Q_INVOKABLE void setLyrics(const QString &raw, const QString &tr = "");

    Q_INVOKABLE [[nodiscard]] LyricsListModel *getLyricsModel() {
        return &(this->mLyricsModel);
    }

    Q_INVOKABLE void setCurrentLyricIndex(int n) {
        auto cntIndex = currentLyricIndex();
        if (this->mCurrentLyricIndex != cntIndex) {
            this->mCurrentLyricIndex = cntIndex;
            emit this->currentLyricIndexChanged(this->mCurrentLyricIndex);
        }
    }

    Q_INVOKABLE [[nodiscard]] int currentLyricIndex() const {
        return this->mLyricsModel.getCurrentLyricIndex(this->currentTime());
    }

    Q_INVOKABLE void handleMediaIsReady(bool ok, const Media &m);

    Q_INVOKABLE void handleMediaCoverIsReady(bool ok, const QString &m);

    Q_INVOKABLE void handleMediaLyricsIsReady(bool ok, const QString &raw, const QString &trans);

    Q_INVOKABLE void handleCoverColorIsReady(bool ok, const QColor &color);

signals:

    void currentMediaIsVideoChanged(bool n);

    void isPlayingChanged(bool n);

    void totalTimeChanged(double n);

    void currentTimeChanged(double n);

    void volumeChanged(double n);

    void isMutedChanged(bool n);

    void isFullScreenChanged(bool n);

    void currentMediaUrlChanged(QString n);

    void currentMediaTitleChanged(QString n);

    void currentMediaArtistChanged(QString n);

    void currentMediaAlbumChanged(QString n);

    void currentMediaCoverChanged(QString n);

    void coverColorChanged(const QColor &color);

    void isMediaLoadedChanged(bool n);

    void isLyricLoadedChanged(int n);

    void currentLyricIndexChanged(int n);

    void mediaParseRequired(const Media &m);

    void mediaCoverRequired(const Media &m);

    void mediaLyricsRequired(const Media &m);

    void coverColorRequired(const QString &cover);
};


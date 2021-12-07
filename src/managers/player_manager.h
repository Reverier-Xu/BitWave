/**
 * @file player_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QObject>

#include "dbus/screensaver.h"
#include "engines/mpv_engine.h"
#include "models/media.h"
#include "models/ui/lyrics_list_model.h"

class PlayerManager : public QObject {
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
    Q_PROPERTY(
        QString currentMediaUrl MEMBER mCurrentMediaUrl READ currentMediaUrl
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
    Q_PROPERTY(bool isMediaLoading MEMBER mIsMediaLoading READ isMediaLoading
                   WRITE setIsMediaLoading NOTIFY isMediaLoadingChanged)
    Q_PROPERTY(bool isMediaLoaded MEMBER mIsMediaLoaded READ isMediaLoaded WRITE
                   setIsMediaLoaded NOTIFY isMediaLoadedChanged)
    Q_PROPERTY(int isLyricLoaded MEMBER mIsLyricLoaded READ isLyricLoaded WRITE
                   setIsLyricLoaded NOTIFY isLyricLoadedChanged)
    Q_PROPERTY(
        int currentLyricIndex MEMBER mCurrentLyricIndex READ currentLyricIndex
            WRITE setCurrentLyricIndex NOTIFY currentLyricIndexChanged)
    Q_PROPERTY(QColor coverColor MEMBER mCoverColor READ coverColor WRITE
                   setCoverColor NOTIFY coverColorChanged)
    Q_PROPERTY(bool isReady MEMBER mIsReady READ isReady WRITE setIsReady NOTIFY
                   isReadyChanged)
    Q_PROPERTY(int lightness MEMBER mLightness READ lightness WRITE setLightness
                   NOTIFY lightnessChanged)
    Q_PROPERTY(int contrast MEMBER mContrast READ contrast WRITE setContrast
                   NOTIFY contrastChanged)
    Q_PROPERTY(int saturation MEMBER mSaturation READ saturation WRITE
                   setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(
        int gamma MEMBER mGamma READ gamma WRITE setGamma NOTIFY gammaChanged)
    Q_PROPERTY(int hue MEMBER mHue READ hue WRITE setHue NOTIFY hueChanged)
   private:
    bool mCurrentMediaIsVideo = false;
    bool mIsPlaying = false;
    double mTotalTime = 0.0;
    double mCurrentTime = 0.0;
    double mVolume = 1.0;
    bool mIsMuted = false;
    bool mIsReady = false;
    int mLightness = 0;
    int mContrast = 0;
    int mSaturation = 0;
    int mGamma = 0;
    int mHue = 0;
    QString mCurrentMediaUrl = "";
    QString mCurrentMediaTitle = tr("No media");
    QString mCurrentMediaArtist = tr("No artist");
    QString mCurrentMediaAlbum = tr("No album");
    QString mCurrentMediaCover = "qrc:/assets/music-colorful.svg";
    bool mIsMediaLoading = false;
    bool mIsMediaLoaded = false;
    int mIsLyricLoaded = 0;
    int mCurrentLyricIndex = 0;

    QColor mCoverColor = QColor(0x00, 0x78, 0xd6);

    LyricsListModel mLyricsModel{};

    MpvEngine *mEngine = nullptr;

    Screensaver *mScreensaver;

   protected:
    explicit PlayerManager(QObject *parent);

    ~PlayerManager() override;

    static PlayerManager *mInstance;

    void connectSignals();

   public:
    static PlayerManager *instance(QObject *parent = nullptr);

    void loadSettings();

    void saveSettings() const;

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
        if (n < 0.0) n = 0.0;
        if (n > 1.0) n = 1.0;

        this->mVolume = n;
        this->mEngine->setProperty("volume", mVolume * 100);
        emit this->volumeChanged(n);

        if (this->isMediaLoaded() && this->currentMediaIsVideo())
            emit this->showTips(QString("qrc:/assets/volume-%1.svg").arg(3),
                                QString::asprintf("%02.1f%%", n * 100));
    }

    [[nodiscard]] bool isMuted() const { return this->mIsMuted; }

    void setIsMuted(bool n) {
        this->mIsMuted = n;
        this->mEngine->setMute(n);
        emit this->isMutedChanged(n);
        if (this->isMediaLoaded() && this->currentMediaIsVideo())
            emit this->showTips(
                QString("qrc:/assets/volume-%1.svg").arg(n ? 0 : 3),
                n ? tr("Muted") : tr("UnMuted"));
    }

    [[nodiscard]] int lightness() const { return this->mLightness; }

    void setLightness(int n) {
        if (n < -100) n = -100;
        if (n > 100) n = 100;

        this->mLightness = n;
        this->mEngine->setOption("contrast", n);
        emit this->lightnessChanged(n);
    }

    [[nodiscard]] int contrast() const { return this->mContrast; }

    void setContrast(int n) {
        if (n < -100) n = -100;
        if (n > 100) n = 100;

        this->mContrast = n;
        this->mEngine->setOption("contrast", n);
        emit this->contrastChanged(n);
    }

    [[nodiscard]] int saturation() const { return this->mSaturation; }

    void setSaturation(int n) {
        if (n < -100) n = -100;
        if (n > 100) n = 100;

        this->mSaturation = n;
        this->mEngine->setOption("saturation", n);
        emit this->saturationChanged(n);
    }

    [[nodiscard]] int gamma() const { return this->mGamma; }

    void setGamma(int n) {
        if (n < -100) n = -100;
        if (n > 100) n = 100;

        this->mGamma = n;
        this->mEngine->setOption("gamma", n);
        emit this->gammaChanged(n);
    }

    [[nodiscard]] int hue() const { return this->mHue; }

    void setHue(int n) {
        if (n < -100) n = -100;
        if (n > 100) n = 100;

        this->mHue = n;
        this->mEngine->setOption("hue", n);
        emit this->hueChanged(n);
    }

    [[nodiscard]] QString currentMediaUrl() const {
        return this->mCurrentMediaUrl;
    }

    void setCurrentMediaUrl(const QString &n) {
        this->mCurrentMediaUrl = n;
        emit this->currentMediaUrlChanged(n);
    }

    [[nodiscard]] bool isReady() const { return this->mIsReady; }

    void setIsReady(bool n) {
        this->mIsReady = n;
        emit this->isReadyChanged(n);
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

    [[nodiscard]] bool isMediaLoading() const { return this->mIsMediaLoading; }

    void setIsMediaLoading(bool n) {
        this->mIsMediaLoading = n;
        emit this->isMediaLoadingChanged(n);
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

    [[nodiscard]] QColor coverColor() const { return this->mCoverColor; }

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

    Q_INVOKABLE void stop();

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

    Q_INVOKABLE void handleMediaLyricsIsReady(bool ok, const QString &raw,
                                              const QString &trans);

    Q_INVOKABLE void handleCoverColorIsReady(bool ok, const QColor &color);

    Q_INVOKABLE void handlePlayQueueEnded();

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

    void isMediaLoadingChanged(bool n);

    void isMediaLoadedChanged(bool n);

    void isLyricLoadedChanged(int n);

    void currentLyricIndexChanged(int n);

    void mediaParseRequired(const Media &m);

    void mediaCoverRequired(const Media &m);

    void mediaLyricsRequired(const Media &m);

    void coverColorRequired(const QString &cover);

    void stateChanged();

    void isReadyChanged(bool n);

    void showTips(const QString &icon, const QString &info);

    void lightnessChanged(int n);

    void contrastChanged(int n);

    void saturationChanged(int n);

    void gammaChanged(int n);

    void hueChanged(int n);
};

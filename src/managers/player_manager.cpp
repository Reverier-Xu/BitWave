/**
 * @file player_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "player_manager.h"

#include <utilities/time_helper.h>

#include <QPixmap>
#include <QSettings>

#include "lyric_provider_manager.h"
#include "malloc.h"
#include "parser_manager.h"
#include "utilities/memory_helper.h"

PlayerManager *PlayerManager::mInstance = nullptr;

PlayerManager::PlayerManager(QObject *parent) : QObject(parent) {
    try {
        mEngine = MpvEngine::instance(this);
        setIsReady(true);
    } catch (...) {
        setIsReady(false);
    }

    mScreensaver = Screensaver::GetScreensaver();

    connectSignals();
    loadSettings();
}

PlayerManager::~PlayerManager() { saveSettings(); }

void PlayerManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("Player");
    setVolume(settings.value("Volume", volume()).toDouble());
    setIsMuted(settings.value("Muted", isMuted()).toBool());
    settings.endGroup();
}

void PlayerManager::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Player");
    settings.setValue("Volume", volume());
    settings.setValue("Muted", isMuted());
    settings.endGroup();

    settings.sync();
}

PlayerManager *PlayerManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new PlayerManager(parent);
    return mInstance;
}

void PlayerManager::connectSignals() {
    connect(mEngine, &MpvEngine::durationChanged,
            [=](double secs) { setTotalTime(secs); });

    connect(mEngine, &MpvEngine::newMusicOpened,
            [=]() { setCurrentMediaIsVideo(false); });

    connect(mEngine, &MpvEngine::newVideoOpened,
            [=]() { setCurrentMediaIsVideo(true); });

    connect(mEngine, &MpvEngine::positionChanged, [=](double msecs) {
        setCurrentTime(msecs);
        if (!currentMediaIsVideo()) setCurrentLyricIndex(0);
    });
    connect(mEngine, &MpvEngine::resumed, [=]() {
        setPlaying(true);
        // qDebug() << "resumed";
        emit stateChanged();
    });
    connect(mEngine, &MpvEngine::paused, [=]() {
        setPlaying(false);
        // qDebug() << "paused";
        emit stateChanged();
    });
    connect(mEngine, &MpvEngine::started, [=]() {
        setPlaying(true);
        setIsMediaLoaded(true);
        // qDebug() << "started";
        emit stateChanged();
    });
    connect(mEngine, &MpvEngine::ended, [=]() {
        setPlaying(false);
        setIsMediaLoaded(false);
        // qDebug() << "ended";
        emit stateChanged();
    });

    connect(this, &PlayerManager::mediaParseRequired, ParserManager::instance(),
            &ParserManager::handleParseMediaRequest, Qt::QueuedConnection);
    connect(this, &PlayerManager::mediaCoverRequired, ParserManager::instance(),
            &ParserManager::handleGetMediaCoverRequest, Qt::QueuedConnection);
    connect(this, &PlayerManager::coverColorRequired, ParserManager::instance(),
            &ParserManager::handleGetMediaCoverColorRequest,
            Qt::QueuedConnection);
    connect(this, &PlayerManager::mediaLyricsRequired,
            LyricProviderManager::instance(),
            &LyricProviderManager::handleGetLyricsRequest,
            Qt::QueuedConnection);

    connect(ParserManager::instance(), &ParserManager::mediaCoverIsReady, this,
            &PlayerManager::handleMediaCoverIsReady, Qt::QueuedConnection);
    connect(ParserManager::instance(), &ParserManager::mediaIsReady, this,
            &PlayerManager::handleMediaIsReady, Qt::QueuedConnection);
    connect(LyricProviderManager::instance(),
            &LyricProviderManager::lyricsIsReady, this,
            &PlayerManager::handleMediaLyricsIsReady, Qt::QueuedConnection);
    connect(ParserManager::instance(), &ParserManager::mediaCoverColorIsReady,
            this, &PlayerManager::handleCoverColorIsReady,
            Qt::QueuedConnection);

    if (mScreensaver != nullptr) {
        connect(mEngine, &MpvEngine::started, [=]() {
            // qDebug() << "Started";
            if (currentMediaIsVideo()) {
                // qDebug() << "Inhibit";
                mScreensaver->Inhibit();
            }
        });
        connect(mEngine, &MpvEngine::ended, [=]() {
            mScreensaver->UnInhibit();
            // qDebug() << "UnInhibit";
        });
        connect(this, &PlayerManager::isPlayingChanged, [=](bool n) {
            if (currentMediaIsVideo() and n) {
                // qDebug() << "Inhibit";
                mScreensaver->Inhibit();
            } else {
                // qDebug() << "UnInhibit";
                mScreensaver->UnInhibit();
            }
        });
    }
}

void PlayerManager::resetPlayer() {
    setPlaying(false);
    setCurrentMediaIsVideo(false);
    setCurrentTime(0);
    setTotalTime(0);
    setCurrentLyricIndex(0);
    setCurrentMediaCover("qrc:/assets/music-colorful.svg");
    setCurrentMediaTitle("No Media");
    setCoverColor(QColor(0x00, 0x78, 0xd6));
    setCurrentMediaAlbum("No Album");
    setCurrentMediaArtist("No Artist");
    setIsLyricLoaded(0);
    setLyrics("", "");
    stop();
}

void PlayerManager::handleUserSeekRequest(double t) {
    mEngine->setTimePos(t);
    if (isMediaLoaded() && currentMediaIsVideo())
        emit showTips("qrc:/assets/play.svg", TimeHelper::getTimeString(t));
}

void PlayerManager::play(const Media &m) {
    qDebug() << "Current Media:" << m.title();

    // MemoryHelper::assertMemory("PlayerManager::play");

    setCurrentMediaIsVideo(m.type() == VIDEO);
    // qDebug() << media.collection();

    setCurrentMediaAlbum(m.collection());
    setCurrentMediaArtist(m.artist());
    setCurrentMediaTitle(m.title());
    setIsMediaLoading(true);
    mEngine->pause();

    emit mediaCoverRequired(m);
    emit mediaParseRequired(m);
    if (m.type() == AUDIO) {
        setLyrics("[00:00.00]Loading Lyrics...", "");
        emit mediaLyricsRequired(m);
    }
    // qDebug() << "mediaParseRequired is emitted.";
}

void PlayerManager::pause() {
    mEngine->pause();
    if (isMediaLoaded() && currentMediaIsVideo())
        emit showTips(QString("qrc:/assets/pause.svg"), tr("Paused"));
    // qDebug() << "paused";
}

void PlayerManager::resume() {
    mEngine->resume();
    if (isMediaLoaded() && currentMediaIsVideo())
        emit showTips(QString("qrc:/assets/play.svg"), tr("Resumed"));
    // qDebug() << "resumed";
}

void PlayerManager::stop() {
    mEngine->stop();
    setIsLyricLoaded(0);
    setLyrics("", "");
#ifdef Q_OS_LINUX
    malloc_trim(0);  // free memories.
#endif
    // qDebug() << "stopped";
}

void PlayerManager::playUrl(const QString &m) {
    // MemoryHelper::assertMemory("PlayerManager::playUrl");
    // qDebug() << "Playing URL:" << m;
    mEngine->playMedia(m);
}

void PlayerManager::setLyrics(const QString &raw, const QString &tr) {
    mLyricsModel.parseLyrics(raw, tr);
    setIsLyricLoaded(1);
}

void PlayerManager::handleMediaIsReady(bool ok, const Media &m) {
    if (ok) {
        playUrl(m.rawUrl());
        setIsMediaLoading(false);
        mEngine->resume();  // prevent show video tips.
    } else {
        showTips("qrc:/assets/warning.svg", "Play Failed");
        emit playFailed();
    }
}

void PlayerManager::handleMediaCoverIsReady(bool ok, const QString &m) {
    if (ok) {
        setCurrentMediaCover(m);
        emit coverColorRequired(m);
    } else {
        setCurrentMediaCover("qrc:/assets/archive-colorful.svg");
        setCoverColor(QColor(0xd7, 0x75, 0x87));
    }
}

void PlayerManager::handleMediaLyricsIsReady(bool ok, const QString &raw,
                                             const QString &trans) {
    if (ok)
        setLyrics(raw, trans);
    else {
        setLyrics(tr(""));
        setIsLyricLoaded(0);
    }
}

void PlayerManager::handleCoverColorIsReady(bool ok, const QColor &color) {
    if (ok)
        setCoverColor(color);
    else
        setCoverColor(QColor(0x00, 0x78, 0xd6));
}

bool PlayerManager::currentMediaIsVideo() const { return mCurrentMediaIsVideo; }

void PlayerManager::setCurrentMediaIsVideo(bool n) {
    mCurrentMediaIsVideo = n;
    emit currentMediaIsVideoChanged(n);
}

bool PlayerManager::isPlaying() const { return mIsPlaying; }

void PlayerManager::setPlaying(bool n) {
    mIsPlaying = n;
    emit isPlayingChanged(n);
}

double PlayerManager::totalTime() const { return mTotalTime; }

void PlayerManager::setTotalTime(double n) {
    if (n < 0.0) {
        n = 0.0;
    }
    mTotalTime = n;
    emit totalTimeChanged(n);
}

double PlayerManager::currentTime() const { return mCurrentTime; }

void PlayerManager::setCurrentTime(double n) {
    if (n < 0.0) {
        n = 0.0;
    }
    mCurrentTime = n;
    emit currentTimeChanged(n);
}

double PlayerManager::volume() const { return mVolume; }

void PlayerManager::setVolume(double n) {
    if (n < 0.0) n = 0.0;
    if (n > 1.0) n = 1.0;

    mVolume = n;
    mEngine->setProperty("volume", mVolume * 100);
    emit volumeChanged(n);

    if (isMediaLoaded() && currentMediaIsVideo())
        emit showTips(QString("qrc:/assets/volume-%1.svg").arg(3),
                      QString::asprintf("%02.1f%%", n * 100));
}

bool PlayerManager::isMuted() const { return mIsMuted; }

void PlayerManager::setIsMuted(bool n) {
    mIsMuted = n;
    mEngine->setMute(n);
    emit isMutedChanged(n);
    if (isMediaLoaded() && currentMediaIsVideo())
        emit showTips(QString("qrc:/assets/volume-%1.svg").arg(n ? 0 : 3),
                      n ? tr("Muted") : tr("UnMuted"));
}

int PlayerManager::lightness() const { return mLightness; }

void PlayerManager::setLightness(int n) {
    if (n < -100) n = -100;
    if (n > 100) n = 100;

    mLightness = n;
    mEngine->setOption("contrast", n);
    emit lightnessChanged(n);
}

int PlayerManager::contrast() const { return mContrast; }

void PlayerManager::setContrast(int n) {
    if (n < -100) n = -100;
    if (n > 100) n = 100;

    mContrast = n;
    mEngine->setOption("contrast", n);
    emit contrastChanged(n);
}

int PlayerManager::saturation() const { return mSaturation; }

void PlayerManager::setSaturation(int n) {
    if (n < -100) n = -100;
    if (n > 100) n = 100;

    mSaturation = n;
    mEngine->setOption("saturation", n);
    emit saturationChanged(n);
}

int PlayerManager::gamma() const { return mGamma; }

void PlayerManager::setGamma(int n) {
    if (n < -100) n = -100;
    if (n > 100) n = 100;

    mGamma = n;
    mEngine->setOption("gamma", n);
    emit gammaChanged(n);
}

int PlayerManager::hue() const { return mHue; }

void PlayerManager::setHue(int n) {
    if (n < -100) n = -100;
    if (n > 100) n = 100;

    mHue = n;
    mEngine->setOption("hue", n);
    emit hueChanged(n);
}

QString PlayerManager::currentMediaUrl() const { return mCurrentMediaUrl; }

void PlayerManager::setCurrentMediaUrl(const QString &n) {
    mCurrentMediaUrl = n;
    emit currentMediaUrlChanged(n);
}

bool PlayerManager::isReady() const { return mIsReady; }

void PlayerManager::setIsReady(bool n) {
    mIsReady = n;
    emit isReadyChanged(n);
}

QString PlayerManager::currentMediaTitle() const { return mCurrentMediaTitle; }

void PlayerManager::setCurrentMediaTitle(const QString &n) {
    mCurrentMediaTitle = n;
    emit currentMediaTitleChanged(n);
}

QString PlayerManager::currentMediaArtist() const {
    return mCurrentMediaArtist;
}

void PlayerManager::setCurrentMediaArtist(const QString &n) {
    mCurrentMediaArtist = n;
    emit currentMediaArtistChanged(n);
}

QString PlayerManager::currentMediaAlbum() const { return mCurrentMediaAlbum; }

void PlayerManager::setCurrentMediaAlbum(const QString &n) {
    mCurrentMediaAlbum = n;
    emit currentMediaAlbumChanged(n);
}

QString PlayerManager::currentMediaCover() const { return mCurrentMediaCover; }

void PlayerManager::setCurrentMediaCover(const QString &n) {
    mCurrentMediaCover = n;
    emit currentMediaCoverChanged(n);
}

bool PlayerManager::isMediaLoading() const { return mIsMediaLoading; }

void PlayerManager::setIsMediaLoading(bool n) {
    mIsMediaLoading = n;
    emit isMediaLoadingChanged(n);
}

bool PlayerManager::isMediaLoaded() const { return mIsMediaLoaded; }

void PlayerManager::setIsMediaLoaded(bool n) {
    mIsMediaLoaded = n;
    emit isMediaLoadedChanged(n);
}

int PlayerManager::isLyricLoaded() const { return mIsLyricLoaded; }

void PlayerManager::setIsLyricLoaded(int n) {
    mIsLyricLoaded = n;
    emit isLyricLoadedChanged(n);
}

QColor PlayerManager::coverColor() const { return mCoverColor; }

void PlayerManager::setCoverColor(const QColor &n) {
    mCoverColor = n;
    emit coverColorChanged(n);
}

//
// Created by Reverier-Xu on 2021/6/25.
//

#include "player_manager.h"

#include <utilities/time_helper.h>

#include <QPixmap>
#include <QSettings>

#include "lyric_provider_manager.h"
#include "malloc.h"
#include "parser_manager.h"
#include "queue_manager.h"
#include "utilities/memory_helper.h"

PlayerManager *PlayerManager::mInstance = nullptr;

PlayerManager::PlayerManager(QObject *parent)
        : BaseManager(parent) {
    try {
        this->mEngine = MpvEngine::instance(this);
        this->setIsReady(true);
    } catch (...) {
        this->setIsReady(false);
    }

    this->mScreensaver = Screensaver::GetScreensaver();

    this->connectSignals();
    this->loadSettings();
}

PlayerManager::~PlayerManager() {
    this->saveSettings();
}

void PlayerManager::loadSettings() {
    QSettings settings;
    settings.beginGroup("Player");
    this->setVolume(settings.value("Volume", this->volume()).toDouble());
    this->setIsMuted(settings.value("Muted", this->isMuted()).toBool());
    settings.endGroup();
}

void PlayerManager::saveSettings() {
    QSettings settings;
    settings.beginGroup("Player");
    settings.setValue("Volume", this->volume());
    settings.setValue("Muted", this->isMuted());
    settings.endGroup();

    settings.sync();
}

PlayerManager *PlayerManager::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new PlayerManager(parent);
    return mInstance;
}

void PlayerManager::connectSignals() {
    connect(this->mEngine, &MpvEngine::durationChanged, [=](double secs) {
        this->setTotalTime(secs);
    });
    connect(this->mEngine, &MpvEngine::newMusicOpened, [=]() {
        this->setCurrentMediaIsVideo(false);
    });
    connect(this->mEngine, &MpvEngine::newVideoOpened, [=]() {
        this->setCurrentMediaIsVideo(true);
    });
    connect(this->mEngine, &MpvEngine::positionChanged, [=](double msecs) {
        this->setCurrentTime(msecs);
        this->setCurrentLyricIndex(0);
    });
    connect(this->mEngine, &MpvEngine::resumed, [=]() {
        this->setPlaying(true);
        // qDebug() << "resumed";
        emit this->stateChanged();
    });
    connect(this->mEngine, &MpvEngine::paused, [=]() {
        this->setPlaying(false);
        // qDebug() << "paused";
        emit this->stateChanged();
    });
    connect(this->mEngine, &MpvEngine::started, [=]() {
        this->setPlaying(true);
        this->setIsMediaLoaded(true);
        // qDebug() << "started";
        emit this->stateChanged();
    });
    connect(this->mEngine, &MpvEngine::ended, [=]() {
        this->setIsMediaLoaded(false);
        // qDebug() << "ended";
        emit this->stateChanged();
        if (!QueueManager::instance(this->parent())->queueEnded())
            if (this->totalTime() - this->currentTime() < 0.5)
                QueueManager::instance(this->parent())->next();
    });
    connect(QueueManager::instance(this->parent()),
            &QueueManager::currentMediaChanged,
            this,
            &PlayerManager::play);

    connect(this,
            &PlayerManager::mediaParseRequired,
            ParserManager::instance(),
            &ParserManager::handleParseMediaRequest,
            Qt::QueuedConnection);
    connect(this,
            &PlayerManager::mediaCoverRequired,
            ParserManager::instance(),
            &ParserManager::handleGetMediaCoverRequest,
            Qt::QueuedConnection);
    connect(this,
            &PlayerManager::coverColorRequired,
            ParserManager::instance(),
            &ParserManager::handleGetMediaCoverColorRequest,
            Qt::QueuedConnection);
    connect(this,
            &PlayerManager::mediaLyricsRequired,
            LyricProviderManager::instance(),
            &LyricProviderManager::handleGetLyricsRequest,
            Qt::QueuedConnection);

    connect(ParserManager::instance(),
            &ParserManager::mediaCoverIsReady,
            this,
            &PlayerManager::handleMediaCoverIsReady,
            Qt::QueuedConnection);
    connect(ParserManager::instance(),
            &ParserManager::mediaIsReady,
            this,
            &PlayerManager::handleMediaIsReady,
            Qt::QueuedConnection);
    connect(LyricProviderManager::instance(),
            &LyricProviderManager::lyricsIsReady,
            this,
            &PlayerManager::handleMediaLyricsIsReady,
            Qt::QueuedConnection);
    connect(ParserManager::instance(),
            &ParserManager::mediaCoverColorIsReady,
            this,
            &PlayerManager::handleCoverColorIsReady,
            Qt::QueuedConnection);
    connect(QueueManager::instance(),
            &QueueManager::playQueueEnded,
            this,
            &PlayerManager::handlePlayQueueEnded,
            Qt::QueuedConnection);

    if (this->mScreensaver != nullptr) {
        connect(this->mEngine, &MpvEngine::started, [=]() {
            // qDebug() << "Started";
            if (this->currentMediaIsVideo()) {
                // qDebug() << "Inhibit";
                this->mScreensaver->Inhibit();
            }
        });
        connect(this->mEngine, &MpvEngine::ended, [=]() {
            this->mScreensaver->UnInhibit();
            // qDebug() << "UnInhibit";
        });
        connect(this, &PlayerManager::isPlayingChanged, [=](bool n) {
            if (this->currentMediaIsVideo() and n) {
                // qDebug() << "Inhibit";
                this->mScreensaver->Inhibit();
            } else {
                // qDebug() << "UnInhibit";
                this->mScreensaver->UnInhibit();
            }
        });
    }
}

void PlayerManager::handlePlayQueueEnded() {
    this->setPlaying(false);
    this->setCurrentMediaIsVideo(false);
    this->setCurrentTime(0);
    this->setTotalTime(0);
    this->setCurrentLyricIndex(0);
    this->setCurrentMediaCover("qrc:/assets/music-big.svg");
    this->setCurrentMediaTitle("No Media");
    this->setCoverColor(QColor(0x00, 0x78, 0xd6));
    this->setCurrentMediaAlbum("No Album");
    this->setCurrentMediaArtist("No Artist");
    this->setIsLyricLoaded(0);
    this->setLyrics("", "");
    this->stop();
}

void PlayerManager::userDragHandler(double t) {
    this->mEngine->setTimePos(t);
    if (this->isMediaLoaded() && this->currentMediaIsVideo())
            emit this->showTips("qrc:/assets/play-large.svg",
                                TimeHelper::getTimeString(t));
}

void PlayerManager::play(const Media &m) {
    qDebug() << "Current Media:" << m.title();

    // MemoryHelper::assertMemory("PlayerManager::play");

    this->setCurrentMediaIsVideo(m.type() == VIDEO);
    // qDebug() << media.collection();

    this->setCurrentMediaAlbum(m.collection());
    this->setCurrentMediaArtist(m.artist());
    this->setCurrentMediaTitle(m.title());
    this->setIsMediaLoading(true);
    this->mEngine->pause();

    emit this->mediaCoverRequired(m);
    emit this->mediaParseRequired(m);
    if (m.type() == AUDIO) {
        this->setLyrics("[00:00.00]Loading Lyrics...", "");
        emit this->mediaLyricsRequired(m);
    }
    // qDebug() << "mediaParseRequired is emitted.";
}

void PlayerManager::pause() {
    this->mEngine->pause();
    if (this->isMediaLoaded() && this->currentMediaIsVideo())
            emit this->showTips(QString("qrc:/assets/pause-large.svg"), tr("Paused"));
    // qDebug() << "paused";
}

void PlayerManager::resume() {
    this->mEngine->resume();
    if (this->isMediaLoaded() && this->currentMediaIsVideo())
            emit this->showTips(QString("qrc:/assets/play-large.svg"), tr("Resumed"));
    // qDebug() << "resumed";
}

void PlayerManager::stop() {
    this->mEngine->stop();
    this->setIsLyricLoaded(0);
    this->setLyrics("", "");
#ifdef Q_OS_LINUX
    malloc_trim(0); // free memories.
#endif
    // qDebug() << "stopped";
}

void PlayerManager::playUrl(const QString &m) {
    // MemoryHelper::assertMemory("PlayerManager::playUrl");
    // qDebug() << "Playing URL:" << m;
    this->mEngine->playMedia(m);
}

void PlayerManager::setLyrics(const QString &raw, const QString &tr) {
    this->mLyricsModel.parseLyrics(raw, tr);
    this->setIsLyricLoaded(1);
}

void PlayerManager::handleMediaIsReady(bool ok, const Media &m) {
    if (ok) {
        this->playUrl(m.rawUrl());
        this->setIsMediaLoading(false);
        this->mEngine->resume(); // prevent show video tips.
    } else {
        this->showTips("qrc:/assets/warning.svg", "Play Failed");
        QueueManager::instance(this->parent())->next();
    }
}

void PlayerManager::handleMediaCoverIsReady(bool ok, const QString &m) {
    if (ok) {
        this->setCurrentMediaCover(m);
        emit this->coverColorRequired(m);
    } else {
        this->setCurrentMediaCover("qrc:/assets/archive-big.svg");
        this->setCoverColor(QColor(0xd7, 0x75, 0x87));
    }
}

void PlayerManager::handleMediaLyricsIsReady(bool ok,
                                             const QString &raw,
                                             const QString &trans) {
    if (ok)
        this->setLyrics(raw, trans);
    else {
        this->setLyrics(tr(""));
        this->setIsLyricLoaded(0);
    }
}

void PlayerManager::handleCoverColorIsReady(bool ok, const QColor &color) {
    if (ok)
        this->setCoverColor(color);
    else
        this->setCoverColor(QColor(0x00, 0x78, 0xd6));
}

//
// Created by Reverier-Xu on 2021/6/25.
//

#include "player_manager.h"

PlayerManager *PlayerManager::mInstance = nullptr;

PlayerManager::PlayerManager(QObject *parent) : QObject(parent) {
    this->mEngine = MpvEngine::getInstance(this);
    this->connectSignals();
}

PlayerManager *PlayerManager::getInstance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new PlayerManager(parent);
    return mInstance;
}

void PlayerManager::connectSignals() {
    connect(this->mEngine, &MpvEngine::durationChanged,
            [=](double msecs) { this->setTotalTime(msecs); });
    connect(this->mEngine, &MpvEngine::newMusicOpened,
            [=]() { this->setCurrentMediaIsVideo(false); });
    connect(this->mEngine, &MpvEngine::newVideoOpened,
            [=]() { this->setCurrentMediaIsVideo(true); });
    connect(this->mEngine, &MpvEngine::positionChanged, [=](double msecs) {
        this->setCurrentTime(msecs);
        this->setCurrentLyricIndex(0);
    });
    connect(this->mEngine, &MpvEngine::resumed,
            [=]() { this->setPlaying(true); });
    connect(this->mEngine, &MpvEngine::paused,
            [=]() { this->setPlaying(false); });
    connect(this->mEngine, &MpvEngine::started,
            [=]() { this->setPlaying(true); });
    connect(this->mEngine, &MpvEngine::ended,
            [=]() { this->setPlaying(false); });
}

void PlayerManager::userDragHandler(double t) { this->mEngine->setTimePos(t); }

void PlayerManager::play(const Media &m) { this->playUrl(m.rawUrl()); }

void PlayerManager::pause() { this->mEngine->pause(); }

void PlayerManager::resume() { this->mEngine->resume(); }

void PlayerManager::playUrl(const QString &m) { this->mEngine->playMedia(m); }

void PlayerManager::setLyrics(const QString &raw, const QString &tr) {
    this->mLyricsModel.parseLyrics(raw, tr);
    this->setIsLyricLoaded(2);
}

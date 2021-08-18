//
// Created by Reverier-Xu on 2021/6/25.
//

#include "player_manager.h"

#include <QSettings>
#include <QPixmap>

#include "parser_manager.h"
#include "queue_manager.h"

PlayerManager *PlayerManager::mInstance = nullptr;

PlayerManager::PlayerManager(QObject *parent) : BaseManager(parent) {
    this->mEngine = MpvEngine::instance(this);
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
    connect(this->mEngine, &MpvEngine::paused, [=]() {
        this->setPlaying(false);
        // qDebug() << "paused";
    });
    connect(this->mEngine, &MpvEngine::started,
            [=]() { this->setPlaying(true); });
    connect(this->mEngine, &MpvEngine::ended, [=]() {
        // qDebug() << "ended";
        if (this->totalTime() - this->currentTime() < 0.5)
            QueueManager::instance(this->parent())->next();
    });
    connect(QueueManager::instance(this->parent()), &QueueManager::currentMediaChanged, this,
            &PlayerManager::play);

    connect(this, &PlayerManager::mediaParseRequired, ParserManager::instance(),
            &ParserManager::handleParseMediaRequest, Qt::QueuedConnection);
    connect(this, &PlayerManager::mediaCoverRequired, ParserManager::instance(),
            &ParserManager::handleGetMediaCoverRequest, Qt::QueuedConnection);
    connect(this, &PlayerManager::coverColorRequired, ParserManager::instance(),
            &ParserManager::handleGetMediaCoverColorRequest, Qt::QueuedConnection);
    connect(this, &PlayerManager::mediaLyricsRequired,
            ParserManager::instance(),
            &ParserManager::handleGetMediaLyricsRequest, Qt::QueuedConnection);

    connect(ParserManager::instance(), &ParserManager::mediaCoverIsReady, this,
            &PlayerManager::handleMediaCoverIsReady, Qt::QueuedConnection);
    connect(ParserManager::instance(), &ParserManager::mediaIsReady, this,
            &PlayerManager::handleMediaIsReady, Qt::QueuedConnection);
    connect(ParserManager::instance(), &ParserManager::mediaLyricsIsReady, this,
            &PlayerManager::handleMediaLyricsIsReady, Qt::QueuedConnection);
    connect(ParserManager::instance(), &ParserManager::mediaCoverColorIsReady, this,
            &PlayerManager::handleCoverColorIsReady, Qt::QueuedConnection);
}

void PlayerManager::userDragHandler(double t) { this->mEngine->setTimePos(t); }

void PlayerManager::play(const Media &m) {
    // qDebug() << "playing: " << m.title();

    this->setCurrentMediaIsVideo(m.type() == VIDEO);
    // qDebug() << media.collection();

    this->setCurrentMediaAlbum(m.collection());
    this->setCurrentMediaArtist(m.artist());
    this->setCurrentMediaTitle(m.title());

    emit this->mediaCoverRequired(m);
    emit this->mediaParseRequired(m);
    if (m.type() == AUDIO) emit this->mediaLyricsRequired(m);
    // qDebug() << "mediaParseRequired is emitted.";

    this->resume();
}

void PlayerManager::pause() {
    this->mEngine->pause();
    // qDebug() << "paused";
}

void PlayerManager::resume() {
    this->mEngine->resume();
    // qDebug() << "resumed";
}

void PlayerManager::playUrl(const QString &m) { this->mEngine->playMedia(m); }

void PlayerManager::setLyrics(const QString &raw, const QString &tr) {
    this->mLyricsModel.parseLyrics(raw, tr);
    this->setIsLyricLoaded(2);
}

void PlayerManager::handleMediaIsReady(bool ok, const Media &m) {
    if (ok)
        this->playUrl(m.rawUrl());
    else
        QueueManager::instance(this->parent())->next();
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

void PlayerManager::handleMediaLyricsIsReady(bool ok, const QString &raw,
                                             const QString &trans) {
    if (ok)
        this->setLyrics(raw, trans);
    else
        this->setLyrics(tr("[00:00.00] Lyrics Not Found."));
}

void PlayerManager::handleCoverColorIsReady(bool ok, const QColor &color) {
    if (ok)
        this->setCoverColor(color);
    else
        this->setCoverColor(QColor(0x00, 0x78, 0xd6));
}

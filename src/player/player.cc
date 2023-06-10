/**
 * @file player.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include <QMutex>
#include <QtConcurrent>
#include "player.h"
#include "parser/parser.h"
#include "codec/codec.h"


Player* Player::m_instance = nullptr;

Player::Player(QObject* parent) : QObject(parent) {
    m_engine = new Engine(this);
    m_cover.load(":/assets/music-colorful.svg");

    connectSignals();
}

Player::~Player() = default;

double Player::totalTime() const {
    return m_totalTime;
}

void Player::setTotalTime(double n) {
    m_totalTime = n;
    emit totalTimeChanged(n);
}

double Player::currentTime() const {
    return m_currentTime;
}

void Player::setCurrentTime(double n) {
    m_currentTime = n;
    emit currentTimeChanged(n);
}

Player* Player::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr)
            m_instance = new Player(parent);
        locker.unlock();
    }
    return m_instance;
}

bool Player::playing() const {
    return m_playing;
}

void Player::setPlaying(bool n) {
    m_playing = n;
    emit playingChanged(n);
}

double Player::volume() const {
    return m_volume;
}

void Player::setVolume(double n) {
    m_volume = n;
    emit volumeChanged(n);
}

bool Player::muted() const {
    return m_muted;
}

void Player::setMuted(bool n) {
    m_muted = n;
    emit mutedChanged(n);
}

void Player::resume() {
    if (valid()) {
        if (ended())
            m_engine->play(media().url());
        m_engine->resume();
    }
}

void Player::pause() {
    if (valid() && !ended())
        m_engine->pause();
}

void Player::seek(double n) {
    if (valid()) {
        if (ended())
            m_engine->play(media().url());
        m_engine->seek((double) (n));
    }
}

void Player::toggleMute() {
    m_engine->setMute(!muted());
    setMuted(!muted());
}

void Player::togglePause() {
    if (playing()) pause();
    else resume();
}

void Player::connectSignals() {
    connect(m_engine, &Engine::currentTimeChanged, this, [=](double secs) {
        setCurrentTime(secs);
    });
    connect(m_engine, &Engine::totalTimeChanged, this, [=](double secs) {
        setTotalTime(secs);
//        qDebug() << "total time changed: " << secs;
    });
    connect(m_engine, &Engine::volumeChanged, this, [=](double vol) {
        setVolume(vol);
    });
    connect(m_engine, &Engine::started, this, [=]() {
        resume();
        setEnded(false);
        qDebug() << "started";
    });
    connect(m_engine, &Engine::ended, this, [=]() {
        setPlaying(false);
        setEnded(true);
        qDebug() << "ended";
    });
    connect(m_engine, &Engine::paused, this, [=]() {
        setPlaying(false);
        qDebug() << "paused";
    });
    connect(m_engine, &Engine::resumed, this, [=]() {
        setPlaying(true);
        qDebug() << "resumed";
    });
}

void Player::toggleVolume(double n) {
    m_engine->setVolume(n);
}

Engine* Player::engine() const {
    return m_engine;
}

void Player::play(const Media& media) {
    if (valid()) {
        reset();
    }
    setMedia(media);

    setCoverLoading(true);
    setLoading(true);
    setCoverPath("");
    auto* coverWatcher = new QFutureWatcher<QImage>(this);
    connect(coverWatcher, &QFutureWatcher<QImage>::finished, this, [=]() {
        try {
            setCover(coverWatcher->result());
        } catch (...) {
            QImage image;
            image.load(":/assets/music-colorful.svg");
            setCover(image);
        }
        setCoverLoading(false);
    });
    coverWatcher->setFuture(QtConcurrent::run(&Parser::extractCover, media));

    auto decodeWatcher = new QFutureWatcher<QString>(this);
    connect(decodeWatcher, &QFutureWatcher<QString>::finished, this, [=]() {
        setLoading(false);
        try {
            m_engine->play(decodeWatcher->result());
            setValid(true);
        } catch (...) {
            setValid(false);
            // TODO: warn user here.
        }
    });
    decodeWatcher->setFuture(QtConcurrent::run(&Codec::decode, media));
}

Media Player::media() const {
    return m_media;
}

void Player::setMedia(const Media& n) {
    m_media = n;
    setValid(true);
    emit mediaChanged(n);
}

bool Player::valid() const {
    return m_valid;
}

void Player::setValid(bool n) {
    m_valid = n;
    emit validChanged(n);
}

bool Player::ended() const {
    return m_ended;
}

void Player::setEnded(bool n) {
    m_ended = n;
    emit endedChanged(n);
}

void Player::reset() {
    m_engine->stop();
    setPlaying(false);
    setEnded(false);
    setValid(false);
    setMedia(Media());
    setCurrentTime(0);
    setTotalTime(0);
    setVolume(0);
    setMuted(false);
}

QImage Player::cover() const {
    return m_cover;
}

void Player::setCover(const QImage& n) {
    m_cover = n;
    auto tempCoverPath =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/BitWave/Covers/" + m_media.title() + ".jpg";
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/BitWave/Covers/");
    n.save(tempCoverPath);
    setCoverPath(QUrl::fromLocalFile(tempCoverPath).toString());

    emit coverChanged(n);
}

bool Player::loading() const {
    return m_loading;
}

void Player::setLoading(bool n) {
    m_loading = n;
    emit loadingChanged(n);
}

bool Player::coverLoading() const {
    return m_coverLoading;
}

void Player::setCoverLoading(bool n) {
    m_coverLoading = n;
    emit coverLoadingChanged(n);
}

QString Player::coverPath() const {
    return m_coverPath;
}

void Player::setCoverPath(const QString& n) {
    m_coverPath = n;
    emit coverPathChanged(n);
}

void Player::playUrl(const QString& url) {
    try {
        auto media = Parser::parse(url);
        play(media);
    } catch (...) {
        // ...
    }
}

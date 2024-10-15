/**
 * @file play_queue.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-17
 *
 * @copyright 2023 Woo Tech
 */

#include "play_queue.h"

#include <QtConcurrent>
#include <random>

#include "library/library.h"
#include "parser/parser.h"
#include "playlist/playlist.h"
#include "storage/storage.h"

PlayQueue::PlayQueue(QObject* parent) : QObject(parent) {
    m_playlist = new QList<Media>;
    m_model = new MediaList(this);
    m_model->setList(m_playlist);
    loadStorage();
    loadSettings();
}

PlayQueue::~PlayQueue() {
    saveSettings();
    delete m_playlist;
}

int PlayQueue::mode() const { return m_mode; }

QVector<int> generatePlayOrder(QueueMode mode, int cursor, int* cursorPos, int size) {
    QVector<int> future;
    switch (mode) {
    case LOOP_ALL:
        for (int i = 0; i < size; ++i) {
            future.push_back(i);
        }
        break;
    case IN_ORDER:
        for (int i = 0; i < size; ++i) {
            future.push_back(i);
        }
        future.push_back(-1);
        break;
    case REVERSED:
        for (int i = size - 1; i >= 0; --i) {
            future.push_back(i);
        }
        future.push_back(-1);
        break;
    case LOOP_ONE:
        for (int i = 0; i < size; ++i) {
            future.push_back(cursor);
        }
        break;
    case RANDOM:
        for (int i = 0; i < size; ++i) {
            future.push_back(i);
        }
        std::shuffle(future.begin(), future.end(), std::mt19937(std::random_device()()));
        break;
    }
    *cursorPos = future.indexOf(cursor);
    return future;
}

void PlayQueue::setMode(int mode) {
    if (m_mode == mode) {
        return;
    }
    m_mode = static_cast<QueueMode>(mode);
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
    emit modeChanged(mode);
    emit modeIconChanged(modeIcon());
}

QVector<Media>* PlayQueue::queue() { return m_playlist; }

MediaList* PlayQueue::model() { return m_model; }

int PlayQueue::cursor() const { return m_cursor; }

void PlayQueue::setCursor(int n) {
    if (n < 0 || n >= (int)m_playlist->size()) {
        return;
    }
    m_cursor = n;
    emit cursorChanged(n);
    emit mediaChanged(current());
}

void PlayQueue::addMedia(const Media& media) {
    if (m_playlist->empty()) {
        m_playlist->append(media);
        m_model->reload();
    } else {
        m_playlist->insert(cursor() + 1, media);
        m_model->insertMedia(cursor() + 1);
    }
    // qDebug() << "after addMedia" << m_playlist->length() <<
    // m_playlist->first().title();
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
}

void PlayQueue::addMediaThenPlay(const Media& media) {
    addMedia(media);
    play(cursor() + 1);
}

void PlayQueue::removeMedia(int pos) {
    if (pos < 0 || pos >= (int)m_playlist->size()) {
        return;
    }
    m_playlist->remove(pos);
    m_model->removeMedia(pos);
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
    prev();
    saveStorage();
}

void PlayQueue::clearMedia() {
    m_playlist->clear();
    m_queuedPlayOrder.clear();
    m_model->reload();
    saveStorage();
}

void PlayQueue::moveMedia(int from, int to) {
    m_playlist->move(from, to);
    m_model->removeMedia(from);
    m_model->insertMedia(to);
}

void PlayQueue::loadPlaylist(const QVector<Media>& playlist) {
    m_playlist->clear();
    loadPlaylist_(playlist);
    saveStorage();
}

void PlayQueue::toggleChangeMode() { setMode((mode() + 1) % 5); }

void PlayQueue::next() {
    if (m_faillock > 3) {
        m_faillock = 0;
        return;
    }
    m_faillock++;
    //    qDebug() << "next" << m_cursorPos << m_mode << m_queuedPlayOrder;
    if (m_playlist->empty()) return;
    //    qDebug() << m_cursorPos << m_queuedPlayOrder;
    if (m_cursorPos >= m_queuedPlayOrder.size() - 1) {
        m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
        m_cursorPos = 0;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else if (m_cursorPos >= 0) {
        if (m_queuedPlayOrder[++m_cursorPos] != -1) setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else {
        m_cursorPos = 0;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    }
}

void PlayQueue::prev() {
    //    qDebug() << "prev" << m_cursorPos << m_mode << m_queuedPlayOrder;
    if (m_playlist->empty()) return;
    if (m_cursorPos <= 0 && m_mode != RANDOM) {
        m_cursorPos = (int)m_queuedPlayOrder.size() - 1;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else if (m_cursorPos <= 0) {
        m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
        m_cursorPos = (int)m_queuedPlayOrder.size() - 1;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else {
        if (m_queuedPlayOrder[--m_cursorPos] != -1) setCursor(m_queuedPlayOrder[m_cursorPos]);
    }
}

const Media& PlayQueue::current() {
    if (m_playlist->empty() || m_cursor < 0 || m_cursor >= m_playlist->size()) return Media::null();
    return (*m_playlist)[m_cursor];
}

void PlayQueue::play(int pos) {
    if (pos >= (int)m_playlist->size()) {
        pos = (int)m_playlist->size() - 1;
    } else if (pos < 0) {
        pos = 0;
    }
    setCursor(pos);
    // qDebug() << pos;
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
}

QString PlayQueue::modeIcon() const {
    switch (m_mode) {
    case IN_ORDER:
        return "qrc:/qt/qml/RxUI/assets/play-order.svg";
    case REVERSED:
        return "qrc:/qt/qml/RxUI/assets/play-reverse.svg";
    case LOOP_ALL:
        return "qrc:/qt/qml/RxUI/assets/play-repeat-all.svg";
    case LOOP_ONE:
        return "qrc:/qt/qml/RxUI/assets/play-repeat-one.svg";
    case RANDOM:
        return "qrc:/qt/qml/RxUI/assets/play-random.svg";
    }
    return "qrc:/qt/qml/RxUI/assets/play-order.svg";
}

void PlayQueue::addMediaByUrl(const QString& url) {
    try {
        auto media = Parser::parse(url);
        addMedia(media);
        saveStorage();
    } catch (...) {
        // ...
    }
}

void PlayQueue::addMediaByUrlThenPlay(const QString& url) {
    try {
        auto media = Parser::parse(url);
        addMediaThenPlay(media);
        saveStorage();
    } catch (...) {
        // ...
    }
}

void PlayQueue::addMediasByUrlsThenPlay(const QList<QUrl>& urls) {
    setLoading(true);
    auto* taskWatcher = new QFutureWatcher<Media>(this);
    auto future = QtConcurrent::mapped(urls, [=](const QUrl& path) {
        try {
            auto media = Parser::parse(QByteArray::fromPercentEncoding(path.toString().toUtf8()));
            return media;
        } catch (...) {
            return Media::null();
        }
    });
    connect(taskWatcher, &QFutureWatcher<Media>::finished, this, [=]() {
        for (auto& m : future.results()) {
            if (m.type() != UNKNOWN) addMedia(m);
        }
        setLoading(false);
        play(cursor() + 1);
        saveStorage();
    });
    taskWatcher->setFuture(future);
}

bool PlayQueue::loading() const { return m_loading; }

void PlayQueue::setLoading(bool n) {
    if (m_loading == n) return;
    m_loading = n;
    emit loadingChanged(n);
}

void PlayQueue::loadSettings() {
    QSettings settings;
    settings.beginGroup("Queue");
    setMode(settings.value("Mode", IN_ORDER).toInt());
    settings.endGroup();
}

void PlayQueue::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Queue");
    settings.setValue("Mode", m_mode);
    settings.endGroup();
}

void PlayQueue::loadStorage() {
    setLoading(true);
    auto* taskWatcher = new QFutureWatcher<QVector<Media>>(this);
    auto future = QtConcurrent::run([=]() {
        try {
            return Storage::instance()->loadPlayQueue();
        } catch (std::runtime_error& e) {
            return QVector<Media>();
        }
    });
    connect(taskWatcher, &QFutureWatcher<QVector<Media>>::finished, this, [=]() {
        loadPlaylist_(future.result());
        setLoading(false);
        taskWatcher->deleteLater();
    });
    taskWatcher->setFuture(future);
}

void PlayQueue::saveStorage() {
    if (m_saving || loading()) return;
    auto* taskWatcher = new QFutureWatcher<void>(this);
    m_saving = true;
    auto future = QtConcurrent::run([=]() {
        try {
            // copy playlist here.
            // database operation is slow, read the playlist with const
            // reference may cause crash.
            auto storedPlaylist = *m_playlist;
            // qDebug() << "in saveStorage" << m_playlist->length() <<
            // m_playlist->first().title();
            Storage::instance()->storePlayQueue(storedPlaylist);
        } catch (std::runtime_error& e) {
            // ...
        }
    });
    connect(taskWatcher, &QFutureWatcher<void>::finished, this, [=]() {
        //        qDebug() << "Play queue saving finished";
        m_saving = false;
        taskWatcher->deleteLater();
    });
    taskWatcher->setFuture(future);
}

void PlayQueue::loadPlaylist_(const QVector<Media>& playlist) {
    // copy playlist
    m_playlist->append(playlist);
    m_model->reload();
}

void PlayQueue::loadFromLibrary(int pos) {
    m_playlist->clear();
    loadPlaylist_(Library::instance()->currentMedias());
    play(pos);
    saveStorage();
}

void PlayQueue::loadFromPlaylist(int pos) {
    m_playlist->clear();
    loadPlaylist_(Playlist::instance()->currentMedias());
    play(pos);
    saveStorage();
}

Q_INVOKABLE void PlayQueue::addToPlaylist(const QString& playlist) {
    auto media = current();
    Storage::instance()->linkMediaToPlaylist(media, playlist);
}

Q_INVOKABLE void PlayQueue::addMediaIndexToPlaylist(const int index, const QString& playlist) {
    auto media = m_playlist->at(index);
    Storage::instance()->linkMediaToPlaylist(media, playlist);
    Playlist::instance()->reload();
}

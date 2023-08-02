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

#include "parser/parser.h"

PlayQueue::PlayQueue(QObject* parent) : QObject(parent) {
    m_playlist = new QVector<Media>;
    m_model = new QueueModel(this);
    m_model->setQueue(m_playlist);
    loadSettings();
}

PlayQueue::~PlayQueue() {
    saveSettings();
    delete m_playlist;
}

int PlayQueue::mode() const { return m_mode; }

QVector<int> generatePlayOrder(QueueMode mode, int cursor, int* cursorPos,
                               int size) {
    QVector<int> future;
    switch (mode) {
        case LOOP_ALL:
        case IN_ORDER:
            for (int i = 0; i < size; ++i) {
                future.push_back(i);
                if (i == cursor) *cursorPos = (int)future.size() - 1;
            }
            break;
        case REVERSED:
            for (int i = size - 1; i >= 0; --i) {
                future.push_back(i);
                if (i == cursor) *cursorPos = (int)future.size() - 1;
            }
            break;
        case LOOP_ONE:
            for (int i = 0; i < size; ++i) {
                future.push_back(cursor);
                *cursorPos = 0;
            }
            break;
        case RANDOM:
            for (int i = 0; i < size; ++i) {
                future.push_back(i);
                if (i == cursor) *cursorPos = (int)future.size() - 1;
            }
            std::shuffle(future.begin(), future.end(),
                         std::mt19937(std::random_device()()));
            break;
    }
    return future;
}

void PlayQueue::setMode(int mode) {
    if (m_mode == mode) {
        return;
    }
    m_mode = static_cast<QueueMode>(mode);
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
                                          (int)m_playlist->size());
    emit modeChanged(mode);
    emit modeIconChanged(modeIcon());
}

QVector<Media>* PlayQueue::queue() { return m_playlist; }

QueueModel* PlayQueue::model() { return m_model; }

int PlayQueue::cursor() const { return m_cursor; }

void PlayQueue::setCursor(int n) {
    if (n < 0 || n >= (int)m_playlist->size()) {
        return;
    }
    m_cursor = n;
    //    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
    //    (int) m_playlist->size());
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
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
                                          (int)m_playlist->size());
    if (m_playlist->size() == 1) m_cursorPos = -1;
}

void PlayQueue::removeMedia(int pos) {
    if (pos < 0 || pos >= (int)m_playlist->size()) {
        return;
    }
    m_playlist->remove(pos);
    m_model->removeMedia(pos);
    m_queuedPlayOrder = generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
                                          (int)m_playlist->size());
    if (cursor() > pos) {
        m_cursor -= 1;
    } else if (cursor() == pos) {
        setCursor(cursor() - 1);
    }
}

void PlayQueue::clearMedia() {
    m_playlist->clear();
    m_queuedPlayOrder.clear();
    m_model->reload();
}

void PlayQueue::moveMedia(int from, int to) {
    m_playlist->move(from, to);
    m_model->removeMedia(from);
    m_model->insertMedia(to);
}

void PlayQueue::loadPlaylist(const QVector<Media>& playlist) {
    // copy playlist
    m_playlist->clear();
    m_playlist->append(playlist);
    m_model->reload();
}

void PlayQueue::toggleChangeMode() { setMode((mode() + 1) % 5); }

void PlayQueue::next() {
    //    qDebug() << "next" << m_cursorPos << m_mode << m_queuedPlayOrder;
    if (m_playlist->empty()) return;
    //    qDebug() << m_cursorPos << m_queuedPlayOrder;
    if (m_cursorPos >= m_queuedPlayOrder.size() - 1) {
        if (m_mode == IN_ORDER || m_mode == REVERSED) {
            m_cursorPos = -1;
            return;
        }
        generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
                          (int)m_playlist->size());
        m_cursorPos = 0;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else {
        setCursor(m_queuedPlayOrder[++m_cursorPos]);
    }
}

void PlayQueue::prev() {
    //    qDebug() << "prev" << m_cursorPos << m_mode << m_queuedPlayOrder;
    if (m_playlist->empty()) return;
    if (m_cursorPos <= 0 && m_mode != RANDOM) {
        m_cursorPos = (int)m_queuedPlayOrder.size() - 1;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else if (m_cursorPos <= 0) {
        generatePlayOrder(m_mode, m_cursor, &m_cursorPos,
                          (int)m_playlist->size());
        m_cursorPos = (int)m_queuedPlayOrder.size() - 1;
        setCursor(m_queuedPlayOrder[m_cursorPos]);
    } else {
        setCursor(m_queuedPlayOrder[--m_cursorPos]);
    }
}

const Media& PlayQueue::current() {
    if (m_playlist->empty() || m_cursor < 0 || m_cursor >= m_playlist->size())
        return Media::null();
    return (*m_playlist)[m_cursor];
}

void PlayQueue::play(int pos) {
    if (pos >= (int)m_playlist->size()) {
        pos = (int)m_playlist->size() - 1;
    } else if (pos < 0) {
        pos = 0;
    }
    setCursor(pos);
    generatePlayOrder(m_mode, m_cursor, &m_cursorPos, (int)m_playlist->size());
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
    } catch (...) {
        // ...
    }
}

void PlayQueue::addMediasByUrlsThenPlay(const QStringList& urls) {
    setLoading(true);
    auto* taskWatcher = new QFutureWatcher<Media>(this);
    auto future = QtConcurrent::mapped(urls, [=](const QString& path) {
        try {
            auto media = Parser::parse(path);
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
    setMode(settings.value("mode", IN_ORDER).toInt());
    settings.endGroup();
}

void PlayQueue::saveSettings() const {
    QSettings settings;
    settings.beginGroup("Queue");
    settings.setValue("mode", m_mode);
    settings.endGroup();
}

int QueueModel::rowCount(const QModelIndex& parent) const {
    return (int)m_queue->size();
}

QVariant QueueModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= m_queue->size()) {
        return {};
    }

    switch (role) {
        case MediaTitleRole:
            return m_queue->at(index.row()).title();
        case MediaTypeRole:
            return m_queue->at(index.row()).type();
        case MediaArtistsRole:
            return m_queue->at(index.row()).artists();
        case MediaAlbumRole:
            return m_queue->at(index.row()).album();
        case MediaTimeRole:
            return m_queue->at(index.row()).time();
        default:
            return {};
    }
}

QHash<int, QByteArray> QueueModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MediaTitleRole] = "mediaTitle";
    roles[MediaTypeRole] = "mediaType";
    roles[MediaArtistsRole] = "mediaArtists";
    roles[MediaAlbumRole] = "mediaAlbum";
    roles[MediaTimeRole] = "mediaTime";
    return roles;
}

void QueueModel::setQueue(QVector<Media>* queue) {
    beginResetModel();
    m_queue = queue;
    endResetModel();
}

void QueueModel::removeMedia(int pos) {
    beginRemoveRows(QModelIndex(), pos, pos);
    endRemoveRows();
}

void QueueModel::insertMedia(int pos) {
    beginInsertRows(QModelIndex(), pos, pos);
    endInsertRows();
}

void QueueModel::reload() {
    beginResetModel();
    endResetModel();
}

QueueModel::QueueModel(QObject* parent) : QAbstractListModel(parent) {}

QueueModel::~QueueModel() = default;

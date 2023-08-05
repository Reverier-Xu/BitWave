/**
 * @file library.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "library.h"
#include <QDebug>
#include <QMutex>


Library* Library::m_instance = nullptr;

Library::Library(QObject* parent) : QObject(parent) { }

Library::~Library() = default;

Library* Library::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Library(parent);
        locker.unlock();
    }
    return m_instance;
}

QString Library::filter() const {
    return m_filter;
}

void Library::setFilter(const QString& filter) {
    if (m_filter == filter) {
        return;
    }
    m_filter = filter;
    emit filterChanged(m_filter);
}

void Library::load(const QString& route) {
    auto filter = route;
    filter.replace("libraries/", "");
    if (filter.isEmpty()) {
        filter = "music";
    }
    setFilter(filter);
}

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

Library::Library(QObject* parent) : QObject(parent) {
    setTitle(tr("Library"));
}

Library::~Library() = default;

QString Library::title() const {
    return m_title;
}

void Library::setTitle(const QString& title) {
    if (m_title == title) {
        return;
    }
    m_title = title;
    emit titleChanged(m_title);
}

void Library::load(const QString& adapter, const QString& filter) {
//    qDebug() << "load" << adapter << filter;
    if (adapter == "local") {
        setTitle(tr("Local Library"));
    } else if (adapter == "online") {
        setTitle(tr("Online Media"));
    } else {
        setTitle(tr("Library"));
    }
    setAdapter(adapter);
    if (filter.isEmpty())
        setFilter("music");
    else
        setFilter(filter);

    // TODO: implement
}

Library* Library::instance(QObject* parent) {
    static QMutex mutex;
    if (m_instance == nullptr) {
        QMutexLocker locker(&mutex);
        if (m_instance == nullptr) m_instance = new Library(parent);
        locker.unlock();
    }
    return m_instance;
}

QString Library::adapter() const {
    return m_adapter;
}

void Library::setAdapter(const QString& adapter) {
    if (m_adapter == adapter) {
        return;
    }
    m_adapter = adapter;
    emit adapterChanged(m_adapter);
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

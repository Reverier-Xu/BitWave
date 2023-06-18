/**
 * @file router.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-06-10
 *
 * @copyright 2023 Woo Tech
 */

#include "router.h"

#include <QDebug>


Router::Router(QObject* parent) : QObject(parent) {
    m_currentRoute = "player";
    m_history.push("player");
}

Router::~Router() = default;

QString Router::currentRoute() const {
    return m_currentRoute;
}

void Router::setCurrentRoute(const QString& path) {
    if (m_currentRoute == path) return;
    m_currentRoute = path;
    emit currentRouteChanged(path);
}

bool Router::hasPrevious() const {
    return m_history.length() > 1;
}

void Router::setHasPrevious(bool hasPrevious) {
    Q_UNUSED(hasPrevious)
    emit hasPreviousChanged(m_history.length() > 1);
}

void Router::push(const QString& path) {
    if (m_currentRoute == path) return;
    setCurrentRoute(path);
    m_history.push(path);
//    qDebug() << "History pushed: " << path;
    setHasPrevious(true);
}

void Router::pop() {
    if (m_history.empty()) return;
    m_history.pop();
    if (m_history.empty()) {
        push("player");
        return;
    }
    setCurrentRoute(m_history.top());
    // will automatically detect, just feel free at here.
    setHasPrevious(false);
}

void Router::clear() {
    m_history.clear();
    setCurrentRoute("player");
}

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
    m_history.push(path);
    emit currentRouteChanged(path);
}

bool Router::hasPrevious() const {
    return m_history.length() > 1;
}

void Router::setHasPrevious(bool hasPrevious) {
    if (hasPrevious == (m_history.length() > 1)) return;
    emit hasPreviousChanged(m_history.length() > 1);
}

void Router::push(const QString& path) {
    setCurrentRoute(path);
    m_history.push(path);
}

void Router::pop() {
    if (m_history.empty()) return;
    m_history.pop();
    if (m_history.empty()) {
        setCurrentRoute("player");
        return;
    }
    setCurrentRoute(m_history.top());
}

void Router::clear() {
    m_history.clear();
    setCurrentRoute("player");
}

/**
 * @file dbusscreensaver.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "dbusscreensaver.h"

#include <QApplication>
#include <QDBusInterface>
#include <QDBusReply>
#include <utility>

DBusScreensaver::DBusScreensaver(QString service, QString path, QString interface)
    : m_service(std::move(service)), m_path(std::move(path)), m_interface(std::move(interface)) {}

void DBusScreensaver::inhibit() {
    QDBusInterface screensaver(m_service, m_path, m_interface);
    QDBusReply<quint32> reply =
        screensaver.call("Inhibit", QApplication::applicationName(), QObject::tr("Video Playing"));
    if (reply.isValid()) {
        m_cookie = reply.value();
    }
}

void DBusScreensaver::unInhibit() {
    QDBusInterface screensaver(m_service, m_path, m_interface);
    screensaver.call("UnInhibit", m_cookie);
}

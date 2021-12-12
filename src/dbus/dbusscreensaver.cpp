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

DBusScreensaver::DBusScreensaver(QString service, QString path,
                                 QString interface)
    : service_(std::move(service)),
      path_(std::move(path)),
      interface_(std::move(interface)) {}

void DBusScreensaver::Inhibit() {
    QDBusInterface screensaver(service_, path_, interface_);
    QDBusReply<quint32> reply =
        screensaver.call("Inhibit", QApplication::applicationName(),
                         QObject::tr("Video Playing"));
    if (reply.isValid()) {
        cookie_ = reply.value();
    }
}

void DBusScreensaver::UnInhibit() {
    QDBusInterface screensaver(service_, path_, interface_);
    screensaver.call("UnInhibit", cookie_);
}

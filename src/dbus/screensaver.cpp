/**
 * @file screensaver.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#include "screensaver.h"

#include <QtGlobal>

#ifdef Qt5DBus_FOUND

#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include "dbusscreensaver.h"

#endif

#ifdef Q_OS_WIN32
#include "windowsscreensaver.h"
#endif

#include <QtDebug>

const char *Screensaver::kGnomeService = "org.gnome.ScreenSaver";
const char *Screensaver::kGnomePath = "/";
const char *Screensaver::kGnomeInterface = "org.gnome.ScreenSaver";
const char *Screensaver::kKdeService = "org.freedesktop.ScreenSaver";
const char *Screensaver::kKdePath = "/ScreenSaver";
const char *Screensaver::kKdeInterface = "org.freedesktop.ScreenSaver";

Screensaver *Screensaver::screensaver_ = nullptr;

Screensaver *Screensaver::GetScreensaver() {
    if (!screensaver_) {
#ifdef Qt5DBus_FOUND
        // qDebug() << "ScreenSaver inited here.";
        if (QDBusConnection::sessionBus().interface()->isServiceRegistered(
                kGnomeService)) {
            screensaver_ =
                new DBusScreensaver(kGnomeService, kGnomePath, kGnomeInterface);
        } else if (QDBusConnection::sessionBus()
                       .interface()
                       ->isServiceRegistered(kKdeService)) {
            screensaver_ =
                new DBusScreensaver(kKdeService, kKdePath, kKdeInterface);
        }
        // qDebug() << &screensaver_;
#endif
#ifdef Q_OS_WIN32
        screensaver_ = new WindowsScreensaver();
#endif
    }
    // qDebug() << screensaver_;
    return screensaver_;
}

/**
 * @file screensaver.cc
 * @author Reverier-Xu (reverier.xu@woooo.tech)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "screensaver.h"

#include <QtGlobal>

#ifdef QT_DBUS_LIB

#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include "dbusscreensaver.h"

#endif

#ifdef Q_OS_WIN32
#include "windowsscreensaver.h"
#endif

const char *Screensaver::kGnomeService = "org.gnome.ScreenSaver";
const char *Screensaver::kGnomePath = "/";
const char *Screensaver::kGnomeInterface = "org.gnome.ScreenSaver";
const char *Screensaver::kKdeService = "org.freedesktop.ScreenSaver";
const char *Screensaver::kKdePath = "/ScreenSaver";
const char *Screensaver::kKdeInterface = "org.freedesktop.ScreenSaver";

Screensaver *Screensaver::m_screensaver = nullptr;

Screensaver *Screensaver::getScreensaver() {
    if (!m_screensaver) {
#ifdef QT_DBUS_LIB
        // qDebug() << "ScreenSaver inited here.";
        if (QDBusConnection::sessionBus().interface()->isServiceRegistered(
                kGnomeService)) {
            m_screensaver =
                new DBusScreensaver(kGnomeService, kGnomePath, kGnomeInterface);
        } else if (QDBusConnection::sessionBus().interface()
                       ->isServiceRegistered(kKdeService)) {
            m_screensaver =
                new DBusScreensaver(kKdeService, kKdePath, kKdeInterface);
        }
        // qDebug() << &screensaver;
#endif
#ifdef Q_OS_WIN32
        m_screensaver = new WindowsScreensaver();
#endif
    }
    // qDebug() << screensaver;
    return m_screensaver;
}

/**
 * @file entry.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include <QApplication>
#include <QObject>
#include <QFont>
#include <QIcon>
#include <QObject>
#include <SingleApplication>

#ifdef Q_OS_LINUX

#include <malloc.h>

#endif

#include "managers/app_manager.h"

int main(int argc, char *argv[]) {
#ifdef Q_OS_LINUX
    mallopt(M_ARENA_MAX, 1);
#endif
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    std::setlocale(LC_NUMERIC, "C");

    SingleApplication app(argc, argv, true);

    if (app.isSecondary()) {
        bool ok = app.sendMessage((app.arguments().join(' ')).toUtf8(), 1000);
        if (ok) return 0;
    }

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("BitWave");
    QApplication::setOrganizationName("Wootec");
    QApplication::setOrganizationDomain("woooo.tech");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));

    auto main_app = AppManager();
    main_app.initialize();

    QObject::connect(&app, &SingleApplication::receivedMessage, &main_app,
            &AppManager::onSecondaryInstanceStarted);

    return QApplication::exec();
}
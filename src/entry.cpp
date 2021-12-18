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

#ifdef __unix__

#include <malloc.h>

#endif

#include "managers/app_manager.h"
#include "utilities/single_app_guard.h"

int main(int argc, char *argv[]) {
#ifdef Q_OS_LINUX
    mallopt(M_ARENA_MAX, 1);
#endif
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    std::setlocale(LC_NUMERIC, "C");

    RunGuard guard("BitWave");
    if (!guard.tryToRun()) return 0;

    QApplication app(argc, argv);

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("BitWave");
    QApplication::setOrganizationName("Wootec");
    QApplication::setOrganizationDomain("woooo.tech");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));

    auto main_app = AppManager();
    main_app.initialize();

    return QApplication::exec();
}
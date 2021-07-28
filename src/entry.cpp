//
// Created by Reverier-Xu on 2021/6/11.
//

#include <QApplication>
#include <QIcon>
#include <QFont>
#include "managers/app_manager.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    std::setlocale(LC_NUMERIC, "C");
    QApplication app(argc, argv);

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("BitWave");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));

    auto main_app = AppManager();
    main_app.initialize();

    return QApplication::exec();
}
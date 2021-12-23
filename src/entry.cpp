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
#include <QCommandLineParser>
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

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("BitWave");
    QApplication::setOrganizationName("Wootec");
    QApplication::setOrganizationDomain("woooo.tech");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));

    QCommandLineParser parser;
    parser.setApplicationDescription("A simple media player based on MPV and Qt.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QObject::tr("file"), QObject::tr("The media file to play."));
    
    parser.process(app);

    QStringList args = parser.positionalArguments();

    if (args.size() > 1) {
        qWarning() << "Only one media file can be specified.";
        return 1;
    }

    QString playFile = args.isEmpty() ? "" : args.first();

    if (app.isSecondary()) {
        bool ok = app.sendMessage(playFile.toUtf8(), 1000);
        if (ok) return 0;
    }

    auto main_app = AppManager();
    main_app.initialize(playFile);

    QObject::connect(&app, &SingleApplication::receivedMessage, &main_app,
            &AppManager::onSecondaryInstanceStarted);

    return QApplication::exec();
}
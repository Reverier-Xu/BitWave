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

    SingleApplication app(argc, argv, true,
                          SingleApplication::SecondaryNotification);

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("BitWave");
    QApplication::setOrganizationName("Wootec");
    QApplication::setOrganizationDomain("woooo.tech");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));
    QApplication::setDesktopFileName("tech.woooo.bitwave");

    QCommandLineParser parser;
    parser.setApplicationDescription(
        "A simple media player based on MPV and Qt.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QObject::tr("file"),
                                 QObject::tr("The media file to play."));

    QCommandLineOption resumeOption(QStringList() << "resume",
                                    QObject::tr("Resume the last playback."));
    parser.addOption(resumeOption);
    QCommandLineOption pauseOption(QStringList() << "pause",
                                   QObject::tr("Pause the playback."));
    parser.addOption(pauseOption);
    QCommandLineOption nextOption(QStringList() << "next",
                                  QObject::tr("Play the next media."));
    parser.addOption(nextOption);
    QCommandLineOption previousOption(QStringList() << "previous",
                                      QObject::tr("Play the previous media."));
    parser.addOption(previousOption);

    parser.process(app);

    if (parser.positionalArguments().size() > 1) {
        qWarning() << "Only one media file can be specified.";
        return 1;
    }

    QString playFile = parser.positionalArguments().isEmpty()
                           ? ""
                           : parser.positionalArguments().first();

    bool resume = parser.isSet(resumeOption);
    bool pause = parser.isSet(pauseOption);
    bool next = parser.isSet(nextOption);
    bool previous = parser.isSet(previousOption);

    if (resume + pause + next + previous + (!playFile.isEmpty()) > 1) {
        qWarning() << "Only one playback control option can be specified.";
        return 1;
    }

    char flags = resume << 0 | pause << 1 | next << 2 | previous << 3 | 1 << 4;

    if (app.isSecondary()) {
            app.sendMessage(flags + parser.positionalArguments().join(" ").toUtf8(), 3000);
        return 0;
    }

    auto main_app = AppManager();
    main_app.initialize(playFile);

    QObject::connect(&app, &SingleApplication::receivedMessage, &main_app,
                     &AppManager::onSecondaryInstanceMessageReceived);
    QObject::connect(&app, &SingleApplication::instanceStarted, &main_app,
                     &AppManager::onSecondaryInstanceStarted);

    return QApplication::exec();
}

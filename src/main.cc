/**
 * @file main.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include "codec/codec.h"
#include "parser/parser.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QIcon>
#include <QObject>
#include <SingleApplication>
#include <app/app.h>
#include <rxui/rxui.h>

int main(int argc, char* argv[]) {
    register_resources();
    std::setlocale(LC_NUMERIC, "C");

    SingleApplication app(argc, argv, true, SingleApplication::SecondaryNotification);

#ifdef Q_OS_WIN32
    auto defaultFont = QFont("Microsoft YaHei");
    QApplication::setFont(defaultFont);
#endif

    QApplication::setApplicationDisplayName("Bit Wave");
    QApplication::setApplicationName("Bit Wave");
    QApplication::setOrganizationName("Wootec");
    QApplication::setOrganizationDomain("woooo.tech");
    QApplication::setWindowIcon(QIcon(":/assets/logo-fill.svg"));
    QApplication::setDesktopFileName("tech.woooo.bitwave");

    QCommandLineParser parser;
    parser.setApplicationDescription("A simple media player based on MPV and Qt.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QObject::tr("file"), QObject::tr("The media file to play."));

    QCommandLineOption resumeOption(QStringList() << "resume", QObject::tr("Resume the last playback."));
    parser.addOption(resumeOption);
    QCommandLineOption pauseOption(QStringList() << "pause", QObject::tr("Pause the playback."));
    parser.addOption(pauseOption);
    QCommandLineOption nextOption(QStringList() << "next", QObject::tr("Play the next media."));
    parser.addOption(nextOption);
    QCommandLineOption previousOption(QStringList() << "previous", QObject::tr("Play the previous media."));
    parser.addOption(previousOption);

    parser.process(app);

    if (parser.positionalArguments().size() > 1) {
        qWarning() << "Only one media file can be specified.";
        return 1;
    }

    QString playFile = parser.positionalArguments().isEmpty() ? "" : parser.positionalArguments().first();

    bool resume = parser.isSet(resumeOption);
    bool pause = parser.isSet(pauseOption);
    bool next = parser.isSet(nextOption);
    bool previous = parser.isSet(previousOption);

    if (resume + pause + next + previous + (!playFile.isEmpty()) > 1) {
        qWarning() << "Only one playback control option can be specified.";
        return 1;
    }

    char flags = (char)(resume << 0 | pause << 1 | next << 2 | previous << 3 | 1 << 4);

    if (app.isSecondary()) {
        app.sendMessage(flags + playFile.toUtf8(), 3000);
        return 0;
    }

    Codec::initCodecs();
    Parser::initParsers();

    auto app_window = App(nullptr);
    app_window.initialize(playFile);

    QObject::connect(&app, &SingleApplication::receivedMessage, &app_window, &App::onSecondaryInstanceMessageReceived);
    QObject::connect(&app, &SingleApplication::instanceStarted, &app_window, &App::onSecondaryInstanceStarted);

    auto ret = QApplication::exec();

    Codec::destroyCodecs();
    Parser::destroyParsers();

    return ret;
}

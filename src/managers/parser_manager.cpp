/**
 * @file parser_manager.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "parser_manager.h"

#include <QColor>
// #include <QDebug>
#include <QImage>
#include <QUrl>

#include "parsers/local_music_parser.h"
#include "parsers/local_netease_music_parser.h"
#include "parsers/local_video_parser.h"
#include "utilities/color_helper.h"
// #include "utilities/memory_helper.h"

ParserManager *ParserManager::mInstance = nullptr;

ParserManager *ParserManager::instance(QObject *parent) {
    if (mInstance == nullptr) mInstance = new ParserManager(parent);
    return mInstance;
}

ParserManager::ParserManager(QObject *parent) : QObject(parent) {
    registerParsersInFactory();
}

void ParserManager::registerParsersInFactory() {
    auto factory = ParserFactory::instance(this);
    ParserFactory::registerParser(new LocalMusicParser(factory));
    ParserFactory::registerParser(new LocalVideoParser(factory));
    ParserFactory::registerParser(new LocalNeteaseMusicParser(factory));
    // qDebug() << "Finished register parsers";
}

void ParserManager::handleParseMediaRequest(const Media &media) {
    // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest Common
    // Begin"); qDebug() << "parseMediaRequest received here.";
    auto parser = ParserFactory::getParser(media);
    if (parser == nullptr) {
        qDebug() << "No parser found for media:" << media.rawUrl();
        emit mediaIsReady(false, Media());
        return;
    }
    parser->setParserId(QUuid::createUuid());
    mParseMediaTaskId = parser->parserId();
    auto res = QtConcurrent::run(parser, &BaseParser::parseMedia, media);
    auto *resWatcher = new QFutureWatcher<Media>(this);
    resWatcher->setFuture(res);
    // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest Common
    // End");
    connect(resWatcher, &QFutureWatcher<Media>::finished, [=]() {
        // qDebug() << "Parse Finished here.";
        try {
            // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest
            // Run Begin");
            auto m = resWatcher->result();
            if (parser->parserId() == mParseMediaTaskId) {
                // qDebug() << "Parse Result is current media.";
                emit mediaIsReady(true, m);
            } else {
                // qDebug() << "Parse Result is discarded.";
            }
            parser->deleteLater();
            resWatcher->deleteLater();
            // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest
            // Run End");
        } catch (...) {
            //            qDebug() << "Parse Result creates an exception.";
            emit mediaIsReady(false, Media());
            parser->deleteLater();
            resWatcher->deleteLater();
        }
    });
    // qDebug() << "parse finished here.";
}

void ParserManager::handleGetMediaInfoRequest(const QString &path) {
    // qDebug() << "handleGetMediaInfoRequest received here.";
    // MemoryHelper::assertMemory("ParserManager::handleGetMediaInfoRequest
    // Begin");
    auto parser = ParserFactory::getParser(path);
    if (parser == nullptr) {
        qDebug() << "No parser found for media:" << path;
        emit mediaInfoIsReady(false, Media());
        return;
    }
    try {
        auto res = parser->getMedia(path);
        parser->deleteLater();
        // MemoryHelper::assertMemory("ParserManager::handleGetMediaInfoRequest
        // End");
        emit mediaInfoIsReady(true, res);
    } catch (...) {
        parser->deleteLater();
        emit mediaInfoIsReady(false, Media());
    }
}

void ParserManager::handleGetMediaCoverRequest(const Media &media) {
    // qDebug() << "Parse extern media started.";
    // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverRequest
    // Begin");
    auto parser = ParserFactory::getParser(media);
    if (parser == nullptr) {
        qDebug() << "No parser found for media:" << media.rawUrl();
        emit mediaCoverIsReady(false, "");
        return;
    }
    try {
        auto url = parser->getMediaCover(media);
        parser->deleteLater();
        // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverRequest
        // End"); qDebug() << "cover finished here.";
        emit mediaCoverIsReady(true, url);
    } catch (...) {
        parser->deleteLater();
        emit mediaCoverIsReady(false, "");
    }
}

// void ParserManager::handleGetExternMediaInfoRequest(const QString &path) {
//     // qDebug() << "Analyze extern media info started.";
//     //
//     MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
//     // Begin");
//     auto parser = ParserFactory::getParser(path);
//     //
//     MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
//     // GotParser");
//     if (!parser) {
//         emit mediaIsReady(false, Media());
//         return;
//     }
//     try {
//         auto res = parser->getMedia(path);
//         parser->deleteLater();
//         //
//         MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
//         // End");
//         emit externMediaInfoIsReady(true, res);
//     } catch (...) {
//         parser->deleteLater();
//         emit externMediaInfoIsReady(false, Media());
//     }
// }

void ParserManager::handleGetMediaCoverColorRequest(const QString &cover) {
    // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverColorRequest
    // Begin"); get cover theme color here.

    // -*- begin -*-
    /**
     * ???????????????????????????????????????????????????????????????color??????????????????return???
     * ???????????????????????????????????????????????????????????????????????????
     * ??????????????????????????????????????????????????????
     */
    // qDebug() << "Analyze cover color started.";
    auto image = QImage();
    try {
        image.load(QUrl(cover).path());

        // code here.
        if (image.isNull()) throw std::exception();

        auto color = ColorHelper::getImageThemeColor(image.scaled(100, 100));

        // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverColorRequest
        // End");

        emit mediaCoverColorIsReady(true, color);

        // qDebug() << "Analyze cover color succeeded.";

    } catch (...) {
        emit mediaCoverColorIsReady(false, QColor());
        // qDebug() << "Analyze cover color failed.";
    }
}

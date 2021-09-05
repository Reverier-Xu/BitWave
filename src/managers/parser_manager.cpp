/*
 * parser_manager.h
 *
 * Summary: manage parsers and do parse works in new thread.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-06-26
 * Last Modified: 2021-08-12
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
    if (mInstance == nullptr)
        mInstance = new ParserManager(parent);
    return mInstance;
}

ParserManager::ParserManager(QObject *parent)
        : QObject(parent) {
    this->registerParsersInFactory();
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
        emit this->mediaIsReady(false, Media());
        return;
    }
    parser->setParserId(QUuid::createUuid());
    this->mParseMediaTaskId = parser->parserId();
    auto res = QtConcurrent::run(parser, &BaseParser::parseMedia, media);
    auto *resWatcher = new QFutureWatcher<Media>(this);
    resWatcher->setFuture(res);
    // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest Common
    // End");
    connect(resWatcher, &QFutureWatcher<Media>::finished, [=]() {
        // qDebug() << "Parse Finished here.";
        try {
            // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest Run
            // Begin");
            auto m = resWatcher->result();
            if (parser->parserId() == this->mParseMediaTaskId) {
                // qDebug() << "Parse Result is current media.";
                emit this->mediaIsReady(true, m);
            } else {
                // qDebug() << "Parse Result is discarded.";
            }
            parser->deleteLater();
            resWatcher->deleteLater();
            // MemoryHelper::assertMemory("ParserManager::handleParseMediaRequest Run
            // End");
        } catch (...) {
            //            qDebug() << "Parse Result creates an exception.";
            emit this->mediaIsReady(false, Media());
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
    try {
        auto res = parser->getMedia(path);
        parser->deleteLater();
        // MemoryHelper::assertMemory("ParserManager::handleGetMediaInfoRequest
        // End");
        emit this->mediaInfoIsReady(true, res);
    } catch (...) {
        parser->deleteLater();
        emit this->mediaInfoIsReady(false, Media());
    }
}

void ParserManager::handleGetMediaCoverRequest(const Media &media) {
    // qDebug() << "Parse extern media started.";
    // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverRequest
    // Begin");
    auto parser = ParserFactory::getParser(media);
    try {
        auto url = parser->getMediaCover(media);
        parser->deleteLater();
        // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverRequest
        // End"); qDebug() << "cover finished here.";
        emit this->mediaCoverIsReady(true, url);
    } catch (...) {
        parser->deleteLater();
        emit this->mediaCoverIsReady(false, "");
    }
}

void ParserManager::handleGetMediaLyricsRequest(const Media &media) {
    emit this->mediaLyricsIsReady(true, tr("[00:00.00] No Lyrics."), "");
}

void ParserManager::handleGetExternMediaInfoRequest(const QString &path) {
    // qDebug() << "Analyze extern media info started.";
    // MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
    // Begin");
    auto parser = ParserFactory::getParser(path);
    // MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
    // GotParser");
    if (!parser) {
        emit this->mediaIsReady(false, Media());
        return;
    }
    try {
        auto res = parser->getMedia(path);
        parser->deleteLater();
        // MemoryHelper::assertMemory("ParserManager::handleGetExternMediaInfoRequest
        // End");
        emit this->externMediaInfoIsReady(true, res);
    } catch (...) {
        parser->deleteLater();
        emit this->externMediaInfoIsReady(false, Media());
    }
}

void ParserManager::handleGetMediaCoverColorRequest(const QString &cover) {
    // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverColorRequest
    // Begin"); get cover theme color here.

    // -*- begin -*-
    /* 在这里提取音乐封面的主色调，提取完毕后塞到color里就行，不要return。
   * 此实例跑在另一个线程上，因此不用过于担心效率问题，
   * 理论上讲在一首歌播完之前跑出来就行。
   */
    // qDebug() << "Analyze cover color started.";
    auto image = QImage();
    try {
        image.load(QUrl(cover).path());

        // code here.
        if (image.isNull())
            throw std::exception();

        auto color = ColorHelper::getImageThemeColor(image.scaled(100, 100));

        // MemoryHelper::assertMemory("ParserManager::handleGetMediaCoverColorRequest
        // End");

        emit this->mediaCoverColorIsReady(true, color);

        // qDebug() << "Analyze cover color succeeded.";

    } catch (...) {
        emit this->mediaCoverColorIsReady(false, QColor());
        // qDebug() << "Analyze cover color failed.";
    }
}

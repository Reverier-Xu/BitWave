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

#include <QUrl>
#include <QDebug>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include "parser_manager.h"
#include "parsers/local_music_parser.h"
#include "parsers/local_netease_music_parser.h"
#include "parsers/local_video_parser.h"
#include "utilities/color_helper.h"


ParserManager *ParserManager::mInstance = nullptr;

ParserManager *ParserManager::instance(QObject *parent) {
    if (mInstance == nullptr)
        mInstance = new ParserManager(parent);
    return mInstance;
}

ParserManager::ParserManager(QObject *parent) : QObject(parent) {
    this->registerParsersInFactory();
}

void ParserManager::registerParsersInFactory() {
    ParserFactory::registerParser(new LocalMusicParser(this));
    ParserFactory::registerParser(new LocalVideoParser(this));
    ParserFactory::registerParser(new LocalNeteaseMusicParser(this));
    // qDebug() << "Finished register parsers";
}

void ParserManager::handleParseMediaRequest(const Media &media) {
    // qDebug() << "parseMediaRequest received here.";
    auto parser = ParserFactory::getParser(media);
    auto res = parser->parseMedia(media);
    // qDebug() << "parse finished here.";
    parser->deleteLater();
    emit this->mediaIsReady(true, res);
}

void ParserManager::handleGetMediaInfoRequest(const QString &path) {
    auto parser = ParserFactory::getParser(path);
    auto res = parser->getMedia(path);
    parser->deleteLater();
    emit this->mediaInfoIsReady(true, res);
}

void ParserManager::handleGetMediaCoverRequest(const Media &media) {
    auto parser = ParserFactory::getParser(media);
    try {
        auto url = parser->getMediaCover(media);
        parser->deleteLater();
        // qDebug() << "cover finished here.";
        emit this->mediaCoverIsReady(true, url);
    } catch (std::exception& err) {
        parser->deleteLater();
        emit this->mediaCoverIsReady(false, "");
    }

}

void ParserManager::handleGetMediaLyricsRequest(const Media &media) {
    emit this->mediaLyricsIsReady(true, tr("[00:00.00] No Lyrics."), "");
}

void ParserManager::handleGetExternMediaInfoRequest(const QString &path) {
    auto parser = ParserFactory::getParser(path);
    auto res = parser->getMedia(path);
    parser->deleteLater();
    emit this->externMediaInfoIsReady(true, res);
}

void ParserManager::handleGetMediaCoverColorRequest(const QString &cover) {
    // get cover theme color here.

    // -*- begin -*-
    /* TODO: 在这里提取音乐封面的主色调，提取完毕后塞到color里就行，不要return。
     * 此实例跑在另一个线程上，因此不用过于担心效率问题，
     * 理论上讲在一首歌播完之前跑出来就行。
     */
    // qDebug() << "analyze color started.";
    auto image = QImage();
    image.load(QUrl(cover).path());

    // code here.

    auto color = ColorHelper::getImageThemeColor(image.scaled(100, 100));

    // -*- END -*-

    emit this->mediaCoverColorIsReady(true, color);
}

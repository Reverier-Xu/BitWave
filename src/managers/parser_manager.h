/**
 * @file parser_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QFuture>
#include <QObject>
#include <QUuid>
#include <QtConcurrent/QtConcurrent>

#include "parsers/base_parser.h"
#include "parsers/parser_factory.h"

class ParserManager : public QObject {
    Q_OBJECT
   private:
    QUuid mParseMediaTaskId;

   protected:
    explicit ParserManager(QObject *parent);

    static ParserManager *mInstance;

   public:
    static ParserManager *instance(QObject *parent = nullptr);

    void registerParsersInFactory();

   public slots:

    void handleParseMediaRequest(const Media &media);

    void handleGetMediaInfoRequest(const QString &path);

    void handleGetMediaCoverRequest(const Media &media);

    void handleGetMediaLyricsRequest(const Media &media);

    // void handleGetExternMediaInfoRequest(const QString &path);

    void handleGetMediaCoverColorRequest(const QString &cover);

   signals:

    void mediaInfoIsReady(bool ok, const Media &m);

    // void externMediaInfoIsReady(bool ok, const Media &m);

    void mediaIsReady(bool ok, const Media &m);

    void mediaCoverIsReady(bool ok, const QString &m);

    void mediaLyricsIsReady(bool ok, const QString &raw, const QString &trans);

    void mediaCoverColorIsReady(bool ok, const QColor &color);
};

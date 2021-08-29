/*
 * parser_manager.cpp
 *
 * Summary: manage parsers and do parse works in new thread.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-06-26
 * Last Modified: 2021-08-12
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

    void handleGetExternMediaInfoRequest(const QString &path);

    void handleGetMediaCoverColorRequest(const QString &cover);

signals:

    void mediaInfoIsReady(bool ok, const Media &m);

    void externMediaInfoIsReady(bool ok, const Media &m);

    void mediaIsReady(bool ok, const Media &m);

    void mediaCoverIsReady(bool ok, const QString &m);

    void mediaLyricsIsReady(bool ok, const QString &raw, const QString &trans);

    void mediaCoverColorIsReady(bool ok, const QColor &color);
};

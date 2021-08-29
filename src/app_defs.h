//
// Created by Reverier-Xu on 2021/2/3.
//

#pragma once

#include <QByteArray>
#include <QString>

/*
 * Netease Cloud Music API keys.
 *
 */

class BitWaveConstants {
public:
    static QByteArray webApiAesKey();

    static QByteArray webApiAesIv();

    static QString webApiRsaPubkeyN();

    static long webApiRsaPubkeyE() { return 65537; };

    static QByteArray linuxApiAesKey();

    static QByteArray cliApiDigestSalt();

    static QByteArray cliApiDataSalt();

    static QByteArray cliApiAesKey();

    static QByteArray base62Table();

    static QByteArray cliApiUa();

    static QByteArray ncmCoreKey();

    static QByteArray ncmModifyKey();

    static QByteArray pngHeader();

    static QString getNeteaseLyricsApiAddress(const QString &musicId);
};

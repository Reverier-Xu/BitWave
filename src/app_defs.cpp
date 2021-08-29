//
// Created by Reverier-Xu on 2021/2/17.
//

#include "app_defs.h"

/*
 * Netease Cloud Music API Keys.
 *
 */
const char *WEB_API_AES_KEY = "0CoJUm6Qyw8W8jud"; // cbc
const char *WEB_API_AES_IV = "0102030405060708"; // cbc
const char *WEB_API_RSA_PUBKEY_N = "0x00e0b509f6259df8642dbc35662901477df22677ec152b5ff68ace615bb7b725152b3ab1"
                                   "7a876aea8a5aa76d2e417629ec4ee341f56135fccf695280104e0312ecbda92557c9387011"
                                   "4af6c9d05c4f7f0c3685b7a46bee255932575cce10b424d813cfe4875d3e82047b97ddef52"
                                   "741d546b8e289dc6935b3ece0462db0a22b8e7";
const int WEB_API_RSA_PUBKEY_E = 65537; // e
const char *LINUX_API_AES_KEY = "rFgB&h#%2?^eDg:Q"; // ecb
const char *CLI_API_DIGEST_SALT = "nobody%(url)suse%(text)smd5forencrypt";
const char *CLI_API_DATA_SALT = "%(url)s-36cd479b6b5-%(text)s-36cd479b6b5-%(digest)s";
const char *CLI_API_AES_KEY = "e82ckenh8dichen8"; // ecb
const char *BASE62 = "PJArHa0dpwhvMNYqKnTbitWfEmosQ9527ZBx46IXUgOzD81VuSFyckLRljG3eC";
const char *CLI_API_UA = "NeteaseMusic/7.2.24.1597753235(7002024);Dalvik/2.1.0 (Linux; U; Android "
                         "11; Pixel 2 XL Build/RP1A.200720.009)";

/*
 *  NCM Format Audio File Decrypt keys.
 *
 */
const unsigned char NCM_DEC_S_CORE_KEY[16] = {0x68, 0x7A, 0x48, 0x52,
                                              0x41, 0x6D, 0x73, 0x6F,
                                              0x35, 0x6B, 0x49, 0x6E,
                                              0x62, 0x61, 0x78, 0x57};
const unsigned char NCM_DEC_S_MODIFY_KEY[16] = {0x23, 0x31, 0x34, 0x6C,
                                                0x6A, 0x6B, 0x5F, 0x21,
                                                0x5C, 0x5D, 0x26, 0x30,
                                                0x55, 0x3C, 0x27, 0x28};
const unsigned char NCM_DEC_PNG_HEADER[8] = {0x89, 0x50, 0x4E, 0x47,
                                             0x0D, 0x0A, 0x1A, 0x0A};

QByteArray
BitWaveConstants::webApiAesKey() {
    return QByteArray(WEB_API_AES_KEY);
}

QByteArray
BitWaveConstants::webApiAesIv() {
    return QByteArray(WEB_API_AES_IV);
}

QString
BitWaveConstants::webApiRsaPubkeyN() {
    return QString(WEB_API_RSA_PUBKEY_N);
}

QByteArray
BitWaveConstants::linuxApiAesKey() {
    return QByteArray(LINUX_API_AES_KEY);
}

QByteArray
BitWaveConstants::cliApiDigestSalt() {
    return QByteArray(CLI_API_DIGEST_SALT);
}

QByteArray
BitWaveConstants::cliApiDataSalt() {
    return QByteArray(CLI_API_DATA_SALT);
}

QByteArray
BitWaveConstants::cliApiAesKey() {
    return QByteArray(CLI_API_AES_KEY);
}

QByteArray
BitWaveConstants::base62Table() {
    return QByteArray(BASE62);
}

QByteArray
BitWaveConstants::cliApiUa() {
    return QByteArray(CLI_API_UA);
}

QByteArray
BitWaveConstants::ncmCoreKey() {
    QByteArray res;
    for (auto i : NCM_DEC_S_CORE_KEY) {
        res.append(i);
    }
    return res;
}

QByteArray
BitWaveConstants::ncmModifyKey() {
    QByteArray res;
    for (auto i : NCM_DEC_S_MODIFY_KEY) {
        res.append(i);
    }
    return res;
}

QByteArray
BitWaveConstants::pngHeader() {
    QByteArray res;
    for (auto i : NCM_DEC_PNG_HEADER) {
        res.append(i);
    }
    return res;
}

QString
BitWaveConstants::getNeteaseLyricsApiAddress(const QString &musicId) {
    return "https://music.163.com/api/song/lyric?id=" + musicId + "&lv=1&kv=-1&tv=1";
}

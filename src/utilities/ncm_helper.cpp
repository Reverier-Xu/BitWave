//
// Created by Reverier-Xu on 2021/2/17.
//

#include "ncm_helper.h"
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <fstream>
#include <QtWidgets/QLabel>
#include <QStandardPaths>

extern "C" {
#include <libavformat/avformat.h>
}

#include "app_defs.h"

QString getFileFormat(const QString &key);

Media NcmHelper::getMediaFromPath(const QString &path) {
    QFile ncmFile(QUrl(path).path());
    Media media;
    media.setRawUrl(path);
    media.setType(AUDIO);
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    quint32 header;
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4e455443) {
        throw std::exception();
    }
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4d414446) {
        throw std::exception();
    }
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char *>(&key_len), sizeof(key_len));
    ncmFile.seek(ncmFile.pos() + key_len);

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char *>(&metadata_len), sizeof(metadata_len));
    QByteArray modifyData;

    if (metadata_len > 0) {
        modifyData = ncmFile.read(metadata_len);

        for (int i = 0; i < metadata_len; i++) {
            modifyData[i] = modifyData[i] ^ 0x63;
        }
        media.setComment(modifyData);
        NcmHelper::getMetadataFrom163Key(media);
        ncmFile.close();
        return media;
    } else {
        ncmFile.close();
        throw std::exception();
    }
}

QString NcmHelper::dump(const Media &media) {
    QFile ncmFile(QUrl(media.rawUrl()).path());
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    quint32 header;
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4e455443) {
        throw std::exception();
    }
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4d414446) {
        throw std::exception();
    }
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char *>(&key_len), sizeof(key_len));
    if (key_len <= 0)
        throw std::exception();
    QByteArray core_key_data = ncmFile.read(key_len);

    for (int i = 0; i < key_len; i++) {
        core_key_data[i] = core_key_data[i] ^ 0x64;
    }

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray m_core_key = encryption.decode(core_key_data, BitWaveConstants::ncmCoreKey());

    m_core_key = m_core_key.left(m_core_key.length() - m_core_key[m_core_key.length() - 1]);

    // get key box
    QByteArray mCoreKeyBox;
    for (int i = 0; i < 256; i++) {
        mCoreKeyBox.append((quint8) i);
    }

    unsigned char swap;
    unsigned char c;
    unsigned char last_byte = 0;
    int key_offset = 0;

    for (int i = 0; i < 256; i++) {
        swap = mCoreKeyBox[i];
        quint8 b = (swap + last_byte + (unsigned char) (m_core_key[17 + key_offset++]));
        c = (b & 0xff);
        if (key_offset >= m_core_key.length() - 17) key_offset = 0;
        mCoreKeyBox[i] = mCoreKeyBox[c];
        mCoreKeyBox[c] = swap;
        last_byte = c;
    }

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char *>(&metadata_len), sizeof(metadata_len));
    ncmFile.seek(ncmFile.pos() + metadata_len);

    ncmFile.seek(ncmFile.pos() + 9);
    quint32 imgSize;
    ncmFile.read(reinterpret_cast<char *>(&imgSize), sizeof(imgSize));
    ncmFile.seek(ncmFile.pos() + imgSize);

    // dump the main music.
    QByteArray musicContent;
    auto cachePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/BitWave";
    auto outPath = cachePath + "/CachedSongs/" + QFileInfo(QUrl(media.rawUrl()).path()).baseName();
    if (getFileFormat(media.comment()) == "mp3") outPath += ".mp3";
    else outPath += ".flac";
    QFile outMusic(outPath);
    outMusic.open(QFile::WriteOnly);

    while (!ncmFile.atEnd()) {
        musicContent = ncmFile.read(0x8000);
        // qDebug() << musicContent.size();
        for (int i = 0; i < musicContent.size(); i++) {
            int j = (i + 1) & 0xff;
            musicContent[i] =
                    musicContent[i] ^ mCoreKeyBox[(mCoreKeyBox[j] + mCoreKeyBox[(mCoreKeyBox[j] + j) & 0xff]) & 0xff];
        }
        outMusic.write(musicContent);
    }
    outMusic.close();
    ncmFile.close();
    return outPath;
}

QString getFileFormat(const QString &key) {
    QByteArray modifyData = key.toLocal8Bit();
    QByteArray swapModifyData;
    QByteArray modifyOutData;
    QByteArray modifyDecryptData;

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);

    swapModifyData = modifyData.right(modifyData.length() - 22);

    modifyOutData = QByteArray::fromBase64(swapModifyData);

    modifyDecryptData = encryption.decode(modifyOutData, BitWaveConstants::ncmModifyKey());

    // escape `music:`
    modifyDecryptData = modifyDecryptData.right(modifyDecryptData.length() - 6);

    modifyDecryptData = modifyDecryptData.left(
            modifyDecryptData.length() - modifyDecryptData[modifyDecryptData.length() - 1]);
    QJsonObject meta_obj = QJsonDocument::fromJson(modifyDecryptData).object();
    return meta_obj["format"].toString();
}

void NcmHelper::getMetadataFrom163Key(Media &dst) {
    QByteArray modifyData = dst.comment().toLocal8Bit();
    QByteArray swapModifyData;
    QByteArray modifyOutData;
    QByteArray modifyDecryptData;

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);

    swapModifyData = modifyData.right(modifyData.length() - 22);

    modifyOutData = QByteArray::fromBase64(swapModifyData);

    modifyDecryptData = encryption.decode(modifyOutData, BitWaveConstants::ncmModifyKey());

    // escape `music:`
    modifyDecryptData = modifyDecryptData.right(modifyDecryptData.length() - 6);

    modifyDecryptData = modifyDecryptData.left(
            modifyDecryptData.length() - modifyDecryptData[modifyDecryptData.length() - 1]);
    QJsonObject meta_obj = QJsonDocument::fromJson(modifyDecryptData).object();
    dst.setTitle(meta_obj["musicName"].toString());
    dst.setCollection(meta_obj["album"].toString());
    dst.setCoverUrl(meta_obj["albumPic"].toString());

    dst.setDuration(meta_obj["duration"].toVariant().toDouble() / 1000);

    auto artists = meta_obj["artist"].toArray();
    QString artist;
    for (auto i: artists) {
        artist += i.toArray()[0].toString() + "/";
    }
    artist = artist.left(artist.length() - 1);
    dst.setArtist(artist);
}

QString NcmHelper::dumpMediaCover(const Media &media) {
    QFile ncmFile(QUrl(media.rawUrl()).path());
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    quint32 header;
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4e455443) {
        ncmFile.close();
        throw std::exception();
    }
    ncmFile.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header != (unsigned int) 0x4d414446) {
        ncmFile.close();
        throw std::exception();
    }
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char *>(&key_len), sizeof(key_len));
    ncmFile.seek(ncmFile.pos() + key_len);

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char *>(&metadata_len), sizeof(metadata_len));
    ncmFile.seek(ncmFile.pos() + metadata_len);

    ncmFile.seek(ncmFile.pos() + 9);
    quint32 imgSize;
    ncmFile.read(reinterpret_cast<char *>(&imgSize), sizeof(imgSize));

    if (imgSize > 0) {
        QByteArray img = ncmFile.read(imgSize);
        QImage avatar;
        avatar.loadFromData(img);
        auto cachePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/BitWave";
        auto temp_loc = cachePath + "/Covers/" + media.title().replace("/", "").replace("\\", "") + ".jpg";
        avatar = avatar.scaled(320, 320);
        avatar.save(temp_loc);
        ncmFile.close();
        return "file://" + temp_loc;
    } else {
        ncmFile.close();
        throw std::exception();
    }
}

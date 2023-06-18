/**
 * @file ncm_codec.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2020-01-12
 *
 * @copyright 2020 Woo Tech
 */

#include <QFileInfo>
#include <QUrl>
#include <QStandardPaths>
#include "ncm_codec.h"
#include "utils/ncm_helper.h"
#include "qaesencryption.h"


const QStringList& NcmCodec::supportedFormats() {
    return m_supportedFormats;
}

bool NcmCodec::accepted(const Media& src) {
    auto fileInfo = QFileInfo(QUrl(src.url()).path());
    return m_supportedFormats.contains(fileInfo.suffix());
}

QString NcmCodec::decode(const Media& src) {
    QFile ncmFile(QUrl(src.url()).path());
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    if (!validateNcmFile(ncmFile))
        throw std::runtime_error("Not a ncm file.");
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
    if (key_len <= 0) throw std::runtime_error("No key in ncm file.");
    QByteArray core_key_data = ncmFile.read(key_len);
    for (int i = 0; i < key_len; i++) {
        core_key_data[i] ^= (qint8) 0x64;
    }
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray m_core_key =
        encryption.decode(core_key_data, QByteArray::fromRawData((const char*) NCM_DEC_S_CORE_KEY, 16));

    m_core_key = m_core_key.left(m_core_key.length() - m_core_key[m_core_key.length() - 1]);

    // get key box
    QByteArray mCoreKeyBox;
    for (int i = 0; i < 256; i++)
        mCoreKeyBox.append((qint8) i);

    quint8 swap;
    quint8 c;
    quint8 last_byte = 0;
    int key_offset = 0;

    for (int i = 0; i < 256; i++) {
        swap = mCoreKeyBox[i];
        quint8 b = swap + last_byte + (quint8)(m_core_key[17 + key_offset++]);
        c = (b & 0xff);
        if (key_offset >= m_core_key.length() - 17) key_offset = 0;
        mCoreKeyBox[i] = mCoreKeyBox[c];
        mCoreKeyBox[c] = (qint8)swap;
        last_byte = c;
    }

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char*>(&metadata_len), sizeof(metadata_len));
    ncmFile.seek(ncmFile.pos() + metadata_len);

    ncmFile.seek(ncmFile.pos() + 9);
    quint32 imgSize;
    ncmFile.read(reinterpret_cast<char*>(&imgSize), sizeof(imgSize));
    ncmFile.seek(ncmFile.pos() + imgSize);

    // dump the main music.
    QByteArray musicContent;
    auto cachePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/BitWave";
    auto outPath = cachePath + "/CachedSongs/" + QFileInfo(src.url()).baseName() + "."
        + getDecryptedMetadata(src.comment()).value("format").toString();
    QFile outMusic(outPath);
    outMusic.open(QFile::WriteOnly);

    while (!ncmFile.atEnd()) {
        musicContent = ncmFile.read(0x8000);
        for (int i = 0; i < musicContent.size(); i++) {
            int j = (i + 1) & 0xff;
            musicContent[i] ^= mCoreKeyBox[(mCoreKeyBox[j] + mCoreKeyBox[(mCoreKeyBox[j] + j) & 0xff]) & 0xff];
        }
        outMusic.write(musicContent);
    }
    outMusic.close();
    ncmFile.close();
    return outPath;
}

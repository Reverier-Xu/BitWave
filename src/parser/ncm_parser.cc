/**
 * @file ncm_parser.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#include "ncm_parser.h"

#include <QImage>

#include "utils/ncm_helper.h"

Media NcmParser::parse(const QString& path) {
    QFile ncmFile(QUrl(path).path());
    Media media;
    media.setUrl(path);
    media.setType(MUSIC);
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    if (!validateNcmFile(ncmFile)) {
        ncmFile.close();
        throw std::runtime_error("Not a valid ncm file.");
    }
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
    ncmFile.seek(ncmFile.pos() + key_len);

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char*>(&metadata_len), sizeof(metadata_len));
    QByteArray modifyData;

    if (metadata_len > 0) {
        modifyData = ncmFile.read(metadata_len);

        for (int i = 0; i < metadata_len; i++) {
            modifyData[i] ^= 0x63;
        }
        media.setComment(modifyData);
        auto metadata = getDecryptedMetadata(modifyData);
        media.setTitle(metadata["musicName"].toString());
        auto artists = metadata["artist"].toArray();
        //        qDebug() << artists;
        QStringList artistList;
        for (auto artist : artists) {
            //            qDebug() << artist;
            artistList.append(artist.toArray()[0].toString());
        }
        media.setArtists(artistList);
        media.setTime(metadata["duration"].toDouble() / 1000);
        media.setAlbum(metadata["album"].toString());
        ncmFile.close();
        return media;
    } else {
        ncmFile.close();
        throw std::runtime_error("No metadata in ncm file.");
    }
}

QImage NcmParser::extractCover(const Media& src) {
    QFile ncmFile(QUrl(src.url()).path());
    ncmFile.open(QFile::ReadOnly);
    // evaluate is ncm file or not.
    validateNcmFile(ncmFile);
    ncmFile.seek(10);
    quint32 key_len;
    ncmFile.read(reinterpret_cast<char*>(&key_len), sizeof(key_len));
    ncmFile.seek(ncmFile.pos() + key_len);

    quint32 metadata_len;
    ncmFile.read(reinterpret_cast<char*>(&metadata_len), sizeof(metadata_len));
    ncmFile.seek(ncmFile.pos() + metadata_len);

    ncmFile.seek(ncmFile.pos() + 9);
    quint32 imgSize;
    ncmFile.read(reinterpret_cast<char*>(&imgSize), sizeof(imgSize));

    if (imgSize > 0) {
        QByteArray img = ncmFile.read(imgSize);
        QImage avatar;
        avatar.loadFromData(img);
        return avatar;
    } else {
        ncmFile.close();
        throw std::runtime_error("No cover in ncm file.");
    }
}

bool NcmParser::accepted(const QString& path) {
    auto fileInfo = QFileInfo(path);
    return m_supportedFormats.contains(fileInfo.suffix());
}

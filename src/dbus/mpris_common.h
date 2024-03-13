/**
 * @file mpris_common.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QDateTime>
#include <QObject>
#include <QStringList>
#include <QVariantMap>

namespace mpris {

inline void AddMetadata(const QString &key, const QString &metadata,
                        QVariantMap *map) {
    if (!metadata.isEmpty()) (*map)[key] = metadata;
}

inline void AddMetadataAsList(const QString &key, const QString &metadata,
                              QVariantMap *map) {
    if (!metadata.isEmpty()) (*map)[key] = QStringList() << metadata;
}

inline void AddMetadata(const QString &key, int metadata, QVariantMap *map) {
    if (metadata > 0) (*map)[key] = metadata;
}

inline void AddMetadata(const QString &key, qint64 metadata, QVariantMap *map) {
    if (metadata > 0) (*map)[key] = metadata;
}

inline void AddMetadata(const QString &key, const QStringList &metadata,
                        QVariantMap *map) {
    if (!metadata.isEmpty()) (*map)[key] = metadata;
}

inline void AddMetadata(const QString &key, double metadata, QVariantMap *map) {
    if (metadata != 0.0) (*map)[key] = metadata;
}

inline void AddMetadata(const QString &key, const QDateTime &metadata,
                        QVariantMap *map) {
    if (metadata.isValid()) (*map)[key] = metadata;
}

inline void AddMetadata(const QString &key, const QDBusObjectPath &metadata, QVariantMap *map) {
    (*map)[key] = metadata;
}

inline QString AsMPRISDateTimeType(uint time) {
    return time != -1 ? QDateTime::fromSecsSinceEpoch(time).toString(Qt::ISODate) : "";
}

}  // namespace mpris

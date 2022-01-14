/**
 * @file string_helper.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-14
 * 
 * @copyright Copyright (c) 2022 Wootec
 * 
 */

#include "string_helper.h"

#include <QCryptographicHash>

namespace StringHelper {
    QString getHashedIdFromPath(const QString &path) {
        auto hash = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Sha256);
        QString str = hash.toHex();
        return QString("PL%1").arg(str.left(8));
    }
    
    QString getNameFromPath(const QString &path) {
        auto pos = path.lastIndexOf('/');
        if (pos == -1) {
            return path;
        }
        return path.mid(pos + 1);
    }
}

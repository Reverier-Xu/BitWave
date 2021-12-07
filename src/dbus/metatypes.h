/**
 * @file metatypes.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QDBusObjectPath>
#include <QMetaType>

Q_DECLARE_METATYPE(QList<QByteArray>)

typedef QMap<QString, QVariantMap> InterfacesAndProperties;
typedef QMap<QDBusObjectPath, InterfacesAndProperties> ManagedObjectList;

Q_DECLARE_METATYPE(InterfacesAndProperties)

Q_DECLARE_METATYPE(ManagedObjectList)

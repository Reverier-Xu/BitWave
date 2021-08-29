#pragma once

#include <QDBusObjectPath>
#include <QMetaType>

Q_DECLARE_METATYPE(QList<QByteArray>)

typedef QMap<QString, QVariantMap> InterfacesAndProperties;
typedef QMap<QDBusObjectPath, InterfacesAndProperties> ManagedObjectList;

Q_DECLARE_METATYPE(InterfacesAndProperties)

Q_DECLARE_METATYPE(ManagedObjectList)

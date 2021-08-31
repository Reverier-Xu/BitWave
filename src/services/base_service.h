//
// Created by reverier on 2021/8/31.
//

#pragma once

#include <QObject>

class BaseService : public QObject {
Q_OBJECT
    Q_PROPERTY(QString name MEMBER mName READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString router READ router WRITE setRouter NOTIFY routerChanged)
    Q_PROPERTY(bool isEndpoint MEMBER mIsEndpoint WRITE setIsEndpoint NOTIFY isEndpointChanged)
public:

};


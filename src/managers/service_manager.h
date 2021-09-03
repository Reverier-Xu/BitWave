//
// Created by reverier on 2021/8/29.
//

#pragma once

#include <QObject>
#include <QMap>

#include "services/base_service.h"

class ServiceManager : public QObject {
Q_OBJECT
private:
    BaseService* mRootService{};
protected:
    explicit ServiceManager(QObject *parent);

    static ServiceManager *mInstance;

public:
    [[nodiscard]] static ServiceManager *instance(QObject *parent);

    ~ServiceManager() override;

    void loadSettings();

    void saveSettings() const;
};

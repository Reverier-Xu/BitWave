//
// Created by reverier on 2021/8/29.
//

#pragma once

#include <QObject>

#include "base_manager.h"

class ServiceManager : public BaseManager {
Q_OBJECT
protected:
    explicit ServiceManager(QObject *parent);

    static ServiceManager *mInstance;

public:
    [[nodiscard]] static ServiceManager *instance(QObject *parent);

    void loadSettings() override;

    void saveSettings() override;
};

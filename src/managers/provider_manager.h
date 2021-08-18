//
// Created by Reverier-Xu on 2021/6/25.
//

#pragma once

#include <QObject>

#include "base_manager.h"

class ProviderManager : public BaseManager {
    Q_OBJECT
protected:
    explicit ProviderManager(QObject *parent);

    ~ProviderManager() override;

    static ProviderManager *mInstance;

public:

    static ProviderManager *instance(QObject *parent = nullptr);

    void loadSettings() override;

    void saveSettings() override;
};

//
// Created by reverier on 2021/8/29.
//

#pragma once

#include <QObject>
#include "base_manager.h"

class RouterManager : public BaseManager {
    Q_OBJECT
protected:
    explicit RouterManager(QObject* parent);

    static RouterManager* mInstance;

public:
    [[nodiscard]] static RouterManager *instance(QObject *parent);

    void loadSettings() override;

    void saveSettings() override;
};

/**
 * @file service_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QMap>
#include <QObject>

#include "services/base_service.h"

class ServiceManager : public QObject {
    Q_OBJECT
   private:
    BaseService *mRootService{};

   protected:
    explicit ServiceManager(QObject *parent);

    static ServiceManager *mInstance;

   public:
    [[nodiscard]] static ServiceManager *instance(QObject *parent);

    ~ServiceManager() override;

    void loadSettings();

    void saveSettings() const;
};

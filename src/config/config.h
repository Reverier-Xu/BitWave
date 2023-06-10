/**
 * @file config.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include "display_config.h"


class Config : public QObject {
   Q_OBJECT

   private:
    DisplayConfig* m_displayConfig;

   protected:
    static Config* m_instance;

    explicit Config(QObject* parent = nullptr);

    ~Config() override;

   public:
    [[nodiscard]] static Config* instance(QObject* parent = nullptr);

   public slots:
    Q_INVOKABLE void saveConfig();

    Q_INVOKABLE void loadConfig();

    [[nodiscard]] Q_INVOKABLE DisplayConfig* displayConfig();
};

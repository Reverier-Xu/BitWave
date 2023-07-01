/**
 * @file ui.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "colorize.h"
#include "config.h"
#include "router.h"

class Ui : public QObject {
    Q_OBJECT

   private:
    QQmlApplicationEngine* m_engine;
    Colorize* m_colorize;
    Router* m_router;
    UiConfig* m_uiConfig;

   public:
    explicit Ui(QObject* parent = nullptr);

    ~Ui() override;

    void initialize();

    void exportProperties();

    void connectSignals();

    void createUi();

   public slots:
    Q_INVOKABLE void onSecondaryInstanceStarted();
};

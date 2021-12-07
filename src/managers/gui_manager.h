/**
 * @file gui_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

class GuiManager : public QObject {
    Q_OBJECT
   private:
    QQmlApplicationEngine *mUiEngine;

   protected:
    explicit GuiManager(QObject *parent);

    static GuiManager *mInstance;

   public:
    static GuiManager *instance(QObject *parent = nullptr);

    void createUI();

    void exportManagers();

    static void exportComponents();
};

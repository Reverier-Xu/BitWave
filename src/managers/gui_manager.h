//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_GUI_MANAGER_H
#define BITWAVE_GUI_MANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>

class GuiManager : public QObject {
    Q_OBJECT
   private:
    QQmlApplicationEngine* mUiEngine;

   protected:
    explicit GuiManager(QObject* parent);

    static GuiManager* mInstance;

   public:
    static GuiManager* getInstance(QObject* parent = nullptr);

    void createUI();

    void exportManagers();

    static void exportComponents();
};

#endif  // BITWAVE_GUI_MANAGER_H

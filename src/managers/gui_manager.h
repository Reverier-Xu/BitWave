/*
 * gui_manager.h
 *
 * Summary: manager of QML Frontend and engines.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
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

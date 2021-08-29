/*
 * base_manager.h
 *
 * Summary: the base class of managers.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 *
 * Created: 2021-08-18
 * Last Modified: 2021-08-18
 *
 */

#pragma once

#include <QObject>

class BaseManager : public QObject {
Q_OBJECT
public:
    explicit BaseManager(QObject *parent = nullptr)
            : QObject(parent) {
    }

    ~BaseManager() override = default;

    virtual void loadSettings() = 0;

    virtual void saveSettings() = 0;
};

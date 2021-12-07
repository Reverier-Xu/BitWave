/**
 * @file dbusscreensaver.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <QString>

#include "screensaver.h"

class DBusScreensaver : public Screensaver {
   public:
    DBusScreensaver(QString service, QString path, QString interface);

    void Inhibit() override;

    void UnInhibit() override;

   private:
    QString service_;
    QString path_;
    QString interface_;

    quint32 cookie_{};
};

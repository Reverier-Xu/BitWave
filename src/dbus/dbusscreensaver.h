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

    void inhibit() override;

    void unInhibit() override;

  private:
    QString m_service;
    QString m_path;
    QString m_interface;

    quint32 m_cookie{};
};

#pragma once

#include <QString>

#include "screensaver.h"

class DBusScreensaver : public Screensaver {
 public:
  DBusScreensaver(QString  service, QString  path,
                  QString  interface);

  void Inhibit() override;
  void UnInhibit() override;

 private:
  QString service_;
  QString path_;
  QString interface_;

  quint32 cookie_{};
};

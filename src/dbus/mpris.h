/**
 * @file mpris.h
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

namespace mpris {

class Mpris1;

class Mpris2;

class Mpris : public QObject {
    Q_OBJECT

   public:
    explicit Mpris(QObject *parent = nullptr);

   signals:

    void RaiseMainWindow();

   private:
    Mpris1 *mpris1_;
    Mpris2 *mpris2_;
};

}  // namespace mpris

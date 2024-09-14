/**
 * @file mpris.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "mpris.h"

#include "mpris2.h"

namespace mpris {

Mpris::Mpris(QObject* parent) : QObject(parent), mpris2_(new mpris::Mpris2(this)) {
    connect(mpris2_, SIGNAL(RaiseMainWindow()), SIGNAL(RaiseMainWindow()));
}

}    // namespace mpris

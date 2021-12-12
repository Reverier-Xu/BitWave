/**
 * @file time_helper.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "time_helper.h"

#include <QtMath>

QString TimeHelper::getTimeString(double n) {
    int secs = floor(n);
    int mins = secs / 60;
    secs = secs % 60;
    return QString::asprintf("%02d:%02d", mins, secs);
}

//
// Created by reverier on 2021/8/25.
//

#include "time_helper.h"
#include <QtMath>

QString TimeHelper::getTimeString(double n) {
    int secs = floor(n);
    int mins = secs / 60;
    secs = secs % 60;
    return QString::asprintf("%02d:%02d", mins, secs);
}

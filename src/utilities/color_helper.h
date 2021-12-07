/**
 * @file color_helper.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */


#pragma once

#include <QColor>
#include <QImage>

namespace ColorHelper {
QColor getImageThemeColor(const QImage &image);
}

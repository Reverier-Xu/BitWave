/**
 * @file string_helper.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-01-14
 *
 * @copyright Copyright (c) 2022 Wootec
 *
 */

#pragma once

#include <QString>

namespace StringHelper {
[[nodiscard]] QString getHashedIdFromPath(const QString &path);
[[nodiscard]] QString getNameFromPath(const QString &path);
};  // namespace StringHelper

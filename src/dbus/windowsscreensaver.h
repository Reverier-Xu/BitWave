/**
 * @file windowsscreensaver.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#pragma once

#include <windows.h>

#include "screensaver.h"

class WindowsScreensaver : public Screensaver {
   public:
    WindowsScreensaver();

    void Inhibit() override;

    void UnInhibit() override;

   private:
    EXECUTION_STATE previous_state_;
};

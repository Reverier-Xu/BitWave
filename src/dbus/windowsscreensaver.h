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

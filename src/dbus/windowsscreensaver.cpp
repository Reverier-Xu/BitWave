#include "windowsscreensaver.h"

WindowsScreensaver::WindowsScreensaver()
        : previous_state_(0) {
}

void WindowsScreensaver::Inhibit() {
    // TODO: use PowerCreateRequest on Win7+
    previous_state_ = SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
}

void WindowsScreensaver::UnInhibit() {
    SetThreadExecutionState(ES_CONTINUOUS | previous_state_);
}

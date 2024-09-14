#pragma once

class Screensaver {
  public:
    virtual ~Screensaver() = default;

    static const char* kGnomeService;
    static const char* kGnomePath;
    static const char* kGnomeInterface;

    static const char* kKdeService;
    static const char* kKdePath;
    static const char* kKdeInterface;

    virtual void inhibit() = 0;

    virtual void unInhibit() = 0;

    static Screensaver* getScreensaver();

  private:
    static Screensaver* m_screensaver;
};

#ifndef SCREENSAVER_H
#define SCREENSAVER_H

class Screensaver {
public:
    virtual ~Screensaver() = default;

    static const char *kGnomeService;
    static const char *kGnomePath;
    static const char *kGnomeInterface;

    static const char *kKdeService;
    static const char *kKdePath;
    static const char *kKdeInterface;

    virtual void Inhibit() = 0;

    virtual void UnInhibit() = 0;

    static Screensaver *GetScreensaver();

private:
    static Screensaver *screensaver_;
};

#endif

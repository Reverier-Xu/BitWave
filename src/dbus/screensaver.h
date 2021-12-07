/**
 * @file screensaver.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

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

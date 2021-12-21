/**
 * @file display_manager.h
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
#include <QObject>
#include <QTimer>
#include <QTranslator>

#include "queue_manager.h"

class DisplayManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int activeTabIndex MEMBER mActiveTabIndex READ activeTabIndex
                   WRITE setActiveTabIndex NOTIFY activeTabIndexChanged)
    Q_PROPERTY(
        int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(int queueBarIndex MEMBER mQueueBarIndex READ queueBarIndex WRITE
                   setQueueBarIndex NOTIFY queueBarIndexChanged)
    Q_PROPERTY(bool colorStyle MEMBER mColorStyle READ colorStyle WRITE
                   setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(QColor themeColor MEMBER mThemeColor READ themeColor WRITE
                   setThemeColor NOTIFY themeColorChanged)
    Q_PROPERTY(QColor contentColor READ contentColor WRITE setContentColor
                   NOTIFY contentColorChanged)
    Q_PROPERTY(QColor alertColor MEMBER mAlertColor READ alertColor WRITE
                   setAlertColor NOTIFY alertColorChanged)
    Q_PROPERTY(bool sideBarExpanded MEMBER mSideBarExpanded READ sideBarExpanded
                   WRITE setSideBarExpanded NOTIFY sideBarExpandedChanged)
    Q_PROPERTY(
        bool queueBarExpanded MEMBER mQueueBarExpanded READ queueBarExpanded
            WRITE setQueueBarExpanded NOTIFY queueBarExpandedChanged)
    Q_PROPERTY(bool mouseIsActive MEMBER mMouseIsActive READ mouseIsActive WRITE
                   setMouseIsActive NOTIFY mouseIsActiveChanged)
    Q_PROPERTY(bool isFullScreen MEMBER mIsFullScreen READ isFullScreen WRITE
                   setFullScreen NOTIFY isFullScreenChanged)
    Q_PROPERTY(bool showVideoTime MEMBER mShowVideoTime READ showVideoTime WRITE
                   setShowVideoTime NOTIFY showVideoTimeChanged)
    Q_PROPERTY(QString language MEMBER mLanguage READ language WRITE setLanguage
                   NOTIFY languageChanged)
    Q_PROPERTY(bool flatSystemTray MEMBER mFlatSystemTray READ flatSystemTray
                   WRITE setFlatSystemTray NOTIFY flatSystemTrayChanged)

   private:
    int mActiveTabIndex = -1;
    int mQueueBarIndex = -1;
    bool mColorStyle = false;
    bool mSideBarExpanded = true;
    bool mQueueBarExpanded = false;
    bool mMouseIsActive = true;
    bool mIsFullScreen = false;
    bool mShowVideoTime = true;
    QColor mThemeColor = QColor(0x00, 0x78, 0xd6);
    QColor mAlertColor = QColor(0xff, 0x60, 0x33);
    QTimer *mHideTimer;
    QString mLanguage = "en_US";
    QTranslator mTranslator;
    bool mFlatSystemTray = false;

   protected:
    explicit DisplayManager(QObject *parent);

    ~DisplayManager() override;

    void loadSettings();

    void saveSettings() const;

    static DisplayManager *mInstance;

   public:
    static DisplayManager *instance(QObject *parent = nullptr);

    [[nodiscard]] int activeTabIndex() const;

    void setActiveTabIndex(int n);

    [[nodiscard]] int pageIndex() const;

    void setPageIndex(int n);

    [[nodiscard]] int queueBarIndex() const;

    void setQueueBarIndex(int n);

    [[nodiscard]] bool colorStyle() const;

    void setColorStyle(bool value);

    [[nodiscard]] bool sideBarExpanded() const;

    void setSideBarExpanded(bool value);

    [[nodiscard]] bool queueBarExpanded() const;

    void setQueueBarExpanded(bool value);

    [[nodiscard]] bool mouseIsActive() const;

    void setMouseIsActive(bool value);

    [[nodiscard]] QColor themeColor() const;

    void setThemeColor(const QColor &value);

    void setThemeColor(const QString &value);

    [[nodiscard]] QColor alertColor() const;

    void setAlertColor(const QColor &value);

    void setAlertColor(const QString &value);

    [[nodiscard]] QColor contentColor() const;

    void setContentColor(const QColor &value);

    [[nodiscard]] bool isFullScreen() const;

    void setFullScreen(bool n);

    [[nodiscard]] bool showVideoTime() const;

    void setShowVideoTime(bool n);

    [[nodiscard]] QString language() const;

    void setLanguage(const QString &n);

    [[nodiscard]] bool flatSystemTray() const;

    void setFlatSystemTray(bool n);

   public slots:

    Q_INVOKABLE void delayedHide();

    Q_INVOKABLE void blockDelayedHide();

    Q_INVOKABLE void raiseWindow();

   signals:

    void activeTabIndexChanged(int n);

    void pageIndexChanged(int n);

    void queueBarIndexChanged(int n);

    void colorStyleChanged(bool n);

    void sideBarExpandedChanged(bool n);

    void queueBarExpandedChanged(bool n);

    void mouseIsActiveChanged(bool n);

    void themeColorChanged(QColor n);

    void alertColorChanged(QColor n);

    void isFullScreenChanged(bool n);

    void showVideoTimeChanged(bool n);

    void contentColorChanged(QColor n);

    void languageChanged(const QString &n);

    void flatSystemTrayChanged(bool n);

    void raiseWindowRequested();

    void showTips(const QString &icon, const QString &info);
};

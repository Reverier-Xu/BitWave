/* 
 * display_manager.h
 *
 * Summary: misc variables for QML frontend.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
 * 
 */

#pragma once

#include <QObject>
#include <QColor>
#include "base_manager.h"

class DisplayManager : public BaseManager {
Q_OBJECT
    Q_PROPERTY(int activeTabIndex MEMBER mActiveTabIndex READ activeTabIndex WRITE setActiveTabIndex
                       NOTIFY activeTabIndexChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
    Q_PROPERTY(bool colorStyle MEMBER mColorStyle READ colorStyle WRITE
                       setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(QColor themeColor MEMBER mThemeColor READ themeColor WRITE
                       setThemeColor NOTIFY themeColorChanged)
    Q_PROPERTY(QColor alertColor MEMBER mAlertColor READ alertColor WRITE
                       setAlertColor NOTIFY alertColorChanged)
private:
    int mActiveTabIndex = -1;
    bool mColorStyle = false;
    QColor mThemeColor = QColor(0x00, 0x78, 0xd6);
    QColor mAlertColor = QColor(0xff, 0x60, 0x33);
protected:
    explicit DisplayManager(QObject *parent);

    ~DisplayManager() override;

    void loadSettings() override;
    void saveSettings() override;

    static DisplayManager *mInstance;
public:
    static DisplayManager *instance(QObject *parent = nullptr);

    [[nodiscard]] int activeTabIndex() const { return this->mActiveTabIndex; }

    void setActiveTabIndex(int n) {
        this->mActiveTabIndex = n;
        emit this->activeTabIndexChanged(n);
        emit this->pageIndexChanged(this->pageIndex());
    }

    [[nodiscard]] int pageIndex() const {
        if (this->activeTabIndex() == -1) return 0;
        else if (this->activeTabIndex() >= 0) return 1;
        else if (this->activeTabIndex() == -2) return 3;
        else if (this->activeTabIndex() == -3) return 2;
        else {
            throw std::exception();
        }
    }

    void setPageIndex(int n) {
        emit this->pageIndexChanged(n);
    }


    [[nodiscard]] bool colorStyle() const { return this->mColorStyle; }

    void setColorStyle(bool value) {
        this->mColorStyle = value;
        emit this->colorStyleChanged(value);
    }

    [[nodiscard]] QColor themeColor() const { return this->mThemeColor; }

    void setThemeColor(const QColor &value) {
        this->mThemeColor = value;
        emit this->themeColorChanged(value);
    }

    void setThemeColor(const QString &value) {
        this->mThemeColor.setNamedColor(value);
        emit this->themeColorChanged(value);
    }

    [[nodiscard]] QColor alertColor() const { return this->mAlertColor; }

    void setAlertColor(const QColor &value) {
        this->mAlertColor = value;
        emit this->alertColorChanged(value);
    }

    void setAlertColor(const QString &value) {
        this->mAlertColor.setNamedColor(value);
        emit this->alertColorChanged(value);
    }

signals:

    void activeTabIndexChanged(int n);

    void pageIndexChanged(int n);

    void colorStyleChanged(bool n);

    void themeColorChanged(QColor n);

    void alertColorChanged(QColor n);
};

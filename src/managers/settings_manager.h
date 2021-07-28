//
// Created by Reverier-Xu on 2021/6/11.
//

#ifndef BITWAVE_SETTINGS_MANAGER_H
#define BITWAVE_SETTINGS_MANAGER_H

#include <QColor>
#include <QObject>

class SettingsManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool colorStyle MEMBER mColorStyle READ colorStyle WRITE
                   setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(QColor themeColor MEMBER mThemeColor READ themeColor WRITE
                   setThemeColor NOTIFY themeColorChanged)
    Q_PROPERTY(QColor alertColor MEMBER mAlertColor READ alertColor WRITE
                   setAlertColor NOTIFY alertColorChanged)
   private:
    bool mColorStyle = false;
    QColor mThemeColor = QColor(0x00, 0x78, 0xd6);
    QColor mAlertColor = QColor(0xff, 0x60, 0x33);

   protected:
    explicit SettingsManager(QObject *parent);

    static SettingsManager *mInstance;

   public:
    static SettingsManager *getInstance(QObject *parent = nullptr);

    [[nodiscard]] bool colorStyle() const { return this->mColorStyle; }
    void setColorStyle(bool value) {
        this->mColorStyle = value;
        emit this->colorStyleChanged(value);
    }

    [[nodiscard]] QColor themeColor() const { return this->mThemeColor; }
    void setThemeColor(QColor value) {
        this->mThemeColor = value;
        emit this->themeColorChanged(value);
    }
    void setThemeColor(const QString &value) {
        this->mThemeColor.setNamedColor(value);
        emit this->themeColorChanged(value);
    }

    [[nodiscard]] QColor alertColor() const { return this->mAlertColor; }
    void setAlertColor(QColor value) {
        this->mAlertColor = value;
        emit this->alertColorChanged(value);
    }
    void setAlertColor(const QString &value) {
        this->mAlertColor.setNamedColor(value);
        emit this->alertColorChanged(value);
    }

   signals:
    void colorStyleChanged(bool n);
    void themeColorChanged(QColor n);
    void alertColorChanged(QColor n);
};

#endif  // BITWAVE_SETTINGS_MANAGER_H

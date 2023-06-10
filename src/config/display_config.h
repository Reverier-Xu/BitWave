/**
 * @file display_config.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>


class DisplayConfig : public QObject {
   Q_OBJECT

    Q_PROPERTY(bool colorStyle READ colorStyle WRITE setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(int tabIndex READ tabIndex WRITE setTabIndex NOTIFY tabIndexChanged)
   private:
    bool m_colorStyle = false;

    int m_tabIndex = 0;

   public:
    explicit DisplayConfig(QObject* parent = nullptr);

    ~DisplayConfig() override;

    [[nodiscard]] bool colorStyle() const;

    void setColorStyle(bool colorStyle);

    [[nodiscard]] int tabIndex() const;

    void setTabIndex(int tabIndex);

   public slots:
    Q_INVOKABLE void saveConfig() const;

    Q_INVOKABLE void loadConfig();

   signals:

    void colorStyleChanged(bool n);

    void tabIndexChanged(int n);
};

/**
 * @file ui_config.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-06-25
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QTranslator>

class UiConfig : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool colorStyle READ colorStyle WRITE setColorStyle NOTIFY
                   colorStyleChanged)
    Q_PROPERTY(
        QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY
                   fullscreenChanged)
   private:
    QString m_language;
    QTranslator m_translator;
    bool m_colorStyle = false;
    bool m_fullscreen = false;

   public:
    explicit UiConfig(QObject* parent = nullptr);

    ~UiConfig() override;

    void setColorStyle(bool n);

    [[nodiscard]] bool colorStyle() const;

    void setLanguage(const QString& n);

    [[nodiscard]] QString language() const;

    void setFullscreen(bool n);

    [[nodiscard]] bool fullscreen() const;

    void loadSettings();

    void saveSettings() const;

   signals:

    void colorStyleChanged(bool n);

    void languageChanged(const QString& n);

    void raiseWindowRequested();

    void fullscreenChanged(bool n);
};

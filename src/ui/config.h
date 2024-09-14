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

#include "router.h"
#include <QObject>
#include <QTimer>
#include <QTranslator>

class UiConfig : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool colorStyle READ colorStyle WRITE setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(bool flatSystemTray READ flatSystemTray WRITE setFlatSystemTray NOTIFY flatSystemTrayChanged)
    Q_PROPERTY(bool hideControls READ hideControls WRITE setHideControls NOTIFY hideControlsChanged)
    Q_PROPERTY(bool sideBarExpanded READ sideBarExpanded WRITE setSideBarExpanded NOTIFY sideBarExpandedChanged)
    Q_PROPERTY(bool controlWidgetExpanded READ controlWidgetExpanded WRITE setControlWidgetExpanded NOTIFY
                   controlWidgetExpandedChanged)
  private:
    QString m_language;

    QTranslator m_translator;

    bool m_colorStyle = false;

    bool m_fullscreen = false;

    bool m_flatSystemTray = false;

    QTimer* m_hideTimer;

    bool m_hideControls = false;

    bool m_sideBarExpanded = true;

    bool m_controlWidgetExpanded = false;

    Router* m_router;

  public:
    explicit UiConfig(QObject* parent = nullptr, Router* router = nullptr);

    ~UiConfig() override;

    void setColorStyle(bool n);

    [[nodiscard]] bool colorStyle() const;

    void setLanguage(const QString& n);

    [[nodiscard]] QString language() const;

    void setFullscreen(bool n);

    [[nodiscard]] bool fullscreen() const;

    void setFlatSystemTray(bool n);

    [[nodiscard]] bool flatSystemTray() const;

    void setHideControls(bool n);

    [[nodiscard]] bool hideControls() const;

    void setSideBarExpanded(bool n);

    [[nodiscard]] bool sideBarExpanded() const;

    void setControlWidgetExpanded(bool n);

    [[nodiscard]] bool controlWidgetExpanded() const;

    void loadSettings();

    void saveSettings() const;

  public slots:
    Q_INVOKABLE void autoHideControls();

    Q_INVOKABLE void blockHideControls();

  signals:

    void colorStyleChanged(bool n);

    void languageChanged(const QString& n);

    void raiseWindowRequested();

    void fullscreenChanged(bool n);

    void flatSystemTrayChanged(bool n);

    void hideControlsChanged(bool n);

    void sideBarExpandedChanged(bool n);

    void controlWidgetExpandedChanged(bool n);
};

/**
 * @file ui.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include "colorize.h"
#include "router.h"


class Ui : public QObject {
   Q_OBJECT
    Q_PROPERTY(bool colorStyle READ colorStyle WRITE setColorStyle NOTIFY colorStyleChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

   private:
    QQmlApplicationEngine* m_engine;
    Colorize* m_colorize;
    Router* m_router;
    QString m_language;
    QTranslator m_translator;
    bool m_colorStyle = false;

   public:
    explicit Ui(QObject* parent = nullptr);

    ~Ui() override;

    void initialize();

    void loadSettings();

    void saveSettings() const;

    void exportProperties();

    void connectSignals();

    void createUi();

    void setColorStyle(bool n);

    [[nodiscard]] bool colorStyle() const;

    void setLanguage(const QString& n);

    [[nodiscard]] QString language() const;

   public slots:
    Q_INVOKABLE void onSecondaryInstanceStarted();

   signals:
    void raiseWindowRequested();

    void colorStyleChanged(bool n);

    void languageChanged(const QString& n);
};

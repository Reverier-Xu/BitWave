/**
 * @file app.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include "player/player.h"


class Ui;

class App : public QObject {
   Q_OBJECT

   private:
    Ui* m_ui;

   protected:
    void registerTypes();

   public:
    explicit App(QObject* parent = nullptr);

    ~App() override;

    void initialize(const QString& file);

   public slots:
    Q_INVOKABLE void onSecondaryInstanceMessageReceived(quint32 instanceId, const QByteArray& message);

    Q_INVOKABLE void onSecondaryInstanceStarted();
};

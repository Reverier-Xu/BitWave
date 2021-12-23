/**
 * @file app_manager.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QObject>

class AppManager : public QObject {
    Q_OBJECT
   private:
    QThread *mParserThread{};
    QThread *mLyricProviderThread{};

   public:
    explicit AppManager(QObject *parent = nullptr);

    ~AppManager() override;

    void initialize(const QString &file);

    static void registerTypes();

   public slots:
    Q_INVOKABLE void onSecondaryInstanceMessageReceived(quint32 instanceId, QByteArray message);
    Q_INVOKABLE void onSecondaryInstanceStarted();
};

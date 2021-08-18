/* 
 * app_manager.h
 *
 * Summary: the whole app manager.
 * Author: Reverier-Xu <reverier.xu@outlook.com>
 * 
 * Created: 2021-06-25
 * Last Modified: 2021-08-11
 * 
 */

#pragma once

#include <QObject>

class AppManager : public QObject {
    Q_OBJECT
private:
    QThread *parser_thread{};
   public:
    explicit AppManager(QObject *parent = nullptr);

    ~AppManager() override;

    void initialize();

    static void registerTypes();
};

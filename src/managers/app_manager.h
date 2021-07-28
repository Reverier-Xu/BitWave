//
// Created by Reverier-Xu on 2021/6/25.
//

#ifndef BITWAVE_APP_MANAGER_H
#define BITWAVE_APP_MANAGER_H

#include <QObject>

class AppManager : public QObject {
    Q_OBJECT
   public:
    explicit AppManager(QObject *parent = nullptr);

    void initialize();
};

#endif  // BITWAVE_APP_MANAGER_H

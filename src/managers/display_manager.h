//
// Created by Reverier-Xu on 2021/7/29.
//

#ifndef BITWAVE_DISPLAY_MANAGER_H
#define BITWAVE_DISPLAY_MANAGER_H

#include <QObject>

class DisplayManager : public QObject {
Q_OBJECT
    Q_PROPERTY(
            int activeTabIndex MEMBER mActiveTabIndex READ activeTabIndex WRITE setActiveTabIndex NOTIFY activeTabIndexChanged)
    Q_PROPERTY(int pageIndex READ pageIndex WRITE setPageIndex NOTIFY pageIndexChanged)
private:
    int mActiveTabIndex = -1;
protected:
    explicit DisplayManager(QObject *parent);

    static DisplayManager *mInstance;
public:
    static DisplayManager *getInstance(QObject *parent = nullptr);

    [[nodiscard]] int activeTabIndex() const { return this->mActiveTabIndex; }

    void setActiveTabIndex(int n) {
        this->mActiveTabIndex = n;
        emit this->activeTabIndexChanged(n);
        emit this->pageIndexChanged(this->pageIndex());
    }

    [[nodiscard]] int pageIndex() const {
        if (this->activeTabIndex() == -1) return 0;
        else if(this->activeTabIndex() >= 0) return 1;
        else if(this->activeTabIndex() == -2) return 3;
        else if(this->activeTabIndex() == -3) return 2;
        else {
            throw std::exception();
        }
    }

    void setPageIndex(int n) {
        emit this->pageIndexChanged(n);
    }

signals:

    void activeTabIndexChanged(int n);
    void pageIndexChanged(int n);
};


#endif //BITWAVE_DISPLAY_MANAGER_H

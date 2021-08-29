#pragma once

#include <QObject>

namespace mpris {

    class Mpris1;

    class Mpris2;

    class Mpris : public QObject {
    Q_OBJECT

    public:
        explicit Mpris(QObject *parent = nullptr);

    signals:

        void RaiseMainWindow();

    private:
        Mpris1 *mpris1_;
        Mpris2 *mpris2_;
    };

} // namespace mpris

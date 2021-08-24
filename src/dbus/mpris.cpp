#include "mpris.h"

#include "mpris2.h"

namespace mpris {

Mpris::Mpris(QObject* parent)
    : QObject(parent), mpris2_(new mpris::Mpris2(this)) {
  connect(mpris2_, SIGNAL(RaiseMainWindow()), SIGNAL(RaiseMainWindow()));
}

}  // namespace mpris

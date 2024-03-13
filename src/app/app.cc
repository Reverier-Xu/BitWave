/**
 * @file app.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-11
 *
 * @copyright 2023 Woo Tech
 */

#include "app.h"

#include <ui/ui.h>

#ifdef QT_DBUS_LIB
#include "dbus/mpris2.h"
#endif
#include "models/media.h"
#include "storage/storage.h"

App::App(QObject* parent) : QObject(parent) { m_ui = Ui::instance(this); }

App::~App() = default;

void App::initialize(const QString& file) {
    registerTypes();
    Storage::instance(this);
    m_ui->initialize();
    if (!file.isEmpty()) {
        Player::instance(this->parent())->queue()->addMediaByUrl(file);
        Player::instance()->queue()->play(
            Player::instance()->queue()->cursor() + 1);
    }

#ifdef QT_DBUS_LIB
    new mpris::Mpris2(this);
#endif
}

void App::onSecondaryInstanceMessageReceived(quint32 instanceId,
                                             const QByteArray& message) {
    const char flag = message[0];
    const QString file = message.mid(1);
    // flag is here:
    // char flags = (char) (resume << 0 | pause << 1 | next << 2 | previous << 3 | 1 << 4);
    const bool resume = flag & 0x01;
    const bool pause = flag & 0x02;
    const bool next = flag & 0x04;
    const bool previous = flag & 0x08;
    if (resume) {
        Player::instance()->resume();
    }
    if (pause) {
        Player::instance()->pause();
    }
    if (next) {
        Player::instance()->queue()->next();
    }
    if (previous) {
        Player::instance()->queue()->prev();
    }
    if (!file.isEmpty()) {
        Player::instance()->queue()->addMediaByUrl(file);
        Player::instance()->queue()->play(
            Player::instance()->queue()->cursor() + 1);
    }
}

void App::onSecondaryInstanceStarted() { m_ui->onSecondaryInstanceStarted(); }

void App::registerTypes() { qRegisterMetaType<Media>("Media"); }

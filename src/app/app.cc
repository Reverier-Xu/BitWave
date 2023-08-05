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
    // TODO: resume file
    registerTypes();
    Storage::instance(this);
    m_ui->initialize();

#ifdef QT_DBUS_LIB
    new mpris::Mpris2(this);
#endif
}

void App::onSecondaryInstanceMessageReceived(quint32 instanceId,
                                             const QByteArray& message) {}

void App::onSecondaryInstanceStarted() { m_ui->onSecondaryInstanceStarted(); }

void App::registerTypes() { qRegisterMetaType<Media>("Media"); }

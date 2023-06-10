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
#include "models/media.h"
#include <ui/ui.h>


App::App(QObject* parent) : QObject(parent) {
    m_ui = new Ui(this);
}

App::~App() = default;

void App::initialize(const QString& file) {
    // TODO: resume file
    registerTypes();
    m_ui->initialize();
}

void App::onSecondaryInstanceMessageReceived(quint32 instanceId, const QByteArray& message) {

}

void App::onSecondaryInstanceStarted() {
    m_ui->onSecondaryInstanceStarted();
}

void App::registerTypes() {
    qRegisterMetaType<Media>("Media");
}

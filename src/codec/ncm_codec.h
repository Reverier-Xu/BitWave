/**
 * @file ncm_codec.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once
#include "interface.h"

class NcmCodec : public ICodec {
  private:
    QStringList m_supportedFormats = {"ncm"};

  public:
    NcmCodec() = default;
    ~NcmCodec() override = default;
    const QStringList& supportedFormats() override;
    bool accepted(const Media& src) override;
    QString decode(const Media& src) override;
};

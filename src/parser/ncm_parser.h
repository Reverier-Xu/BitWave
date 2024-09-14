/**
 * @file ncm_parser.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include "interface.h"

class NcmParser : public IParser {
  private:
    QStringList m_supportedFormats = {"ncm"};

  public:
    bool accepted(const QString& path) override;

    Media parse(const QString& path) override;

    QImage extractCover(const Media& src) override;
};

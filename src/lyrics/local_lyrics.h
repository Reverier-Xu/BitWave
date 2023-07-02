#pragma once

#include "interface.h"

class LocalLyrics : public ILyrics {
   public:
    explicit LocalLyrics(QObject* parent = nullptr);

    ILyrics* clone() override;

    bool accepted(const Media& media) override;

    void requestFetch(const Media& media) override;
};

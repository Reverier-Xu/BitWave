#pragma once

#include "interface.h"
class QNetworkAccessManager;
class QNetworkReply;

class NcmLyrics : public ILyrics {
   private:
    QNetworkAccessManager* m_manager;

    void processReply(QNetworkReply* reply);

   public:
    explicit NcmLyrics(QObject* parent = nullptr);

    ~NcmLyrics() override;

    ILyrics* clone() override;

    bool accepted(const Media& media) override;

    void requestFetch(const Media& media) override;
};

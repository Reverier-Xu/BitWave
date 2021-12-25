/**
 * @file local_music_service.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "base_service.h"

class LocalMusicService : public BaseService {
    Q_OBJECT
   public:
    explicit LocalMusicService(BaseService *parent = nullptr);
};

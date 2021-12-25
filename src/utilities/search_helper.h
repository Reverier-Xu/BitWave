/**
 * @file search_helper.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-25
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <QList>
#include <QString>

#include "models/media.h"

class SearchHelper {
    public:
    static QList<Media> search(const QString &keyword,
                                 const QList<Media> &mediaList);
};

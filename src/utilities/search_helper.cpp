/**
 * @file search_helper.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-25
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "search_helper.h"

QList<Media> SearchHelper::search(const QString &keyword,
                                  const QList<Media> &mediaList) {
    QList<Media> result;
    for (const auto &media : mediaList) {
        if (media.title().contains(keyword, Qt::CaseInsensitive) ||
            media.artist().contains(keyword, Qt::CaseInsensitive) ||
            media.collection().contains(keyword, Qt::CaseInsensitive)) {
            result.append(media);
        }
    }
    return result;
}

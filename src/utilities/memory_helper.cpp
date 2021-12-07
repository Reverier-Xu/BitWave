/**
 * @file memory_helper.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */

#include "memory_helper.h"

#include <QFile>

void MemoryHelper::assertMemory(const QString &prev) {
    QFile file("/proc/self/status");
    file.open(QFile::ReadOnly);
    auto res = file.readAll();
    auto str = QString::fromLocal8Bit(res).split("\n");
    for (auto &i : str)
        if (i.contains("RssAnon"))
            qDebug() << QString("[%1]").arg(prev).toStdString().c_str()
                     << i.replace("\t", "")
                            .replace("RssAnon", "Memory Usage")
                            .toStdString()
                            .c_str();
}

/**
 * @file color_helper.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021 Wootec
 * 
 */


#include "color_helper.h"

#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QtMath>

using namespace ColorHelper;

struct Node {
    QPoint point;
    QColor color;
};

struct Clust {
    Node center;
    QVector<Node> buff;
};

int getColorDis(const QColor &a, const QColor &b) {
    return (int)sqrt((a.red() - b.red()) + (a.blue() - b.blue()) +
                     (a.green() - b.green()));
}

QColor ColorHelper::getImageThemeColor(const QImage &image) {
    const int CLUST_NUM = 25;

    int width = image.width(), height = image.height();
    int stepLen = width / 5;

    QPoint points[CLUST_NUM];
    int x = 0;
    for (auto &point : points) {
        point.setX(width / 10 + stepLen * (x++ % 5));
        point.setY(height / 10 + stepLen * ((x / 5) % 5));
    }

    Clust vec[CLUST_NUM];

    for (int i = 0; i < CLUST_NUM; ++i) {
        vec[i].center.point = points[i];
        vec[i].center.color = image.pixelColor(points[i]);
        Node node;
        node.point = vec[i].center.point;
        node.color = vec[i].center.color;
        vec[i].buff.push_back(node);
    }

    for (auto iterTimes = 0; iterTimes < 100; iterTimes++) {
        for (int rows = 0; rows < height; ++rows) {
            for (int cols = 0; cols < width; ++cols) {
                int dis[CLUST_NUM] = {0};
                auto posColor = image.pixelColor(cols, rows);
                for (int i = 0; i < CLUST_NUM; ++i) {
                    dis[i] = getColorDis(vec[i].center.color, posColor);
                }
                int minDisClustId = 0;
                for (int i = 1; i < CLUST_NUM; ++i)
                    if (dis[i] < dis[minDisClustId]) minDisClustId = i;
                Node node;
                node.color = posColor;
                node.point = QPoint(cols, rows);
                vec[minDisClustId].buff.push_back(node);
            }
        }

        for (auto &centerClust : vec) {
            int avgR = 0, avgG = 0, avgB = 0;
            for (auto &item : centerClust.buff) {
                avgR += item.color.red();
                avgG += item.color.green();
                avgB += item.color.blue();
            }
            avgR /= centerClust.buff.size();
            avgG /= centerClust.buff.size();
            avgB /= centerClust.buff.size();
            QColor avgColor(avgR, avgG, avgB);
            int currentId = 0, minId = 0;
            double currentDis = MAXFLOAT, minDis = MAXFLOAT;
            for (auto &item : centerClust.buff) {
                currentDis = getColorDis(item.color, avgColor);
                if (currentDis < minDis) {
                    minId = currentId;
                    minDis = currentDis;
                }
                currentId++;
            }
            centerClust.center.point = centerClust.buff.at(minId).point;
            centerClust.center.color = centerClust.buff.at(minId).color;
        }

        int flag = 0;
        for (auto point : points)
            for (auto &j : vec)
                if (point == j.center.point) {
                    flag++;
                    break;
                }

        if (flag == CLUST_NUM) {
            break;
        }

        for (auto &clust : vec) {
            clust.buff.clear();
            Node node;
            node.color = clust.center.color;
            node.point = clust.center.point;
            clust.buff.push_back(node);
        }
    }

    Clust *dst = vec;
    int max = 0;
    for (auto &i : vec)
        if (i.buff.count() > max) {
            if (i.center.color.lightness() < 80 ||
                i.center.color.lightness() > 180)
                continue;
            max = i.buff.count();
            dst = &i;
        }
    // qDebug() << dst->center.color.lightness();
    auto res = dst->center.color;
    while (res.lightness() < 80 and res.lightness() > 0) res = res.lighter();
    while (res.lightness() > 180 and res.lightness() < 255) res = res.darker();
    if (res.lightness() == 0 or res.lightness() == 255)
        res = QColor(0x80, 0x80, 0x80);
    return res;
}

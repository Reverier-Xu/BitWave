/**
 * @file colorize.cc
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief 
 * @version 0.1.0
 * @date 2023-05-14
 *
 * @copyright 2023 Woo Tech
 */

#include "colorize.h"
#include <QColor>
#include <QImage>
#include <QtConcurrent/QtConcurrent>


struct Node {
    QPoint point;
    QColor color;
};

struct Clust {
    Node center;
    QVector<Node> buff;
};

int getColorDis(const QColor& a, const QColor& b) {
    return (int) sqrt((a.red() - b.red()) + (a.blue() - b.blue()) +
        (a.green() - b.green()));
}

/**
 * @brief get theme color from image.
 *        effects may be not so good.
 *        algorithm is based on k-means.
 * @param image the QImage object of source image.
 * @return QColor result color.
 */
QColor Colorize::colorize(const QImage& image) {

    int width = image.width(), height = image.height();
    int stepLen = width / 4;

    QPoint points[m_clustNum];
    int x = 0;
    for (auto& point : points) {
        point.setX(width / 8 + stepLen * (x++ % 4));
        point.setY(height / 8 + stepLen * ((x / 4) % 4));
    }

    Clust vec[m_clustNum];

    for (int i = 0; i < m_clustNum; ++i) {
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
                int dis[m_clustNum];
                memset(dis, 0, sizeof(dis));
                auto posColor = image.pixelColor(cols, rows);
                for (int i = 0; i < m_clustNum; ++i) {
                    dis[i] = getColorDis(vec[i].center.color, posColor);
                }
                int minDisClustId = 0;
                for (int i = 1; i < m_clustNum; ++i)
                    if (dis[i] < dis[minDisClustId]) minDisClustId = i;
                Node node;
                node.color = posColor;
                node.point = QPoint(cols, rows);
                vec[minDisClustId].buff.push_back(node);
            }
        }

        for (auto& centerClust : vec) {
            int avgR = 0, avgG = 0, avgB = 0;
            for (auto& item : centerClust.buff) {
                avgR += item.color.red();
                avgG += item.color.green();
                avgB += item.color.blue();
            }
            auto buffSize = (int) (centerClust.buff.size());
            avgR /= buffSize;
            avgG /= buffSize;
            avgB /= buffSize;
            QColor avgColor(avgR, avgG, avgB);
            int currentId = 0, minId = 0;
            double currentDis = MAXFLOAT, minDis = MAXFLOAT;
            for (auto& item : centerClust.buff) {
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
            for (auto& j : vec)
                if (point == j.center.point) {
                    flag++;
                    break;
                }

        if (flag == m_clustNum) {
            break;
        }

        for (auto& clust : vec) {
            clust.buff.clear();
            Node node;
            node.color = clust.center.color;
            node.point = clust.center.point;
            clust.buff.push_back(node);
        }
    }

    Clust* dst = vec;
    int max = 0;
    for (auto& i : vec)
        if (i.buff.count() > max) {
            if (i.center.color.lightness() < 80 ||
                i.center.color.lightness() > 180)
                continue;
            max = (int) (i.buff.count());
            dst = &i;
        }
//    qDebug() << dst->center.color.lightness();
    auto res = dst->center.color;
//    qDebug() << res;
    while (res.lightness() < 80 && res.lightness() > 0) res = res.lighter();
    while (res.lightness() > 180 && res.lightness() < 255) res = res.darker();
    if (res.lightness() == 0 or res.lightness() == 255)
        res = QColor(0x80, 0x80, 0x80);
    return res;
}

Colorize::Colorize(QObject* parent) : QObject(parent) { }

void Colorize::requestColorize(const QImage& src) {
    auto* watcher = new QFutureWatcher<QColor>(this);
    connect(watcher, &QFutureWatcher<QColor>::finished, this, [=]() {
        setColor(watcher->result());
        watcher->deleteLater();
    });
    watcher->setFuture(QtConcurrent::run(&Colorize::colorize, this, src));
}

QColor Colorize::color() const {
    return m_color;
}

void Colorize::setColor(const QColor& color) {
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

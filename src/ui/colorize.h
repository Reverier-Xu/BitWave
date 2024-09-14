/**
 * @file colorize.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-14
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QColor>
#include <QObject>

class Colorize : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  private:
    int m_clustNum{16};
    QColor m_color{128, 128, 128};
    int m_taskId{0};

  public:
    explicit Colorize(QObject* parent = nullptr);

    QColor colorize(const QImage& image);

    [[nodiscard]] QColor color() const;

    void setColor(const QColor& color);

  public slots:
    Q_INVOKABLE void requestColorize(const QImage& image);

  signals:
    void colorChanged(const QColor& color);
};

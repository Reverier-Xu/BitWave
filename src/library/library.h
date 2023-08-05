/**
 * @file library.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include "models/media.h"


class Library : public QObject {
   Q_OBJECT

    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

   private:
    QVector<Media> m_musics{};

    QVector<Media> m_videos{};

    QString m_filter{};

   protected:
    static Library* m_instance;

    explicit Library(QObject* parent = nullptr);

    ~Library() override;

   public:

    static Library* instance(QObject* parent = nullptr);

    [[nodiscard]] QString filter() const;

    void setFilter(const QString& filter);

   public slots:

    Q_INVOKABLE void load(const QString& route);

   signals:

    void filterChanged(const QString& filter);
};

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


class Library : public QObject {
   Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString adapter READ adapter WRITE setAdapter NOTIFY adapterChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

   private:
    QString m_title;

    QString m_adapter;

    QString m_filter;

   protected:
    static Library* m_instance;

    explicit Library(QObject* parent = nullptr);

    ~Library() override;

   public:

    static Library* instance(QObject* parent = nullptr);

    [[nodiscard]] QString title() const;

    void setTitle(const QString& title);

    [[nodiscard]] QString adapter() const;

    void setAdapter(const QString& adapter);

    [[nodiscard]] QString filter() const;

    void setFilter(const QString& filter);

   public slots:
    Q_INVOKABLE void load(const QString& adapter, const QString& filter);

   signals:

    void titleChanged(const QString& title);

    void adapterChanged(const QString& adapter);

    void filterChanged(const QString& filter);
};

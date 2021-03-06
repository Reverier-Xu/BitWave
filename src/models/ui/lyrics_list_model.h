/**
 * @file lyrics_list_model.h
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#pragma once

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QObject>
#include <QStringList>

class LyricContent : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString content MEMBER mContent READ content WRITE setContent
                   NOTIFY contentChanged)
    Q_PROPERTY(double startTime MEMBER mStartTime READ startTime WRITE
                   setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(double endTime MEMBER mEndTime READ endTime WRITE setEndTime
                   NOTIFY endTimeChanged)
   private:
    QString mContent;
    double mStartTime;
    double mEndTime;

   public:
    explicit LyricContent(QObject *parent = nullptr,
                          const QString &contentSrc = QString(),
                          const QString &contentTr = QString(),
                          double start = 0, double end = 0);

    LyricContent(const LyricContent &other);

    explicit LyricContent(QObject *parent = nullptr,
                          const QString &content = QString(), double start = 0,
                          double end = 0);

    ~LyricContent() override = default;

    [[nodiscard]] const QString &content() const;

    void setContent(const QString &content);

    [[nodiscard]] double startTime() const;

    void setStartTime(double startTime);

    [[nodiscard]] double endTime() const;

    void setEndTime(double endTime);

    bool operator<(const LyricContent &b) const;

    LyricContent &operator=(const LyricContent &b);

   signals:

    void contentChanged();

    void contentSrcChanged();

    void contentTrChanged();

    void startTimeChanged();

    void endTimeChanged();
};

class LyricsListModel : public QAbstractListModel {
    Q_OBJECT
   private:
    QList<LyricContent> mLyrics{};

   public:
    enum LyricRoles {
        LyricContentRole = Qt::UserRole + 1,
        LyricStartTimeRole,
        LyricEndTimeRole
    };

    explicit LyricsListModel(QObject *parent = nullptr);

    ~LyricsListModel() override = default;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void addLyric(const QString &content, double start, double end);

    void addLyric(const LyricContent &lyric);

    void insertLyric(const QString &content, double start, double end, int row);

    void insertLyric(const LyricContent &lyric, int row);

    void removeLyric(int row);

    void parseLyrics(const QString &raw, const QString &tr = QString());

    [[nodiscard]] Q_INVOKABLE int getCurrentLyricIndex(
        double currentTime) const;
};

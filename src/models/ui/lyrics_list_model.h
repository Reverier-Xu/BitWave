//
// Created by Reverier-Xu on 2021/7/26.
//

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
    Q_PROPERTY(double endTime MEMBER mEndTime READ endTime WRITE setEndTime NOTIFY
                       endTimeChanged)
private:
    QString mContent;
    double mStartTime;
    double mEndTime;

public:
    explicit LyricContent(QObject *parent = nullptr,
                          const QString &contentSrc = QString(),
                          const QString &contentTr = QString(),
                          double start = 0,
                          double end = 0)
            : QObject(parent) {
        mStartTime = start;
        mEndTime = end;
        mContent = contentSrc + "\n" + contentTr;
    }

    LyricContent(const LyricContent &other)
            : QObject(other.parent()) {
        mContent = other.mContent;
        mStartTime = other.mStartTime;
        mEndTime = other.mEndTime;
    }

    explicit LyricContent(QObject *parent = nullptr,
                          const QString &content = QString(),
                          double start = 0,
                          double end = 0) {
        mContent = content;
        mStartTime = start;
        mEndTime = end;
    }

    ~LyricContent() override = default;

    [[nodiscard]] const QString &content() const { return mContent; }

    void setContent(const QString &content) {
        if (mContent != content) {
            mContent = content;
            emit contentChanged();
        }
    }

    [[nodiscard]] double startTime() const { return mStartTime; }

    void setStartTime(double startTime) {
        if (mStartTime != startTime) {
            mStartTime = startTime;
            emit startTimeChanged();
        }
    }

    [[nodiscard]] double endTime() const { return mEndTime; }

    void setEndTime(double endTime) {
        if (mEndTime != endTime) {
            mEndTime = endTime;
            emit endTimeChanged();
        }
    }

    bool operator<(const LyricContent &b) const {
        return this->startTime() < b.startTime();
    }

    LyricContent &operator=(const LyricContent &b) {
        mContent = b.mContent;
        mStartTime = b.mStartTime;
        mEndTime = b.mEndTime;
        return *this;
    }

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
    QList<LyricContent> mLyrics{ };

public:
    enum LyricRoles {
        LyricContentRole = Qt::UserRole + 1,
        LyricStartTimeRole,
        LyricEndTimeRole
    };

    explicit LyricsListModel(QObject *parent = nullptr)
            : QAbstractListModel(parent) {
    }

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

    [[nodiscard]] Q_INVOKABLE int getCurrentLyricIndex(double currentTime) const;
};

#pragma once

#include <QObject>
#include <QString>

#include "models/media.h"

typedef struct Lyric {
    QString content;
    QString translation;    // optional
    double time;
} Lyric;

class ILyrics : public QObject {
    Q_OBJECT
    Q_PROPERTY(qint64 taskId READ taskId WRITE setTaskId NOTIFY taskIdChanged)
  private:
    qint64 m_taskId;

  public:
    explicit ILyrics(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~ILyrics() = default;

    virtual ILyrics* clone() = 0;

    virtual bool accepted(const Media& media) = 0;

    virtual void requestFetch(const Media& media) = 0;

    [[nodiscard]] qint64 taskId() const { return m_taskId; }

    void setTaskId(qint64 taskId) {
        if (m_taskId == taskId) {
            return;
        }
        m_taskId = taskId;
        emit taskIdChanged(m_taskId);
    }

  signals:
    void lyricsFetched(QString lyrics, QString translation = "");

    void taskIdChanged(qint64 taskId);
};

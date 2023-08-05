/**
 * @file lyrics.h
 * @author Reverier-Xu (reverier.xu[at]woooo.tech)
 * @brief
 * @version 0.1.0
 * @date 2023-05-13
 *
 * @copyright 2023 Woo Tech
 */

#pragma once

#include <QObject>
#include <QAbstractListModel>
#include "interface.h"

class LyricsModel : public QAbstractListModel {
    Q_OBJECT
   private:
    QList<Lyric> m_lyrics;

   public:
    enum LyricsRoles {
        LyricsIdRole = Qt::UserRole + 1,
        LyricsContentRole,
        LyricsTranslationRole,
    };

    explicit LyricsModel(QObject* parent = nullptr);

    ~LyricsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setLyrics(const QList<Lyric>& lyrics);
    
    void clearLyrics();

    QList<Lyric>* lyrics();
};

class Lyrics : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
   private:
    QList<ILyrics*> m_lyrics;
    bool m_loading{false};
    qint64 m_taskId{0};
    LyricsModel* m_lyricsModel{nullptr};
    int m_currentIndex{0};

    void initLyrics();

    void destroyLyrics();

   protected:
    static Lyrics* m_instance;

    explicit Lyrics(QObject* parent = nullptr);

    ~Lyrics() override;

   public:
    static Lyrics* instance(QObject* parent = nullptr);

    [[nodiscard]] bool loading() const;

    void setLoading(bool loading);

    [[nodiscard]] int currentIndex() const;

    LyricsModel* lyricsModel();

   public slots:
    Q_INVOKABLE void requestFetch(const Media& media);

    Q_INVOKABLE void handleLyrics(const QString& lyrics,
                                  const QString& translation);
    
    Q_INVOKABLE void updateCurrentIndex(double time);

   signals:
    void loadingChanged(bool n);

    void currentIndexChanged(int n);
};

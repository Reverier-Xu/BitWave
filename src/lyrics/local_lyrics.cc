#include "local_lyrics.h"

LocalLyrics::LocalLyrics(QObject* parent) : ILyrics(parent) {}

ILyrics* LocalLyrics::clone() {
    return new LocalLyrics(this->parent());
}

bool LocalLyrics::accepted(const Media& media) {
    const auto fileUrl = QUrl(media.url());
    if (fileUrl.isLocalFile()) {
        auto file = fileUrl.toLocalFile();
        // replace media suffix with .lrc
        auto suffix = file.split(".").last();
        file.replace(QString("." + suffix), ".lrc");
        // qDebug() << "local lyric file: " << file;
        if (QFile::exists(file)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void LocalLyrics::requestFetch(const Media& media) {
    const auto fileUrl = QUrl(media.url());
    if (fileUrl.isLocalFile()) {
        auto file = fileUrl.toLocalFile();
        auto suffix = file.split(".").last();
        file.replace(QString("." + suffix), ".lrc");
        if (QFile::exists(file)) {
            QFile fd(file);
            if (fd.open(QFile::ReadOnly)) {
                QString raw = fd.readAll().trimmed();
                QString source;
                QString translation;
                QString head = "IMP0SS1B1E_TIM3_H3@D";
                for (auto& i : raw.split('\n')) {
                    if (i.startsWith(head)) {
                        translation += i + '\n';
                    } else {
                        source += i + '\n';
                        head = i.left(i.indexOf(']') + 1);
                    }
                }
                emit lyricsFetched(source, translation);
                return;
            }
        }
    }
    emit lyricsFetched(tr("[00:00.00]Lyrics not found"));
}

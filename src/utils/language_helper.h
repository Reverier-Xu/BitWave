#pragma once

#include "models/media.h"
#include <QLocale>
#include <QString>

inline QLocale::Language getChLanguage(const QChar ch) {
    if (ch.unicode() >= 0x4e00 && ch.unicode() <= 0x9fa5) {
        return QLocale::Chinese;
    } else if (ch.unicode() >= 0x3040 && ch.unicode() <= 0x309f) {
        return QLocale::Japanese;
    } else if (ch.unicode() >= 0x1100 && ch.unicode() <= 0x11ff) {
        return QLocale::Korean;
    } else {
        return QLocale::English;
    }
}

inline int transformLocaleId(QLocale::Language lang) {
    switch (lang) {
    case QLocale::Chinese:
        return 1;
    case QLocale::Japanese:
        return 2;
    case QLocale::Korean:
        return 3;
    case QLocale::English:
        return 0;
    default:
        return 0;
    }
}

inline bool cmpStringWithLocale(const QString& a, const QString& b) {
    if (a.length() <= 0)
        return true;
    else if (b.length() <= 0)
        return false;
    const auto ch1 = a.at(0);
    const auto ch2 = b.at(0);
    const auto ch1Lang = getChLanguage(ch1);
    const auto ch2Lang = getChLanguage(ch2);
    if (ch1Lang == ch2Lang) {
        auto locale = QLocale(ch1Lang);
        auto collector = QCollator(locale);
        return collector.compare(a, b) < 0;
    } else {
        return transformLocaleId(ch1Lang) < transformLocaleId(ch2Lang);
    }
}

inline bool cmpTitleAsc(const Media& m1, const Media& m2) { return cmpStringWithLocale(m1.title(), m2.title()); }

inline bool cmpTitleDesc(const Media& m1, const Media& m2) { return cmpStringWithLocale(m2.title(), m1.title()); }

inline bool cmpArtistsAsc(const Media& m1, const Media& m2) {
    if (m1.artists().length() == 0)
        return true;
    else if (m2.artists().length() == 0)
        return false;
    return cmpStringWithLocale(m1.artists()[0], m2.artists()[0]);
}

inline bool cmpArtistsDesc(const Media& m1, const Media& m2) {
    if (m1.artists().length() == 0)
        return false;
    else if (m2.artists().length() == 0)
        return true;
    return cmpStringWithLocale(m2.artists()[0], m1.artists()[0]);
}

inline bool cmpAlbumAsc(const Media& m1, const Media& m2) { return cmpStringWithLocale(m1.album(), m2.album()); }

inline bool cmpAlbumDesc(const Media& m1, const Media& m2) { return cmpStringWithLocale(m2.album(), m1.album()); }

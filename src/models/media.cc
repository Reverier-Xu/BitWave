/**
 * @file media.cpp
 * @author Reverier-Xu (reverier.xu@outlook.com)
 * @brief
 * @version 0.1
 * @date 2021-12-08
 *
 * @copyright Copyright (c) 2021 Wootec
 *
 */

#include "media.h"

#include "parser/parser.h"

Media::Media(const Media& media) {
    m_url = media.m_url;
    m_time = media.m_time;
    m_title = media.m_title;
    m_artists = media.m_artists;
    m_album = media.m_album;
    m_type = media.m_type;
    m_comment = media.m_comment;
    m_embeddedLyrics = media.m_embeddedLyrics;
}

Media& Media::operator=(const Media& media) {
    m_url = media.m_url;
    m_time = media.m_time;
    m_title = media.m_title;
    m_artists = media.m_artists;
    m_album = media.m_album;
    m_type = media.m_type;
    m_comment = media.m_comment;
    m_embeddedLyrics = media.m_embeddedLyrics;
    return *this;
}

QString Media::url() const { return m_url; }

void Media::setUrl(const QString& n) { m_url = n; }

double Media::time() const { return m_time; }

void Media::setTime(double n) { m_time = n; }

QString Media::title() const { return m_title; }

void Media::setTitle(const QString& n) { m_title = n; }

QStringList Media::artists() const { return m_artists; }

void Media::setArtists(const QStringList& n) { m_artists = n; }

MediaType Media::type() const { return m_type; }

void Media::setType(MediaType n) { m_type = n; }

const QString& Media::album() const { return m_album; }

void Media::setAlbum(const QString& n) { m_album = n; }

const QString& Media::comment() const { return m_comment; }

void Media::setComment(const QString& n) { m_comment = n; }

const QString& Media::embeddedLyrics() const { return m_embeddedLyrics; }

void Media::setEmbeddedLyrics(const QString& n) { m_embeddedLyrics = n; }

Media::Media(const QString& rawUrl, const QString& title, const QStringList& artists, const QString& album,
             MediaType type, double duration, const QString& comment, const QString& embeddedLyrics) {
    m_url = rawUrl;
    m_title = title;
    m_artists = artists;
    m_album = album;
    m_type = type;
    m_time = duration;
    m_comment = comment;
    m_embeddedLyrics = embeddedLyrics;
}

const Media nullMedia{};

const Media& Media::null() { return nullMedia; }

bool Media::operator==(const Media& media) const {
    return m_url == media.m_url && m_time == media.m_time && m_title == media.m_title && m_artists == media.m_artists &&
           m_album == media.m_album && m_type == media.m_type && m_comment == media.m_comment &&
           m_embeddedLyrics == media.m_embeddedLyrics;
}

bool Media::operator!=(const Media& media) const { return !operator==(media); }

bool Media::operator<(const Media& media) const {
    if (m_title == media.m_title) {
        // Compare first artists if exists
        if (!m_artists.empty() && !media.m_artists.empty()) {
            return m_artists[0] < media.m_artists[0];
        } else {
            // Compare album if exists
            if (!m_album.isEmpty() && !media.m_album.isEmpty()) {
                return m_album < media.m_album;
            } else {
                // Compare url
                return m_url < media.m_url;
            }
        }
    }
    return m_title < media.m_title;
}

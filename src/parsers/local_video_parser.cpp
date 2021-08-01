//
// Created by Reverier-Xu on 2021/6/25.
//

#include "local_video_parser.h"
#include <QFile>
#include <QFileInfo>

const QStringList& acceptTypes() {
    static QStringList types;
    if (types.isEmpty()) {
        types = QStringList() << "*.mp4"
                              << "*.avi"
                              << "*.mkv"
                              << "*.flv"
                              << "*.mov"
                              << "*.wmv"
                              << "*.mpg"
                              << "*.mpeg"
                              << "*.m4v"
                              << "*.3gp"
                              << "*.3g2"
                              << "*.mts"
                              << "*.m2ts"
                              << "*.ts"
                              << "*.m3u8"
                              << "*.m3u"
                              << "*.mpd"
                              << "*.rm"
                              << "*.rmvb"
                              << "*.asf"
                              << "*.wma"
                              << "*.wmv"
                              << "*.flv"
                              << "*.f4v"
                              << "*.f4p"
                              << "*.f4a"
                              << "*.f4b";
    }
    return types;
}
bool accepted(const Media& media) {
    QFileInfo info(media.rawUrl());
    return acceptTypes().contains(info.suffix(), Qt::CaseInsensitive);
}

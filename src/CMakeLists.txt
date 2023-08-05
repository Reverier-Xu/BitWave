set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    APP_SRCS
    app/app.cc
)

set(
    CODEC_SRCS
    codec/codec.cc
    codec/mpv_codec.cc
    codec/ncm_codec.cc
)

set(
    LIBRARY_SRCS
    library/library.cc
)

set(
    LYRICS_SRCS
    lyrics/interface.h
    lyrics/lyrics.cc
    lyrics/ncm_lyrics.cc
    lyrics/local_lyrics.cc
)

set(
    MODELS_SRCS
    models/media.cc
    models/media.h
    models/media_list.cc
    models/media_list.h
)

set(
    PARSER_SRCS
    parser/parser.cc
    parser/ffmpeg_parser.cc
    parser/ncm_parser.cc
)

set(
    PLAYER_SRCS
    player/player.cc
    player/engine.cc
    player/video_player.cc
    player/play_queue.cc
)

set(
    PLAYLIST_SRCS
    playlist/playlist.cc
    playlist/interface.h
)

set(
    UI_SRCS
    ui/colorize.cc
    ui/ui.cc
    ui/config.cc
    ui/router.cc
    dbus/screensaver.cc
    ui/main.qrc
)

set(
    STORAGE_SRCS
    storage/storage.cc
    storage/storage.h
)

set(
    SRCS
    ${APP_SRCS}
    ${CODEC_SRCS}
    ${LIBRARY_SRCS}
    ${LYRICS_SRCS}
    ${MODELS_SRCS}
    ${STORAGE_SRCS}
    ${PARSER_SRCS}
    ${PLAYER_SRCS}
    ${PLAYLIST_SRCS}
    ${UI_SRCS}
    main.cc
)

set(
    QML_FILES
    ui/Main.qml

    # Components
    ui/components/ActiveTab.qml
    ui/components/ControlBar.qml
    ui/components/FramelessWindow.qml
    ui/components/PauseButton.qml
    ui/components/SideBar.qml
    ui/components/TimeButton.qml
    ui/components/TitleBar.qml
    ui/components/VolumeButton.qml
    ui/components/OptionWidget.qml
    ui/components/QueueWidget.qml
    ui/components/MediaRow.qml
    ui/components/Lyric.qml
    ui/components/SystemTray.qml
    ui/components/KeyTapEvent.qml

    # Views
    ui/views/PlayerView.qml
    ui/views/ExploreView.qml
    ui/views/LibraryView.qml
    ui/views/PlaylistView.qml
    ui/views/SearchView.qml
    ui/views/SettingsView.qml
)

# DBUS and MPRIS - Unix specific
if (UNIX)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dbus)
    # MPRIS 2.0 DBUS interfaces
    qt_add_dbus_adaptor(
        SRCS
            dbus/org.mpris.MediaPlayer2.Player.xml
            dbus/mpris2.h mpris::Mpris2 dbus/mpris2_player Mpris2Player
    )
    qt_add_dbus_adaptor(
        SRCS
            dbus/org.mpris.MediaPlayer2.xml
            dbus/mpris2.h mpris::Mpris2 dbus/mpris2_root Mpris2Root
    )

    # org.freedesktop.Notifications DBUS interface
    qt_add_dbus_interface(
        SRCS
            dbus/org.freedesktop.Notifications.xml
            dbus/notification
    )

    # org.gnome.SettingsDaemon interface
    qt_add_dbus_interface(
        SRCS
            dbus/org.gnome.SettingsDaemon.MediaKeys.xml
            dbus/gnomesettingsdaemon
    )

    # org.kde.KGlobalAccel interfaces
    # these are taken from the KGlobalAccel sources (LGPL 2.1)
    qt_add_dbus_interface(
        SRCS
            dbus/org.kde.KGlobalAccel.xml
            dbus/kglobalaccel
    )
    qt_add_dbus_interface(
        SRCS
            dbus/org.kde.kglobalaccel.Component.xml
            dbus/kglobalaccelcomponent
    )
endif (UNIX)

if (Qt6DBus_FOUND)
    list(
        APPEND SRCS
        dbus/mpris.cc
        dbus/mpris.h
        dbus/mpris2.cc
        dbus/mpris2.h
        dbus/dbusscreensaver.cc
        dbus/dbusscreensaver.h
    )
endif ()

if (WIN32)
    list(
        APPEND SRCS
        dbus/windowsscreensaver.cpp
        dbus/windowsscreensaver.h
    )
endif ()

qt_add_executable(${PROJECT_NAME} ${SRCS})

qt_add_qml_module(
    ${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QML_FILES}
    SOURCES ${SRCS}
    RESOURCE_PREFIX "/"
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qml/${PROJECT_NAME}"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${QT_LIBRARIES}
    RxUIplugin
    ${MPV_LIBRARIES}
    FFmpeg::avformat
    FFmpeg::avutil
    SingleApplication::SingleApplication
    QtAES
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

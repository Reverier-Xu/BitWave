cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

qt5_add_resources(RESOURCES ./resources/main.qrc)

set(MANAGERS_SRCS
        managers/base_manager.cpp
        managers/base_manager.h
        managers/gui_manager.cpp
        managers/gui_manager.h
        managers/app_manager.cpp
        managers/app_manager.h
        managers/player_manager.cpp
        managers/player_manager.h
        managers/parser_manager.cpp
        managers/parser_manager.h
        managers/display_manager.cpp
        managers/display_manager.h
        managers/queue_manager.cpp
        managers/queue_manager.h
        )

set(MODELS_SRCS
        models/media.cpp
        models/media.h
        models/ui/lyrics_list_model.cpp
        models/ui/lyrics_list_model.h
        )

set(PARSERS_SRCS
        parsers/base_parser.cpp
        parsers/base_parser.h
        parsers/parser_factory.cpp
        parsers/parser_factory.h
        parsers/local_music_parser.cpp
        parsers/local_music_parser.h
        parsers/local_video_parser.cpp
        parsers/local_video_parser.h
        parsers/local_netease_music_parser.cpp
        parsers/local_netease_music_parser.h
        parsers/online_netease_music_parser.cpp
        parsers/online_netease_music_parser.h
        parsers/online_bilibili_video_parser.cpp
        parsers/online_bilibili_video_parser.h
        )

set(ENGINES_SRCS
        engines/mpv_engine.cpp
        engines/mpv_engine.h
        engines/sqlite_engine.cpp
        engines/sqlite_engine.h
        )

set(UTILITIES_SRCS
        utilities/mpv_helper.h
        utilities/color_helper.cpp
        utilities/color_helper.h
        utilities/ncm_helper.cpp
        utilities/ncm_helper.h)

set(COMPONENTS_SRCS
        resources/components/video_player.cpp
        resources/components/video_player.h
        )

set(SRCS
        entry.cpp
        app_defs.cpp 
        app_defs.h
        ${PARSERS_SRCS}
        ${ENGINES_SRCS}
        ${UTILITIES_SRCS}
        ${COMPONENTS_SRCS}
        ${MANAGERS_SRCS}
        ${MODELS_SRCS}
        ${RESOURCES}
        )

# DBUS and MPRIS - Unix specific
if(UNIX AND Qt5DBus_FOUND)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dbus)
    message("DBUS FOUND")
    # MPRIS 2.0 DBUS interfaces
    qt5_add_dbus_adaptor(SRCS
            dbus/org.mpris.MediaPlayer2.Player.xml
            dbus/mpris2.h mpris::Mpris2 dbus/mpris2_player Mpris2Player)
    qt5_add_dbus_adaptor(SRCS
            dbus/org.mpris.MediaPlayer2.xml
            dbus/mpris2.h mpris::Mpris2 dbus/mpris2_root Mpris2Root)

    # org.freedesktop.Notifications DBUS interface
    qt5_add_dbus_interface(SRCS
            dbus/org.freedesktop.Notifications.xml
            dbus/notification)

    # org.gnome.SettingsDaemon interface
    qt5_add_dbus_interface(SRCS
            dbus/org.gnome.SettingsDaemon.MediaKeys.xml
            dbus/gnomesettingsdaemon)

    # org.kde.KGlobalAccel interfaces
    # these are taken from the KGlobalAccel sources (LGPL 2.1)
    qt5_add_dbus_interface(SRCS
            dbus/org.kde.KGlobalAccel.xml
            dbus/kglobalaccel)
    qt5_add_dbus_interface(SRCS
            dbus/org.kde.kglobalaccel.Component.xml
            dbus/kglobalaccelcomponent)
endif(UNIX AND Qt5DBus_FOUND)

optional_source(Qt5DBus_FOUND
        SRCS
        dbus/mpris.cpp
        dbus/mpris.h
        dbus/mpris2.cpp
        dbus/mpris2.h
        dbus/dbusscreensaver.cpp
        dbus/dbusscreensaver.h
        )

add_executable(BitWave ${SRCS})

target_link_libraries(BitWave ${QT_LIBRARIES} ${MPV_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} QtAES)

set_target_properties(BitWave
        PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


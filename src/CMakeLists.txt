cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

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
        managers/provider_manager.cpp
        managers/provider_manager.h
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

set(PROVIDERS_SRCS
        providers/local_music_provider.cpp
        providers/local_music_provider.h
        providers/local_video_provider.cpp
        providers/local_video_provider.h
        providers/netease_provider.cpp
        providers/netease_provider.h
        providers/bilibili_provider.cpp
        providers/bilibili_provider.h
        providers/osu_provider.cpp
        providers/osu_provider.h
        providers/playlists_provider.cpp
        providers/playlists_provider.h
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
        ${PROVIDERS_SRCS}
        ${ENGINES_SRCS}
        ${UTILITIES_SRCS}
        ${COMPONENTS_SRCS}
        ${MANAGERS_SRCS}
        ${MODELS_SRCS}
        ${RESOURCES}
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


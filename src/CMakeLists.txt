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
    codec/qmc_codec.cc
)

set(
    CONFIG_SRCS
    config/config.cc
    config/display_config.cc
)

set(
    LIBRARY_SRCS
    library/library.cc
    library/local_library.cc
)

set(
    LYRICS_SRCS
    lyrics/lyrics.cc
)

set(
    MODELS_SRCS
    models/media.cc
    models/media.h
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
)

set(
    PLAYLIST_SRCS
    playlist/playlist.cc
)

set(
    UI_SRCS
    ui/colorize.cc
    ui/ui.cc
    ui/router.cc
    ui/main.qrc
)

set(
    SRCS
    ${APP_SRCS}
    ${CODEC_SRCS}
    ${CONFIG_SRCS}
    ${LIBRARY_SRCS}
    ${LYRICS_SRCS}
    ${MODELS_SRCS}
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

    # Views
    ui/views/PlayerView.qml
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

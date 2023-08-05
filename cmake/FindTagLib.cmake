if (NOT TAGLIB_MIN_VERSION)
    set(TAGLIB_MIN_VERSION "1.10")
endif (NOT TAGLIB_MIN_VERSION)

if (NOT WIN32)
    find_program(TAGLIBCONFIG_EXECUTABLE NAMES taglib-config PATHS
                 ${BIN_INSTALL_DIR}
                 )
endif (NOT WIN32)

#reset vars
set(TAGLIB_LIBRARIES)
set(TAGLIB_CFLAGS)

# if taglib-config has been found
if (TAGLIBCONFIG_EXECUTABLE)

    exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --version RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_VERSION)

    if (TAGLIB_VERSION STRLESS "${TAGLIB_MIN_VERSION}")
        message(STATUS "TagLib version too old: version searched :${TAGLIB_MIN_VERSION}, found ${TAGLIB_VERSION}")
        set(TAGLIB_FOUND FALSE)
    else (TAGLIB_VERSION STRLESS "${TAGLIB_MIN_VERSION}")

        exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --libs RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_LIBRARIES)

        exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --cflags RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_CFLAGS)

        if (TAGLIB_LIBRARIES AND TAGLIB_CFLAGS)
            set(TAGLIB_FOUND TRUE)
        endif (TAGLIB_LIBRARIES AND TAGLIB_CFLAGS)
        string(REGEX REPLACE " *-I" ";" TAGLIB_INCLUDES "${TAGLIB_CFLAGS}")
    endif (TAGLIB_VERSION STRLESS "${TAGLIB_MIN_VERSION}")
    mark_as_advanced(TAGLIB_CFLAGS TAGLIB_LIBRARIES TAGLIB_INCLUDES)

else (TAGLIBCONFIG_EXECUTABLE)

    find_path(TAGLIB_INCLUDES
              NAMES
              tag.h
              PATH_SUFFIXES taglib
              PATHS
              ${KDE4_INCLUDE_DIR}
              ${INCLUDE_INSTALL_DIR}
              )

    if (NOT WIN32)
        # on non-win32 we don't need to take care about WIN32_DEBUG_POSTFIX

        find_library(TAGLIB_LIBRARIES tag PATHS ${KDE4_LIB_DIR} ${LIB_INSTALL_DIR})

    else (NOT WIN32)

        # 1. get all possible libnames
        set(args PATHS ${KDE4_LIB_DIR} ${LIB_INSTALL_DIR})
        set(newargs "")
        set(libnames_release "")
        set(libnames_debug "")

        list(LENGTH args listCount)

        # just one name
        list(APPEND libnames_release "tag")
        list(APPEND libnames_debug "tagd")

        set(newargs ${args})

        # search the release lib
        find_library(TAGLIB_LIBRARIES_RELEASE
                     NAMES ${libnames_release}
                     ${newargs}
                     )

        # search the debug lib
        find_library(TAGLIB_LIBRARIES_DEBUG
                     NAMES ${libnames_debug}
                     ${newargs}
                     )

        if (TAGLIB_LIBRARIES_RELEASE AND TAGLIB_LIBRARIES_DEBUG)

            # both libs found
            set(TAGLIB_LIBRARIES optimized ${TAGLIB_LIBRARIES_RELEASE}
                debug ${TAGLIB_LIBRARIES_DEBUG})

        else (TAGLIB_LIBRARIES_RELEASE AND TAGLIB_LIBRARIES_DEBUG)

            if (TAGLIB_LIBRARIES_RELEASE)

                # only release found
                set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARIES_RELEASE})

            else (TAGLIB_LIBRARIES_RELEASE)

                # only debug (or nothing) found
                set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARIES_DEBUG})

            endif (TAGLIB_LIBRARIES_RELEASE)

        endif (TAGLIB_LIBRARIES_RELEASE AND TAGLIB_LIBRARIES_DEBUG)

        mark_as_advanced(TAGLIB_LIBRARIES_RELEASE)
        mark_as_advanced(TAGLIB_LIBRARIES_DEBUG)

    endif (NOT WIN32)

    include(FindPackageMessage)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Taglib DEFAULT_MSG TAGLIB_INCLUDES TAGLIB_LIBRARIES)

endif (TAGLIBCONFIG_EXECUTABLE)


if (TAGLIB_FOUND)
    if (NOT Taglib_FIND_QUIETLY AND TAGLIBCONFIG_EXECUTABLE)
#        message(STATUS "Taglib found: ${TAGLIB_LIBRARIES}")
    endif (NOT Taglib_FIND_QUIETLY AND TAGLIBCONFIG_EXECUTABLE)
else (TAGLIB_FOUND)
    if (Taglib_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Taglib")
    endif (Taglib_FIND_REQUIRED)
endif (TAGLIB_FOUND)

######################################################################################
# CMake module to search for the mpv libraries.
#
# WARNING: This module is experimental work in progress.
#
# Based one FindVLC.cmake by:
# Copyright (c) 2011 Michael Jansen <info@michael-jansen.biz>
# Modified by Tobias Hieta <tobias@hieta.se>, Reverier-Xu <reverier.xu@outlook.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
######################################################################################
#
# CHANGE LOG
# 0.1.0 Initial version
# 0.1.1 Added support for Windows MPV libs.
#
######################################################################################

#
### Global Configuration Section
#
SET(_MPV_REQUIRED_VARS MPV_INCLUDE_DIR MPV_LIBRARIES)

#
### MPV uses pkgconfig.
#
if (PKG_CONFIG_FOUND)
    pkg_check_modules(PC_MPV QUIET mpv)
endif (PKG_CONFIG_FOUND)


#
### Look for the include files.
#
find_path(
        MPV_INCLUDE_DIR
        NAMES mpv/client.h
        HINTS
        ${PC_MPV_INCLUDEDIR}
        ${PC_MPV_INCLUDE_DIRS} # Unused for MPV but anyway
        DOC "MPV include directory"
)

if (NOT MPV_INCLUDE_DIR)
    if (WIN32)
        set(MPV_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/libmpv/include)
        message("MPV not found on this Windows PC, Using 3rd/libmpv instead.")
    endif (WIN32)
    if (NOT WIN32)
        message(FATAL_ERROR "MPV not found on this Unix PC, please install it.")
    endif (NOT WIN32)
endif (NOT MPV_INCLUDE_DIR)

# message(STATUS "MPV_INCLUDE_DIR: ${MPV_INCLUDE_DIR}")

#
### Look for the libraries
#
set(_MPV_LIBRARIES_NAMES mpv)
if (PC_MPV_LIBRARIES)
    set(_MPV_LIBRARIES_NAMES ${PC_MPV_LIBRARIES})
endif (PC_MPV_LIBRARIES)
if (WIN32)
    if (NOT PC_MPV_LIBDIR)
        set(PC_MPV_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/libmpv)
        # message(STATUS "MPV_LIBDIR: ${PC_MPV_LIBDIR}")
    endif (NOT PC_MPV_LIBDIR)
endif (WIN32)

# message("MPV_LIB_NAMES: ${_MPV_LIBRARIES_NAMES}")

foreach (l ${_MPV_LIBRARIES_NAMES})
    # message("${l}")
    find_library(
            MPV_LIBRARIES_${l}
            NAMES ${l}
            HINTS
            ${PC_MPV_LIBDIR}
            ${PC_MPV_LIBRARIES_DIRS} # Unused for MPV but anyway
            PATH_SUFFIXES lib${LIB_SUFFIX}
    )
    list(APPEND MPV_LIBRARIES ${MPV_LIBRARIES_${l}})
endforeach ()

get_filename_component(_MPV_LIBRARIES_DIR ${MPV_LIBRARIES_mpv} PATH)
mark_as_advanced(MPV_LIBRARIES)

set(MPV_LIBRARIES_DIRS _MPV_LIBRARIES_DIR)
list(REMOVE_DUPLICATES MPV_LIBRARIES_DIRS)

mark_as_advanced(MPV_INCLUDE_DIR)
mark_as_advanced(MPV_LIBRARIES_DIRS)
set(MPV_INCLUDE_DIRS ${MPV_INCLUDE_DIR})

#
### Check if everything was found and if the version is sufficient.
#
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        MPV
        REQUIRED_VARS ${_MPV_REQUIRED_VARS}
        VERSION_VAR MPV_VERSION_STRING
)

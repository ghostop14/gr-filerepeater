INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FILEREPEATER filerepeater)

FIND_PATH(
    FILEREPEATER_INCLUDE_DIRS
    NAMES filerepeater/api.h
    HINTS $ENV{FILEREPEATER_DIR}/include
        ${PC_FILEREPEATER_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FILEREPEATER_LIBRARIES
    NAMES gnuradio-filerepeater
    HINTS $ENV{FILEREPEATER_DIR}/lib
        ${PC_FILEREPEATER_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/filerepeaterTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FILEREPEATER DEFAULT_MSG FILEREPEATER_LIBRARIES FILEREPEATER_INCLUDE_DIRS)
MARK_AS_ADVANCED(FILEREPEATER_LIBRARIES FILEREPEATER_INCLUDE_DIRS)

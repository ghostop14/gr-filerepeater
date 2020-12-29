# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT filerepeater_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT filerepeater_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND filerepeater_python_files
    AdvFileSink_python.cc
    file_repeater_ex_python.cc
    flowsync_python.cc
    StateTimer_python.cc
    TimeOfDay_python.cc
    VectorToTxtFile_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(filerepeater 
   ../..
   gr::filerepeater
   "${filerepeater_python_files}")

install(TARGETS filerepeater_python DESTINATION ${GR_PYTHON_DIR}/filerepeater COMPONENT pythonapi)

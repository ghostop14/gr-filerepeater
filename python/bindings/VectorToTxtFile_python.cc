/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(VectorToTxtFile.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(a182af9d860f59b34002b32934a8b020)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <filerepeater/VectorToTxtFile.h>
// pydoc.h is automatically generated in the build directory
#include <VectorToTxtFile_pydoc.h>

void bind_VectorToTxtFile(py::module& m)
{

    using VectorToTxtFile    = ::gr::filerepeater::VectorToTxtFile;


    py::class_<VectorToTxtFile, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<VectorToTxtFile>>(m, "VectorToTxtFile", D(VectorToTxtFile))

        .def(py::init(&VectorToTxtFile::make),
           py::arg("filename"),
           py::arg("vectorLen"),
           py::arg("frequency"),
           py::arg("sampleRate"),
           py::arg("notes"),
           py::arg("append"),
           py::arg("updateRateSec"),
           py::arg("precision"),
           py::arg("WriteTimeHeader"),
           D(VectorToTxtFile,make)
        )
        



        ;




}









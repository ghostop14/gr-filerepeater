/* -*- c++ -*- */

#define FILEREPEATER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "filerepeater_swig_doc.i"

%{
#include "filerepeater/file_repeater_ex.h"
%}


%include "filerepeater/file_repeater_ex.h"
GR_SWIG_BLOCK_MAGIC2(filerepeater, file_repeater_ex);

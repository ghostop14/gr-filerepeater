/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_FILEREPEATER_FILE_REPEATER_EX_IMPL_H
#define INCLUDED_FILEREPEATER_FILE_REPEATER_EX_IMPL_H

#include <filerepeater/file_repeater_ex.h>
#include <chrono>
#include <ctime>
#include <gnuradio/thread/thread.h>

#define DTYPE_COMPLEX 1
#define DTYPE_FLOAT 2
#define DTYPE_INT 3
#define DTYPE_SHORT 4
#define DTYPE_BYTE 5

#define CONVTYPE_NONE 0
// HackRF
#define CONVTYPE_SIGNED8 1
// RTL-SDR
#define CONVTYPE_UNSIGNED8 2

namespace gr {
  namespace filerepeater {

    class file_repeater_ex_impl : public file_repeater_ex
    {
     private:
        size_t d_itemsize;
        FILE *d_fp = NULL;
        FILE *d_new_fp;
        bool d_repeat=false;
        bool d_updated;
        float d_delayFirstStart;  // in seconds.  Wait this long before starting to play the file for the first time (outputs zeros till then)
    	std::chrono::time_point<std::chrono::steady_clock> start, end;
    	bool bFirstWorkCall;
    	bool bDelayingFirst;
        int d_repeat_delay=0; /* in ms */
        int d_repeat_times=0; /* 0 for continued repeat, > 0 for limit */
        int d_complex_conv;
        bool convData;
        char *convBuffer;
        int iMaxItems;
        gr::thread::mutex fp_mutex;
        int d_dataType;

        int cur_repeat_cycle=0;
        std::chrono::time_point<std::chrono::steady_clock> stopped_transmitting;
        bool holdingTransmit=false;

        bool workDone;
        bool d_verbose;

        void do_update();

     public:
      file_repeater_ex_impl(size_t itemsize, const char *filename, int complex_conv, float delayFirstStartSec, bool repeat,
    		  int repeat_delay, int repeat_times, bool verbose);
      ~file_repeater_ex_impl();

      bool seek(long seek_point, int whence);
      void open(const char *filename, bool repeat,int repeat_delay, int repeat_times);
      void close();

      virtual bool stop();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace filerepeater
} // namespace gr

#endif /* INCLUDED_FILEREPEATER_FILE_REPEATER_EX_IMPL_H */


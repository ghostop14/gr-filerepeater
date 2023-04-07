/* -*- c++ -*- */
/* 
 * Copyright 2017 ghostop14.
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

#include <gnuradio/io_signature.h>
#include "file_repeater_ex_impl.h"

#include <gnuradio/thread/thread.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <stdio.h>
#include <pmt/pmt.h>

// win32 (mingw/msvc) specific
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef O_BINARY
#define	OUR_O_BINARY O_BINARY
#else
#define	OUR_O_BINARY 0
#endif
// should be handled via configure
#ifdef O_LARGEFILE
#define	OUR_O_LARGEFILE	O_LARGEFILE
#else
#define	OUR_O_LARGEFILE 0
#endif

namespace gr {
  namespace filerepeater {

  file_repeater_ex::sptr file_repeater_ex::make(size_t itemsize,
                                                const char* filename,
                                                int complex_conv,
                                                float delayFirstStartSec,
                                                bool repeat,
                                                int repeat_delay,
                                                int repeat_times,
                                                bool verbose)
  {
      return gnuradio::make_block_sptr<file_repeater_ex_impl>(itemsize,
                                                              filename,
                                                              complex_conv,
                                                              delayFirstStartSec,
                                                              repeat,
                                                              repeat_delay,
                                                              repeat_times,
                                                              verbose);
  }

file_repeater_ex_impl::file_repeater_ex_impl(size_t itemsize, const char *filename, int complex_conv, float delayFirstStartSec,
		bool repeat, int repeat_delay,int repeat_times, bool verbose)
: sync_block("filerepeater",
			io_signature::make(0, 0, 0),
			io_signature::make(1, 1, itemsize)),
	d_itemsize(itemsize), d_fp(0), d_new_fp(0), d_repeat(repeat),d_repeat_delay(repeat_delay),d_repeat_times(repeat_times),
	d_updated(false), d_delayFirstStart(delayFirstStartSec),bFirstWorkCall(true),d_complex_conv(complex_conv)
{
		d_verbose = verbose;

		if (d_delayFirstStart > 0.0) {
		  bDelayingFirst = true;
		}

		workDone = false;

		// Some checks for efficiency in work runtime routine to key off bool rather than multiple calls every time
		if (d_itemsize == sizeof(gr_complex) && (d_complex_conv > 0)) {
			convData = true;
			iMaxItems=gr::block::max_noutput_items();
	    	if (iMaxItems==0)
	    		iMaxItems=8192;

	    	convBuffer = new char[iMaxItems*2];
		}
		else {
			convData = false;
			convBuffer = NULL;
		}

		open(filename, repeat,repeat_delay,repeat_times);
		do_update();
}

bool file_repeater_ex_impl::stop() {
	if (convBuffer) {
		delete[] convBuffer;
		convBuffer = NULL;
	}

    if(d_new_fp) {
      fclose ((FILE*)d_new_fp);
      d_new_fp = NULL;
    }

    if(d_fp) {
      fclose ((FILE*)d_fp);
      d_fp = NULL;
    }

	return true;
}

  file_repeater_ex_impl::~file_repeater_ex_impl()
  {
	  // Swig doesn't always manage destructors correctly but gnuradio calls stop (which can be called multiple times here.
	  stop();
  }

  bool
  file_repeater_ex_impl::seek(long seek_point, int whence)
  {
    return fseek((FILE*)d_fp, seek_point *d_itemsize, whence) == 0;
  }


  void
  file_repeater_ex_impl::open(const char *filename, bool repeat, int repeat_delay,int repeat_times)
  {
    // obtain exclusive access for duration of this function
    gr::thread::scoped_lock lock(fp_mutex);

    int fd;

    // we use "open" to use to the O_LARGEFILE flag
    if((fd = ::open(filename, O_RDONLY | OUR_O_LARGEFILE | OUR_O_BINARY)) < 0) {
      perror(filename);
      throw std::runtime_error("can't open file");
    }

    if(d_new_fp) {
      fclose(d_new_fp);
      d_new_fp = NULL;
    }

    if((d_new_fp = fdopen (fd, "rb")) == NULL) {
      perror(filename);
      ::close(fd);	// don't leak file descriptor if fdopen fails
      throw std::runtime_error("can't open file");
    }

    //Check to ensure the file will be consumed according to item size
    fseek(d_new_fp, 0, SEEK_END);
    int file_size = ftell(d_new_fp);
    rewind (d_new_fp);

    //Warn the user if part of the file will not be consumed.
    if(file_size % d_itemsize){
      GR_LOG_WARN(d_logger, "WARNING: File will not be fully consumed with the current output type");
    }

    if(d_fp)
      fclose(d_fp);

    d_fp = d_new_fp;    // install new file pointer
    d_new_fp = 0;
    d_updated = false;

    d_repeat = repeat;
    d_repeat_delay = repeat_delay;
    d_repeat_times = repeat_times;
  }

  void
  file_repeater_ex_impl::close()
  {
    // obtain exclusive access for duration of this function
    gr::thread::scoped_lock lock(fp_mutex);

    if(d_new_fp != NULL) {
      fclose(d_new_fp);
      d_new_fp = NULL;
    }
    d_updated = true;
  }

  void
  file_repeater_ex_impl::do_update()
  {
    if(d_updated) {
      gr::thread::scoped_lock lock(fp_mutex); // hold while in scope

      if(d_fp)
        fclose(d_fp);

      d_fp = d_new_fp;    // install new file pointer
      d_new_fp = 0;
      d_updated = false;
    }
  }

  int
  file_repeater_ex_impl::work(int noutput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items)
  {
    do_update();       // update d_fp is reqd

    gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function

    if(d_fp == NULL)
      throw std::runtime_error("work with file not open");

    if (workDone)
    	return WORK_DONE;

    // standard / original data out
    char *o = (char*)output_items[0];

    int i;

    // First check if we're in our initial delay, if so, just return zero's and exit.
	if (bFirstWorkCall && bDelayingFirst) {
		// This sets the start clock on our first work call rather than when the flowgraph starts
    	start = std::chrono::steady_clock::now();
    	bFirstWorkCall = false;

    	memset((void *)o,0x00,d_itemsize*noutput_items);
    	return noutput_items;
	}
	else {
		if (bFirstWorkCall)
			bFirstWorkCall = false;

		// We could be here if it's just not the first time into work.
		if (bDelayingFirst) {
	    	end = std::chrono::steady_clock::now();
	    	std::chrono::duration<double> elapsed_seconds = end-start;

	    	if (elapsed_seconds.count() <= d_delayFirstStart) {
	            // I'm holding the transmit and my initial timeout hasn't expired yet.
	        	memset((void *)o,0x00,d_itemsize*noutput_items);
	        	return noutput_items;
	    	}
	    	else {
	    		// we can release our hold
	    		bDelayingFirst = false;
	    	}
		}
	}

    // check if we're in a hold-down delay between file repeats.  If so return 0's.
    if (holdingTransmit) {
        auto tnow = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tnow - stopped_transmitting);

        if (diff.count() <= d_repeat_delay) {
            // I'm holding the transmit and my timeout hasn't expired yet.
        	memset((void *)o,0x00,d_itemsize*noutput_items);
            return noutput_items;
        }
        else {
        	holdingTransmit = false;
        }
    }

    long itemsRead;

    if (!convData) {
    	itemsRead = fread(o, d_itemsize, noutput_items, (FILE*)d_fp);
    }
    else {
    	// In this mode we're reading signed/unsigned byte complex for conversion.  So override size to 2 byte complex size
    	if (noutput_items < iMaxItems)
    		itemsRead = fread(convBuffer, 2, noutput_items, (FILE*)d_fp);
    	else
    		itemsRead = fread(convBuffer, 2, iMaxItems, (FILE*)d_fp);
    }

    if (itemsRead > 0) {
    	// we read some data
		// If we're towards the end of the file and we don't have enough data to satisfy the request, return what we have.

		if (convData) {
			// We're converting so we're going to have to iterate through.
			gr_complex *complexout = (gr_complex *)o;
			float newI,newQ;
			int i;
			char *bPointer = convBuffer;

			for (i=0;i<itemsRead;i++) {
				if (d_complex_conv == CONVTYPE_UNSIGNED8) {
					// unsigned8 / rtl_sdr
					newI = ((float)bPointer[0])/((float)UCHAR_MAX/2.0)-1.0;
					newQ = ((float)bPointer[1])/((float)UCHAR_MAX/2.0)-1.0;
				}
				else {
					// signed8 / hackrf
					newI = ((float)((signed char)bPointer[0]))/(float)SCHAR_MAX;
					newQ = ((float)((signed char)bPointer[1]))/(float)SCHAR_MAX;
				}

				gr_complex newComplex(newI,newQ);

				complexout[i] = newComplex;

				bPointer += 2;
			}
		}

		return itemsRead;
    }
    else {
    	// if itemsRead = 0, then we've reached the end of the file.
        // Check repeat times and reset the file
    	// std::cout << "DEBUG: Reached end of file." << std::endl;

    	if (d_verbose)
    		std::cout << "[File Repeater] End of file reached." << std::endl;

        // If we've reached the end and we're not repeating, we're done.
    	if (!d_repeat) {
    		workDone = true;
    		return 0;
    	}

    	// We should be repeating.  If we're limiting repeats, increment our counter.
        if (d_repeat_times > 0) {
            cur_repeat_cycle += 1;
        }

        // If we have a repeat count and we've hit our limit, we're done.
        if ((d_repeat_times > 0) && (cur_repeat_cycle > d_repeat_times)) {
          	// memset((void *)o,0x00,d_itemsize*noutput_items);
        	workDone = true;

          	return 0; // noutput_items;
        }

        // We should still be repeating if we're here so reset the file back to the beginning.
        if(fseek ((FILE *) d_fp, 0, SEEK_SET) == -1) {
        	// If we're here, there was an error resetting the file.
          fprintf(stderr, "[%s] File reset / fseek failed\n", __FILE__);
          exit(-1);
        }

        // if we're here:
        // 1. we should be repeating
        // 2. we should either continue repeating or we're within our cycle limit
        // 3. We're back at the beginning of the file, so see if we need a repeat delay.

        if (d_repeat_delay > 0) {
            stopped_transmitting = std::chrono::steady_clock::now();
            holdingTransmit = true;
        	return 0; // let the loop read pick up on next call.
        }

        return 0;
    }

    return noutput_items;
  }

  } /* namespace filerepeater */
} /* namespace gr */


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


#ifndef INCLUDED_FILEREPEATER_FILE_REPEATER_EX_H
#define INCLUDED_FILEREPEATER_FILE_REPEATER_EX_H

#include <filerepeater/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace filerepeater {

    /*!
     * \brief <+description of block+>
     * \ingroup filerepeater
     *
     */
    class FILEREPEATER_API file_repeater_ex : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<file_repeater_ex> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of filerepeater::file_repeater_ex.
       *
       * To avoid accidental use of raw pointers, filerepeater::file_repeater_ex's
       * constructor is in a private implementation
       * class. filerepeater::file_repeater_ex::make is the public interface for
       * creating new instances.
       */

      /*!
       * \brief Create a file source.
       *
       * Opens \p filename as a source of items into a flowgraph. The
       * data is expected to be in binary format, item after item. The
       * \p itemsize of the block determines the conversion from bits
       * to items.
       *
       * If \p repeat is turned on, the file will repeat the file after
       * it's reached the end.
       *
       * \param itemsize	the size of each item in the file, in bytes
       * \param filename	name of the file to source from
       * \param complex_conv	Perform complex conversion (if any)
       * \param delayFirstStartSec	Seconds as float to wait before starting to play file for the first time
       * \param repeat	repeat file from start
       * \param repeat_delay	ms to wait before starting to transmit the file again (0 for continuous)
       * \param repeat_times	repeat number of times to repeat the file (0 for no limit)
       * \param verbose Provide additional info at runtime
       */
      /* static sptr make(size_t itemsize, const char *filename, bool repeat = false); */
      static sptr make(size_t itemsize, const char *filename, int complex_conv, float delayFirstStartSec=0.0, bool repeat=false,
    		  	  	  int repeat_delay=0,int repeat_times=0, bool verbose=false);

      /*!
       * \brief seek file to \p seek_point relative to \p whence
       *
       * \param seek_point	sample offset in file
       * \param whence	one of SEEK_SET, SEEK_CUR, SEEK_END (man fseek)
       */
      virtual bool seek(long seek_point, int whence) = 0;

      /*!
       * \brief Opens a new file.
       *
       * \param filename	name of the file to source from
       * \param repeat	repeat file from start
       * \param repeat_delay	ms to wait before starting to transmit the file again (0 for continuous)
       * \param repeat_times	repeat number of times to repeat the file (0 for no limit)
       */
      virtual void open(const char *filename, bool repeat,int repeat_delay, int repeat_times) = 0;

      /*!
       * \brief Close the file handle.
       */
      virtual void close() = 0;

    };

  } // namespace filerepeater
} // namespace gr

#endif /* INCLUDED_FILEREPEATER_FILE_REPEATER_EX_H */


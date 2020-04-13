/* -*- c++ -*- */
/*
 * Copyright 2019 ghostop14.
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

#ifndef INCLUDED_FILEREPEATER_FLOWSYNC_H
#define INCLUDED_FILEREPEATER_FLOWSYNC_H

#include <filerepeater/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace filerepeater {

    /*!
     * \brief <+description of block+>
     * \ingroup filerepeater
     *
     */
    class FILEREPEATER_API flowsync : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<flowsync> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of filerepeater::flowsync.
       *
       * To avoid accidental use of raw pointers, filerepeater::flowsync's
       * constructor is in a private implementation
       * class. filerepeater::flowsync::make is the public interface for
       * creating new instances.
       */
      static sptr make(int datasize, int nconnections);
    };

  } // namespace filerepeater
} // namespace gr

#endif /* INCLUDED_FILEREPEATER_FLOWSYNC_H */


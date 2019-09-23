# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

install(FILES
    filerepeater_file_repeater_ex.block.yml
    filerepeater_flowsync.block.yml
    filerepeater_AdvFileSink.block.yml
    filerepeater_StateTimer.block.yml
    filerepeater_StateAnd.block.yml
    filerepeater_StateOr.block.yml
    filerepeater_StateToBool.block.yml
    filerepeater_TimeOfDay.block.yml 
    filerepeater_msg_to_file.block.yml
    filerepeater_MetaToPair.block.yml
    filerepeater_VectorToTxtFile.block.yml DESTINATION share/gnuradio/grc/blocks
)

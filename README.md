# gr-filerepeater
GNURadio block to play back files with delays between replays and/or limited replay counts.

This block is based on the standard gnuradio File Source block with replay enhancements.

## Building
Build is pretty standard:

mkdir build

cd build

cmake ..

make

make install

ldconfig

## Parameters
replay delay: The delay between replays should be provided in milliseconds.  If this is set to 0, there will be no delay.
	Once the end of a file has been reached, the source will send a set_mute message that can be fed
	into a Mute block.  Once the delay is over a set_mute false message will be sent as the file starts
	playing again.
	
replay count: Only play the file n many times.  If this is set to 0, it will play continuously.

Note: These two attributes can be combined, such as: Only play a file 3 times but wait 1000 ms in between each play.

See the sample flowgraph for a basic example.


# gr-filerepeater
GNURadio block with more control over how files are played.  Files can be started with an initial delay before starting to output data 
(output 0's until the timer is hit), played with delays in between (output 0's in between), or played once then output zero's rather than just returning no samples.

## Building
Build is pretty standard:

mkdir build

cd build

cmake ..

make

make install

ldconfig

## Parameters

Delay first start (sec): float delay in seconds (can be fractional) before the file data stream is started.  Will output zeros until this delay is hit.

replay delay in ms: The delay between replays should be provided in milliseconds.  If this is set to 0, there will be no delay.
	During the delay, zero's will be output (similar to a mute).  Once the delay is over the file will restart from the beginning.
	
replay count: Only play the file n many times.  If this is set to 0, it will play continuously.

Notes: 

- These can be combined, such as: Wait 2 seconds, then play the file 3 times but wait 1000 ms in between each play.

- Also note that if you want a file to output 0's after the end is reached rather than just returning no samples, set repeat to "yes" and replay count to 1.

See the sample flowgraph for a basic example.


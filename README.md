# gr-filerepeater
A set of GNURadio blocks with more control over how files are played.  

Shows in widgits now as [Advanced File] Group
Advanced File Source: Files can be started with an initial delay before starting to output data 
(output 0's until the timer is hit), played with delays in between (output 0's in between), or played once then output zero's rather than  
just returning no samples.  The block can also directly play signed8/HackRF and unsigned8/RTL-SDR saved IQ data and dynamically convert 
it to native complex.

The delay capabilities can all be useful if you're trying to combine multiple sample files into a single larger signal with different 
intervals or trying to do blind signal analysis and decoding where the standard file source repeat option causes signal discontinuities when it restarts.

Flow Input Synchronizer: If you play two file sources (even if you create two file source blocks but assign the same file to each), the data point 
flows will not be point-synchronized.  The Flow Input Synchronizer is a sync block that just puts all inputs and outputs in step.  In other words 
if you plotted the same file from two different file sources on a frequency sink using this block, the plots would now overlap/align whereas they do 
not without it.
 
## Building
Build is pretty standard:

mkdir build

cd build

cmake ..

make

make install

ldconfig

## Advanced File Source Parameters

Complex Conversion: If you have a native HackRF (signed8) raw file or RTL-SDR (unsigned8) raw file, select the appropriate conversion type.  
Leave as None for standard GNURadio float32-based complex recordings. 

Delay first start (sec): float delay in seconds (can be fractional) before the file data stream is started.  Will output zeros until this delay is hit.

replay delay in ms: The delay between replays should be provided in milliseconds.  If this is set to 0, there will be no delay.
	During the delay, zero's will be output (similar to a mute).  Once the delay is over the file will restart from the beginning.
	
replay count: Only play the file n many times.  If this is set to 0, it will play continuously.

Notes: 

- These can be combined, such as: Wait 2 seconds, then play the file 3 times but wait 1000 ms in between each play.

- Also note that if you want a file to output 0's after the end is reached rather than just returning no samples, set repeat to "yes" and replay count to 1.

See the sample flowgraph for a basic example.


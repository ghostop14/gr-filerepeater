# gr-filerepeater
gr-filerepeater is a GNU Radio OOT module with advance file source/sink capabilities and supporting message blocks to provide more granular control over file activities.  The core blocks are:  

**Advanced File Source**: This block provides a number of options to adjust file playback.  Data sourcing can be delayed (output 0's until the timer is hit), delays can be inserted between repeats (originally added to avoid odd discontinuities when the first sample immediately follows the last sample such as phase shifts), and the number of repeats can be controlled.  The block also supports converting directly from signed 8 or unsigned 8 such that files from other systems such as HackRF (signed 8) or RTL-SDR (unsigned 8) can be used without additional conversion.  

**Advanced File Sink**: This block provides a number of enhancements to writing files.  A base directory and base filename are specified, and an optional message input can be used to control file rotation.  Limits can also be placed on file size or record time.

**State Timer**: This block brings record timer capabilites to GNU Radio.  This can be used directly to control the advanced file sink via its message port.

**Time of Day**: Recordings can now be controlled based on time-of-day when paired with the advanced file sink block.

**Message State Blocks**: While not unique to file activities, these blocks were first developed here to provide some AND and OR logic for the file record message port.  Messages from the state timer, time-of-day, or even other sources such as gpredict-doppler can be combined in any AND/OR way to control file recording.

There is also a supporting test tool called test-filethroughput that can be used to measure disk write performance.  This can be useful if you have a high-speed application and you're worried that disk IO may be your bottleneck.

## Building
Build is pretty standard:

mkdir build

cd build

cmake ..

make

(sudo if necessary) make install

ldconfig


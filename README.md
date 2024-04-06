# libhfsaver

A Thread-safe library for saving http-flv streams to disk.

## Progressing

1. [x] Save http-flv streams to disk
2. [X] Split audio from http-flv streams to mp3
3. [X] Get keyframes from http-flv streams
4. [ ] hfs_cli example

## Build

1. Install needed packages:
   _Ubuntu_:

   ```bash
   sudo apt install cmake pkg-config ffmpeg libavcodec-dev libmp3lame-dev libavformat-dev libavfilter-dev libavdevice-dev
   ```

2. Build:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

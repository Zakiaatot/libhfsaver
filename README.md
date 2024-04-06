# libhfsaver

A Thread-safe library for saving http-flv streams to disk.

## Build

1. Install needed packages:

   _`Ubuntu`_:

   ```bash
   sudo apt install cmake pkg-config ffmpeg libavcodec-dev libmp3lame-dev libavformat-dev libavfilter-dev libavdevice-dev
   ```

   _`Windows`_:

   (1) [Install cmake](https://cmake.org/download/)

   (2) [Install pkg-config](https://blog.csdn.net/LuckyHanMo/article/details/125471360)

   (3) [Install ffmpeg
   ](https://github.com/BtbN/FFmpeg-Builds/releases)

   - download ffmpeg-master-latest-win64-gpl-shared.zip
   - extract to `C:\ffmpeg`
   - add `C:\ffmpeg\bin` to `PATH`(otherwise you need to change CMakelist.txt to your ffmpeg path)

   (4) Install visual studio and c++ build tools

2. Build:

   _`Ubuntu`_:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

   _`Windows`_:

   (1) generate sln

   ```cmd
   mkdir build && cd build
   cmake ..
   ```

   (2) open `hfsaver.sln` and build, finally get `hfsaver.dll` and `hfsaver.lib`

## Progressing

1. [x] Save http-flv streams to disk
2. [x] Split audio from http-flv streams to mp3
3. [x] Get keyframes from http-flv streams
4. [x] Add java native interface

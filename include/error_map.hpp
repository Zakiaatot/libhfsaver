#ifndef _ERROR_MAP_HPP_
#define _ERROR_MAP_HPP_

#include "libhfsaver.h"
#include <unordered_map>

extern const std::unordered_map<HfsRet, const char*> g_error_map;
#define ERROR_UNKNOWN -1
#define ERROR_FFMPEG_INPUT_STREAM -2
#define ERROR_FFMPEG_STREAM_INFO -3
#define ERROR_FFMPEG_OUTPUT_OPEN -4
#define ERROR_FFMPEG_OUTPUT_CONTEXT -5
#define ERROR_FFMPEG_CODEC_FIND -6
#define ERROR_FFMPEG_OUTPUT_CREATE -7
#define ERROR_FFMPEG_CODEC_COPY -8
#define ERROR_FFMPEG_WRITE_HEADER -9
#define ERROR_FFMPEG_WRITE_PACKET -10
#define ERROR_FFMPEG_READ_PACKET -11
#define ERROR_HFS_MAX_TASK -12
#define ERROR_HFS_TASK_NOT_FOUND -13
#define ERROR_HFS_CMD_EXEC -14
#define ERROR_JNI_OBJECT_DEFINE -15
#define ERROR_JNI_MEMORY_ALLOC -16

#endif //!_ERROR_MAP_HPP_

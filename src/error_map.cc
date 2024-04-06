#include "error_map.hpp"

const std::unordered_map<HfsRet, const char*> g_error_map =
{
    {OK, "ok"},
    {ERROR_UNKNOWN,"unknown error"},
    {ERROR_FFMPEG_INPUT_STREAM,"failed to open input stream"},
    {ERROR_FFMPEG_STREAM_INFO,"failed to get stream info"},
    {ERROR_FFMPEG_OUTPUT_OPEN,"failed to open output file"},
    {ERROR_FFMPEG_OUTPUT_CONTEXT,"failed to allocate output context"},
    {ERROR_FFMPEG_CODEC_FIND,"failed to find codec"},
    {ERROR_FFMPEG_OUTPUT_CREATE,"failed to create output file"},
    {ERROR_FFMPEG_CODEC_COPY,"failed to copy codec"},
    {ERROR_FFMPEG_WRITE_HEADER,"failed to write header"},
    {ERROR_FFMPEG_WRITE_PACKET,"failed to write packet"},
    {ERROR_FFMPEG_READ_PACKET,"failed to read packet"},
    {ERROR_HFS_MAX_TASK,"reached max task number"},
    {ERROR_HFS_TASK_NOT_FOUND,"task not found"},
    {ERROR_HFS_CMD_EXEC,"cmd execute failed"},
    {ERROR_JNI_OBJECT_DEFINE,"java object defined error"},
    {ERROR_JNI_MEMORY_ALLOC,"jni malloc failed"}
};
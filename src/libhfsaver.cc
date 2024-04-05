#include <cstring>
#include "libhfsaver.h"
#include "error_map.hpp"
#include "hfs.hpp"

HfsRet hfs_task_begin(IN const char* url, IN const char* save_path, OUT int* task_id)
{
    auto res = Singleton<Hfs>::get_instance().task_begin(url, save_path);
    if (res)
    {
        *task_id = res.ret;
    }
    return res.msg_code;
}

HfsRet hfs_task_end(IN const int task_id)
{
    return Singleton<Hfs>::get_instance().task_end(task_id).msg_code;
}

HfsRet hfs_task_query(IN const int task_id, OUT HfsTaskInfo* task_info)
{
    auto res = Singleton<Hfs>::get_instance().task_query(task_id);
    if (res)
    {
        *task_info = res.ret;
    }
    return res.msg_code;
}

HfsRet hfs_task_query_all_size(OUT int* task_num)
{
    auto res = Singleton<Hfs>::get_instance().task_query_all();
    if (res)
    {
        *task_num = res.ret.size();
    }
    return res.msg_code;
}

HfsRet hfs_task_query_all(OUT HfsTaskInfo* task_info_array)
{
    auto res = Singleton<Hfs>::get_instance().task_query_all();
    if (res)
    {
        for (int i = 0; i < res.ret.size(); i++)
        {
            task_info_array[i] = res.ret[i];
        }
    }
    return res.msg_code;
}

HfsRet hfs_error_message_query(IN const HfsRet code, OUT char* err_buf)
{
    auto res = g_error_map.find(code);
    if (res != g_error_map.cend())
    {
        strcpy(err_buf, res->second);
    }
    else
    {
        strcpy(err_buf, g_error_map.at(ERROR_UNKNOWN));
    }
    return OK;
}

HfsRet hfs_utils_split_mp3_from_flv(IN const char* flv_save_path, IN const char* mp3_save_path)
{
    return OK;
}

HfsRet hfs_utils_get_key_frame(IN const char* flv_save_path, IN const char* key_frame_save_path)
{
    return OK;
}


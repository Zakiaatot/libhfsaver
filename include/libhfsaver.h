#ifndef _LIBHFSAVER_H_
#define _LIBHFSAVER_H_
#ifdef __cplusplus
extern "C"
{
#endif //!__cplusplus


#define IN
#define OUT
#define OK 0
#define HFS_ERROR_BUF_SIZE 64

    typedef int HfsRet;
    typedef enum {
        Processing,
        Finish
    } HfsTaskStatus;
    typedef struct
    {
        int task_id;
        HfsTaskStatus status;
        HfsRet last_error;
        unsigned long last_save_time;
        unsigned long saved_size;
    }HfsTaskInfo;

    // http-flv save task
    HfsRet hfs_task_begin(IN const char* url, IN const char* save_path, OUT int* task_id);
    HfsRet hfs_task_end(IN const int task_id);
    HfsRet hfs_task_query(IN const int task_id, OUT HfsTaskInfo* task_info);
    HfsRet hfs_task_query_all_size(OUT int* task_num);
    HfsRet hfs_task_query_all(OUT HfsTaskInfo* task_info_array);

    // error message
    HfsRet hfs_error_message_query(IN const HfsRet code, OUT char* err_buf);

    // utils
    HfsRet hfs_utils_split_mp3_from_flv(IN const char* flv_save_path, IN const char* mp3_save_path);
    HfsRet hfs_utils_get_key_frame(IN const char* flv_save_path, IN const char* key_frame_save_path, IN const float fps);


#ifdef __cplusplus
}
#endif //!__cplusplus
#endif //!_LIBHFSAVER_H_

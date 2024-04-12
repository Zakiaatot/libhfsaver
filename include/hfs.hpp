#ifndef _HFS_HPP_
#define _HFS_HPP_

#include <unordered_map>
#include <vector>
#include <mutex>
#include <atomic>
#include "libhfsaver.h"
#include "singleton.hpp"
#include "thread_pool.hpp"

#define HFS_MAX_TASK_NUM 12

class Hfs {
    friend class Singleton<Hfs>;
public:
    template <typename T>
    struct Status {
        Status(HfsRet msg_code, const T& ret) :msg_code(msg_code), ret(ret) {}
        HfsRet msg_code = OK;
        T ret;
        operator bool() const { return msg_code == OK; }
        bool operator!() const { return msg_code != OK; }
        static Status<T> ok() { return Status<T>(OK, T()); }
        static Status<T> ok(const T& res) { return Status<T>(OK, res); }
        static Status<T> ok(const T&& res) { return Status<T>(OK, std::move(res)); }
        static Status<T> err(HfsRet msg_code) { return Status<T>(msg_code, T()); }
    };
    using StatusVoid = typename Hfs::Status<void*>;
public:
    Status<int> task_begin(std::string url, std::string save_path);
    StatusVoid task_end(const int task_id);
    Status<HfsTaskInfo> task_query(const int task_id);
    Status<std::vector<HfsTaskInfo>> task_query_all();
    StatusVoid utils_split_mp3_from_flv(std::string flv_save_path, std::string mp3_save_path);
    StatusVoid utils_get_key_frame(std::string flv_save_path, std::string key_frame_save_path, const float fps);
    StatusVoid utils_flv_to_mp4(std::string flv_save_path, std::string mp4_save_path);
private:
    Hfs();
    ~Hfs();
    static void task_process(int task_id, std::string url, std::string save_path);
    static void task_ret(int task_id, HfsRet msg_code);
    static void task_info_update(int task_id, std::string save_path);
    std::atomic<int> task_id_counter_;
    ThreadPool* p_thread_pool_;
    std::unordered_map<int, HfsTaskInfo> task_info_map_;
    std::mutex map_mutex_;
};

#endif //!_HFS_HPP_

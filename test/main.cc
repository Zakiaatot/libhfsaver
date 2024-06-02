#include <iostream>
#include <thread>
#include <libhfsaver.h>

std::ostream& operator<<(std::ostream& os, const HfsTaskInfo& info)
{
    os << "---HfsTaskInfo---" << std::endl;
    os << "task_id: " << info.task_id << std::endl;
    os << "status: " << info.status << std::endl;
    os << "last_error: " << info.last_error << std::endl;
    os << "start_time: " << info.start_time << std::endl;
    os << "last_save_time: " << info.last_save_time << std::endl;
    os << "saved_size: " << info.saved_size << std::endl;
    return os;
}

int main(int argc, char const* argv[])
{
    int task_id = -1;
    HfsRet ret = hfs_task_begin(argv[1], argv[2], &task_id);
    if (ret == OK)
    {
        HfsTaskInfo info{ 0 };
        for (int i = 0;i < 10;i++)
        {
            if ((ret = hfs_task_query(task_id, &info)) == OK)
            {
                std::cout << info << std::endl;
            }
            else
            {
                break;
            }
            if (info.status == HfsTaskStatus::Finish)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (info.saved_size > 0)
        {
            hfs_utils_split_mp3_from_flv(argv[2], "1.mp3");
            hfs_utils_get_key_frame(argv[2], "frames", 500.0f / (info.last_save_time - info.start_time));
        }
    }

    char buf[HFS_ERROR_BUF_SIZE]{ 0 };
    hfs_error_message_query(ret, buf);
    std::cout << buf << std::endl;

    return task_id;
}

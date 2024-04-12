#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavformat/avformat.h>
}
#include <string>
#include "hfs.hpp"
#include "error_map.hpp"
#include "utils.hpp"

Hfs::Hfs() :
    task_id_counter_(0),
    p_thread_pool_(new ThreadPool(HFS_MAX_TASK_NUM))
{

}

Hfs::~Hfs()
{
    delete p_thread_pool_;
}

Hfs::Status<int> Hfs::task_begin(std::string url, std::string save_path)
{
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (task_info_map_.size() >= HFS_MAX_TASK_NUM)
    {
        return Status<int>::err(ERROR_HFS_MAX_TASK);
    }
    int task_id = task_id_counter_++;
    task_info_map_.insert(
        std::make_pair(
            task_id,
            HfsTaskInfo
            {
                task_id,
                HfsTaskStatus::Processing,
                OK,
                Utils::get_timestamp_ms(),
                0,
                0
            }
        )
    );
    p_thread_pool_->enqueue(Hfs::task_process, task_id, url, save_path);
    return Status<int>::ok(task_id);
}

Hfs::StatusVoid Hfs::task_end(const int task_id)
{
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (task_info_map_.find(task_id) == task_info_map_.end())
    {
        return StatusVoid::err(ERROR_HFS_TASK_NOT_FOUND);
    }
    task_info_map_[task_id].status = HfsTaskStatus::Finish;
    return StatusVoid::ok();
}

Hfs::Status<HfsTaskInfo> Hfs::task_query(const int task_id)
{
    std::unique_lock<std::mutex> lock(map_mutex_);
    if (task_info_map_.find(task_id) == task_info_map_.end())
    {
        return Status<HfsTaskInfo>::err(ERROR_HFS_TASK_NOT_FOUND);
    }
    return Status<HfsTaskInfo>::ok(task_info_map_[task_id]);
}

Hfs::Status<std::vector<HfsTaskInfo>> Hfs::task_query_all()
{
    std::unique_lock<std::mutex> lock(map_mutex_);
    std::vector<HfsTaskInfo> res;
    for (auto& it : task_info_map_)
    {
        res.push_back(it.second);
    }
    return Status<std::vector<HfsTaskInfo>>::ok(res);
}

void Hfs::task_ret(int task_id, HfsRet msg_code)
{
    Hfs& hfs = Singleton<Hfs>::get_instance();
    std::unique_lock<std::mutex> lock(hfs.map_mutex_);
    hfs.task_info_map_[task_id].last_error = msg_code;
    hfs.task_info_map_[task_id].status = HfsTaskStatus::Finish;
}

void Hfs::task_info_update(int task_id, std::string save_path)
{
    Hfs& hfs = Singleton<Hfs>::get_instance();
    std::unique_lock<std::mutex> lock(hfs.map_mutex_);
    hfs.task_info_map_[task_id].last_save_time = Utils::get_timestamp_ms();
    if (hfs.task_info_map_[task_id].start_time == 0)
    {
        hfs.task_info_map_[task_id].start_time = hfs.task_info_map_[task_id].last_save_time;
    }
    unsigned long file_size = Utils::get_file_size(save_path.c_str());
    if (file_size > 0)
    {
        hfs.task_info_map_[task_id].saved_size = file_size;
    }
}

void Hfs::task_process(int task_id, std::string url, std::string save_path)
{
    AVFormatContext* in_fmt_ctx = NULL, * out_fmt_ctx = NULL;
    AVIOContext* out_avio_ctx = NULL;
    int ret = 0;
    HfsRet hfs_ret = OK;
    Hfs& hfs = Singleton<Hfs>::get_instance();

    do
    {
        // 打开输入流
        if ((ret = avformat_open_input(&in_fmt_ctx, url.c_str(), NULL, NULL)) < 0)
        {
            hfs_ret = ERROR_FFMPEG_INPUT_STREAM;
            break;
        }

        // 获取流信息
        if ((ret = avformat_find_stream_info(in_fmt_ctx, NULL)) < 0)
        {
            hfs_ret = ERROR_FFMPEG_STREAM_INFO;
            break;
        }

        // 打开输出文件
        if ((ret = avio_open2(&out_avio_ctx, save_path.c_str(), AVIO_FLAG_WRITE, NULL, NULL)) < 0)
        {
            hfs_ret = ERROR_FFMPEG_OUTPUT_OPEN;
            break;
        }

        // 创建输出文件上下文
        auto fmt = av_guess_format("flv", NULL, NULL);
        if ((ret = avformat_alloc_output_context2(&out_fmt_ctx, fmt, NULL, NULL)) < 0)
        {
            hfs_ret = ERROR_FFMPEG_OUTPUT_CONTEXT;
            break;
        }
        out_fmt_ctx->pb = out_avio_ctx;
        out_fmt_ctx->url = av_strdup(save_path.c_str());

        // 复制输入流信息到输出上下文
        for (unsigned int i = 0; i < in_fmt_ctx->nb_streams; i++)
        {
            AVStream* in_stream = in_fmt_ctx->streams[i];
            const AVCodec* codec = avcodec_find_decoder(in_stream->codecpar->codec_id);
            if (!codec)
            {
                hfs_ret = ERROR_FFMPEG_CODEC_FIND;
                goto ErrorExit;
            }
            AVStream* out_stream = avformat_new_stream(out_fmt_ctx, NULL);
            if (!out_stream)
            {
                hfs_ret = ERROR_FFMPEG_OUTPUT_CREATE;
                goto ErrorExit;
            }
            ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
            if (ret < 0)
            {
                hfs_ret = ERROR_FFMPEG_CODEC_COPY;
                goto ErrorExit;
            }
        }

        // 写文件头
        if ((ret = avformat_write_header(out_fmt_ctx, NULL)) < 0)
        {
            hfs_ret = ERROR_FFMPEG_WRITE_HEADER;
            break;
        }

        // 复制数据包
        AVPacket pkt{};
        for (;;)
        {
            {
                std::unique_lock<std::mutex> lock(hfs.map_mutex_);
                if (hfs.task_info_map_[task_id].status == HfsTaskStatus::Finish)
                    break;
            }
            ret = av_read_frame(in_fmt_ctx, &pkt);
            if (ret < 0)
            {
                if (ret == AVERROR_EOF)
                {
                    break;
                }
                hfs_ret = ERROR_FFMPEG_READ_PACKET;
                goto ErrorExit;
            }

            av_packet_rescale_ts(&pkt, in_fmt_ctx->streams[pkt.stream_index]->time_base,
                out_fmt_ctx->streams[pkt.stream_index]->time_base);

            ret = av_interleaved_write_frame(out_fmt_ctx, &pkt);
            if (ret < 0)
            {
                hfs_ret = ERROR_FFMPEG_WRITE_PACKET;
                goto ErrorExit;
            }

            av_packet_unref(&pkt);
            task_info_update(task_id, save_path.c_str());
        }

        //正常结束
        avformat_close_input(&in_fmt_ctx);
        if (out_fmt_ctx && !out_avio_ctx)
        {
            avformat_free_context(out_fmt_ctx);
        }
        avio_closep(&out_avio_ctx);
        {
            std::unique_lock<std::mutex> lock(hfs.map_mutex_);
            hfs.task_info_map_[task_id].status = HfsTaskStatus::Finish;
            return;
        }
    } while (false);


    // 异常结束
ErrorExit:
    avformat_close_input(&in_fmt_ctx);
    if (out_fmt_ctx && !out_avio_ctx)
    {
        avformat_free_context(out_fmt_ctx);
    }
    avio_closep(&out_avio_ctx);
    return task_ret(task_id, hfs_ret);
}


Hfs::StatusVoid Hfs::utils_split_mp3_from_flv(std::string flv_save_path, std::string mp3_save_path) {
    // 构造FFmpeg命令用于从FLV文件中提取出音频并转换为MP3
    // ffmpeg -i 输入文件.flv -q:a 5 输出文件.mp3
    // 这里 -q:a 指定了音频的质量，数字越小质量越高
    std::string command = "ffmpeg -i \"";
    command += flv_save_path;
    command += "\" -q:a 5 \"";
    command += mp3_save_path;
    command += "\" -y";

    // 运行FFmpeg命令
    int result = std::system(command.c_str());

    // 检查命令是否成功执行
    if (result != 0)
    {
        return StatusVoid::err(ERROR_HFS_CMD_EXEC);
    }

    return StatusVoid::ok();
}

Hfs::StatusVoid Hfs::utils_get_key_frame(std::string flv_save_path, std::string key_frame_save_path, const float fps)
{
    // ffmpeg -i out1.flv -vf fps=0.2 output_frames/%d.jpg
    std::string command = "ffmpeg -i \"";
    command += flv_save_path;
    command += "\" -vf fps=";
    command += std::to_string(fps);
    command += " \"";
    command += key_frame_save_path;
    command += "/%d.jpg\" -y";

    // 运行FFmpeg命令
    int result = std::system(command.c_str());

    // 检查命令是否成功执行
    if (result != 0)
    {
        return StatusVoid::err(ERROR_HFS_CMD_EXEC);
    }

    return StatusVoid::ok();
}

Hfs::StatusVoid Hfs::utils_flv_to_mp4(std::string flv_save_path, std::string mp4_save_path)
{
    // ffmpeg -i input.flv -c copy output.mp4 -y
    std::string command = "ffmpeg -i \"" + flv_save_path + "\" -c copy \"" + mp4_save_path + "\" -y";
    int result = system(command.c_str());
    if (result != 0)
    {
        return StatusVoid::err(ERROR_HFS_CMD_EXEC);
    }
    return StatusVoid::ok();
}

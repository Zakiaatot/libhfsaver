#include <cstdio>
#include <cstring>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <input_url> <output_file>\n", argv[0]);
        return 1;
    }

    char* input_url = argv[1];
    char* output_file = argv[2];

    AVFormatContext* in_fmt_ctx = NULL, * out_fmt_ctx = NULL;
    AVIOContext* out_avio_ctx = NULL;
    int ret;

    // 打开输入流
    if ((ret = avformat_open_input(&in_fmt_ctx, input_url, NULL, NULL)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to open input stream: %s\n", err_buf);
        return 1;
    }

    // 获取流信息
    if ((ret = avformat_find_stream_info(in_fmt_ctx, NULL)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to get stream info: %s\n", err_buf);
        return 1;
    }

    // 打开输出文件
    if ((ret = avio_open2(&out_avio_ctx, output_file, AVIO_FLAG_WRITE, NULL, NULL)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to open output file: %s\n", err_buf);
        return 1;
    }

    // 创建输出文件上下文
    AVOutputFormat* fmt = av_guess_format("flv", NULL, NULL);
    if ((ret = avformat_alloc_output_context2(&out_fmt_ctx, fmt, NULL, NULL)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to allocate output context: %s\n", err_buf);
        return 1;
    }
    out_fmt_ctx->pb = out_avio_ctx;
    out_fmt_ctx->url = av_strdup(output_file);

    // 复制输入流信息到输出上下文
    for (unsigned int i = 0; i < in_fmt_ctx->nb_streams; i++) {
        AVStream* in_stream = in_fmt_ctx->streams[i];
        AVCodec* codec = avcodec_find_decoder(in_stream->codecpar->codec_id);
        if (!codec) {
            printf("Failed to find codec\n");
            return 1;
        }
        AVStream* out_stream = avformat_new_stream(out_fmt_ctx, NULL);
        if (!out_stream) {
            printf("Failed to create output stream\n");
            return 1;
        }
        ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
        if (ret < 0) {
            char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
            av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
            printf("Failed to copy codec params: %s\n", err_buf);
            return 1;
        }
    }

    // 写文件头
    if ((ret = avformat_write_header(out_fmt_ctx, NULL)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to write header: %s\n", err_buf);
        return 1;
    }

    // 复制数据包
    AVPacket pkt;
    while (1) {
        ret = av_read_frame(in_fmt_ctx, &pkt);
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                break;
            }
            char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
            av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
            printf("Failed to read packet: %s\n", err_buf);
            return 1;
        }

        av_packet_rescale_ts(&pkt, in_fmt_ctx->streams[pkt.stream_index]->time_base,
            out_fmt_ctx->streams[pkt.stream_index]->time_base);

        ret = av_interleaved_write_frame(out_fmt_ctx, &pkt);
        if (ret < 0) {
            char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
            av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
            printf("Failed to write packet: %s\n", err_buf);
            return 1;
        }

        av_packet_unref(&pkt);
    }

    // 写文件尾
    if ((ret = av_write_trailer(out_fmt_ctx)) < 0) {
        char err_buf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
        av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
        printf("Failed to write trailer: %s\n", err_buf);
        return 1;
    }

    // 释放资源
    avformat_close_input(&in_fmt_ctx);
    if (out_fmt_ctx && !out_avio_ctx) {
        avformat_free_context(out_fmt_ctx);
    }
    avio_closep(&out_avio_ctx);

    return 0;
}
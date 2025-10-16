extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

#include <cstdio>
#include <cstdlib>
#include <cstdint>

//УВАГА
//БОЛЬШУЮ ЧАСТЬ ЭТОЙ НУТЕЛЛЫ НАПИСАЛ DEEPSUCK
//НЕ НАДО К НЕЙ ПРИДИРАТЬСЯ 

const char* MP4_NAME = "files/ba.mp4";
const char* ASM_NAME = "files/code.asm";

char* last_frame;

void process_frame(AVFrame *frame, int, FILE* asm_file) {
    int width = frame->width;
    int height = frame->height;
    
    for (int y = 0; y < height-2; y++) {
        for (int x = 0; x < width; x++) {
                int y_index = y * frame->linesize[0] + x;
                int u_index = (y/2) * frame->linesize[1] + (x/2);
                int v_index = (y/2) * frame->linesize[2] + (x/2);
                
                uint8_t Y = frame->data[0][y_index];
                uint8_t U = frame->data[1][u_index];
                uint8_t V = frame->data[2][v_index];
                
                int r = Y + 1.402 * (V - 128);
                int g = Y - 0.344 * (U - 128) - 0.714 * (V - 128);
                int b = Y + 1.772 * (U - 128);
            
                int ind = (y+1) * 80 + x;

                if(r > 70 || g > 70 || b > 70)
                {
                    if(last_frame[ind] != '#')
                    {
                        fprintf(asm_file,\
                            "PUSH %d\n"\
                            "POPR SR1\n"\
                            "PUSH 35\n"\
                            "POPM [SR1]\n", ind);
                        last_frame[ind] = '#';
                     }
                }
                else
                {
                    if(last_frame[ind] != '_')
                    {
                        fprintf(asm_file,\
                            "PUSH %d\n"\
                            "POPR SR1\n"\
                            "PUSH 95\n"\
                            "POPM [SR1]\n", ind);
                        last_frame[ind] = '_';
                    }
                }
        }
    }
}

int main() {
    AVFormatContext *format_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    const AVCodec *codec = NULL;
    AVPacket *packet = NULL;
    AVFrame *frame = NULL;
    int video_stream_index = -1;
    int frame_count = 0;
    const int MAX_FRAMES = 10000;
    
    AVCodecParameters *codec_params = NULL;
    
    format_ctx = NULL;
    codec_ctx = NULL;
    codec = NULL;
    packet = NULL;
    frame = NULL;

    FILE *asm_file = fopen(ASM_NAME, "w+");
    avformat_open_input(&format_ctx, MP4_NAME, NULL, NULL);
    avformat_find_stream_info(format_ctx, NULL);

    for (int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    

    codec_params = format_ctx->streams[video_stream_index]->codecpar;
    codec = avcodec_find_decoder(codec_params->codec_id);
    
    codec_ctx = avcodec_alloc_context3(codec);
    
    avcodec_parameters_to_context(codec_ctx, codec_params);
    avcodec_open2(codec_ctx, codec, NULL);

    printf("=== Информация о видео ===\n");
    printf("Формат: %s\n", format_ctx->iformat->name);
    printf("Длительность: %ld секунд\n", format_ctx->duration / AV_TIME_BASE);
    printf("Разрешение: %dx%d\n", codec_ctx->width, codec_ctx->height);
    printf("Битрейт: %ld\n", format_ctx->bit_rate);
    printf("Кодек: %s\n", codec->name);
    printf("Пиксельный формат: %s\n", av_get_pix_fmt_name(codec_ctx->pix_fmt));
    printf("==========================\n\n");
    
    packet = av_packet_alloc();
    frame = av_frame_alloc();

    last_frame = (char*)calloc(codec_ctx->width * codec_ctx->height, sizeof(char));
    while (av_read_frame(format_ctx, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            int send_ret = avcodec_send_packet(codec_ctx, packet);

            
            while (send_ret >= 0) {
                send_ret = avcodec_receive_frame(codec_ctx, frame);
                if (send_ret == AVERROR(EAGAIN) || send_ret == AVERROR_EOF) {
                    break;
                }     
                if(frame_count % 1 == 0)
                {
                    process_frame(frame, frame_count, asm_file);
                    fprintf(asm_file, "DRAW\n");
                }
                frame_count++;
                
                if (frame_count >= MAX_FRAMES) {
                    printf("\nОбработано %d кадров (сработало ограничение)\n", frame_count);
                    goto cleanup;
                }
            }
        }
        av_packet_unref(packet);
    }

cleanup:
    if (packet) av_packet_free(&packet);
    if (frame) av_frame_free(&frame);
    if (codec_ctx) avcodec_free_context(&codec_ctx);
    if (format_ctx) avformat_close_input(&format_ctx);
    fprintf(asm_file, "HLT\n");
    printf("\nОбработка завершена. Всего обработано кадров: %d\n", frame_count);
    
    return 0;
}
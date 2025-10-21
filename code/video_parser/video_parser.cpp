extern "C" 
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include "../constants.h"

const int DEFAULT_VIDEO_STREAM = 0;

const char* MP4_NAME = "files/ba.mp4";
const char* ASM_NAME = "files/code.asm";

const size_t MAX_FRAMES = 10000;

char* last_frame = NULL;

static void process_frame(AVFrame*, FILE*);

static void process_frame(AVFrame* frame, FILE* asm_file) 
{
    for (int y = 0; y < frame->height-2; y++) 
    {
        for (int x = 0; x < frame->width; x++) 
        {
            int y_idx = y * frame->linesize[0] + x;

            uint8_t Y = frame->data[0][y_idx];
            int brightness = (int)Y;
            char pixel = (brightness > 150) ? '@' : ' ';
            int index = y * 80 + x;
            
            if (last_frame[index] != pixel) 
            {
                fprintf(asm_file, 
                    "PUSH %d\n"
                    "POPR SR1\n"
                    "PUSH %d\n"
                    "POPM [SR1]\n", index, pixel);
                last_frame[index] = pixel;
            }
        }
    }
}

int main() 
{
    last_frame = (char*)calloc(RAM_SIZE, sizeof(char));

    AVFormatContext* fmt_ctx = NULL;
    avformat_open_input(&fmt_ctx, MP4_NAME, NULL, NULL);

    avformat_find_stream_info(fmt_ctx, NULL);

    AVCodecParameters* codec_par = fmt_ctx->streams[DEFAULT_VIDEO_STREAM]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codec_par->codec_id);
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codec_par);
    avcodec_open2(codec_ctx, codec, NULL);
    
    FILE* asm_file = fopen(ASM_NAME, "w");
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    size_t frame_count = 0;
    
    while (av_read_frame(fmt_ctx, packet) >= 0 && frame_count < MAX_FRAMES) 
    {
        if (packet->stream_index == DEFAULT_VIDEO_STREAM) 
        {
            avcodec_send_packet(codec_ctx, packet);
            if (avcodec_receive_frame(codec_ctx, frame) == 0) 
            {
                process_frame(frame, asm_file);
                fprintf(asm_file, "DRAW\n");
                frame_count++;
            }
        }
        av_packet_unref(packet);
    }
    
    fprintf(asm_file, "HLT\n");
    fclose(asm_file);
    
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    
    free(last_frame);

    printf("FRAMES PROCESSED: %lu\n", frame_count);
    return 0;
}
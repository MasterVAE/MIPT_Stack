#include <stdio.h>
#include <stdlib.h>
#include <png.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} Pixel;

const char* PNG_NAME = "files/ass.png";
const char* ASM_NAME = "files/code.asm";
int main()
{
    FILE *fp = fopen(PNG_NAME, "rb");
    FILE *asm_file = fopen(ASM_NAME, "w+");
    if (!fp || !asm_file) {
        printf("ERROR: opening file %s\n", PNG_NAME);
        return 1;
    }
    png_byte sig[8];
    fread(sig, 1, 8, fp);
    if (!png_check_sig(sig, 8)) {
        fclose(fp);
        printf("ERROR: not png\n");
        return 1;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        printf("ERROR: creating png_struct\n");
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        printf("ERROR: creating info_struct\n");
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        printf("ERROR: reading PNG\n");
        return 1;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    printf("IMAGE SIZE: %dx%d\n", width, height);
    printf("COLOR TYPE: %d, COLOR DEPTH: %d\n", color_type, bit_depth);

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    size_t bytes_per_pixel = 4; // RGBA
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * bytes_per_pixel]);
            
            unsigned char red = px[0]/16;
            unsigned char green = px[1]/16;
            unsigned char blue = px[2]/16;
            unsigned char alpha = px[3]/16;

            if(alpha > 0)   fprintf(asm_file,\
                            "PUSH %d\n"\
                            "POPR SR1\n"\
                            "PUSH 35\n"\
                            "POPM [SR1]\n", y * 80 + x);

            else            fprintf(asm_file,\
                            "PUSH %d\n"\
                            "POPR SR1\n"\
                            "PUSH 95\n"\
                            "POPM [SR1]\n", y * 80 + x);
            //printf("PIXEL [%2d,%2d]: R=%2d, G=%2d, B=%2d, A=%2d\n", 
//                   x, y, red, green, blue, alpha);

        }
    }

    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}
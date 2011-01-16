#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#define READ_MOUSE 8
typedef unsigned int u32_t;
typedef unsigned char u8_t;
typedef char s8_t;

typedef struct{
    int w;
    int h;
    int bpp;
    u8_t *mem;
}fb_info;

typedef struct{
    int x;
    int y;
}Dot;

typedef struct{
    int x;
    int y;
    int z;
    int button;
}mevent_t;
/**//***************basic_shape.c********************/
/*extern void drawretangle(fb_info fb,Dot start,Dot end,u32_t color);*/
/*extern int fb_line(fb_info fb,int x1,int y1,int x2,int y2,u32_t color);*/

/**//***************draw_board.c********************/
/*extern void print_board(int horizen,int vertical,int blank,int offset,u32_t color);*/
/**//***************mice.c********************/
/*extern int mouse_test(fb_info fb);*/

/*//extern void print_board(fb_info fb,int horizen,int vertical,int blank,int offset,int offset_y,u32_t color);*/
#endif

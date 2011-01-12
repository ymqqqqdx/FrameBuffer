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

typedef unsigned int u32_t;
typedef unsigned char u8_t;

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
#endif

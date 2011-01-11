#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct{
    int w;
    int h;
    int bpp;
    u8 *fbmem;
}fb_info;

#endif

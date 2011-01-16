#include "common.h"
extern int fb_line(fb_info fb,int x1,int y1,int x2,int y2,u32_t color);

void print_board(fb_info fb,int horizen,int vertical,int blank,int offset,int offset_y,u32_t color)
{
    int i;
    for(i = 0; i <= horizen;i++)
        fb_line(fb, offset, i * blank + offset_y, offset + blank * vertical, i * blank + offset_y, color);
    for(i = 0; i <= vertical;i++)
        fb_line(fb, offset + i * blank, offset_y, offset + i * blank, blank * horizen + offset_y, color);
}

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "common.h"
#include <fcntl.h>
#define FB_DEVICE "/dev/fb0"
extern unsigned char kkkk[172808];
int init_fb(fb_info *fb_inf)
{
    int fd;
    fd = open(FB_DEVICE,O_RDWR);
    if(fd < 0)
    {
        fprintf(stderr, "open file %s:%s\n",FB_DEVICE,strerror(errno));
        return -1;
    }
    fprintf(stdout, "open %s success.\n",FB_DEVICE);
    struct fb_var_screeninfo fb_var;
    if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var) < 0)
    {
        fprintf(stderr, "Error ioctl: %s\n",strerror(errno));
        return -1;
    }
    fb_inf->w = fb_var.xres;
    fb_inf->h = fb_var.yres;
    fb_inf->bpp = fb_var.bits_per_pixel;
    printf("width:%d\nhight:%d\nbpp:%d\n",fb_inf->w,fb_inf->h,fb_inf->bpp);
    fb_inf->fbmem = mmap(NULL,((fb_inf->w)*(fb_inf->h)*(fb_inf->bpp)/8),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(fb_inf->fbmem == MAP_FAILED)
    {
        fprintf(stderr, "Error mmap:%s\n",strerror(errno));
        return -1;
    }
    printf("Map success!\n");
    return 0;
}
void dis_px(fb_info *fb_inf, int x, int y, unsigned char *k)
{
    int i;
    for (i = 0; i < 4; i++) 
}
int main(int argc, const char *argv[])
{
    int i, j;
    fb_info fb_inf;
    init_fb(&fb_inf);
    for(i = 0;i < 1366;i++)
        for(j = 0;j < 768; j++)
            dis_px(&fb_inf,i,j,kkkk);
    return 0;
}

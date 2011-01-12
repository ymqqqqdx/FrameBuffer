#include "common.h"
#include <math.h>
extern unsigned char kkkk[3145736];
extern unsigned char road[2359304];
void create_fb(fb_info *fb_v)
{
    int fd;
    u32_t *p = NULL;
    fd = open("/dev/fb0",O_RDWR);
    if(fd < 0)
    {
        perror("Open Error.");
        exit(1);
    }
    struct fb_var_screeninfo fb_var;
    if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var) < 0)
    {
        perror("ioctl error.");
        exit(1);
    }
    fb_v->w = fb_var.xres;
    fb_v->h = fb_var.yres;
    fb_v->bpp = fb_var.bits_per_pixel;
    printf("w = %d\nh = %d\nbpp = %d\n",fb_v->w,fb_v->h,fb_v->bpp);
    fb_v->mem = mmap(NULL, fb_v->w * fb_v->h * fb_v->bpp / 8, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        perror("map error");
        exit(1);
    }
    close(fd);
}

void draw_pix(fb_info fb,int x,int y, u32_t color)
{
    u32_t *p;
    p = (u32_t *)(fb.mem + 1408 * y * 4 + x * 4 );
    *p = color;
}
void draw_pic(fb_info fb,int x,int y, u8_t *kk)
{
    int i,j,k;
    u32_t temp;
    for(i = 0;i < 550;i++)
    {
        for(j = 0; j < 1024;j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * i * 4 + j * 4 + k];
            }
            draw_pix(fb,x + j,y + i,temp);
        }
        usleep(4000);
    }
}
void draw_pic_2(fb_info fb,int x,int y, u8_t *kk)
{
    int i,j,k;
    u32_t temp;
    for(i = 0;i < 1024;i++)
    {
        for(j = 0; j < 550;j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb,x + i,y + j,temp);
        }
        usleep(4000);
    }
}
void draw_pic_3(fb_info fb,int x,int y, u8_t *kk)
{
    int i,j,k,l;
    u32_t temp;
    for(i = 0;i < 110;i++)
    {
        for(l = 0; l < 5; l++)
        {
            for(j = 0; j < 1024;j++)
            {
                temp = 0;
                for(k = 0;k < 4;k++)
                {
                    temp <<= 8;
                    temp |= kk[1024 * (i + 110 * l) * 4 + j * 4 + k];
                }
                draw_pix(fb,x + j,y + i + l * 110,temp);
            }
            usleep(4000);
        }
    }
}
void circle(fb_info fb,Dot mid,int radis,u8_t *kk)
{
    int i,j,k;
    u32_t temp;
    for(i = mid.x - radis;i < mid.x + radis;i++)
    {
        for(j = mid.y - sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j < mid.y + sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
        usleep(1000);
    }
}
void circle2(fb_info fb,Dot mid,int radis,int r0,u8_t *kk)
{
    int i,j,k;
    u32_t temp;
    for(i = mid.x - radis;i < mid.x + radis;i++)
    {
        for(j = mid.y - sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j < mid.y + sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
        usleep(10);
    }
}
void drawretangle(fb_info fb,Dot start,Dot end,u32_t color)
{
    int i,j;
    for(i = start.x; i < end.x; i++)
    {
        for(j = start.y; j < end.y; j++)
        {
            draw_pix(fb,i,j,color);
        }
    }
}
void drawline(fb_info fb,Dot start,Dot end,u32_t color)
{
    int i,j;
    if((end.x - start.x) > (end.y - start.y))
        for(i = start.x; i <= end.x; i++)
        {
            j = start.y + (end.y - start.y) * i / (end.x - start.x);
                draw_pix(fb,i,j,color);
        }
    else
        for(i = start.y; i <= end.y; i++)
        {
            j = start.x + (end.x - start.x) * i / (end.y - start.y);
                draw_pix(fb,j,i,color);
        }
}
int fb_line(fb_info fb,int x1,int y1,int x2,int y2, u32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int inc = (dx * dy > 0) ? 1 : -1;
    int p = 2 * dy - dx;
    dy = abs(dy);
    while(x1 <= x2)
    {
        draw_pix(fb,x1,y1,color);
        x1++;
        if(p < 0)
            p += 2 * dy;
        else
        {
            p += 2 * (dy - dx);
            y1 += inc;
        }
    }
}
int main(int argc, const char *argv[])
{
    fb_info fb_v;
    int i,j;
    Dot mid;
    mid.x = 512;
    mid.y = 275;
    create_fb(&fb_v);
    fb_line(fb_v,0,0,800,0,255);
    fb_line(fb_v,0,600,800,600,255);
    fb_line(fb_v,0,0,800,600,255);
    fb_line(fb_v,0,600,800,0,255);
    //circle(fb_v,mid,i,road);
    //system("clear");
    //draw_pic(fb_v,0,0,road);
    //system("clear");
    //draw_pic_2(fb_v,0,0,road);
    //system("clear");
    //draw_pic_3(fb_v,0,0,road);
    
    return 0;
}

#include "common.h"
#include <math.h>
extern unsigned char road[2359304];
extern unsigned char cursor_16_25[1608];
extern unsigned char gImage_chessboard[2831000];
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

void draw_pic(fb_info fb,int x,int y, int sizex, int sizey, u8_t *kk)
{
    int i,j,k;
    u32_t temp;
    for(i = 0;i < sizey;i++)
    {
        for(j = 0; j < sizex;j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[950 * i * 4 + j * 4 + k];
            }
            draw_pix(fb,x + j,y + i,temp);
        }
        //usleep(4000);
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
    for(i = mid.x - radis;i < mid.x - r0;i++)
    {
        for(j = mid.y - sqrt((radis * radis) - (i - mid.x) * (i - mid.x)) ; j < mid.y + sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
    }
    for(i = mid.x + r0;i < mid.x + radis;i++)
    {
        for(j = mid.y - sqrt((radis * radis) - (i - mid.x) * (i - mid.x)) ; j < mid.y + sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
    }
    for(i = mid.x - r0; i < mid.x + r0; i++)
    {
        for(j = mid.y - sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j <= mid.y - sqrt((r0 * r0) - (i - mid.x) * (i - mid.x)); j++) 
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
        for(j = mid.y + sqrt((radis * radis) - (i - mid.x) * (i - mid.x)); j >= mid.y + sqrt((r0 * r0) - (i - mid.x) * (i - mid.x)); j--) 
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= kk[1024 * j * 4 + i * 4 + k];
            }
            draw_pix(fb, i, j,temp);
        }
    }
}
void draw_piece(fb_info fb,int x,int y,int r,u32_t color)
{
    int i,j;
    for(i = x - r; i < x + r; i++)
    {
        for(j = y - sqrt(r * r - (i - x) * (i - x)); j < y + sqrt(r * r - (i - x) * (i - x)); j++)
            draw_pix(fb, i, j, color);
    }
}
extern void mouse_test(fb_info);
int main(int argc, const char *argv[])
{
    fb_info fb_v;
	pthread_t id;
    int i,j;
    int fd;
	int ret;
    Dot mid;
    mid.x = 512;
    mid.y = 275;
    create_fb(&fb_v);
    system("clear");
    draw_pic(fb_v,400,0,940,720,gImage_chessboard);
    draw_pic(fb_v,300,200,100,100,gImage_chessboard);
    draw_pic(fb_v,300,500,100,100,gImage_chessboard);
    draw_piece(fb_v,350,250,40,0x00000000);
    draw_piece(fb_v,350,550,40,0xffffffff);
    print_board(fb_v,23,30,30,420,15,0x00000000);
    mouse_test(fb_v);
    if(ret = pthread_create(&id, NULL, (void *) mouse_test,&fb_v))
    {
        printf("Create pthread error!\n");
        exit(1);
    }
    udp_server(fb_v);

    //for(i = 1;i < 275;i++)
    //{
        //circle2(fb_v,mid,i+1,i,road);
        //usleep(100000);
    //}
    //system("clear");
    //draw_pic(fb_v,0,0,road);
    //system("clear");
    //draw_pic_2(fb_v,0,0,road);
    //system("clear");
    //draw_pic_3(fb_v,0,0,road);
    
    return 0;
}

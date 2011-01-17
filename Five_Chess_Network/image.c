#include "common.h"
#include <math.h>
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

#include "common.h"
void draw_pix(fb_info fb,int x,int y, u32_t color)
{
    u32_t *p;
    p = (u32_t *)(fb.mem + 1408 * y * 4 + x * 4 );
    *p = color;
}
void swap(int *a,int *b)
{
    int temp = *a;
    *a = *b; *b = temp;
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
int fb_line(fb_info fb,int x1,int y1,int x2,int y2,u32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int p;
    int inc = (dx * dy >= 0) ? 1 : -1;
    if(abs(dx) > abs(dy))
    {
        if(dx < 0)
        {
            swap(&x1,&x2); swap(&y1,&y2);
            dx = -dx; dy = -dy;
        }
        dy = (dy > 0) ? dy : -dy;
        p = 2 * dy - dx;
        while(x1++ <= x2)
        {
            draw_pix(fb,x1-1,y1,color);
            if(p < 0)   p += 2 * dy;
            else
            {
                y1 += inc;
                p += 2 * (dy - dx);
            }
        }
    }
    else
    {
        if(dy < 0)
        {
            swap(&x1,&x2); swap(&y1,&y2);
            dx = -dx; dy = -dy;
        }
        dy = (dy > 0) ? dy : -dy;
        p = 2 * dx - dy;
        while(y1++ < y2)
        {
            draw_pix(fb,x1,y1 - 1,color);
            if(p < 0)   p += 2 * dx;
            else
            {
                x1 += inc;
                p += 2 * (dx - dy);
            }
        }
    }
    return 0;
}
int fb_circle(fb_info fb,int x0, int y0, int r, u32_t color)
{
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;
    while(x <= y)
    {
        draw_pix(fb,x0 + x, y0 + y, color);
        draw_pix(fb,x0 + y, y0 + x, color);
        draw_pix(fb,x0 + x, y0 - y, color);
        draw_pix(fb,x0 + y, y0 - x, color);
        draw_pix(fb,x0 - x, y0 + y, color);
        draw_pix(fb,x0 - y, y0 + x, color);
        draw_pix(fb,x0 - x, y0 - y, color);
        draw_pix(fb,x0 - y, y0 - x, color);

        fb_line(fb, x0 + x, y0 + y, x0 - x, y0 + y, color);
        fb_line(fb, x0 + y, y0 + x, x0 - y, y0 + x, color);
        fb_line(fb, x0 + y, y0 - x, x0 - y, y0 - x, color);
        fb_line(fb, x0 + x, y0 - y, x0 - x, y0 - y, color);
        x++;
        if(p < 0)
            p += 4 * x + 6;
        else
        {
            p += 4 * (x - y) + 10;
            y--;
        }
    }
}

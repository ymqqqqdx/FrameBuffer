#include "common.h"
extern unsigned char cursor_16_25[1608];
unsigned char cursor_save[1608];
u32_t cs[400];
int mouse_open(const char *mdev)
{
    if(mdev == NULL)
        mdev = "/dev/input/mice";
    return (open(mdev,O_RDWR | O_NONBLOCK));
}

int mouse_parse(int fd, mevent_t * mevent)
{
    s8_t buf[READ_MOUSE];
    int n;
    if((n = read(fd, buf, READ_MOUSE)) > 0)
    {
        mevent->button = buf[0] & 0x07;
        mevent->x = buf[1];
        mevent->y = -buf[2];
        mevent->z = buf[3];
    }
    else
        return -1;
    return 0;
}
void save_cursor(fb_info fb, int x, int y, char * cur)
{
    int i, j, k;
    for(j = 0; j < 25; j++)
    {
        for(i = 0; i < 16; i++)
        {
            for(k = 0; k < 4; k++)
            {
                cur[16 * j * 4 + i * 4 + k] = fb.mem[((y + j) * 1408 + x + i ) * 4 + k];                
            }
        }
    }
}
void draw_cursor(fb_info fb, int x, int y, char * cur)
{
    int i,j,k;
    u32_t temp;
    for(i = 0; i < 25; i++)
    {
        for(j = 0; j < 16; j++)
        {
            temp = 0;
            for(k = 0;k < 4;k++)
            {
                temp <<= 8;
                temp |= cur[16 * i * 4 + j * 4 + k];
            }
            if(temp != 0xffffff80)
                draw_pix(fb,x + j,y + i,temp);
        }
    }
}
void restore_cursor(fb_info fb, int x, int y, char * cur)
{
    int i, j, k;
    u32_t temp;
    for(j = 0; j < 25; j++)
    {
        for(i = 0; i < 16; i++)
        {
            temp = 0;
            //for(k = 0;k < 4; k++)
            //{
                temp <<= 8;
                temp |= cur[16 * j * 4 + i * 4 + 0];
                temp <<= 8;
                temp |= cur[16 * j * 4 + i * 4 + 1];
                temp <<= 8;
                temp |= cur[16 * j * 4 + i * 4 + 2];
                temp <<= 8;
                temp |= cur[16 * j * 4 + i * 4 + 3];
                //}
            draw_pix(fb,x + i,y + j,temp);
        }
    }
}
int mouse_test(fb_info fb)
{
    int fd;
    int xx = 123, yy = 234;
    if((fd = mouse_open("/dev/input/mice")) < 0)
    {
        perror("mouse_open");
        exit(1);
    }
    mevent_t mevent;
    u8_t buf[] = {0xf3,0xc8,0xf3,0x64,0xf3,0x50};
    if(write(fd, buf, sizeof(buf)) < sizeof(buf))
    {
        perror("mouse_write");
        fprintf(stderr, "Error write to mice device\n");
    }
    save_cursor(fb,xx,yy,cursor_save);
    while(1)
    {
        if(mouse_parse(fd, &mevent) == 0)
        {
//            printf("dx = %d\tdy = %d\tdz = %d\tbutton = %d\n",mevent.x,mevent.y,mevent.z,mevent.button);
            restore_cursor(fb,xx,yy,cursor_save);
            xx += mevent.x;
            yy += mevent.y;
            if(xx > 1366) xx = 1366;
            if(xx < 0)    xx = 0;
            if(yy > 721)  yy = 721;
            if(yy < 0)    yy = 0;
            save_cursor(fb,xx,yy,cursor_save);
            draw_cursor(fb,xx,yy,cursor_16_25);
        }
        usleep(100);
    }
}
#include "common.h"
extern unsigned char cursor_16_25[1608];
unsigned char cursor_save[1608];
char board[23 * 30] = {0};
char who = 1;
int mouse_open(const char *mdev)
{
    if(mdev == NULL)
        mdev = "/dev/input/mice";
    return (open(mdev,O_RDWR | O_NONBLOCK));
}

int mouse_parse(int fd, mevent_t * mevent)
{
    s8_t buf[READ_MOUSE] = {0};
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
        for(i = 0; i < 16; i++)
            for(k = 0; k < 4; k++)
                cur[16 * j * 4 + i * 4 + k] = fb.mem[((y + j) * 1408 + x + i ) * 4 + k];                
}
void restore_cursor(fb_info fb, int x, int y, char * cur)
{
    int i, j, k;
    for(j = 0; j < 25; j++)
        for(i = 0; i < 16; i++)
            for(k = 0; k < 4; k++)
                fb.mem[((y + j) * 1408 + x + i ) * 4 + k] = cur[16 * j * 4 + i * 4 + k];                
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
int chess_count(int xx, int yy)
{
    int i = (xx + 15 - 420) / 30;
    int j = yy / 30;
    board[i + j * 23] = who;
}
int check(int xx, int yy)
{
    int i = (xx + 15 - 420) / 30;
    int j = yy / 30;
    return board[i + j * 23];
}
int check_five(int x, int y)
{
    int i = 0;
    char counter = 0;
    char storage = 0;
    storage = board[x + y * 23];
    if(storage == 0)
        return 0;
    counter = 1;
    for(i = 1; i < 5; i++)
    {
        if(board[x + i + (y + i) * 23] == storage)
            counter++;
        else
            break;
    }
    if(counter == 5)
        return storage;
    return 0;
}
int check_all(void)
{
    int i = 0;
    int j = 0;

    for(i = 0; i < 30; i++)
        for(j = 0; j < 23; j++)
        {
            if(check_five(i,j))
            {
                printf("%d won\n",who);
                return 1;
            }
        }
    return 0;
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
        if(mouse_parse(fd, &mevent) == 0 && (mevent.x || mevent.y || mevent.z || mevent.button))
        {
            restore_cursor(fb,xx,yy,cursor_save);
            xx += mevent.x;
            yy += mevent.y;
            if(xx > 1366) xx = 1366;
            if(xx < 0)    xx = 0;
            if(yy > 721)  yy = 721;
            if(yy < 0)    yy = 0;
            if(mevent.button == 1 && xx >= 420 && yy <= 710 && xx <= 1320)
            {
                if(! check(xx,yy))
                {
                    draw_piece(fb,(xx + 15)/30 * 30,yy/30 * 30 + 15,13,(who - 1) ? 0x00000000 : 0xffffffff);
                    chess_count(xx, yy);
                    if(check_all())
                        exit(0);
                    printf("%d %d\n",(xx + 15 - 420) / 30, (yy) / 30);
                    who = (who - 1) ? 1 : 2;
                }
            }
            save_cursor(fb,xx,yy,cursor_save);
            draw_cursor(fb,xx,yy,cursor_16_25);
        }
        usleep(100);
    }
}

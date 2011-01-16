#include "common.h"
extern unsigned char cursor_16_25[1608];
unsigned char cursor_save[1608];
char board[23 * 30] = {0};
int who = 2;
mevent_t mevent;
int ready = 0;
extern char buffer[256];
int turn = 0;
int mouse_open(const char *mdev)
{
    if(mdev == NULL)
        mdev = "/dev/input/mice";
    return (open(mdev,O_RDWR | O_NONBLOCK));
}

int mouse_parse(int fd,mevent_t * mevent)
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
int chess_count(int whom, int xx, int yy)
{
    int i = (xx + 15 - 420) / 30;
    int j = yy / 30;
    board[i + j * 23] = whom;
}
int check(int xx, int yy)
{
    int i = (xx + 15 - 420) / 30;
    int j = yy / 30;
    return board[i + j * 23];
}
int send_to_client(int xx, int yy)
{
    int i = (xx + 15 - 420) / 30;
    int j = yy / 30;
    sprintf(buffer, "%1d %2d %2d", who, i, j);
    buffer[strlen(buffer)] = 0;
    ready = 1;
}
int check_five(fb_info fb,int x, int y)
{
    int i = 0, j = 0;
    char counter = 0;
    char storage = 0;
    char n_x[4] = {0, 1, 1, 1};
    char n_y[4] = {1, 0, 1, -1};
    char nx = x, ny = y;
    storage = board[x + y * 23];
    if(storage == 0)
        return 0;
    for(j = 0; j < 4; j++)
    {
        counter = 1;
        nx = x; ny = y;
        for(i = 1; i < 5; i++)
        {
            nx += n_x[j];
            ny += n_y[j];
            if(board[nx + ny * 23] == storage)
                counter++;
            else
                break;
        }
        if(counter == 5)
        {
            //draw_piece(fb,nx * 30 + 420,ny * 30 + 15, 13, 0x00ff0000);
            return storage;
        }
    }
    return 0;
}
int check_all(fb_info fb)
{
    int i = 0;
    int j = 0;
    int winner = 0;
    for(i = 0; i < 30; i++)
        for(j = 0; j < 23; j++)
        {
            if(winner = check_five(fb,i,j))
            {
                printf("%d won\n",winner);
                return 1;
            }
        }
    return 0;
}
void mouse_test(fb_info *fb)
{
    int fd;
    int xx = 110, yy = 220;
    if((fd = mouse_open("/dev/input/mice")) < 0)
    {
        perror("mouse_open");
        exit(1);
    }
    u8_t buf[] = {0xf3,0xc8,0xf3,0x64,0xf3,0x50};
    if(write(fd, buf, sizeof(buf)) < sizeof(buf))
    {
        perror("mouse_write");
        fprintf(stderr, "Error write to mice device\n");
    }
    save_cursor(*fb,xx,yy,cursor_save);
    while(1)
    {
        if(mouse_parse(fd, &mevent) == 0 && (mevent.x || mevent.y || mevent.z || mevent.button))
        {
            restore_cursor(*fb,xx,yy,cursor_save);
            xx += mevent.x;
            yy += mevent.y;
            if(xx > 1366) xx = 1366;
            if(xx < 0)    xx = 0;
            if(yy > 721)  yy = 721;
            if(yy < 0)    yy = 0;
            //printf("xx = %d yy = %d\n",xx,yy);
            if(mevent.button == 1 && xx < 420)
            {
                if(xx >= 300 && yy >=210 && yy <= 290)
                    who = 2;
                else if(xx >= 300 && yy >= 510 && yy <= 590)
                    who = 1;
            }
            if(mevent.button == 1 && xx >= 420 && yy <= 710 && xx <= 1320 && turn)
            {
                if(! check(xx,yy))
                {
                    draw_piece(*fb,(xx + 15)/30 * 30,yy/30 * 30 + 15,13,(who - 1) ? 0x00000000 : 0xffffffff);
                    chess_count(who, xx, yy);
                    send_to_client(xx, yy);
                    if(check_all(*fb))
                        exit(0);
                    //printf("%d %d\n",(xx + 15 - 420) / 30, (yy) / 30);
                    turn = 0;
                    //who = (who - 1) ? 1 : 2;
                }
            }
            save_cursor(*fb,xx,yy,cursor_save);
            draw_cursor(*fb,xx,yy,cursor_16_25);
        }
        usleep(1000);
    }
}

#include "common.h"
#include <math.h>
extern unsigned char cursor_16_25[1608];
extern unsigned char gImage_chessboard[2831000];
extern void draw_pix(fb_info fb,int x,int y, u32_t color);
extern void print_board(fb_info fb,int horizen,int vertical,int blank,int offset,int offset_y,u32_t color);
extern void mouse_test(fb_info *fb);
extern int udp_server(fb_info fb);
extern int udp_client(fb_info fb, char *argv);
extern int turn;
void create_fb(fb_info *fb_v)
{
    int fd;
    u32_t *p = NULL;
    fd = open("/dev/fb0",O_RDWR);
    if(fd < 0)
    {
        perror("Open");
        exit(1);
    }
    struct fb_var_screeninfo fb_var;
    if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var) < 0)
    {
        perror("ioctl");
        exit(1);
    }
    fb_v->w = fb_var.xres;
    fb_v->h = fb_var.yres;
    fb_v->bpp = fb_var.bits_per_pixel;
    printf("w = %d\nh = %d\nbpp = %d\n",fb_v->w,fb_v->h,fb_v->bpp);
    fb_v->mem = mmap(NULL, fb_v->w * fb_v->h * fb_v->bpp / 8, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED)
    {
        perror("map");
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
void draw_piece(fb_info fb,int x,int y,int r,u32_t color)
{
    int i,j;
    for(i = x - r; i < x + r; i++)
    {
        for(j = y - sqrt(r * r - (i - x) * (i - x)); j < y + sqrt(r * r - (i - x) * (i - x)); j++)
            draw_pix(fb, i, j, color);
    }
}
int startup(fb_info fb, char *ip_address)
{
    char ch;
    printf("\n********************\nWelcom to Gobang Game!\n********************\n");
    printf("1.Play as server.\n");
    printf("2.Play as client.\n");
    printf("********************\n");
    printf("Please Enter your choice:");
    ch = getchar();getchar();
    if(ch == '1')
    {
        turn = 0;
    }
    else if(ch == '2')
    {
        printf("Please input the server IP:");
        scanf("%s",ip_address);
        turn = 1;
    }
    return ch - '0';
}
int main(int argc, const char *argv[])
{
    fb_info fb_v;
    fb_info * temp;
	pthread_t id;
	int ret;
    Dot mid;
    mid.x = 512;
    mid.y = 275;
    char ip_address[20];
    int mode;
    create_fb(&fb_v);
    system("clear");
    mode = startup(fb_v, ip_address);
    draw_pic(fb_v,400,0,940,720,gImage_chessboard);
    draw_pic(fb_v,300,200,100,100,gImage_chessboard);
    draw_pic(fb_v,300,500,100,100,gImage_chessboard);
    draw_piece(fb_v,350,250,40,0x00000000);
    draw_piece(fb_v,350,550,40,0xffffffff);
    print_board(fb_v,23,30,30,420,15,0x00000000);
    temp = &fb_v;
    if((ret = pthread_create(&id, NULL, (void *) mouse_test,(void *)temp)) != 0)
    {
        printf("Create pthread error!\n");
        exit(1);
    }
    if(mode == 1)
        udp_server(fb_v);
    else
        udp_client(fb_v, ip_address);
    return 0;
}

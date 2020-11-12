
/********************************
调用了EGE图形库
*******************************/


#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 640
#define WIDTH 800
#define PLANESIZE 70
#define BULLETSIZE 30
#define E_BLOOD 3
#define P_BLOOD 20
#define SCORESIZE 40

int q=0;
int e=0;
FILE *fp;
struct player1
{
    int blood,x,y;
    PIMAGE pic;
}player;

struct
{
    int x,y;
    PIMAGE pic;
} ebullets[40],bullets[40];

struct
{
    int x,y,blood;
    PIMAGE pic;
}enemys[10];


PIMAGE background;
PIMAGE bullet;
PIMAGE enemy;
PIMAGE ebullet;
PIMAGE endbackground,helpbackground,startbackground,startbackground1;
PIMAGE hp;
PIMAGE hpnum[11];
PIMAGE score;
PIMAGE scorenum[11];
PIMAGE enemy1;
PIMAGE enemy2;
PIMAGE bullet1;
MUSIC bgmmusic,bulletsound,boom;

mouse_msg msg;//存放鼠标状态的数据类型，在graphics.h中定义的
int bulletnum=0,enemynum=0,ebulletnum=0,score1=0;

void init();//初始化
void getZoomImage(PIMAGE pimg, const char* fileName, int width, int height);//图片缩放
void putpic();//打印图片

void creatbullet(int num);//创建新的玩家子弹
void createbullet(int x,int y,int num);//创建新的敌人子弹
void createnemy(int num);//创建新的敌人

void doenemy(int num);//敌人运算逻辑
void doebullet(int num);//敌人子弹运算逻辑
void doplayer();//玩家运算逻辑
void dobullet(int num);//子弹运算逻辑

void cleanenemy(int i,int num);//清除敌人
void cleanbullet(int i,int num);//清除子弹
void cleanebullet(int i,int num);//清楚敌人子弹
void start();//开始界面
void help();//帮助界面
void endgame();//结束界面

int main()
{
    init();
    start();
	closegraph();
	return 0;
}

void creatbullet(int num)
{
    bullets[num].x=player.x;
    bullets[num].y=player.y;
    e++;
    if (e==1)
        bullets[num].pic=bullet1;
    else (bullets[num].pic=bullet);
    if (e==2)   e=-1;
}

void createbullet(int x,int y,int num)
{
    ebullets[num].x=x;
    ebullets[num].y=y;
}

void createnemy(int num)
{
    int a;
    a=rand()%LENGTH;
    enemys[num].x=a;
    enemys[num].y=0;
    enemys[num].blood=E_BLOOD;
    q++;
    if (q==0)
        enemys[num].pic=enemy;
    else if (q==1)
        enemys[num].pic=enemy1;
    else if (q==2)
    {
        enemys[num].pic=enemy2;
        q=-1;
    }
}

void doenemy(int num)
{
    int i,j;
    for (i=1;i<=num;i++)
    {
        enemys[i].y=enemys[i].y+1;
        for (j=1;j<=bulletnum;j++)
            if ((enemys[i].x>=bullets[j].x-PLANESIZE&&enemys[i].x<=bullets[j].x+BULLETSIZE)
                &&(enemys[i].y<=bullets[j].y+BULLETSIZE&&enemys[i].y>=bullets[j].y-PLANESIZE))
            {
            enemys[i].blood--;
            cleanbullet(j,bulletnum);
            break;
            }

        if (enemys[i].y>WIDTH||enemys[i].blood<=0)
        {
            DWORD  playstatus3=boom.GetPlayStatus();
            if (playstatus3==MUSIC_MODE_STOP)
            boom.Play(500);
            cleanenemy(i,num);
        }
    }
}

void dobullet(int num)
{
    int i,j;
    for (i=1;i<=num;i++)
    {
        bullets[i].y=bullets[i].y-3;
        if (bullets[i].y<=0)    cleanbullet(i,num);
    }
}

void doebullet(int num)
{
    int i,j;
    for (i=1;i<=num;i++)
    {
        ebullets[i].y=ebullets[i].y+3;
        if (ebullets[i].y>=WIDTH)   cleanebullet(i,num);
    }
}

void doplayer()
{
    int i,j;
    for (i=1;i<=ebulletnum;i++)
        if ((player.x>=ebullets[i].x-PLANESIZE&&player.x<=ebullets[i].x+BULLETSIZE)
        &&(player.y<=ebullets[i].y+BULLETSIZE&&player.y>=ebullets[i].y-PLANESIZE))
        {
            cleanebullet(i,ebulletnum);
            player.blood--;
        }
    for (i=1;i<=enemynum;i++)
        if ((player.x>=enemys[i].x-PLANESIZE&&player.x<=enemys[i].x+PLANESIZE)
            &&(player.y<=enemys[i].y+PLANESIZE&&player.y>=enemys[i].y-PLANESIZE))
    {
        cleanenemy(i,enemynum);
        player.blood=player.blood-3;
    }

    if (player.blood<=0)    endgame();
}

void cleanbullet(int i,int num)
{
    int j;
    for (j=1;j<=num-1;j++)
        bullets[j]=bullets[j+1];
    bullets[num].x=0;bullets[num].y=0;
    bulletnum--;
}

void cleanenemy(int i,int num)
{
    int j;
    for (j=i;j<=num-1;j++)
        enemys[j]=enemys[j+1];
    enemys[num].x=0;enemys[num].y=0;
    enemynum--;score1++;
}

void cleanebullet(int i,int num)
{
    int j;
    for (j=i;j<=num-1;j++)
        ebullets[j]=ebullets[j+1];
    ebullets[num].x=0;ebullets[num].y=0;
    ebulletnum--;
}

void start()
{

    fp=fopen("high_scores.txt","w+");
    for (; is_run();delay_fps(144))
    {
    cleardevice();//清空屏幕
    putimage(0,0,startbackground);
    putimage(420,586,startbackground1);
    while (mousemsg())//鼠标是否产生信息
    {
        msg=getmouse();
        if (msg.is_left()&&
            msg.x>=215&&msg.x<=425
            &&msg.y>=644&&msg.y<=691)
            {
                bulletnum=0;enemynum=0;ebulletnum=0;score1=0;
                player.blood=P_BLOOD;
                putpic();
            }
        if (msg.is_left()&&
            msg.x>=420&&msg.x<=476&&
            msg.y>=586&&msg.y<=636)
        {
            help();
        }

    }
    }
}
void help()
{
    for (; is_run(); delay_fps(144))
    {
    cleardevice();
    putimage(0,0,helpbackground);
    while (mousemsg())
    {
        msg=getmouse();
        if (msg.is_left()&&
            msg.x>=208&&msg.x<=416
            &&msg.y>=480&&msg.y<=600)
            {
                start();
            }
    }
    }
}
void endgame()
{
    int score5;
    fscanf(fp,"%d",score5);
    if (score5<score1)
        fprintf(fp,"%d",score1);
    fclose(fp);

    for (; is_run(); delay_fps(144))
    {
    cleardevice();
    putimage(0,0,endbackground);
    while (mousemsg())
    {
        msg=getmouse();
        if (msg.is_left()&&
            msg.x>=208&&msg.x<=416
            &&msg.y>=480&&msg.y<=600)
            {
                start();
            }
    }
    }
}
void init()
{
    int i;
    char filename[30];
    initgraph(LENGTH, WIDTH);
    bgmmusic.OpenFile("sources\\sound\\bgm.mp3");
    boom.OpenFile("sources\\sound\\boom.mp3");
    bulletsound.OpenFile("sources\\sound\\bullet.mp3");

    player.pic=newimage(50,50);//graphics.h
    background=newimage(LENGTH,WIDTH);
    endbackground=newimage(LENGTH,WIDTH);
    startbackground=newimage(LENGTH,WIDTH);
    helpbackground=newimage(LENGTH,WIDTH);
    startbackground1=newimage(LENGTH,WIDTH);
    bullet1=newimage(20,20);
    enemy1=newimage(50,50);
    enemy2=newimage(50,50);
    enemy=newimage(50,50);
    bullet=newimage(20,20);
    ebullet=newimage(20,20);
    hp=newimage(50,50);
    score=newimage(50,50);
    player.blood=P_BLOOD;//初始化玩家血量


    getZoomImage(enemy1,"sources\\enemy1.jpg",PLANESIZE,PLANESIZE);
    getZoomImage(enemy2,"sources\\enemy2.jpg",PLANESIZE,PLANESIZE);
    getZoomImage(player.pic,"sources\\player.jpg",PLANESIZE,PLANESIZE);//为了防止读取文件出错，采用相对路径
    getZoomImage(bullet,"sources\\bullet.jpg",BULLETSIZE,BULLETSIZE);
    getZoomImage(bullet1,"sources\\bullet1.jpg",BULLETSIZE,BULLETSIZE);
    getZoomImage(enemy,"sources\\enemy.jpg",PLANESIZE,PLANESIZE);
    getZoomImage(ebullet,"sources\\ebullet.jpg",BULLETSIZE,BULLETSIZE);
    getZoomImage(background,"sources\\background.jpg",LENGTH,WIDTH);
    getZoomImage(endbackground,"sources\\endbackground.jpg",LENGTH,WIDTH);
    getZoomImage(startbackground,"sources\\startbackground.jpg",LENGTH,WIDTH);
    getZoomImage(helpbackground,"sources\\helpbackground.jpg",LENGTH,WIDTH);
    getZoomImage(startbackground1,"sources\\startbackground1.jpg",50,50);

    for (i=0;i<=9;i++)
    {
        hpnum[i]=newimage(50,50);
        scorenum[i]=newimage(50,50);
        sprintf(filename,"sources\\hp\\%d.jpg",i);//库文件里面
        getZoomImage(hpnum[i],filename,SCORESIZE,SCORESIZE);
        sprintf(filename,"sources\\score\\%d.jpg",i);
        getZoomImage(scorenum[i],filename,SCORESIZE,SCORESIZE);
    }

    getZoomImage(hp,"sources\\hp\\hp.jpg",SCORESIZE*2,SCORESIZE);
    getZoomImage(score,"sources\\score\\score.jpg",SCORESIZE*3,SCORESIZE);

    srand((unsigned) time(NULL));//生成随机种子


    for (i=1;i<=9;i++)
    {
        if (i%3==0)
        enemys[i].pic=enemy;
        else if (i%3==1)
        enemys[i].pic=enemy1;
        else if (i%3==2)
            enemys[i].pic=enemy2;
    }
    for (i=1;i<=19;i++)
    {
        bullets[i].pic=bullet;
        ebullets[i].pic=ebullet;
    }

}


void putpic()
{
    int count1=0,count2=0,i,j,k;
    bgmmusic.Play(3000);//第3000ms开始播放
    for (; is_run(); delay_fps(144))//最高刷新率144HZ
    {
    DWORD  playstatus=bgmmusic.GetPlayStatus();
    if (playstatus==MUSIC_MODE_STOP)    bgmmusic.Play(3000);//如果BGM播放完了

    while (mousemsg())
    {
        msg=getmouse();//获取当前鼠标信息
        player.x=msg.x-35;
        player.y=msg.y-35;
    }

    if (count1>=50) {

                    DWORD  playstatus2=bulletsound.GetPlayStatus();
                    if (playstatus2==MUSIC_MODE_STOP)
                    bulletsound.Play(800);  //播放子弹音乐
                    bulletnum++;
                    creatbullet(bulletnum);
                    for (i=1;i<=enemynum;i++)
                    {
                    ebulletnum++;
                    createbullet(enemys[i].x,enemys[i].y,ebulletnum);
                    }
                    count1=0;
                    }

        else count1++;

    if (count2>=150){enemynum++;createnemy(enemynum);count2=0;}
        else count2++;


    cleardevice();//graphc.h

    putimage(0,0,background);

    putimage(0,0,score);
    j=score1%10;k=score1/10;
    switch(k)
    {
    case(0):putimage_withalpha(NULL,scorenum[0],SCORESIZE*3,0);break;
    case(1):putimage_withalpha(NULL,scorenum[1],SCORESIZE*3,0);break;
    case(2):putimage_withalpha(NULL,scorenum[2],SCORESIZE*3,0);break;
    case(3):putimage_withalpha(NULL,scorenum[3],SCORESIZE*3,0);break;
    case(4):putimage_withalpha(NULL,scorenum[4],SCORESIZE*3,0);break;
    case(5):putimage_withalpha(NULL,scorenum[5],SCORESIZE*3,0);break;
    case(6):putimage_withalpha(NULL,scorenum[6],SCORESIZE*3,0);break;
    case(7):putimage_withalpha(NULL,scorenum[7],SCORESIZE*3,0);break;
    case(8):putimage_withalpha(NULL,scorenum[8],SCORESIZE*3,0);break;
    case(9):putimage_withalpha(NULL,scorenum[9],SCORESIZE*3,0);break;
    default: break;
    }

    switch(j)
    {
    case(0):putimage_withalpha(NULL,scorenum[0],SCORESIZE*4,0);break;
    case(1):putimage_withalpha(NULL,scorenum[1],SCORESIZE*4,0);break;
    case(2):putimage_withalpha(NULL,scorenum[2],SCORESIZE*4,0);break;
    case(3):putimage_withalpha(NULL,scorenum[3],SCORESIZE*4,0);break;
    case(4):putimage_withalpha(NULL,scorenum[4],SCORESIZE*4,0);break;
    case(5):putimage_withalpha(NULL,scorenum[5],SCORESIZE*4,0);break;
    case(6):putimage_withalpha(NULL,scorenum[6],SCORESIZE*4,0);break;
    case(7):putimage_withalpha(NULL,scorenum[7],SCORESIZE*4,0);break;
    case(8):putimage_withalpha(NULL,scorenum[8],SCORESIZE*4,0);break;
    case(9):putimage_withalpha(NULL,scorenum[9],SCORESIZE*4,0);break;
    default: break;
    }

    putimage_withalpha(NULL,hp,450,700);
    j=player.blood%10;k=player.blood/10;
    switch(k)
    {
        case(0):putimage_withalpha(NULL,hpnum[0],450+SCORESIZE*2,700);break;
        case(1):putimage_withalpha(NULL,hpnum[1],450+SCORESIZE*2,700);break;
        case(2):putimage_withalpha(NULL,hpnum[2],450+SCORESIZE*2,700);break;
        case(3):putimage_withalpha(NULL,hpnum[3],450+SCORESIZE*2,700);break;
        case(4):putimage_withalpha(NULL,hpnum[4],450+SCORESIZE*2,700);break;
        case(5):putimage_withalpha(NULL,hpnum[5],450+SCORESIZE*2,700);break;
        case(6):putimage_withalpha(NULL,hpnum[6],450+SCORESIZE*2,700);break;
        case(7):putimage_withalpha(NULL,hpnum[7],450+SCORESIZE*2,700);break;
        case(8):putimage_withalpha(NULL,hpnum[8],450+SCORESIZE*2,700);break;
        case(9):putimage_withalpha(NULL,hpnum[9],450+SCORESIZE*2,700);break;
    default: break;
    }
    switch(j)
    {
        case(0):putimage_withalpha(NULL,hpnum[0],450+SCORESIZE*3,700);break;
        case(1):putimage_withalpha(NULL,hpnum[1],450+SCORESIZE*3,700);break;
        case(2):putimage_withalpha(NULL,hpnum[2],450+SCORESIZE*3,700);break;
        case(3):putimage_withalpha(NULL,hpnum[3],450+SCORESIZE*3,700);break;
        case(4):putimage_withalpha(NULL,hpnum[4],450+SCORESIZE*3,700);break;
        case(5):putimage_withalpha(NULL,hpnum[5],450+SCORESIZE*3,700);break;
        case(6):putimage_withalpha(NULL,hpnum[6],450+SCORESIZE*3,700);break;
        case(7):putimage_withalpha(NULL,hpnum[7],450+SCORESIZE*3,700);break;
        case(8):putimage_withalpha(NULL,hpnum[8],450+SCORESIZE*3,700);break;
        case(9):putimage_withalpha(NULL,hpnum[9],450+SCORESIZE*3,700);break;
    default: break;
    }

   // if (/*msg.is_move()*/1)
    doplayer();
    putimage_withalpha(NULL,player.pic,player.x,player.y);///////////////////////////


    dobullet(bulletnum);
    for (i=1;i<=bulletnum;i++)
    {
        putimage_withalpha(NULL, bullets[i].pic, bullets[i].x,bullets[i].y);
    }

    doenemy(enemynum);
    for (i=1;i<=enemynum;i++)
    {
        putimage_withalpha(NULL,enemys[i].pic,enemys[i].x,enemys[i].y);
    }


    doebullet(ebulletnum);
    for (i=1;i<=ebulletnum;i++)
    {
        putimage_withalpha(NULL,ebullets[i].pic,ebullets[i].x,ebullets[i].y);
    }

    }
}


void getZoomImage(PIMAGE pimg, const char* fileName, int width, int height)//缩放图片
{
	PIMAGE temp = newimage();
	getimage(temp, fileName);

	if (getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);

	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
}



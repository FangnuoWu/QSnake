#include "wall.h"

#define NUMBER_AUTO_WALL 5
#define WIDTH 47
#define HEIGHT 36

Wall::Wall()
{
    map = new char*[WIDTH];
    for (int i = 0; i < WIDTH; ++i) {
        map[i] = new char[HEIGHT];
        for (int j = 0; j < HEIGHT; ++j) {
            map[i][j] = '0';
        }
    }

    map_auto = new int*[NUMBER_AUTO_WALL];
    for (int i = 0; i < NUMBER_AUTO_WALL; ++i) {
        map_auto[i] = new int[2];
        for (int j = 0; j < 2; ++j) {
            map_auto[i][j] = 0;
        }
    }
}

void Wall::createWall()
{
    //将屏幕分成九块，生成均匀墙壁，同时避免产生死角
    int BASE_X[3] = {0, 15, 30};
    int BASE_Y[3] = {0, 12, 24};

    int x, y, x_length, y_length;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {

            //-型地形
            x = BASE_X[i] + qrand()%15;
            y = BASE_Y[j] + qrand()%12;
            x_length = qrand()%4+5;
            for(int var=x; var<x+x_length && var<WIDTH; ++var)
                map[var][y] = 'w';

            //|型地形
            x = BASE_X[i] + qrand()%15;
            y = BASE_Y[j] + qrand()%12;
            y_length = qrand()%4+5;
            for(int var=y; var<y+y_length && var<HEIGHT; ++var)
                map[x][var] = 'w';
        }
    }

    //随机砖块
    for(int i=0;i<NUMBER_AUTO_WALL;++i){
        map_auto[i][0]=qrand()%WIDTH;
        map_auto[i][1]=qrand()%HEIGHT;

        //根据生成的砖块 改map内容
        map[map_auto[i][0]][map_auto[i][1]]='w';
    }
}

void Wall::updateWall(Snake *snake)
{
    for(int i=0;i<NUMBER_AUTO_WALL;++i){
        //将原先的砖块位置设置为0
        map[map_auto[i][0]][map_auto[i][1]]='0';

        //生成新的随机砖块
        map_auto[i][0]=qrand()%WIDTH;
        map_auto[i][1]=qrand()%HEIGHT;

        //如果随机生成的砖块在蛇身的位置
        bool flag=false;
        while(!flag)
        {
            flag=true;
            for(int i=snake->bodycount;i>0;--i){
                for(int j=0;j<NUMBER_AUTO_WALL;++j){
                    if(map_auto[j][0]==snake->snake[i][0] && map_auto[j][1]==snake->snake[i][1])
                    {
                        map_auto[j][0] = qrand()%WIDTH;
                        map_auto[j][1] = qrand()%HEIGHT;
                        flag=false;
                    }
                }
            }
        }
        //如果随机生成的砖块在蛇头的附近
        for(int j=0;j<NUMBER_AUTO_WALL;++j){
            if((abs(map_auto[j][0]-snake->snake[0][0])<2) && (abs(map_auto[j][1]-snake->snake[0][1])<2))
            {
                map_auto[j][0] = qrand()%WIDTH;
                map_auto[j][1] = qrand()%HEIGHT;
            }
        }

        //根据生成的砖块修改map内容
        map[map_auto[i][0]][map_auto[i][1]]='w';
    }

}

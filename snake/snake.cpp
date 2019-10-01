#include "snake.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

Snake::Snake()
{
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));
}

void Snake::createSnake(int w, int h)
{
    snake[0][0] = 1 + qrand()%(w-2);
    snake[0][1] = 1 + qrand()%(h-2);
    direction = qrand()%4;
}


void Snake::updateSnake(int food_x, int food_y)
{
    memcpy(snake_before,snake,sizeof(snake));//记录当前各节点位置到snakeBefore

    eatFood = false;

    //when the snake eat food
    if(snake[0][0]==food_x && snake[0][1]==food_y)
    {
        bodycount++;//增加蛇的长度
        eatFood = true;//记录是否改变蛇长为true
    }
    switch(direction)
    {
        case UP:
            snake[0][1]--;break;
        case DOWN:
            snake[0][1]++;break;
        case LEFT:
            snake[0][0]--;break;
        case RIGHT:
            snake[0][0]++;break;
    }

    for(int i=bodycount;i>0;--i)//更新蛇
    {
        snake[i][0]=snake_before[i-1][0];
        snake[i][1]=snake_before[i-1][1];
    }

}

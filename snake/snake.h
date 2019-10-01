#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QTime>

class Snake
{
public:
    int direction;//蛇运动方向
    int bodycount=0;//记录蛇身体长度
    int snake[2304][2];//记录当前蛇每个节点的位置
    int snake_before[2304][2];//记录前一时刻蛇每个节点的位置

    bool eatFood = false;

public slots:
    Snake();
    void createSnake(int w, int h);
    void updateSnake(int food_x, int food_y);
};

#endif // SNAKE_H

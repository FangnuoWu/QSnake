#ifndef FOOD_H
#define FOOD_H

#include "scene.h"

class Food
{
public:
    int food_x;
    int food_y;
    int food_type;
    int food_type_before;

public slots:
    Food();
    void createFood(int w, int h, Snake*snake);//经典模式生成食物
    void createFood(int w, int h, Snake*snake, char** map);//障碍模式生成食物
    void createFood(int w, int h, Snake*snakeOne, Snake* snakeTwo);//对战模式生成食物
    void updateFood(int w, int h, Snake*snake);//经典模式、对战模式更新食物
    void updateFood(int w, int h, Snake*snake, char** map);//障碍模式更新食物
};

#endif // FOOD_H

#ifndef WALL_H
#define WALL_H

#include <QString>
#include "snake.h"

class Wall
{
public:
    char **map;//记录地图上每点，有墙为'w',没有为'0'
    int **map_auto;//存放随机砖块的坐标位置

    Wall();
    void createWall();
    void updateWall(Snake *snake);
};

#endif // WALL_H

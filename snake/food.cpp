#include "food.h"

Food::Food()
{
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));
}

void Food::createFood(int w, int h, Snake*snake)
{
    do{
        food_x = 1 + qrand()%(w-2);
        food_y = 1 + qrand()%(h-2);
    }while(snake->snake[0][0]==food_x && snake->snake[0][1]==food_y);
}

void Food::createFood(int w, int h, Snake*snake, char**map)
{
    do{
        food_x = 1 + qrand()%(w-2);
        food_y = 1 + qrand()%(h-2);
    }while((snake->snake[0][0]==food_x && snake->snake[0][1]==food_y) || map[food_x][food_y]=='w');
    food_type = food_type_before = 0;
}

void Food::createFood(int w, int h, Snake*snakeOne, Snake* snakeTwo)
{
    do{
        food_x = 1 + qrand()%(w-2);
        food_y = 1 + qrand()%(h-2);
    }while((snakeOne->snake[0][0]==food_x && snakeOne->snake[0][1]==food_y) || (snakeTwo->snake[0][0]==food_x && snakeTwo->snake[0][1]==food_y));
}

void Food::updateFood(int w, int h, Snake* snake)
{
    food_x = 1 + qrand()%(w-2);
    food_y = 1 + qrand()%(h-2);

    //如果生成的食物出现在蛇身的位置，重新生成食物
    bool flag=false;
    while(!flag)
    {
        flag=true;
        for(int i=snake->bodycount;i>0;--i){

            if( (snake->snake[i][0]==food_x && snake->snake[i][1]==food_y))
            {
                food_x = 1 + qrand()%(w-2); // update food_x
                food_y = 1 + qrand()%(h-2); // update food_y
                flag=false;
            }

        }
    }
}

void Food::updateFood(int w, int h, Snake* snake, char** map)
{
    food_x = 1 + qrand()%(w-2);
    food_y = 1 + qrand()%(h-2);

    //如果生成的食物出现在蛇身的位置或者在墙的位置，重新生成食物
    bool flag = false;
    while(!flag)
    {
        flag=true;
        for(int i=snake->bodycount;i>0;--i){
            if( (snake->snake[i][0]==food_x && snake->snake[i][1]==food_y)|| map[food_x][food_y]=='w')
            {
                food_x = 1 + qrand()%(w-2);
                food_y = 1 + qrand()%(h-2);
                flag=false;
            }
        }
    }
}

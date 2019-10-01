#include "doublegamewidget.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define X 64
#define Y 36

DoubleGameWidget::DoubleGameWidget(QWidget *parent) : QWidget(parent)
{
    //Scene
    scene = new Scene;
    scene->createBasic(this);
    scene->createBattleScore(this);

    //QSrand
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

    //snake
    snakeOne = new Snake;
    snakeTwo = new Snake;
    snakeOne->createSnake(X,Y);
    snakeTwo->createSnake(X,Y);

    timer=new QTimer(this);
    this->connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //food
    food = new Food;
    food->createFood(X,Y,snakeOne,snakeTwo);
}

void DoubleGameWidget::paintEvent(QPaintEvent*){

   QPainter painter(this);

   //draw vs label
   painter.drawImage(QRectF(720,20,160,90),QImage("img/vs.jpg"));

   //draw food
   painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/apple.png"));

   //snake one
   //draw snake head
   switch(snakeOne->direction)
   {
   case UP:case DOWN:
       painter.drawImage(QRectF(25*snakeOne->snake[0][0],25*snakeOne->snake[0][1],25,25),QImage("img/head-up.jpg"));
       break;
   case LEFT:case RIGHT:
       painter.drawImage(QRectF(25*snakeOne->snake[0][0],25*snakeOne->snake[0][1],25,25),QImage("img/head-left.jpg"));
       break;
   default: break;
   }

   //draw snake body
   for(int i=1;i<=snakeOne->bodycount;++i)
       painter.drawImage(QRectF(25*snakeOne->snake[i][0],25*snakeOne->snake[i][1],25,25),QImage("img/body.jpg"));

   //snake two
   //draw snake head
   switch(snakeTwo->direction)
   {
   case UP:case DOWN:
       painter.drawImage(QRectF(25*snakeTwo->snake[0][0],25*snakeTwo->snake[0][1],25,25),QImage("img/head-up-blue.jpg"));
       break;
   case LEFT:case RIGHT:
       painter.drawImage(QRectF(25*snakeTwo->snake[0][0],25*snakeTwo->snake[0][1],25,25),QImage("img/head-left-blue.jpg"));
       break;
   default: break;
   }

   //draw snake body
   for(int i=1;i<=snakeTwo->bodycount;++i)
       painter.drawImage(QRectF(25*snakeTwo->snake[i][0],25*snakeTwo->snake[i][1],24,24),QImage("img/body-blue.jpg"));

}

void DoubleGameWidget::startbtn_click()
{
    timer->start(speed);
}

void DoubleGameWidget::stopbtn_click()
{
    timer->stop();
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT","Quit or not？",QMessageBox::Yes|QMessageBox::No))
    {
        this->releaseKeyboard();
        delete this;      
        return;
    }
    else {
        timer->start(speed);
    }
}

void DoubleGameWidget::keyPressEvent(QKeyEvent*event)
{
    switch (event->key())
    {
        case Qt::Key_A:
            if (snakeOne->direction != RIGHT || snakeOne->bodycount == 0)
                snakeOne->direction=LEFT;
            break;
        case Qt::Key_D:
            if (snakeOne->direction != LEFT || snakeOne->bodycount == 0)
                snakeOne->direction=RIGHT;
            break;
        case Qt::Key_W:
            if (snakeOne->direction != DOWN || snakeOne->bodycount == 0)
                snakeOne->direction=UP;
            break;
        case Qt::Key_S:
            if (snakeOne->direction != UP || snakeOne->bodycount == 0)
                snakeOne->direction=DOWN;
            break;
        case Qt::Key_Left:
            if (snakeTwo->direction != RIGHT || snakeTwo->bodycount == 0)
            snakeTwo->direction=LEFT;
            break;
        case Qt::Key_Right:
            if (snakeTwo->direction != LEFT || snakeTwo->bodycount == 0)
             snakeTwo->direction=RIGHT;
            break;
        case Qt::Key_Up:
            if (snakeTwo->direction != DOWN || snakeTwo->bodycount == 0)
                snakeTwo->direction=UP;
            break;
        case Qt::Key_Down:
            if (snakeTwo->direction != UP || snakeTwo->bodycount == 0)
                snakeTwo->direction=DOWN;
            break;
        case Qt::Key_Space:
            startbtn_click();
            break;
        case Qt::Key_E:
            stopbtn_click();
    }
}

void DoubleGameWidget::time_out()
{
    //update snake
    snakeOne->updateSnake(food->food_x, food->food_y);
    snakeTwo->updateSnake(food->food_x, food->food_y);

    if(checkDie())return;

    //snake one eat food
    if(snakeOne->eatFood)
    {
        food->updateFood(X,Y,snakeOne);
        food->updateFood(X,Y,snakeTwo);
        scene->scoreOne+=10;
        scene->updateBattleScore();
    }

    //snake two eat food
    if (snakeTwo->eatFood)
    {
        food->updateFood(X,Y,snakeOne);
        food->updateFood(X,Y,snakeTwo);
        scene->scoreTwo+=10;
        scene->updateBattleScore();
    }

    this->update();
}

bool DoubleGameWidget::checkDie(){
    //超出边界情况
    if(snakeOne->snake[0][0]<0 || snakeOne->snake[0][0]>=X || snakeOne->snake[0][1]<0 || snakeOne->snake[0][1]>=Y)
    {
        stopGame("Red snake runs outside!");
        return 1;
    }

    if(snakeTwo->snake[0][0]<0 || snakeTwo->snake[0][0]>=X || snakeTwo->snake[0][1]<0 || snakeTwo->snake[0][1]>=Y)
    {
        stopGame("Blue snake runs outside!");
        return 1;
    }

    //when the head meet the body
    for(int i=0;i<=snakeOne->bodycount;++i)
    {
        //i=0自己的头和头相撞忽略不计
        if(i!=0 && snakeOne->snake[i][0]==snakeOne->snake[0][0] && snakeOne->snake[i][1]==snakeOne->snake[0][1])
        {
            stopGame("Blue snake runs into itself");
            return 1;
        }
        if(snakeOne->snake[i][0]==snakeTwo->snake[0][0] && snakeOne->snake[i][1]==snakeTwo->snake[0][1])
        {
            stopGame("Blue snake hits Red one");
            return 1;
        }
    }

    //when the head meet the body
    for(int i=0;i<=snakeTwo->bodycount;++i)
    {
        //i=0自己的头和头相撞忽略不计
        if(i!=0 && snakeTwo->snake[i][0]==snakeTwo->snake[0][0] && snakeTwo->snake[i][1]==snakeTwo->snake[0][1])
        {
            stopGame("Red snake number runs into itself");
            return 1;
        }
        if(snakeTwo->snake[i][0]==snakeOne->snake[0][0] && snakeTwo->snake[i][1]==snakeOne->snake[0][1])
        {
            stopGame("Red snake hits Blue one");
            return 1;
        }
    }

    //只有头时头和头相撞的情况
    if(snakeOne->bodycount == 0 && snakeTwo->bodycount ==0){
        if(snakeOne->snake[0][0]==snakeTwo->snake_before[0][0] && snakeOne->snake[0][1]==snakeTwo->snake_before[0][1] && snakeTwo->snake[0][0]==snakeOne->snake_before[0][0] && snakeTwo->snake[0][1]==snakeOne->snake_before[0][1])
        {
            stopGame("Will run to each other");
            return 1;
        }
        else if (snakeOne->snake[0][0]==snakeTwo->snake_before[0][0] && snakeOne->snake[0][1]==snakeTwo->snake_before[0][1])
        {
            stopGame("Red snake will hit Blue one");
            return 1;
        }
        else if (snakeOne->snake[0][0]==snakeTwo->snake_before[0][0] && snakeOne->snake[0][1]==snakeTwo->snake_before[0][1])
        {
            stopGame("Blue snake will hit Red one");
            return 1;
        }
    }
    return 0;
}

void DoubleGameWidget::stopGame(QString reason){
    //将现在各节点位置更新为上一次各节点位置，结束画面显示死去时候位置
    memcpy(snakeOne->snake,snakeOne->snake_before,sizeof(snakeOne->snake_before));
    memcpy(snakeTwo->snake,snakeTwo->snake_before,sizeof(snakeTwo->snake_before));
    timer->stop();
    this->update();
    //弹出死亡原因提示
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT",reason,QMessageBox::Yes))
    {
        this->releaseKeyboard();
        delete this;
        return;
    }
}


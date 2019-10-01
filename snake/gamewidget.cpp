#include "gamewidget.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define X 48
#define Y 36

#define SPEED_SLOW 300
#define SPEED_FAST 100

GameWidget::GameWidget(QWidget *parent) : QWidget(parent)
{    
    //生成初始场景
    scene = new Scene;
    scene->createBasic(this);
    scene->createControlButtons(this);
    scene->createScoreAndLevel(this);

    //设置随机数种子
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

    timer=new QTimer(this);
    this->connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //生成初始蛇
    snake = new Snake;
    snake->createSnake(X, Y);

    //生成初始食物
    food = new Food;
    food->createFood(X,Y,snake);

}

void GameWidget::paintEvent(QPaintEvent*){

    QPainter painter(this);

    //draw side bar
    painter.drawPixmap(1200,0,400,900,QPixmap("img/side maybe.jpg"));

    //draw food
    painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/apple.png"));

    //draw snake head
    switch(snake->direction)
    {
    case UP:case DOWN:
        painter.drawImage(QRectF(25*snake->snake[0][0],25*snake->snake[0][1],24,24),QImage("img/head-up.jpg"));
        break;
    case LEFT:case RIGHT:
        painter.drawImage(QRectF(25*snake->snake[0][0],25*snake->snake[0][1],24,24),QImage("img/head-left.jpg"));
        break;
    default: break;
    }

    //draw snake body
    for(int i = 1; i <= snake->bodycount; ++i)
        painter.drawImage(QRectF(25*snake->snake[i][0],25*snake->snake[i][1],24,24),QImage("img/body.jpg"));
}

void GameWidget::leftbtn_click()
{
    if (snake->direction != RIGHT || snake->bodycount == 0)
    snake->direction=LEFT;
}

void GameWidget::rightbtn_click()
{
    if (snake->direction != LEFT || snake->bodycount == 0)
        snake->direction=RIGHT;
}

void GameWidget::upbtn_click()
{
    if (snake->direction != DOWN || snake->bodycount == 0)
        snake->direction=UP;
}

void GameWidget::downbtn_click()
{
    if (snake->direction != UP || snake->bodycount == 0)
        snake->direction=DOWN;
}

void GameWidget::startbtn_click()
{
    timer->start(speed);
    started = true; //将游戏开始设置为true
}

void GameWidget::stopbtn_click()
{
    timer->stop();
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT","Quit or not？",QMessageBox::Yes|QMessageBox::No))
    {
        delete this;
        return;
    }
    else {
        timer->start(speed);
    }
}

void GameWidget::changeSpeedbtn_click()
{
    if(started)//当游戏已经开始才能变速
    {
        speed = (speed==SPEED_SLOW)?SPEED_FAST:SPEED_SLOW;//每按一次，加速或者减速
        timer->start(speed);//更新刷新时间
    }
}

void GameWidget::keyPressEvent(QKeyEvent*event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        leftbtn_click();
        break;
    case Qt::Key_Right:
        rightbtn_click();
        break;
    case Qt::Key_Up:
        upbtn_click();
        break;
    case Qt::Key_Down:
        downbtn_click();
        break;
    case Qt::Key_Z:
        changeSpeedbtn_click();
        break;
    case Qt::Key_Space:
        startbtn_click();
        break;
    case Qt::Key_E:
        stopbtn_click();
    }
}

//时间槽函数:更新食物位置，更新蛇的位置
void GameWidget::time_out()
{
    snake->updateSnake(food->food_x, food->food_y);
    if(checkDie())return;//判断是否死亡
    if(snake->eatFood){
        food->updateFood(X,Y,snake);
        scene->updateScore();
    }
    this->update();
}

bool GameWidget::checkDie(){
    //超出边界情况
    if(snake->snake[0][0]<0 || snake->snake[0][0]>=X || snake->snake[0][1]<0 || snake->snake[0][1]>=Y)
    {
        stopGame("Snake runs outside!");
        return 1;
    }

    //when the head meet the body
    for(int i=1;i<=snake->bodycount;++i)
    {
        //i=0自己的头和头相撞忽略不计
        if(snake->snake[i][0]==snake->snake[0][0] && snake->snake[i][1]==snake->snake[0][1])
        {
            stopGame("Snake runs into itself");
            return 1;
        }
    }
    return 0;
}

void GameWidget::stopGame(QString reason){
    //将现在各节点位置更新为上一次各节点位置，结束画面显示死去时候位置
    memcpy(snake->snake,snake->snake_before,sizeof(snake->snake_before));

    //弹出死亡原因提示
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT",reason,QMessageBox::Yes))
    {
        timer->stop();
        this->update();
        this->releaseKeyboard();
        delete this;
        return;
    }
}

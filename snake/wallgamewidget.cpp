#include "wallgamewidget.h"
#include "ui_wallgamewidget.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define APPLE 0
#define BANANA 1
#define ORANGE 2
#define GRAPE 3
#define BOOM 4

#define X 48
#define Y 36
#define NUMBER_AUTO_WALL 5

#define SPEED_SLOW 300
#define SPEED_FAST 100
#define SPEED_GRAPE 150

WallGameWidget::WallGameWidget(QWidget *parent) :QWidget(parent),ui(new Ui::WallGameWidget)
{
    ui->setupUi(this);

    //set QSrand
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

    //生成初始场景
    scene = new Scene;
    scene->createBasic(this);
    scene->createControlButtons(this);
    scene->createScoreAndLevel(this);
    scene->createHint(this);//生成游戏提示

    //设置定时更新界面定时器
    timer=new QTimer(this);
    this->connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //设置倒计时定时器
    timer_count = new QTimer(this);
    this->connect(timer_count,SIGNAL(timeout()),this,SLOT(timeCountTen()));

    //随机生成初始地形
    wall = new Wall;
    wall->createWall();

    //生成初始蛇
    snake = new Snake;
    do{
        snake->createSnake(X, Y);
    }while(wall->map[snake->snake[0][0]][snake->snake[0][1]] == 'w');

    //生成初始食物
    food = new Food;
    food->createFood(X,Y,snake,wall->map);
}

WallGameWidget::~WallGameWidget()
{
    delete ui;
    db.close();
}

void WallGameWidget::paintEvent(QPaintEvent*){

    QPainter painter(this);

    //draw side bar
    painter.drawPixmap(1200,0,400,900,QPixmap("img/side maybe.jpg"));

    //draw rank list, only once
    if(!haveDrawRankList){
        //打开数据库
        if(QSqlDatabase::contains("qt_sql_default_connection"))
            db = QSqlDatabase::database("qt_sql_default_connection");
        else
            db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("snake");
        db.setHostName("127.0.0.1");

        QSqlTableModel *model = new QSqlTableModel(this,db);
        model->setTable("user");
        model->select();
        model->setHeaderData(0, Qt::Horizontal, "Name");
        model->setHeaderData(2, Qt::Horizontal, "Score");
        model->removeColumn(1);
        model->setSort(1, Qt::DescendingOrder);
        model->select();//refresh the data using the new sort order

        ui->view->setModel(model);
        ui->view->setEditTriggers(QAbstractItemView::NoEditTriggers);//make the rank list uneditable.
        ui->view->show();
        haveDrawRankList = true;
    }

    //draw hint，Because couting 10 s need to refresh the remain time continuously,setText here.
    scene->timeCount->setText(scene->text);

    //draw food.According to type.
    switch(food->food_type)
    {
        case 0: painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/apple.png"));break;
        case 1: painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/banana.png"));break;
        case 2: painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/orange.png"));break;
        case 3: painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/grape.png"));break;
        case 4: painter.drawImage(QRectF(25*food->food_x,25*food->food_y,25,25),QImage("img/boom.png"));break;
    }

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
    for(int i=1;i<=snake->bodycount;++i)
        painter.drawImage(QRectF(25*snake->snake[i][0],25*snake->snake[i][1],24,24),QImage("img/body.jpg"));

    //draw walls
    for(int i=0;i<X;++i)
        for(int j=0;j<Y;++j)
        {
            if(wall->map[i][j]=='w')
            painter.drawImage(QRectF(i*25,j*25,25,25),QImage("img/wall.jpg"));
        }

}

//Buttons. When the snake's bodycount is greater than 1,it can't have a 180 turn.
void WallGameWidget::leftbtn_click()
{
    if (snake->direction != RIGHT || snake->bodycount == 0)
        snake->direction=LEFT;
}

void WallGameWidget::rightbtn_click()
{
    if (snake->direction != LEFT || snake->bodycount == 0)
        snake->direction=RIGHT;
}

void WallGameWidget::upbtn_click()
{
    if (snake->direction != DOWN || snake->bodycount == 0)
        snake->direction=UP;
}

void WallGameWidget::downbtn_click()
{
    if (snake->direction != UP || snake->bodycount == 0)
        snake->direction=DOWN;
}

void WallGameWidget::startbtn_click()
{
    timer->start(speed);
    started=1;
}


void WallGameWidget::stopbtn_click()
{
    timer->stop();
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT","Quit or not？",QMessageBox::Yes|QMessageBox::No))
    {
        db.close();
        delete this;
        return;
    }
    else {
        timer->start(speed);
    }
}

void WallGameWidget::changeSpeedbtn_click()
{
    if(started && food->food_type_before!=GRAPE)//当游戏已经开始并且上一个吃到的不是葡萄才能变速
    {
        speed = (speed==SPEED_SLOW) ? SPEED_FAST:SPEED_SLOW;//每按一次，加速或者减速
        timer->start(speed);
    }
}

void WallGameWidget::keyPressEvent(QKeyEvent*event)
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

void WallGameWidget::time_out()
{
    snake->eatFood = false;

    snake->updateSnake(food->food_x, food->food_y);

    if(checkDie())return;//判断是否死亡

    if(snake->eatFood){
        food->updateFood(X,Y,snake,wall->map);
        updateAccordingToFoodType();
        wall->updateWall(snake);
        scene->updateScore();
    }
    if(food->food_type_before == BANANA) wall->updateWall(snake);

    this->update();
}

void WallGameWidget::updateAccordingToFoodType()
{
    /*
     * react according to the type of the food eaten.
     * APPLE: score +10.
     * BANANA: create blocks every move.
     * ORANGE: The snake can run into and out of the walls and blocks.
     * GRAPE: The snake will keep running at a faster speed until it eat a food which type is not GRAPE.
     *        Warning: the CHANGE SPEED button is invalid after eat GRAPE.
     * BOOM: The snake must eat another food in 10s.There will be a time counting at the side.
     */
    if (food->food_type_before!=GRAPE) speed_before = speed;
    switch(food->food_type)
    {
    case APPLE:
        speed=speed_before;
        timer_count->stop();
        scene->text = QString("提示:吃到苹果无特效");
        break;
    case BANANA:
        speed=speed_before;
        timer_count->stop();
        scene->text = QString("提示:吃到香蕉每次移动砖块会随机产生");
        break;
    case ORANGE:
        speed=speed_before;
        timer_count->stop();
        scene->text = QString("提示:吃到橘子可以穿墙而过");
        break;
    case GRAPE:
        speed=SPEED_GRAPE;
        timer_count->stop();
        scene->text = QString("提示:吃到葡萄保持加速");
        break;
    case BOOM:
        speed=speed_before;
        scene->timeCnt = 10;
        timer_count->start(1000);
        scene->text = QString("提示:吃到炸弹开始倒计时:10");
        break;
    default:break;
    }
    timer->start(speed);
    food->food_type_before = food->food_type;
    food->food_type = qrand()%5; // update food type
}

bool WallGameWidget::checkDie(){
    //超出边界情况
    if(snake->snake[0][0]<0 || snake->snake[0][0]>=X|| snake->snake[0][1]<0 || snake->snake[0][1]>=Y)
    {
        stopGame("Snake runs outside!");
        return 1;
    }

    //头撞上墙的情况
    if(wall->map[snake->snake[0][0]][snake->snake[0][1]]=='w' && food->food_type_before != ORANGE){
        stopGame("Snake hits wall!");
        return 1;
    }

    //头撞上身体情况
    for(int i=1;i<=snake->bodycount;++i)
    {
        if(snake->snake[i][0]==snake->snake[0][0] && snake->snake[i][1]==snake->snake[0][1])
        {
            stopGame("Snake runs into itself!");
            return 1;
        }
    }

    //超时情况
    if(scene->timeCnt == 0){
        stopGame("Out of 10 s!");
        return 1;
    }

    return 0;
}

void WallGameWidget::timeCountTen()
{
    scene->timeCnt--;
    scene->text = QString("提示:吃到炸弹开始倒计时:") + QString::number(scene->timeCnt);
}

void WallGameWidget::stopGame(QString reason){
    //将现在各节点位置设置为上一次各节点位置,让结束画面可以显示死去时候位置
    memcpy(snake->snake,snake->snake_before,sizeof(snake->snake_before));

    updateScoreInDB();//更新数据库中的分数

    //弹出死亡原因提示
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT",reason,QMessageBox::Yes))
    {
        timer->stop();
        this->update();
        this->releaseKeyboard();
        db.close();
        delete this;
        return;
    }
}

void WallGameWidget::updateScoreInDB()
{
    QSqlQuery query(db);

    //store this user's highest score before in scoreBefore.
    query.prepare("select score from user where name = ?");
    query.addBindValue(user);
    query.exec();
    query.next();
    int scoreBefore = query.value(0).toInt();

    //If the user break the record, update the score.
    if(scene->score>scoreBefore)
    {
        query.prepare("update user set score=? where name=?");
        query.addBindValue(scene->score);
        query.addBindValue(user);
        query.exec();
    }
}

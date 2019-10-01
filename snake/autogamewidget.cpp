#include "autogamewidget.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define X 5
#define Y 5

static int index[4][2]={{0,1},{0,-1},{-1,0},{1,0}};

AutoGameWidget::AutoGameWidget(QWidget *parent) : QWidget(parent)
{
    //设置界面
    scene = new Scene;
    scene->createBasic(this);

    //设置随机数种子
    qsrand(unsigned(QTime(0,0,0).secsTo(QTime::currentTime())));

    //设置定时更新
    timer=new QTimer(this);
    this->connect(timer,SIGNAL(timeout()),this,SLOT(time_out()));

    //snake
    snake[0][0] = 1+qrand()%(X-2);
    snake[0][1] = 1+qrand()%(Y-2);//排除第一次生成在边框update直接撞死的情况
    direction = qrand()%4+1;
    bodycount = 0;

    //food
    do{
        food_x = qrand()%X;
        food_y = qrand()%Y;
    }while(food_x == snake[0][0] && food_y == snake[0][1]);

    //map
    for(int i=0;i<X;++i){
        QVector<Node> tmp;
        for(int j=0;j<Y;++j){
            tmp.push_back(Node(i,j));
        }
        map.push_back(tmp);
    }
    map[snake[0][0]][snake[0][1]].isSnake=true;
}

void AutoGameWidget::paintEvent(QPaintEvent*){

    QPainter painter(this);

    //draw wall
    painter.setPen(QPen(Qt::black));
    painter.drawLine((1600-X*25)/2,(900-Y*25)/2,(1600-X*25)/2+X*25,(900-Y*25)/2);
    painter.drawLine((1600-X*25)/2+X*25,(900-Y*25)/2,(1600-X*25)/2+X*25,(900-Y*25)/2+Y*25);
    painter.drawLine((1600-X*25)/2,(900-Y*25)/2,(1600-X*25)/2,(900-Y*25)/2+Y*25);
    painter.drawLine((1600-X*25)/2,(900-Y*25)/2+Y*25,(1600-X*25)/2+X*25,(900-Y*25)/2+Y*25);

    //draw food
    if(!GameOver)painter.drawImage(QRectF((1600-X*25)/2+25*food_x,(900-Y*25)/2+25*food_y,25,25),QImage("img/apple.png"));

    //draw snake head
    switch(direction)
    {
    case UP: case DOWN:
        painter.drawImage(QRectF((1600-X*25)/2+25*snake[0][0],(900-Y*25)/2+25*snake[0][1],23,23),QImage("img/head-up.jpg"));
        break;
    case LEFT: case RIGHT:
        painter.drawImage(QRectF((1600-X*25)/2+25*snake[0][0],(900-Y*25)/2+25*snake[0][1],23,23),QImage("img/head-left.jpg"));
        break;
    default:
        break;
    }

    //draw snake body
    for(int i=1;i<=bodycount;++i)
        painter.drawImage(QRectF((1600-X*25)/2+25*snake[i][0]+1,(900-Y*25)/2+25*snake[i][1]+1,23,23),QImage("img/body.jpg"));
}

void AutoGameWidget::startbtn_click()
{
    timer->start(speed);
}

void AutoGameWidget::stopbtn_click()
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

void AutoGameWidget::keyPressEvent(QKeyEvent*event)
{
    switch (event->key())
    {
    case Qt::Key_Space:
        startbtn_click();
        break;
    case Qt::Key_E:
        stopbtn_click();
    }
}

void AutoGameWidget::time_out()
{
    updateSnake();

    //************************判断下一次蛇怎么走*******************************
    CanFindFood = findFoodDirection();
    //如果找得到食物
    if(CanFindFood){
        CanFindTail = findTailDirection();//尝试去找尾巴
        //如果走出这一步之后还能找到自己尾巴后的空格，那么去吃食物
        if(CanFindTail){
            direction = CanFindFood;
        }
    }

    //如果找不到食物或去吃食物后找不到自己的尾巴
    if( (CanFindFood ==false) || (CanFindTail == false)){
        CanMove = wander();
        if(CanMove) direction = CanMove;
    }

    if(!CanMove){
        this->update();
        if(QMessageBox::Yes==QMessageBox::question(this,"HINT","Game Over",QMessageBox::Yes))
        {
            timer->stop();
            this->releaseKeyboard();
            delete this;
            return;
        }
    }
    this->update();
}

//蛇吃到食物，重新生成食物
void AutoGameWidget::updateFood()
{
    food_x = qrand()%X; // update food_x
    food_y = qrand()%Y; // update food_y

    //如果生成的食物出现在当前蛇身的位置，重新生成食物
    bool flag=false;
    while(!flag){
        flag=true;
        for(int i=bodycount;i>=0;--i){
            if(snake[i][0]==food_x && snake[i][1]==food_y){
                food_x = qrand()%X;
                food_y = qrand()%Y;
                flag=false;
            }
        }
    }
}

void AutoGameWidget::updateSnake()
{
    memcpy(snakeBefore,snake,sizeof(snake));//记录当前各节点位置到snakeBefore

    eatFood = false;//先认为没吃到食物

    //***************************更新食物和蛇*******************************
    switch(direction)
    {
        case UP:
            snake[0][1]++;break;
        case DOWN:
            snake[0][1]--;break;
        case LEFT:
            snake[0][0]--;break;
        case RIGHT:
            snake[0][0]++;break;
    }
    if(snake[0][0]==food_x && snake[0][1]==food_y) //如果蛇吃到食物
    {
        bodycount++;//增加蛇的长度
        eatFood = true;//记录是否改变蛇长为true
    }
    for(int i=bodycount;i>0;--i)//更新蛇
    {
        snake[i][0]=snakeBefore[i-1][0];
        snake[i][1]=snakeBefore[i-1][1];
    }
    //更新map上的记录
    if(!eatFood)map[snakeBefore[bodycount][0]][snakeBefore[bodycount][1]].isSnake = false;
    map[snake[0][0]][snake[0][1]].isSnake = true;//头置为true

    //现在再更新食物，这样食物可以生成在蛇尾留出的空格
    if(eatFood)updateFood();
}

int AutoGameWidget::findDirectionOfShortestRoad(int start_x, int start_y, int goal_x, int goal_y)
{
    bool res = true;

    OpenList.clear();

    for(int i=0;i<X;++i)
        for (int j=0;j<Y;++j){
            if(map[i][j].isInOpenList | map[i][j].isInCloseList){
                map[i][j].F=0;
                map[i][j].G=0;
                map[i][j].H=0;
                map[i][j].pre=nullptr;
                map[i][j].isInOpenList=false;
                map[i][j].isInCloseList=false;
            }
        }

    if(goal_x==start_x && goal_y==start_y){
        return direction;
    }

    map[start_x][start_y].G = 0;
    map[start_x][start_y].H = abs(goal_x-start_x)+abs(goal_y-start_y);
    map[start_x][start_y].F = map[start_x][start_y].H + map[start_x][start_y].G;
    map[start_x][start_y].isInOpenList = true;

    OpenList.push_back(map[start_x][start_y]);

    while(true){
        //每次取出F值最小的，加入Closelist
        //并将他周围可以走（不在closelist里且不是蛇身）的点加入OpenList
        int now_x = OpenList.first().x;
        int now_y = OpenList.first().y;

        OpenList.removeFirst();

        map[now_x][now_y].isInOpenList=false;
        map[now_x][now_y].isInCloseList=true;

        if(goal_x==now_x && goal_y==now_y){
            res = true;
            break;
        }

        findNeighbor(now_x, now_y,goal_x,goal_y);

        if(OpenList.empty()){
            res = false;
            break;
        }

        std::sort(OpenList.begin(),OpenList.end(),[](const Node &A,const Node &B){return A.F < B.F;});
    }

    int result=0;

    if(res){
        Node *tmp = &map[goal_x][goal_y];
        while(tmp->pre->pre) tmp=tmp->pre;
        //判断蛇头的方向
        if(tmp->x == start_x+1) result = RIGHT;
        if(tmp->x == start_x-1) result = LEFT;
        if(tmp->y == start_y+1) result = UP;
        if(tmp->y == start_y-1) result = DOWN;
    }

    return result;
}

void AutoGameWidget::findNeighbor(int x, int y, int goal_x, int goal_y)
{
    Node *current=&map[x][y];
    int count = 0;
    for (int var = qrand()%4; count < 4; var = (var+1)%4) {
        if(x+index[var][0] < X && x+index[var][0] >= 0 && y+index[var][1] < Y && y+index[var][1] >= 0){
            insertOpenList(x+index[var][0],y+index[var][1],current,goal_x, goal_y);
        }
        count++;
    }
}

void AutoGameWidget::insertOpenList(int x, int y, Node *current,int goal_x, int goal_y)
{
    if (!map[x][y].isSnake && !map[x][y].isInCloseList)//如果不是蛇身也不在CloseList中
        {
            if (map[x][y].isInOpenList)//如果已经在opentable中
            {
                if (map[x][y].G > current->G+1)//但是不是最优路径
                {
                    map[x][y].G = current->G+1;//把G值更新（变小）
                    map[x][y].pre = current;//把该邻点的双亲节点更新
                }
            }
            else //如果不在opentable中，把该点加入opentable中
            {
                map[x][y].G = current->G+1;
                map[x][y].H = abs(goal_x-x)+abs(goal_y-y);
                map[x][y].F = map[x][y].G+map[x][y].H;
                map[x][y].pre = current;
                map[x][y].isInOpenList = true;

                OpenList.push_back(map[x][y]);
            }
        }
}

int AutoGameWidget::wander(){

    int result = 0;
    DirectionFarthest.clear();
    /*
     * 特殊情况：当蛇走满了整个屏幕只剩下最后一个食物
     * 如果先实行wander中的策略1：优先考虑找一个走过后还能找到尾巴的路，那么蛇一定会永远追尾
     * 改为实现只要能吃到食物就吃食物的方案
     */
    if(bodycount == X*Y-2){
        int special = findFoodDirection();
        if(special){

            switch(special)
            {
                case UP:
                    snake[0][1]++;break;
                case DOWN:
                    snake[0][1]--;break;
                case LEFT:
                    snake[0][0]--;break;
                case RIGHT:
                    snake[0][0]++;break;
            }

            bodycount++;//增加蛇的长度

            for(int i=bodycount;i>0;--i)//更新蛇
            {
                snake[i][0]=snakeBefore[i-1][0];
                snake[i][1]=snakeBefore[i-1][1];
            }

            //设置gameover，此时不用画食物
            GameOver = true;
            this->repaint();

            return 0;
        }
    }

    /*
     * 优先考虑找一个走过后还能找到尾巴的路
     * 满足离尾巴最远,来避免追尾
     * 用List“DirectionFarthest”存放符合条件的点离尾巴的曼哈顿距离
     * 然后进行删选
     */

    int f=0;

    //更新头位置
    head_x = snake[0][0];
    head_y = snake[0][1];

    //更新尾巴位置为走后的位置
    tail_x = snake[bodycount][0];
    tail_y = snake[bodycount][1];
    map[tail_x][tail_y].isSnake = false;

    //上下左右四个方向可不可以走（满足不超过边界，不是蛇身，走后可以找到蛇尾）
    int count = 0;
    for (int var = qrand()%4; count < 4; var = (var+1)%4) {
        int x = head_x+index[var][0];
        int y = head_y+index[var][1];
        if( x < X && x >= 0 && y < Y && y >= 0 && !map[x][y].isSnake){
            if (findDirectionOfShortestRoad(x,y,tail_x,tail_y)){
                f = abs(tail_x-x) + abs(tail_y-y);
                DirectionFarthest.push_back(Point(var+1,f));
            }
        }
        count++;
    }

    std::sort(DirectionFarthest.begin(),DirectionFarthest.end(),[](const Point &A,const Point &B){return A.F > B.F;});

    if( !DirectionFarthest.empty()){
        result = DirectionFarthest.first().Direction;
    }

    /*
     * 如果找不到走过后还能找到尾巴的路
     * 上下左右选能走的，尽量离尾巴最远防止追尾
     * 用List“DirectionFarthestFromTail”存放符合条件的点离尾巴的曼哈顿距离
     * 然后进行删选
     */

    else{
        DirectionFarthest.clear();
        for (int var = qrand()%4; count < 4; var = (var+1)%4) {
            int x = head_x+index[var][0];
            int y = head_y+index[var][1];
            if( x < X && x >= 0 && y < Y && y >= 0 && !map[x][y].isSnake){
                f = abs(tail_x-x) + abs(tail_y-head_y);
                DirectionFarthest.push_back(Point(var+1,f));
            }
            count++;
        }

        std::sort(DirectionFarthest.begin(),DirectionFarthest.end(),[](const Point &A,const Point &B){return A.F > B.F;});

        if( !DirectionFarthest.empty()){
            result = DirectionFarthest.first().Direction;
        }else{
            result = 0;
        }
    }
    map[tail_x][tail_y].isSnake = true;

    return result;
}

int AutoGameWidget::findFoodDirection()
{
    //不用让蛇尾留下的空格能走，因为食物肯定不会在那
    return findDirectionOfShortestRoad(snake[0][0],snake[0][1],food_x,food_y);
}


int AutoGameWidget::findTailDirection()
{
    /*
     *判断走完吃食物的一步之后蛇还能不能找到自己的尾巴留下来的空格
     *尾巴留下来的空格为当前尾巴的位置
     *头的位置为走完这步之后头的位置，用virtual_head_x和virtual_head_y记录
     */

    //下面要用到bodycount-2，如果bodycount=0 找周围随便一格走
    if(bodycount==0){
        if(snake[0][0]+1>X/2) return LEFT;
        else return RIGHT;
    }

    //如果bodycount=1，由于第一次判断过，保持原来的方向就可以
    if(bodycount ==1){
        return direction;
    }

    //更新尾巴的位置
    tail_x = snake[bodycount][0];
    tail_y = snake[bodycount][1];
    map[tail_x][tail_y].isSnake = false;

    //更新头的位置
    int virtual_head_x = snake[0][0];
    int virtual_head_y = snake[0][1];
    switch(CanFindFood){
        case 1://UP
            virtual_head_y++;break;
        case 2://DOWN
            virtual_head_y--;break;
        case 3://LEFT
            virtual_head_x--;break;
        case 4://RIGHT
            virtual_head_x++;break;
    }

    int result = findDirectionOfShortestRoad(virtual_head_x,virtual_head_y,tail_x,tail_y);

    //将蛇尾改回来
    map[tail_x][tail_y].isSnake= true;

    return result;
}

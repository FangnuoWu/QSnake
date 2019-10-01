#ifndef AUTOGAMEWIDGET_H
#define AUTOGAMEWIDGET_H

#include "scene.h"

#include <QList>
#include <QVector>

class AutoGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AutoGameWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent*event);

private:

    QTimer *timer;
    Scene *scene;
    //speed
    int speed = 50;

    //snake
    int direction;//蛇运动方向
    int snake[2304][2];//记录当前蛇每个节点的位置
    int snakeBefore[2304][2];//记录前一时刻蛇每个节点的位置
    int bodycount = 0;//记录蛇身体长度
    bool eatFood = false;
    bool firstPaint = true;
    int head_x;
    int head_y;
    int tail_x;
    int tail_y;

    //food
    int food_x;
    int food_y;//食物坐标

    struct Point
    {
        int Direction;
        int F;
        Point(int d,int f):Direction(d),F(f){}
    };

    struct Node
    {
        int x=0,y=0;//该点坐标
        int F=0;//距离估计值,f = g + h
        int G=0;//该点到起点的距离
        int H=0;//该点到终点的曼哈顿距离
        Node* pre = nullptr;
        bool isInOpenList=false;
        bool isInCloseList=false;//是否已经访问
        bool isSnake=false;
        Node(int a=0,int b=0,int f=0):x(a),y(b),F(f){}
        Node(const Node& n)=default;
    };

    int CanFindFood;
    int CanFindTail;
    int CanMove = 1;
    bool GameOver = false;

    QList<Node> OpenList;//存放路径规划过程中待检测的节点
    QVector<QVector<Node>> map;
    QList<Point> DirectionFarthest;

signals:

public slots:
    void startbtn_click();
    void stopbtn_click();
    void time_out();
    void updateSnake();
    void updateFood();

    int findDirectionOfShortestRoad(int start_x, int start_y, int goal_x, int goal_y);
    void findNeighbor(int a, int b,int goal_x, int goal_y);
    void insertOpenList(int x, int y, Node *current,int goal_x, int goal_y);

    int findFoodDirection();
    int findTailDirection();
    int wander();
};

#endif // AUTOGAMEWIDGET_H

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "snake.h"
#include "food.h"

#define SPEED_SLOW 300
#define SPEED_FAST 100

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent*);

private:

    QTimer *timer;//QTimer
    Snake *snake;
    Scene *scene;
    Food *food;
    int speed = SPEED_SLOW;

    //start
    bool started = 0;//记录游戏有没有开始，游戏未开始某些任务不允许执行

public slots:
    //buttons
    void leftbtn_click();
    void rightbtn_click();
    void upbtn_click();
    void downbtn_click();
    void stopbtn_click();
    void startbtn_click();
    void changeSpeedbtn_click();

    void time_out();//到时更新界面
    bool checkDie();//判断是否死亡
    void stopGame(QString reason);//弹出死亡原因，结束游戏
};

#endif // GAMEWIDGET_H

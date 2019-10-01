#ifndef WALLGAMEWIDGET_H
#define WALLGAMEWIDGET_H

#include "snake.h"
#include "food.h"
#include "wall.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>

#define SPEED_SLOW 300

extern QString user;

namespace Ui {
class WallGameWidget;
}

class WallGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WallGameWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent*);
    ~WallGameWidget();

private:
    Ui::WallGameWidget *ui;

    Snake *snake;
    Scene *scene;
    Food *food;
    Wall *wall;

    //计时器
    QTimer *timer;
    QTimer *timer_count;

    //speed
    int speed = SPEED_SLOW;
    int speed_before = 0;

    //control
    bool started = false;//记录游戏有没有开始，游戏未开始某些任务不允许执行

    //排行榜
    QSqlDatabase db;
    bool haveDrawRankList = false;

public slots:

    //按钮槽函数
    void leftbtn_click();
    void rightbtn_click();
    void upbtn_click();
    void downbtn_click();
    void stopbtn_click();
    void startbtn_click();
    void changeSpeedbtn_click();

    //到时更新界面
    void time_out();
    void timeCountTen();//记录倒计10秒

    void updateScoreInDB();//更新数据库中记录的成绩
    void updateAccordingToFoodType();//更新食物位置、种类，提示
    bool checkDie();//判断是否死亡
    void stopGame(QString reason);//弹出死亡原因，结束游戏

};

#endif // WALLGAMEWIDGET_H


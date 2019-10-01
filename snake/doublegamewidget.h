#ifndef DOUBLEGAMEWIDGET_H
#define DOUBLEGAMEWIDGET_H

#include "snake.h"
#include "food.h"
#include "scene.h"

class DoubleGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DoubleGameWidget(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent*);
    void paintEvent(QPaintEvent *);

private:    
    QTimer *timer; //QTimer  
    Scene *scene;//Scene
    Food *food;//food
    
    Snake *snakeOne;
    Snake *snakeTwo;   
    
    int speed = 300;//speed

public slots:
    //buttons
    void stopbtn_click();
    void startbtn_click();

    void time_out();//到时更新
    bool checkDie();//判断是否死亡
    void stopGame(QString reason);//弹出死亡原因，结束游戏
};

#endif // DOUBLEGAMEWIDGET_H

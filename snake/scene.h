#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QIcon>
#include <QPalette>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QFont>
#include <QLineEdit>
#include <QString>
#include <QKeyEvent>

#include"snake.h"

class Scene
{
public:
    //control buttons
    QPushButton *leftbtn;
    QPushButton *rightbtn;
    QPushButton *upbtn;
    QPushButton *downbtn;
    QPushButton *startbtn;
    QPushButton *stopbtn;
    QPushButton *changeSpeedbtn;

    //score and level
    QFont font;

    QLabel *score_label;
    QLineEdit *score_lineEdit;
    int score=0;
    QLabel *level_label;
    QLineEdit *level_lineEdit;
    int level=1;

    //battle
    QLabel *scoreOne_label;
    QLabel *scoreTwo_label;
    int scoreOne=0;
    int scoreTwo=0;

    //hint of obstacle mode
    QLabel *timeCount;
    QFont time_font;
    int timeCnt = 10;
    QString text = QString("提示:");

public slots:
    Scene();
    void createBasic(QWidget* w);
    void createControlButtons(QWidget* w);
    void createScoreAndLevel(QWidget* w);
    void createBattleScore(QWidget* w);
    void createHint(QWidget* w);
    void updateScore();
    void updateBattleScore();
};

#endif // CONTROLS_H

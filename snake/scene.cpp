#include "scene.h"

Scene::Scene()
{

}

void Scene::createBasic(QWidget* w)
{
    //设置界面
    w->setAutoFillBackground(true);//覆盖原先界面
    w->resize(1600,900);//重置窗口大小
    w->setWindowIcon(QIcon("img/img.png"));
    w->setWindowTitle("Playing");

    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap("img/background.jpg")));
    w->setPalette(palette);

    startbtn=new QPushButton("start(Sp)",w);
    startbtn->setGeometry(1240,800,90,40);

    stopbtn=new QPushButton("exit(E)",w);
    stopbtn->setGeometry(1460,800,90,40);

    w->connect(startbtn,SIGNAL(clicked()),w,SLOT(startbtn_click()));
    w->connect(stopbtn,SIGNAL(clicked()),w,SLOT(stopbtn_click()));
}

void Scene::createControlButtons(QWidget* w){
    leftbtn=new QPushButton("←(←)",w);
    leftbtn->setGeometry(1250,650,100,40);

    rightbtn=new QPushButton("→(→)",w);
    rightbtn->setGeometry(1450,650,100,40);

    upbtn=new QPushButton("↑(↑)",w);
    upbtn->setGeometry(1350,600,100,40);

    downbtn=new QPushButton("↓(↓)",w);
    downbtn->setGeometry(1350,700,100,40);

    changeSpeedbtn=new QPushButton("change(Z)",w);
    changeSpeedbtn->setGeometry(1350,800,90,40);

    //建立连接
    w->connect(leftbtn,SIGNAL(clicked()),w,SLOT(leftbtn_click()));
    w->connect(rightbtn,SIGNAL(clicked()),w,SLOT(rightbtn_click()));
    w->connect(upbtn,SIGNAL(clicked()),w,SLOT(upbtn_click()));
    w->connect(downbtn,SIGNAL(clicked()),w,SLOT(downbtn_click()));
    w->connect(changeSpeedbtn,SIGNAL(clicked()),w,SLOT(changeSpeedbtn_click()));
}

void Scene::createScoreAndLevel(QWidget* w){
    //生成分数和等级显示栏
    font.setPointSize(14);

    level_label = new QLabel("level:", w);
    level_label->setFont(font);
    level_label->setGeometry (1270, 100, 200, 40);
    level_lineEdit = new QLineEdit(w);
    level_lineEdit->setFont(font);
    level_lineEdit->setText(QString::number(level));
    level_lineEdit->setAlignment (Qt::AlignRight);
    level_lineEdit->setGeometry (1350, 100, 150, 40);

    score_label = new QLabel("score:", w);
    score_label->setFont(font);
    score_label->setGeometry (1270, 50, 200, 40);
    score_lineEdit = new QLineEdit(w);
    score_lineEdit->setFont(font);
    score_lineEdit->setText(QString::number(score));
    score_lineEdit->setAlignment (Qt::AlignRight);
    score_lineEdit->setGeometry (1350, 50, 150, 40);

}

void Scene::createBattleScore(QWidget* w){
    //设置分数显示栏
    font.setPointSize(14);

    scoreOne_label = new QLabel("0", w);
    scoreOne_label->setFont(font);
    scoreOne_label->setGeometry (670, 20, 50, 90);
    scoreOne_label->setAlignment(Qt::AlignCenter);

    scoreTwo_label = new QLabel("0", w);
    scoreTwo_label->setFont(font);
    scoreTwo_label->setGeometry (880, 20, 50, 90);
    scoreTwo_label->setAlignment(Qt::AlignCenter);

}

void Scene::createHint(QWidget* w)
{
    text = QString("提示:");
    timeCount = new QLabel(text,w);
    time_font.setPointSize(10);
    timeCount->setFont(time_font);
    timeCount->setGeometry (1250, 500, 350, 20);
    timeCount->setAlignment (Qt::AlignLeft);
}

void Scene::updateScore()
{
    score+=10;
    score_lineEdit->setText(QString::number(score));
    if(score>=100) level=2;
    else if(score>=200) level=3;
    else if(score>=500) level=4;
    else if(score>=1000) level=5;
    level_lineEdit->setText(QString::number(level));
}

void Scene::updateBattleScore()
{
    scoreOne_label->setText(QString::number(scoreOne));
    scoreTwo_label->setText(QString::number(scoreTwo));
}

#include "widget.h"
#include "wallgamewidget.h"

Widget::Widget(QWidget *parent) :QWidget(parent)
{
    //设置界面
    this->setFixedSize(1600,900);
    this->setWindowIcon(QIcon("img/img.png"));
    this->setWindowTitle("Eating Snake");

    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap("img/框.jpg")));
    this->setPalette(palette);

    //单人模式、多人模式、障碍模式、自动模式、帮助和退出按钮
    solo_state=new QPushButton(this);
    solo_state->setIcon(QIcon("img/solo.png"));
    solo_state->setIconSize(QSize(400,150));
    solo_state->setGeometry(QRect(750,250,400,150));
    solo_state->setFlat(true);

    double_state=new QPushButton(this);
    double_state->setIcon(QIcon("img/double.png"));
    double_state->setIconSize(QSize(400,150));
    double_state->setGeometry(QRect(1100,250,400,150));
    double_state->setFlat(true);

    wall_state=new QPushButton(this);
    wall_state->setIcon(QIcon("img/zhangai.png"));
    wall_state->setIconSize(QSize(400,150));
    wall_state->setGeometry(QRect(750,390,400,150));
    wall_state->setFlat(true);

    auto_state=new QPushButton(this);
    auto_state->setIcon(QIcon("img/auto.png"));
    auto_state->setIconSize(QSize(400,150));
    auto_state->setGeometry(QRect(1100,390,400,150));
    auto_state->setFlat(true);

    help=new QPushButton(this);
    help->setIcon(QIcon("img/help.png"));
    help->setIconSize(QSize(400,150));
    help->setGeometry(QRect(750,530,400,150));
    help->setFlat(true);

    end=new QPushButton(this);
    end->setIcon(QIcon("img/exit.png"));
    end->setIconSize(QSize(400,150));
    end->setGeometry(QRect(1100,530,400,150));
    end->setFlat(true);

    //连接
    connect(solo_state,SIGNAL(clicked()),this,SLOT(soloClick()));
    connect(double_state,SIGNAL(clicked()),this,SLOT(doubleClick()));
    connect(wall_state,SIGNAL(clicked()),this,SLOT(wallClick()));
    connect(auto_state,SIGNAL(clicked()),this,SLOT(autoClick()));
    connect(help,SIGNAL(clicked()),this,SLOT(helpClick()));
    connect(end,SIGNAL(clicked()),this,SLOT(endClick()));

}

Widget::~Widget()
{
    delete solo_state;
    delete double_state;
    delete wall_state;
    delete auto_state;
    delete end;
    delete help;
}

void Widget::soloClick()
{
    gamewidget=new GameWidget(this);
    gamewidget->show();
    gamewidget->setFocusPolicy(Qt::StrongFocus);
    gamewidget->grabKeyboard();
}

void Widget::autoClick()
{
    autogamewidget=new AutoGameWidget(this);
    autogamewidget->show();
    autogamewidget->setFocusPolicy(Qt::StrongFocus);
    autogamewidget->grabKeyboard();
}

void Widget::wallClick()
{
    wallgamewidget=new WallGameWidget(this);
    LoginDialog login;
    if (login.exec() == QDialog::Accepted)
    {
        wallgamewidget->setFocusPolicy(Qt::StrongFocus);
        wallgamewidget->grabKeyboard();
        wallgamewidget->show();
    }
}

void Widget::doubleClick()
{
    doublegamewidget=new DoubleGameWidget(this);
    doublegamewidget->show();
    doublegamewidget->setFocusPolicy(Qt::StrongFocus);
    doublegamewidget->grabKeyboard();
}

void Widget::helpClick()
{
    helpwindow = new HelpDialog(this);
    helpwindow->show();
}

void Widget::endClick()
{
    if(QMessageBox::Yes==QMessageBox::question(this,"HINT","Quit or not？",QMessageBox::Yes|QMessageBox::No))
    {
        exit(0);
    }
}

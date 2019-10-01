#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPalette>
#include <QPushButton>
#include <QMessageBox>

#include "gamewidget.h"
#include "doublegamewidget.h"
#include "wallgamewidget.h"
#include "autogamewidget.h"
#include "logindialog.h"
#include "helpdialog.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPushButton *solo_state;
    QPushButton *double_state;
    QPushButton *wall_state;
    QPushButton *auto_state;
    QPushButton *help;
    QPushButton *end;

    GameWidget *gamewidget;//经典模式界面
    DoubleGameWidget *doublegamewidget;//对战模式界面
    WallGameWidget *wallgamewidget;//障碍模式界面
    AutoGameWidget *autogamewidget;//自动模式界面
    HelpDialog *helpwindow;//帮助文档界面

public slots:
    void soloClick();
    void doubleClick();
    void wallClick();
    void autoClick();
    void helpClick();
    void endClick();
};

#endif // WIDGET_H

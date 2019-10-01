#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    this->resize(1200,675);
    setWindowFlags (Qt::FramelessWindowHint);

    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap("img/helpw.jpg")));
    this->setPalette(palette);

    exitbtn = new QPushButton;
    exitbtn->setIcon(QIcon("img/help_exit.png"));
    exitbtn->setIconSize(QSize(68,68));
    exitbtn->setGeometry(QRect(1144,44,68,168));
    exitbtn->setFlat(true);

    connect(exitbtn,SIGNAL(clicked()),this,SLOT(exitbtn_click()));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::exitbtn_click()
{
    delete this;
    return;
}

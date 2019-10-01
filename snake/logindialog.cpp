#include "logindialog.h"
#include "ui_logindialog.h"

QString user;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QApplication::setQuitOnLastWindowClosed(false);
    connect(ui->loginbtn,SIGNAL(clicked()),this,SLOT(loginbtn_clicked()));
    connect(ui->cancelbtn,SIGNAL(clicked()),this,SLOT(cancelbtn_clicked()));
    connect(ui->createbtn,SIGNAL(clicked()),this,SLOT(createbtn_clicked()));
    openDatabase();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::openDatabase()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("snake");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("wfn0516!!");

    if(db.open()){
        QSqlQuery query(db);

        //判断表格user是否已存在，不存在则创建user
        query.exec("select count(*) from sys.tables where table='user'");
        if(!query.next())
            query.exec("create table user(name varchar(20),pwd varchar(20),score int default '0')");
        return true;
    }else{
        return false;
    }
}

void LoginDialog::cancelbtn_clicked()
{
    ui->userLineEdit->clear();
    ui->pwdLineEdit->clear();
}

void LoginDialog::loginbtn_clicked()
{
    QString username = ui->userLineEdit->text();
    QString password = ui->pwdLineEdit->text();

    QSqlQuery query(db);
    query.prepare("select *from user where name = ? and pwd = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();

    // 如果错误则弹出警告对话框
    if(!query.next())
    {
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误！"), QMessageBox::Yes);
        // 清空内容并定位光标
        ui->userLineEdit->clear();
        ui->pwdLineEdit->clear();
        ui->userLineEdit->setFocus();
    }else {
        user = username;
        accept();
    }
}

void LoginDialog::createbtn_clicked()
{
    QSqlQuery query(db);
    QString username = ui->userLineEdit->text();
    QString password = ui->pwdLineEdit->text();

    if(username == nullptr){
        QMessageBox::warning(this,tr("警告"),tr("用户名不能为空,请先填好信息再注册"), QMessageBox::Yes);
        // 清空内容并定位光标
        ui->userLineEdit->clear();
        ui->pwdLineEdit->clear();
    }
    else if(password == nullptr){
        QMessageBox::warning(this,tr("警告"),tr("密码不能为空,请先填好信息再注册"), QMessageBox::Yes);
        // 清空内容并定位光标
        ui->userLineEdit->clear();
        ui->pwdLineEdit->clear();
    }
    else{
        //先判断该用户名是否存在
        QString sql = QString("select * from user where name ='%1'").arg(username);
        query.exec(sql);
        bool exist = query.next();//query()得到的结果集就是query本身，query.next指向当前的查询记录的第一条

        //如果用户存在
        if(exist){
            QMessageBox::warning(this,tr("警告"),tr("该账号已存在"), QMessageBox::Yes);
            // 清空内容并定位光标
            ui->userLineEdit->clear();
            ui->pwdLineEdit->clear();
            ui->userLineEdit->setFocus();
        }else{
            query.prepare("insert into user(name,pwd) values(?,?)");
            query.addBindValue(username);
            query.addBindValue(password);
            query.exec();
        }
    }
}

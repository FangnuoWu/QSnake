#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include "wallgamewidget.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QSqlDatabase db;

private slots:
    void cancelbtn_clicked();
    void loginbtn_clicked();
    void createbtn_clicked();
    bool openDatabase();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H

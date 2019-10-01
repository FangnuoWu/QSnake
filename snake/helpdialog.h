#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QPaintEvent>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
    QPushButton* exitbtn;

public slots:
    void exitbtn_click();
};

#endif // HELPDIALOG_H

#include "history.h"
#include "ui_history.h"
#include"signin.h"
#include<QString>
#include"menu.h"
History::History(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::History)
{
    QString win = QString::number(currentPlayer.get_win());
    QString lose = QString::number(currentPlayer.get_lose());
    ui->setupUi(this);
    ui->lose->setText(lose);
    ui->win->setText(win);
}

History::~History()
{
    delete ui;
}


void History::on_backButton_clicked()
{    this->close();
     menu* newPage;
     newPage = new   menu;
     newPage->show();
    newPage->showText();
}


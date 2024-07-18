#include "history.h"
#include "ui_history.h"
#include"signin.h"
#include<QString>
#include"menu.h"
History::History(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::History)
{
    QString name ="  Welcome "+ currentPlayer.get_name();
    QString win = QString::number(currentPlayer.get_win());
    QString lose = QString::number(currentPlayer.get_lose());
    QString coin = QString::number(currentPlayer.get_coin());
    ui->setupUi(this);

    ui->name->setText(name);
    ui->coin->setText(coin);
    ui->lose->setText(lose);
    ui->win->setText(win);
}

History::~History()
{
    delete ui;
}

void History::on_pushButton_clicked()
{
    this->hide();
    menu* newPage;
    newPage = new   menu;
    newPage->show();
}


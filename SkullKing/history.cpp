#include "history.h"
#include "ui_history.h"
#include"signin.h"
#include<QString>
History::History(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::History)
{
    QString name ="Wlcome "+ currentPlayer.get_name();
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

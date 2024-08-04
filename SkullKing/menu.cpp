#include "menu.h"
#include "ui_menu.h"
#include"signin.h"
#include"server.h"
#include"client.h"
#include<QMessageBox>
#include"signup.h"
#include"algorithm"
#include<QTime>
#include"clientorserver.h"
#include"changeinformation.h"
#include"skullking.h"
#include"history.h"
#include"SlowTyper.h"
menu::menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);

      setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->coninsNum->append(QString::number(currentPlayer.get_coin()));
}
//*****************************************************************
menu::~menu()
{
    delete ui;
}
//*****************************************************************
void menu::on_start_clicked()
{
    if(currentPlayer.get_coin()<50){
        QMessageBox mb;
        mb.critical(this,"Error","You don't have enough coins to start the game");
        return;
    }
    else{

        currentPlayer.set_coin(currentPlayer.get_coin()-50);
        auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
        foundPlayer->set_coin(currentPlayer.get_coin());
         writeToFile("myfile.bin");
         ClientOrServer* client_server ;
         client_server = new ClientOrServer(this);
         this->hide();
         client_server->show();

   }

}
//*****************************************************************
void menu::on_histoty_clicked()
{

    this->hide();
    History* newPage ;
    newPage = new History;
    newPage->show();
}
//*****************************************************************
void menu::on_exit_clicked()
{
    this->close();
    Skullking* newPage;
    newPage = new Skullking;
    newPage->show();
    Skullking::delay();
    newPage->Show_TextBrows();
}
//*****************************************************************
void menu::on_change_info_clicked()
{
    changeInformation* w;
    w=new changeInformation(this);
    this->hide();
    w->show();
}
//*****************************************************************
void menu::showText()
{
    QString myText ="Dear ";
    myText+=currentPlayer.get_name();
    myText+=", welcome back...";
    SlowTyper *typer = new SlowTyper(ui->text, myText, 60); // Interval in milliseconds
    typer->start();
}

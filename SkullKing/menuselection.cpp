#include "menuselection.h"
#include "ui_menuselection.h"
#include"signin.h"
#include"server.h"
#include"client.h"
#include<QMessageBox>
#include"signup.h"
#include"algorithm"
#include<QTime>
#include"clientorserver.h"
MenuSelection::MenuSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuSelection)
{
    connect(cln,&Client::startTheGame,this,&MenuSelection::startTheGame);
    ui->setupUi(this);
}
//**********************************************
MenuSelection::~MenuSelection()
{
    delete ui;
}
//***********************************************
void MenuSelection::on_start_clicked()
{
    if(currentPlayer.get_server()){
        if(currentPlayer.get_coin()<50){
            QMessageBox mb;
            mb.critical(0,"Error","You don't have enough coins to start the game");
        }
        else{
            this->hide();
            srv->show();
            srv->play();
       }
    }
    else {
        QMessageBox MQ;
        MQ.warning(0,"Warning","Only server can start the game!");
    }
}
//***********************************************
void MenuSelection::startTheGame(){
    auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
       return( p.get_username() == currentPlayer.get_username());
    });
    if(currentPlayer.get_coin()<50){
        QMessageBox mb;
        mb.critical(0,"Error","You don't have enough coins to start the game");
        //exit....
    }
    else{
    it->set_coin(it->get_coin()-50);
    writeToFile("myfile.bin");
    this->hide();
    cln->show();
    }
}
//***********************************************

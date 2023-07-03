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
#include"changeinformation.h"
#include"skullking.h"
#include"history.h"
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
            //order client  to start the game
            cards server_order;
            server_order.setOrder("Start The Game");
            sendCard.push_back(server_order);
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            srv->get_socket()->write(file_content);
            srv->get_socket()->flush();
            file.close();
            //end
            // decrese money
            currentPlayer.set_coin(currentPlayer.get_coin()-50);
            auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
            foundPlayer->set_coin(currentPlayer.get_coin());
             writeToFile("myfile.bin");
             //end

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

void MenuSelection::on_change_info_clicked()
{
    changeInformation* w;
    w=new changeInformation(this);
    this->hide();
    w->show();
}
//****************************************************************
void MenuSelection::on_exit_clicked()
{
    this->close();
    Skullking* newPage;
    newPage = new Skullking;
    newPage->show();
}
//****************************************************************
void MenuSelection::on_histoty_clicked()
{
    this->hide();
    History* newPage ;
    newPage = new History;
    newPage->show();
}


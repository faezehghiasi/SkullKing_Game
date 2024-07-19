#include "changeinformation.h"
#include "ui_changeinformation.h"
#include"player.h"
#include"signup.h"
#include"signin.h"
#include<QMessageBox>
#include"menu.h"
#include<QRegularExpression>
#include<QRegularExpressionValidator>
#include <QRegularExpression>
#include<QPalette>
changeInformation::changeInformation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::changeInformation)
{
    auto it=find_if(listOfPlayer.begin(),listOfPlayer.end(),[=](Player x){return x.get_username()==currentPlayer.get_username();});

    ui->setupUi(this);
    ui->nameLine->setPlaceholderText(it->get_name());
    ui->addressLine->setPlaceholderText(it->get_address());
    ui->passLine->setPlaceholderText(it->get_password());
    ui->phoneLine->setPlaceholderText(it->get_phoneNumber());
    ui->usernamLine->setPlaceholderText(it->get_username());
}

changeInformation::~changeInformation()
{
    delete ui;
}

void changeInformation::on_change_clicked()
{

    auto it=find_if(listOfPlayer.begin(),listOfPlayer.end(),[=](Player x){return x.get_username()==currentPlayer.get_username();});



    QString newName= ui->nameLine->text()!= "" ?ui->nameLine->text():it->get_name();
    QString newUsername = ui->usernamLine->text()!= "" ?ui->usernamLine->text():it->get_username();
    QString newPassword =ui->passLine->text()!=""?ui->passLine->text():it->get_password();
    QString newAddress = ui->addressLine->text()!=""?ui->addressLine->text():it->get_address();
    QString newPhone = ui->phoneLine->text()!=""?ui->phoneLine->text():it->get_phoneNumber();
     bool isValid2=true;
     QRegularExpression pattern("^[0-9]{11}$");
     QValidator *validator = new QRegularExpressionValidator(pattern,ui->phoneLine);
     ui->phoneLine->setValidator(validator);
           bool isValid = pattern.match(newPhone).hasMatch();
           if (!isValid) {
              ui->phoneLine->setStyleSheet("background-color:rgba(0,0,0,0); border:none;  border-bottom:2px solid rgb(255,0,0); color: rgb(0, 0, 0); padding-bottom:7px;font: 14pt Baskerville Old Face;");
             } else {
                     ui->phoneLine->setStyleSheet("background-color:rgba(0,0,0,0); border:none;  border-bottom:2px solid rgba(105,118,132,255);color: rgb(0, 0, 0); padding-bottom:7px;font: 14pt Baskerville Old Face;");
             }
           auto exitPlayer = find_if(listOfPlayer.begin(),listOfPlayer.end(),
                                    [&]( auto p)
                     { return(p.get_username()==newUsername);});
           if( exitPlayer!=listOfPlayer.end() && exitPlayer->get_username()!= currentPlayer.get_username()){
               ui->usernamLine->setStyleSheet("background-color:rgba(0,0,0,0); border:none;  border-bottom:2px solid rgb(255,0,0); color: rgb(0, 0, 0); padding-bottom:7px;font: 14pt Baskerville Old Face;");
               QMessageBox Mb;
               Mb.critical(this,"Sign Up","Username already exists");
               isValid2=false;

           }
           else if(isValid2){
                ui->usernamLine->setStyleSheet("background-color:rgba(0,0,0,0); border:none;  border-bottom:2px solid rgba(105,118,132,255);color: rgb(0, 0, 0); padding-bottom:7px;font: 14pt Baskerville Old Face;");
           }

           if(isValid&&isValid2){

               it->set_name(newName);
               it->set_username(newUsername);
               it->set_password(newPassword);
               it->set_address(newAddress);
               it->set_phoneNumber(newPhone);
               writeToFile("myfile.bin");
               QMessageBox Mb;
               Mb.information(0,"Change Information","Change information was successful");
               menu* w;
               w=new menu;
               this->hide();
               w->show();

           }
           else return;



}


void changeInformation::on_backBotton_clicked()
{
    this->close();
        menu* newPage;
        newPage = new   menu;
        newPage->show();
       newPage->showText();
}


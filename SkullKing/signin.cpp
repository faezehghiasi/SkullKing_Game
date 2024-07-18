#include "signin.h"
#include"signup.h"
#include "ui_signin.h"
#include"skullking.h"
#include<QMessageBox>
#include"clientorserver.h"
#include"menuselection.h"
#include<QRegularExpressionValidator>
Player currentPlayer;
SignIn::SignIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
}
//************************************************************
SignIn::~SignIn()
{
    delete ui;
}
//************************************************************
void SignIn::on_ButtonLogIn_clicked()
{

  QString userName = ui->lineEditUserName->text();
  QString password = ui->lineEditPassword->text();
  bool flag = false;
  QMessageBox ms;
  for (int i =0 ; i<listOfPlayer.size();i++){
      if (userName == listOfPlayer[i].get_username()&&
              password == listOfPlayer[i].get_password()){
          currentPlayer=listOfPlayer[i];
          flag = true;
          break;
      }
  }
  if(!flag){
      ms.critical(0,"Login failed","The username or password is incorrect");
      this->hide();
      Skullking * main = new Skullking;
      main->show();
      Skullking::delay();
      main->Show_TextBrows();
     return;
  }
  ClientOrServer* client_server ;
//  client_server = new ClientOrServer(this);
//  this->hide();
//  client_server->show();
  MenuSelection* menu ;
  menu = new MenuSelection(this);
  this->close();
  menu->show();


}
//********************************************************
void SignIn::on_ButtonForgetPassword_clicked()
{
   this->hide();
   forgetP = new forgetPassword(this);
   forgetP->show();
}


void SignIn::on_backButton_clicked()
{
    Skullking* back;
    back=new Skullking;
    this->close();
    back->show();
    Skullking::delay();
    back->Show_TextBrows();
}


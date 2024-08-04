#include "signin.h"
#include"signup.h"
#include "ui_signin.h"
#include"skullking.h"
#include<QMessageBox>
#include"clientorserver.h"
#include"menu.h"
#include<QRegularExpressionValidator>
Player currentPlayer;
SignIn::SignIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);

      setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
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
        ms.critical(this,"Login failed","The username or password is incorrect");
        return;
    }
    menu* men ;
    men = new menu(this);
    this->hide();
    men->show();
    men->showText();


}
//********************************************************
void SignIn::on_ButtonForgetPassword_clicked()
{
   this->hide();
   forgetP = new forgetPassword(this);
   forgetP->show();
}
//********************************************************
void SignIn::on_backButton_clicked()
{
    Skullking* back;
    back=new Skullking;
    this->close();
    back->show();
    Skullking::delay();
    back->Show_TextBrows();
}


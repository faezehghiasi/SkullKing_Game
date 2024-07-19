#include "forgetpassword.h"
#include "ui_forgetpassword.h"
#include"player.h"
#include"signup.h"
#include"signin.h"
#include<algorithm>
#include"skullking.h"
#include<QMessageBox>
forgetPassword::forgetPassword(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::forgetPassword)
{
    ui->setupUi(this);
}

forgetPassword::~forgetPassword()
{
    delete ui;
}

void forgetPassword::on_save_clicked()
{
   QString userName = ui->username->text();
   QString phoneNumber = ui->phoneNumber->text();
   QString newPassword = ui->newPassword->text();
   auto updatePlayer = find_if(listOfPlayer.begin(),listOfPlayer.end(),
                            [&]( auto p)
             { return((p.get_username()==userName)&&(p.get_phoneNumber() == phoneNumber)); });
   if (updatePlayer == listOfPlayer.end()) {
         QMessageBox:: critical(0,"Not Found","The information is wrong");
         this->hide();
         Skullking * main = new Skullking;
         main->show();
         Skullking::delay();
         main->Show_TextBrows();
         return;
       }
   updatePlayer->set_password(newPassword);

   writeToFile("myfile.bin");

   QMessageBox:: information(0,"Found","Your password has been successfully changed");
   this->hide();
   Skullking * main = new Skullking;
   main->show();
   Skullking::delay();
   main->Show_TextBrows();
}


void forgetPassword::on_backButton_clicked()
{
    this->close();
    SignIn* signinPage = new SignIn;
    signinPage->show();
}


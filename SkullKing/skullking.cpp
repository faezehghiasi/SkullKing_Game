#include "skullking.h"
#include "ui_skullking.h"
#include<QWindow>
#include<QTime>
#include"SlowTyper.h"
Skullking::Skullking(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Skullking)
{
    ui->setupUi(this);

   setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}
//***************************************************************************************
Skullking::~Skullking()
{
    delete ui;
}
//***************************************************************************************
void Skullking::Show_TextBrows(){

    ui->welcomBrowser->append("");
    ui->welcomBrowser->append("   Welcom");
    delay();
    SlowTyper *typer = new SlowTyper(ui->textBrowser, "Choose the intended option to start the game", 40); // Interval in milliseconds
       typer->start();
    ui->SignUp->setText("Sign Up");
    ui->signIn->setText("Sign In");
    ui->signIn->setEnabled(true);
    ui->SignUp->setEnabled(true);

}
//***************************************************************************************
 void Skullking::delay(){
    QTime dietime=QTime::currentTime().addMSecs(700);
    while(QTime::currentTime()<dietime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
//***************************************************************************************
void Skullking::on_signIn_clicked()
{
    this->hide();
    signIn=new SignIn(this);
    signIn->show();
}
//***************************************************************************************
void Skullking::on_SignUp_clicked()
{
    this->hide();
    signUp=new SignUp(this);
    signUp->show();
}
//***************************************************************************************
void Skullking::on_exit_clicked()
{
    this->close();
}


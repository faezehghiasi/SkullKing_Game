#include "signup.h"
#include "ui_signup.h"
#include"player.h"
#include"skullking.h"
#include<QVector>
QVector<Player> listOfPlayer;
//******************************************************
QTextStream &operator<<(QTextStream& out ,Player p)
{
    QString Win=QString::number(p.get_win());
    QString Lose=QString::number(p.get_lose());
    QString Coin=QString::number(p.get_coin());
    QString server;
    if(p.get_server())server="true";else server="false";
    QString score=QString::number(p.get_score());
    QString turn;
    if(p.get_turn())turn="true";else turn="false";
    QString countOfTurn=QString ::number(p.get_countOfTurn());
    QString geuss=QString::number(p.get_guess());
    QString setWin=QString::number(p.get_setWin());
    out<<p.get_name()<<"\n"<<p.get_username()<<"\n"
      <<p.get_password()<<"\n"<<p.get_address()<<
        "\n"<<p.get_phoneNumber()<<"\n"<<Win<<
        "\n"<<Lose<<"\n"<<Coin<<"\n"<<server<<"\n"<<score<<"\n"
     <<turn<<"\n"<<countOfTurn<<"\n"<<geuss<<"\n"<<setWin<<"\n";
    return out;
}
//*******************************************************
QTextStream& operator>>(QTextStream& in , Player& p){
    p.set_name(in.readLine());
    p.set_username(in.readLine());
    p.set_password(in.readLine());
    p.set_address(in.readLine());
    p.set_phoneNumber(in.readLine());
    p.set_win(in.readLine().toInt());
    p.set_lose(in.readLine().toInt());
    p.set_coin(in.readLine().toInt());
    if(in.readLine()=="true")p.set_server(true);else p.set_server(false);
    p.set_score(in.readLine().toInt());
    if(in.readLine()=="true")p.set_turn(true);else p.set_turn(false);
    p.set_countOfTurn(in.readLine().toInt());
    p.set_guess(in.readLine().toInt());
    p.set_setWin(in.readLine().toInt());
    return in;
}
//**********************************************************
void readFromFile(QString filename)
{
    QFile myfile(filename);
    if(!myfile.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox Mb;
        Mb.critical(0,"File Situation","File didn't open!");
        return ;
    }
   QTextStream in(&myfile);
   Player p("","","","","",0,0,0);
   for(int i=0 ; !in.atEnd();i++){
       in>>p;
       listOfPlayer.append(p);
   }
   myfile.close();
}
//****************************************************
void writeToFile(QString filename)
{
     QFile myfile(filename);
    if(!myfile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"Not opened!";
        return ;
    }
   QTextStream out(&myfile);
   for(int i=0 ; i<listOfPlayer.size();i++)
   {
       out<<listOfPlayer[i];
   }
   myfile.flush();
   myfile.close();
}
//****************************************************
SignUp::SignUp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);

}
//****************************************************
SignUp::~SignUp()
{
    delete ui;
}
//*****************************************************
void SignUp::on_singup_push_clicked()
{
    Player p;
    p.set_name(ui->Name->text());
    p.set_username(ui->username->text());
    p.set_password(ui->password->text());
    p.set_address(ui->address->text());
    p.set_phoneNumber(ui->phone_num->text());
    QString filename= "myfile.bin";
    p.set_coin(1000);
    listOfPlayer.append(p);
    writeToFile(filename);
    QMessageBox Mb;
    Mb.information(0,"Sign Up","Registration was successful");
     this->hide();
    Skullking* w;
    w=new Skullking;
    w->show();
     Skullking::delay();
     w->Show_TextBrows();
}


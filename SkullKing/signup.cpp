#include<QRegularExpressionValidator>
#include <QRegularExpression>
#include "signup.h"
#include "ui_signup.h"
#include"player.h"
#include"skullking.h"
#include<QVector>
#include<QLineEdit>
#include<QPalette>
QVector<Player> listOfPlayer;
//******************************************************
QTextStream &operator<<(QTextStream& out ,Player p)
{
    QString Win=QString::number(p.get_win());
    QString Lose=QString::number(p.get_lose());
    QString Coin=QString::number(p.get_coin());
    out<<p.get_name()<<"\n"<<p.get_username()<<"\n"
        <<p.get_password()<<"\n"<<p.get_address()<<
        "\n"<<p.get_phoneNumber()<<"\n"<<Win<<
        "\n"<<Lose<<"\n"<<Coin<<"\n";
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
    p.set_server(false);
    p.set_score(0);
    p.set_turn(false);
    p.set_countOfTurn(1);
    p.set_guess(0);
    p.set_setWin(0);
    p.set_starterOfEachRound(false);
    return in;
}
//**********************************************************
void readFromFile(QString filename)
{
    QFile myfile(filename);


    if (!myfile.exists()) {

        if (!myfile.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox MQ;
            MQ.warning(0, "File Situation", "can not create the File");
            return;
        }
        myfile.close();
    }

    if (!myfile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox MQ;
        MQ.warning(0, "File Situation", "can not open the File");
        return;
    }

    QTextStream in(&myfile);
    Player p("", "", "", "", "", 0, 0, 0);
    for (int i = 0; !in.atEnd(); i++) {
        in >> p;
        listOfPlayer.append(p);
    }
    myfile.close();
}
//****************************************************
void writeToFile(QString filename)
{
     QFile myfile(filename);
    if(!myfile.open(QFile::WriteOnly | QFile::Text)){
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

      setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

}
//****************************************************
SignUp::~SignUp()
{
    delete ui;
}
//*****************************************************
void SignUp::on_backButton_clicked()
{
    Skullking* back;
    back=new Skullking;
    this->close();
    back->show();
    Skullking::delay();
    back->Show_TextBrows();
}
//*****************************************************
void SignUp::on_nextButton_clicked()
{
    QString name = ui->Name->text();
    bool isValid2 = name==""?false:true;
    if (!isValid2) {
        ui->Name->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    } else {
        ui->Name->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,255,255);");
    }
    QString username = ui->username->text();
    bool isValid3 = username==""?false:true;
    if (!isValid3) {
        ui->username->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    } else {
        ui->username->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,255,255);");
    }
    QString address = ui->address->text();
    bool isValid4 = address==""?false:true;
    if (!isValid4) {
        ui->address->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    } else {
        ui->address->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,255,255);");
    }
    QString password = ui->password->text();
    bool isValid5 = password==""?false:true;
    if (!isValid5) {
        ui->password->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    } else {
        ui->password->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,255,255);");
    }

    QRegularExpression pattern("^[0-9]{11}$");
    QValidator *validator = new QRegularExpressionValidator(pattern, ui->phone_num);
    ui->phone_num->setValidator(validator);
    QString phoneNumber =ui->phone_num->text();
    bool isValid = pattern.match(phoneNumber).hasMatch();
    if (!isValid) {
        ui->phone_num->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    } else {
        ui->phone_num->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,255,255);");
    }
    auto exitPlayer = find_if(listOfPlayer.begin(),listOfPlayer.end(),
                              [&]( auto p)
                              { return(p.get_username()==username);});
    if( exitPlayer!=listOfPlayer.end()){
        ui->username->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
        QMessageBox Mb;
        Mb.critical(this,"Sign Up","Username already exists");
        isValid3 = false;
    }
    else if(!isValid3){
        ui->username->setStyleSheet("border-radius:10px;color: rgb(0, 0, 0);font: 15pt Times New Roman;background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgb(211, 232, 255));border:3px solid; border-color :rgb(225,0,0);");
    }


    if(isValid&&isValid2&&isValid3&&isValid4&&isValid5){

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
        this->hide();
        Skullking* w;
        w=new Skullking;
        w->show();
        Skullking::delay();
        w->Show_TextBrows();
    }
    else return;
}


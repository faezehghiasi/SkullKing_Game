#include "clientorserver.h"
#include "ui_clientorserver.h"
#include<QTime>
#include<QMessageBox>
#include<QLabel>
#include<QPushButton>
#include<QTime>
#include"signin.h"
ClientOrServer::ClientOrServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientOrServer)
{
    ui->setupUi(this);
}
//*************************************************************************
ClientOrServer::~ClientOrServer()
{
    delete ui;
}
//**************************************************************************
void ClientOrServer::on_clientButton_clicked()
{
    currentPlayer.set_server(false);
    ui->clientButton->disconnect();
    ui->serverButton->disconnect();
    QFont font_button("Tw Cen MT Condensed Extra Bold");
    QFont font_line("Algerian");
    connect_button =new QPushButton("Connect",this);
    connect_button->setGeometry(480,370,111,41);
    connect_button->setFont(font_button);
    connect_button->setStyleSheet("font: 16pt;background-color: qradialgradient(spread:repeat, cx:0.5, cy:0.5, radius:0.077, fx:0.5, fy:0.5, stop:0 rgba(200, 168, 74, 255), stop:0.497326 rgba(0, 0, 0, 147), stop:0.990385 rgba(126, 86, 38, 255));color: rgb(255,255,255);border:2px solid;border-color:#000;");
    connect_button->show();
    Ip_line=new QLineEdit(this);
    Ip_line->setStyleSheet("font : 16pt;color: rgb(0,0,0);background:rgba(0,0,0,0);border:2px solid;border-color:#000;");
    Ip_line->setFont(font_line);
    Ip_line->setGeometry(150,370,301,41);
    Ip_line->setPlaceholderText(" Enter the server ip");
    Ip_line->show();
    connect(connect_button,SIGNAL(clicked()),this,SLOT(connect_button_clicked()));
}
//****************************************************************************************************
void ClientOrServer::on_serverButton_clicked()
{
    currentPlayer.set_server(true);
    server_label = new QLabel (this);
    server_label->setGeometry(210,230,331,111);
    server_label->setStyleSheet("color: rgb(2, 2, 2);font: 20pt Snap ITC;");
    server_label->setText("waiting...");
    server_label->show();
    ui->clientButton->disconnect();
    ui->serverButton->disconnect();
    ip_label=new QLabel(this);
    ip_label->setGeometry(20,560,321,20);
    ip_label->setText("");
    ip_label->setStyleSheet("color: rgb(0,0,0);font:20pt Algerian");
    ip_label->show();
    srv = new Server;
    srv->creation();
    connect(srv,&Server::changePage,this,&ClientOrServer::chanePage);

}
//****************************************************************************
void ClientOrServer::connect_button_clicked(){
    Ip=Ip_line->text();
    cln = new Client;
    cln->creation();
    client_label = new QLabel (this);
    client_label->setGeometry(170,320,161,35);
    client_label->setStyleSheet("color: rgb(2, 2, 2);font: 20pt Snap ITC;");
    client_label->setText("waiting...");
    client_label->show();
    delay(3000);
    if(changePage){
        MenuSelection* menu;
        menu = new MenuSelection;
        this->close();
        menu->show();
   }
}

//*******************************************************************************
void ClientOrServer::chanePage(){
    delay(3000);
    MenuSelection* mn;
    mn = new MenuSelection;
    this->hide();
    this->close();
    mn->show();
}
//*******************************************************************************
void ClientOrServer::delay(int ms){
    QTime dietime=QTime::currentTime().addMSecs(ms);
    while(QTime::currentTime()<dietime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
//*****************************************************************************

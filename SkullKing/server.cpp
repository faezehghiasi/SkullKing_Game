#include "server.h"
#include "ui_server.h"
#include<QDebug>
#include"clientorserver.h"
#include"ui_clientorserver.h"
#include"menuselection.h"
#include<QMessageBox>
#include<QDataStream>
#include<QFile>
#include<QRandomGenerator>
#include<QTime>
//******
Server * srv;
Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->setupUi(this);
    server = new QTcpServer;
    server_card.cards_button = new QPushButton(this);
    server_card.cards_button->setGeometry(290,290,101,141);
    client_card.cards_button = new QPushButton(this);
    client_card.cards_button->setGeometry(210,240,101,141);
    server_card.cards_button->hide();
    client_card.cards_button->hide();
}
//************************************************************
Server::~Server()
{
    delete ui;
}
//**************************************************************
void Server::newConnection(){
   socket = server->nextPendingConnection();
   ClientOrServer::server_label->setGeometry(80,280,513,51);
   ClientOrServer::server_label->setStyleSheet("font: 14pt Showcard Gothic; color: rgb(8, 8, 8);");
   ClientOrServer::server_label->setText("A new player has been added to the game...");
   emit changePage();
   connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
   connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));

}
//*****************************************************************
void Server :: readyRead(){

    QByteArray file_content = socket->readAll();
          QFile file("recivedCard.bin");
          if(!file.open(QIODevice::WriteOnly)){
              qDebug()<<"file can not open";
              return;
          }
          file.write(file_content);
          file .close();
          readFromFileCards("recivedCard.bin",recivedCard);
          if(recivedCard.size()==1){
              //client send card
              qDebug()<<"cos khar";
              currentPlayer.set_turn(true);
              client_card = recivedCard[0];
                qDebug()<<client_card.thisCard.getId()<<client_card.thisCard.getNumber();
                if(!currentPlayer.get_starterOfEachRound()){
                availbleCards(client_card);
                }
                else {
                    buttons temp;
                    availbleCards(temp);
                }
              set_picture(client_card);
              if(!client_card.empty()&& !server_card.empty()){
                currentPlayer.calculate(client_card.thisCard);
                move_twoCards();
              }
         }
         else{

            currentPlayer.set_randomCards(recivedCard,currentPlayer.get_countOfTurn());
          }

}
//*****************************************************************
void Server::disconnected(){

    socket->close();
}
//*******************************************************************
void Server::creation(){
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
     for(int nIter=0; nIter<list.count(); nIter++)
      {
        if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ){
           serverIp= list[nIter].toString();
          break;
              }
      }
    QHostAddress myIP;
    myIP.setAddress(serverIp);
    serverIp= "IP: " + serverIp;
    ClientOrServer::ip_label->setText(serverIp);
    if(!server->listen(myIP,9999)){
        QMessageBox MQ;
        MQ.critical(this,"Error","server can not start");
    }
}
//***************************************************************************
QTcpSocket* Server::get_socket()
{
    return socket;
}
//*************************************************************************
QVector<buttons>& Server::get_buttons(){return pushButtons;}
//***************************************************************************
void Server::showCards(QList<cards> cCards){
    switch(cCards.size()){
    case 2:{
        for(int i = 0;i<2;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X =190;
        int Y = 490;
        int height = 181;
        int width =101;
        for(int i=0;i<2;i++,X+=120){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
           pushButtons[i]. cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
    }
              break;
    case 4:{
        for(int i = 0;i<4;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X =100;
        int Y = 510;
        int height = 171;
        int width =101;
        for(int i=0;i<2;i++,X+=90,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=2;i<4;i++,X+=90,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
    }
               break;
    case 6:{
        for(int i = 0;i<6;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X = 90;
        int Y = 530;
        int height = 151;
        int width =91;
        for(int i=0;i<3;i++,X+=70,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=3;i<6;i++,X+=70,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons5_clicked()));
    }
               break;
    case 8:{
        for(int i = 0;i<8;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X = 60;
        int Y = 540;
        int height = 131;
        int width =81;
        for(int i=0;i<4;i++,X+=60,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        Y+=10;
        for(int i=4;i<8;i++,X+=60,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons7_clicked()));
    }
               break;
    case 10:{
        for(int i = 0;i<10;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X = 40;
        int Y = 550;
        int height = 131;
        int width =71;
        for(int i=0;i<5;i++,X+=50,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=5;i<10;i++,X+=50,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }

        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
    }
               break;
    case 12:{
        for(int i = 0;i<12;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X = 5;
        int Y = 560;
        int height = 131;
        int width =71;
        for(int i=0;i<6;i++,X+=50,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=6;i<12;i++,X+=50,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }

        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
        connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons10_clicked()));
        connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons11_clicked()));
    }
               break;
    case 14: {
        for(int i = 0;i<14;i++){
            buttons QP;
            QP.cards_button = new QPushButton(this);
            QP.thisCard= cCards[i];
             pushButtons.append(QP);
        }
        int X =10;
        int Y = 540;
        int height = 140;
        int width =60;
        for(int i=0;i<7;i++,X+=42,Y-=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
          Y+=10;
        for(int i=7;i<14;i++,X+=42,Y+=10){
            pushButtons[i].cards_button->setGeometry(X,Y,width,height);
            pushButtons[i].cards_button->show();
        }
        connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons0_clicked()));
        connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons1_clicked()));
        connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons2_clicked()));
        connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons3_clicked()));
        connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons4_clicked()));
        connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons5_clicked()));
        connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons6_clicked()));
        connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons7_clicked()));
        connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons8_clicked()));
        connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons9_clicked()));
        connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons10_clicked()));
        connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT(  on_Buttons11_clicked()));
        connect(pushButtons[12].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons12_clicked()));
        connect(pushButtons[13].cards_button,SIGNAL(clicked()),this,SLOT( on_Buttons13_clicked()));

     }
               break;

   }


}
//***************************************************************************
void Server::whoShouldStartTheGameFirst(){
    //delay function......
    QTime dietime=QTime::currentTime().addMSecs(3000);
    while(QTime::currentTime()<dietime)
    QCoreApplication::processEvents(QEventLoop::AllEvents,100);

    int server_number =( QRandomGenerator::global()->generate()%8)+1;
    int client_number =( QRandomGenerator::global()->generate()%8)+1;
    tab:if(server_number>client_number){
        currentPlayer.set_turn(true);
        currentPlayer.set_starterOfEachRound(true);
    }
    else if(client_number>server_number){
       currentPlayer.set_turn(false);
       currentPlayer.set_starterOfEachRound(false);
    }
    else{
        server_number =( QRandomGenerator::global()->generate()%8)+1;
        client_number =( QRandomGenerator::global()->generate()%8)+1;
        goto tab;
    }
    server_card.thisCard.setId(3);
    server_card.thisCard.setNumber(server_number);
    server_card.thisCard.setValue(server_number);
    client_card.thisCard.setId(3);
    client_card.thisCard.setNumber(client_number);
    client_card.thisCard.setValue(client_number);
    sendCard.push_back(server_card.thisCard);
    sendCard.push_back(client_card.thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    socket->write(file_content);
   // socket->waitForBytesWritten(3000);
    socket->flush();
    file.close();
}
//*************************************************************************************
void Server::set_picture(struct buttons crd){

    switch(crd.thisCard.getId()){
    case 1:{
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure1.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure2.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure2.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 3:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure3.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure3.png width=150 height=200/></p></body></html>");
             crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure4.png width=150 height=200/></p></body></html>");
            break;
        case 5:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure5.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure5.png width=150 height=200/></p></body></html>");
                crd.cards_button->show();
            break;
        case 6:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure6.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure6.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 7:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure7.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/treasure8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/treasure8.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        }

    }
        break;
    case 2:
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard1.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard2.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard2.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 3:
           crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard3.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard3.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard4.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard5.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard5.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard6.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard6.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 7:
           crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard7.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard7.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/mapcard8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/mapcard8.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        }
        break;
    case 3:
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot1.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
           crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot2.png);");
           crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot2.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 3:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot3.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot3.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 4:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot4.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot4.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
              crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot5.png);");
              crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot5.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
               crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot6.png);");
               crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot6.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 7:
             crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot7.png  width=150 height=200 /></p></body></html>");
           crd.cards_button->show();
            break;
        case 8:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/parrot8.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/parrot8.png  width=150 height=200 /></p></body></html>");
            crd.cards_button->show();
            break;
        }
        break;
    case 4:
        switch(crd.thisCard.getNumber()){
        case 1:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard1.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard1.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 2:
            crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard2.png);");
            crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard2.png width=150 height=200/></p></body></html>");
          crd.cards_button->show();
            break;
        case 3:
             crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard3.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard3.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 4:
             crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard4.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard4.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 5:
              crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard5.png);");
              crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard5.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 6:
               crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard6.png);");
               crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard6.png width=150 height=200/></p></body></html>");
           crd.cards_button->show();
            break;
        case 7:
             crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard7.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard7.png width=150 height=200/></p></body></html>");
            crd.cards_button->show();
            break;
        case 8:
             crd.cards_button->setStyleSheet("border-image: url(:/resource/flagcard8.png);");
             crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/flagcard8.png width=150 height=200/></p></body></html>");
             crd.cards_button->show();
            break;
        }
        break;
    case 5:
         crd.cards_button->setStyleSheet("border-image: url(:/resource/king.png);");
         crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/king.png width=150 height=200/></p></body></html>");
         crd.cards_button->show();
        break;
    case 6:
        crd.cards_button->setStyleSheet("border-image: url(:/resource/queen.png);");
        crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/queen.png width=150 height=200/></p></body></html>");
        crd.cards_button->show();
        break;
    case 7:
        crd.cards_button->setStyleSheet("border-image: url(:/resource/dozd.png);");
        crd.cards_button->setToolTip("<html><head/><body><p><img src=:/resource/dozd.png width=150 height=200/></p></body></html>");
        crd.cards_button->show();
        break;
    }
}
//*************************************************************************************
void Server::availbleCards(buttons rivalCard){
    //mitooni bhine tar koni
    int rivalCardId=rivalCard.thisCard.getId();
   if(rivalCardId==1||rivalCardId==2||rivalCardId==3||rivalCardId==4){
       auto findIt=find_if(pushButtons.begin(),pushButtons.end(),[rivalCardId](buttons b){return (b.thisCard.getId()==rivalCardId);});
       //age az ina dashte bashe...
       if(findIt!=pushButtons.end()){
          for(auto& x:pushButtons){
              if(x.thisCard.getId()!=rivalCardId && x.thisCard.getId()!=5 &&x.thisCard.getId()!=6 &&x.thisCard.getId()!=7){
                  x.cards_button->setEnabled(false);
              }
              else x.cards_button->setEnabled(true);
          }
       }
       //age nadashte bashe....
       else for(auto& x:pushButtons)x.cards_button->setEnabled(true);
   }
   else for(auto& x:pushButtons)x.cards_button->setEnabled(true);

}
//********************************************************************************
void Server :: set_client_card(struct buttons cCard){
    client_card =cCard;
}
//********************************************************************************************
void Server :: set_server_card(struct buttons sCard){
    server_card =sCard;
}
//********************************************************************************************
buttons& Server::get_client_card(){
    return client_card;
}
//**********************************************************************************************
buttons& Server::get_server_card(){
    return server_card;
}
//*************************************************************************************************
void Server::on_Buttons0_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    server_card = pushButtons[0];
    //qDebug()<<"0"<< pushButtons[0].thisCard.getId()<<" "<< pushButtons[0].thisCard.getNumber()<<" "<<pushButtons[0].thisCard.getValue();
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[0].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[0]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons1_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    server_card  = pushButtons[1];
     //qDebug()<<" 1"<< pushButtons[1].thisCard.getId()<<" "<< pushButtons[1].thisCard.getNumber()<<" "<<pushButtons[1].thisCard.getValue();
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[1].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[1]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
       move_twoCards();
    }
}
//**************************************************************************************************
void Server::on_Buttons2_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    // qDebug()<<"2"<< pushButtons[2].thisCard.getId()<<" "<< pushButtons[2].thisCard.getNumber()<<" "<<pushButtons[2].thisCard.getValue();
    server_card  = pushButtons[2];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[2].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[2]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons3_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    // qDebug()<<"3"<< pushButtons[3].thisCard.getId()<<" "<< pushButtons[3].thisCard.getNumber()<<" "<<pushButtons[3].thisCard.getValue();
    server_card  = pushButtons[3];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[3].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[3]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons4_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"4"<< pushButtons[4].thisCard.getId()<<" "<< pushButtons[4].thisCard.getNumber()<<" "<<pushButtons[4].thisCard.getValue();
    server_card  = pushButtons[4];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[4].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[4]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons5_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"5"<< pushButtons[5].thisCard.getId()<<" "<< pushButtons[5].thisCard.getNumber()<<" "<<pushButtons[5].thisCard.getValue();
    server_card  = pushButtons[5];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[5].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[5]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
       move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons6_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    //qDebug()<<"6"<< pushButtons[6].thisCard.getId()<<" "<< pushButtons[6].thisCard.getNumber()<<" "<<pushButtons[6].thisCard.getValue();
    server_card  = pushButtons[6];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[6].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[6]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons7_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"7"<< pushButtons[7].thisCard.getId()<<" "<< pushButtons[7].thisCard.getNumber()<<" "<<pushButtons[7].thisCard.getValue();
    server_card = pushButtons[7];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[7].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[7]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons8_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"8"<< pushButtons[8].thisCard.getId()<<" "<< pushButtons[8].thisCard.getNumber()<<" "<<pushButtons[8].thisCard.getValue();
    server_card  = pushButtons[8];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[8].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[8]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons9_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"9"<< pushButtons[9].thisCard.getId()<<" "<< pushButtons[9].thisCard.getNumber()<<" "<<pushButtons[9].thisCard.getValue();
    server_card  = pushButtons[9];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[9].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[9]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons10_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    // qDebug()<<"10"<< pushButtons[10].thisCard.getId()<<" "<< pushButtons[10].thisCard.getNumber()<<" "<<pushButtons[10].thisCard.getValue();
    server_card  = pushButtons[10];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[10].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[10]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
       move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons11_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"11"<< pushButtons[11].thisCard.getId()<<" "<< pushButtons[11].thisCard.getNumber()<<" "<<pushButtons[11].thisCard.getValue();
    server_card  = pushButtons[11];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[11].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[11]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
       move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons12_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"12"<< pushButtons[12].thisCard.getId()<<" "<< pushButtons[12].thisCard.getNumber()<<" "<<pushButtons[12].thisCard.getValue();
    server_card  = pushButtons[12];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[12].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();

    /// animaition

    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[12]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      move_twoCards();
    }

}
//**************************************************************************************************
void Server::on_Buttons13_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
     //qDebug()<<"13"<< pushButtons[13].thisCard.getId()<<" "<< pushButtons[13].thisCard.getNumber()<<" "<<pushButtons[13].thisCard.getValue();
    server_card  = pushButtons[13];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[13].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    /// animaition
    socket->write(file_content);
    socket->flush();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[13]);
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
       move_twoCards();
    }

}
//**************************************************************************************************
void Server::move_oneCards(buttons& cCards){
    QPropertyAnimation *anim = new QPropertyAnimation(cCards.cards_button, "pos", this);
       anim->setDuration(500);
       anim->setEndValue(QPoint(290, 290));
       anim->start();
       connect(anim,&QAbstractAnimation::finished,this,[&](){cCards.cards_button->disconnect();cCards.cards_button->hide();set_picture(server_card);
           server_card.cards_button->setGeometry(290,290,101,141);
           client_card.cards_button->setGeometry(210,240,101,141);});
}
//****************************************************************************************
void Server::move_twoCards(){
    QPropertyAnimation *animServer = new QPropertyAnimation(server_card.cards_button, "pos", this);
    QPropertyAnimation *animClient = new QPropertyAnimation(client_card.cards_button, "pos", this);
       animServer->setDuration(500);
       animServer->setEndValue(QPoint(-50, 260));
       animServer->start();
       animClient->setDuration(500);
       animClient->setEndValue(QPoint(-50, 260));
       animClient->start();
       server_card.clear();
       client_card.clear();
       connect(animServer,&QAbstractAnimation::finished,this,[&](){server_card.cards_button->hide();});
       connect(animClient,&QAbstractAnimation::finished,this,[&](){client_card.cards_button->hide();});
       connect(animServer,SIGNAL(finished()),animServer,SLOT(deleteLater()));
       connect(animClient,SIGNAL(finished()),animClient,SLOT(deleteLater()));

}




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
#include"signup.h"
#include"player.h"
#include<math.h>
#include<QMutex>
#include"skullking.h"
QMutex mx;
//******
Server * srv;
Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server_card.cards_button = new QPushButton(this);
    server_card.cards_button->setGeometry(290,290,101,141);
    client_card.cards_button = new QPushButton(this);
    client_card.cards_button->setGeometry(210,240,101,141);
    server_card.cards_button->hide();
    client_card.cards_button->hide();
    scoreLabel= new QLabel(this);
    scoreNumber= new QLabel(this);
    scoreLabel->setStyleSheet("font: 14pt Broadway;  color: rgb(255, 255, 255);background-color: rgb(7, 7, 7);");
    scoreNumber->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    scoreLabel->setGeometry(10,50,81,31);
    scoreNumber->setGeometry(10,90,71,31);
    scoreLabel->setText("  Score");
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    scoreLabel->show();
    scoreNumber->show();
    continueTheGameButton = new QPushButton("Continue the game",this);
    continueTheGameButton->setStyleSheet("background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;QPushButton#continueTheGameButton{background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;}QPushButton#continueTheGameButton:hover{ color:rgba(155,168,182,210) ;}QPushButton#continueTheGameButton:pressed{padding-left:5px; padding-top:5px;color:rgba(115 ,128,142,210);}");
    continueTheGameButton->setGeometry(150,300,301,141);
    continueTheGameButton->hide();
    connect(continueTheGameButton,SIGNAL(clicked()),this , SLOT(on_continueTheGameButton_clicked()));
    endOfTheGame=new QPushButton("Wait for end",this);
    endOfTheGame->setStyleSheet("background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;QPushButton#continueTheGameButton{background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;}QPushButton#continueTheGameButton:hover{ color:rgba(155,168,182,210) ;}QPushButton#continueTheGameButton:pressed{padding-left:5px; padding-top:5px;color:rgba(115 ,128,142,210);}");
    endOfTheGame->setGeometry(150,300,301,141);
    endOfTheGame->hide();
    endOfTheGame->setEnabled(false);

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
              if(recivedCard[0].getOrder()=="EXIT"){
                  currentPlayer.set_win(currentPlayer.get_win()+1);
                  currentPlayer.set_coin(currentPlayer.get_coin()+100);
                  auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
                      return(currentPlayer.get_username()==p.get_username());
                  });
                  it->set_win(currentPlayer.get_win());
                  it->set_coin(currentPlayer.get_coin());
                  writeToFile("myfile.bin");
                  this->close();
                  Skullking* newPage2;
                  newPage2=new Skullking;
                  newPage2->show();
                  Skullking::delay();
                  newPage2->Show_TextBrows();
              }


              /////////////////////////////////////////
              else if(recivedCard[0].getOrder()=="SCORE"){
                  int rivalScore=recivedCard[0].getNumber();
                  for(auto& x:pushButtons)delete x.cards_button;
                  pushButtons.clear();
                  caculateScore(rivalScore);
              }
              ////////////////////////////////////////////////
              else{
                   //client send card
              client_card = recivedCard[0];
                if(!currentPlayer.get_starterOfEachRound()){
                    currentPlayer.set_turn(true);
                    availbleCards(client_card);
                }
                else {
                    buttons temp;
                    availbleCards(temp);
                }
                set_picture(client_card);
                if(!client_card.empty()&& !server_card.empty()){
                currentPlayer.calculate(client_card.thisCard);
                ClientOrServer::delay(2000);
                move_twoCards();
                if(currentPlayer.playeCard.size()==0){
                    if(currentPlayer.get_countOfTurn()==7){
                      for(auto& x:pushButtons)x.cards_button->setEnabled(false);
                      endOfTheGame->setEnabled(true);
                      endOfTheGame->show();}

                    else{
                 for(auto& x:pushButtons)x.cards_button->setEnabled(false);
                   continueTheGameButton->setEnabled(true);
                   continueTheGameButton->show();}
                }
                }
             }
            }

         else{
            currentPlayer.set_randomCards(recivedCard,currentPlayer.get_countOfTurn());
            showCards(currentPlayer.playeCard);
            for(int i =0 ; i<pushButtons.size();i++){
               set_picture( pushButtons[i]);
            }
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
             pushButtons.push_back(QP);
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
    QTime dietime=QTime::currentTime().addMSecs(2000);
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
    mx.lock();
    socket->write(file_content);
    socket->flush();
    mx.unlock();
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
             crd.cards_button->show();
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
        switch(crd.thisCard.getNumber())
        {
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
void Server::change_geometry(){
    server_card.cards_button->setGeometry(290,290,101,141);
    client_card.cards_button->setGeometry(210,240,101,141);
}
//************************************************************************************************
void Server::on_Buttons0_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[0].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    server_card = pushButtons[0];
    //qDebug()<<"0"<< pushButtons[0].thisCard.getId()<<" "<< pushButtons[0].thisCard.getNumber()<<" "<<pushButtons[0].thisCard.getValue();
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[0].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[0]);
    ClientOrServer::delay(1000);
    pushButtons[0].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
          else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
            continueTheGameButton->setEnabled(true);
            continueTheGameButton->show();}

      }
    }
}
//**************************************************************************************************
void Server::on_Buttons1_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[1].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    server_card  = pushButtons[1];
     //qDebug()<<" 1"<< pushButtons[1].thisCard.getId()<<" "<< pushButtons[1].thisCard.getNumber()<<" "<<pushButtons[1].thisCard.getValue();
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[1].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[1]);
    ClientOrServer::delay(1000);
      pushButtons[1].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               endOfTheGame->show();
           }
           ////////////////////////////////////////
           else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
            continueTheGameButton->setEnabled(true);
            continueTheGameButton->show();}
       }
    }
}
//**************************************************************************************************
void Server::on_Buttons2_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[2].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    // qDebug()<<"2"<< pushButtons[2].thisCard.getId()<<" "<< pushButtons[2].thisCard.getNumber()<<" "<<pushButtons[2].thisCard.getValue();
    server_card  = pushButtons[2];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[2].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[2]);
        ClientOrServer::delay(1000);
          pushButtons[2].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
          else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
           continueTheGameButton->setEnabled(true);
           continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons3_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[3].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    // qDebug()<<"3"<< pushButtons[3].thisCard.getId()<<" "<< pushButtons[3].thisCard.getNumber()<<" "<<pushButtons[3].thisCard.getValue();
    server_card  = pushButtons[3];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[3].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
     mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[3]);
        ClientOrServer::delay(1000);
          pushButtons[3].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
         else{ for(auto& x:pushButtons)x.cards_button->setEnabled(false);
          continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons4_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[4].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"4"<< pushButtons[4].thisCard.getId()<<" "<< pushButtons[4].thisCard.getNumber()<<" "<<pushButtons[4].thisCard.getValue();
    server_card  = pushButtons[4];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[4].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[4]);
        ClientOrServer::delay(1000);
        pushButtons[4].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
          else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
          continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons5_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[5].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"5"<< pushButtons[5].thisCard.getId()<<" "<< pushButtons[5].thisCard.getNumber()<<" "<<pushButtons[5].thisCard.getValue();
    server_card  = pushButtons[5];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[5].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[5]);
        ClientOrServer::delay(1000);
          pushButtons[5].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               endOfTheGame->show();
           }
           ////////////////////////////////////////
           else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
           continueTheGameButton->setEnabled(true);
           continueTheGameButton->show();}
       }
    }

}
//**************************************************************************************************
void Server::on_Buttons6_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[6].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    //qDebug()<<"6"<< pushButtons[6].thisCard.getId()<<" "<< pushButtons[6].thisCard.getNumber()<<" "<<pushButtons[6].thisCard.getValue();
    server_card  = pushButtons[6];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[6].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
     mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[6]);
        ClientOrServer::delay(1000);
          pushButtons[6].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
         else{ for(auto& x:pushButtons)x.cards_button->setEnabled(false);
          continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons7_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[7].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"7"<< pushButtons[7].thisCard.getId()<<" "<< pushButtons[7].thisCard.getNumber()<<" "<<pushButtons[7].thisCard.getValue();
    server_card = pushButtons[7];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[7].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[7]);
        ClientOrServer::delay(1000);
          pushButtons[7].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
         else{ for(auto& x:pushButtons)x.cards_button->setEnabled(false);
          continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons8_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[8].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"8"<< pushButtons[8].thisCard.getId()<<" "<< pushButtons[8].thisCard.getNumber()<<" "<<pushButtons[8].thisCard.getValue();
    server_card  = pushButtons[8];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[8].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
   // socket->waitForBytesWritten(2000);
    socket->flush();
      mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[8]);
        ClientOrServer::delay(1000);
          pushButtons[8].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
          else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
           continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }

}
//**************************************************************************************************
void Server::on_Buttons9_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[9].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"9"<< pushButtons[9].thisCard.getId()<<" "<< pushButtons[9].thisCard.getNumber()<<" "<<pushButtons[9].thisCard.getValue();
    server_card  = pushButtons[9];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[9].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[9]);
        ClientOrServer::delay(1000);
          pushButtons[9].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
          else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
          continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}
      }
    }
}
//**************************************************************************************************
void Server::on_Buttons10_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[10].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
    // qDebug()<<"10"<< pushButtons[10].thisCard.getId()<<" "<< pushButtons[10].thisCard.getNumber()<<" "<<pushButtons[10].thisCard.getValue();
    server_card  = pushButtons[10];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[10].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
     mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[10]);
        ClientOrServer::delay(1000);
          pushButtons[10].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               endOfTheGame->show();
           }
           ////////////////////////////////////////
           else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
           continueTheGameButton->setEnabled(true);
           continueTheGameButton->show();}
       }
    }
}
//**************************************************************************************************
void Server::on_Buttons11_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[11].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"11"<< pushButtons[11].thisCard.getId()<<" "<< pushButtons[11].thisCard.getNumber()<<" "<<pushButtons[11].thisCard.getValue();
    server_card  = pushButtons[11];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[11].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
     mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[11]);
        ClientOrServer::delay(1000);
          pushButtons[11].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               endOfTheGame->show();
           }
           ////////////////////////////////////////
           else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
            continueTheGameButton->setEnabled(true);
            continueTheGameButton->show();}
       }
    }
}
//**************************************************************************************************
void Server::on_Buttons12_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[12].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"12"<< pushButtons[12].thisCard.getId()<<" "<< pushButtons[12].thisCard.getNumber()<<" "<<pushButtons[12].thisCard.getValue();
    server_card  = pushButtons[12];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[12].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[12]);
        ClientOrServer::delay(1000);
          pushButtons[12].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
      move_twoCards();
      if(currentPlayer.playeCard.size()==0){
          ////////////////////////////////////
          if(currentPlayer.get_countOfTurn()==7){
              endOfTheGame->show();
          }
          ////////////////////////////////////////
         else{ for(auto& x:pushButtons)x.cards_button->setEnabled(false);
           continueTheGameButton->setEnabled(true);
          continueTheGameButton->show();}

      }
    }
}
//**************************************************************************************************
void Server::on_Buttons13_clicked(){
    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[13].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()){
    currentPlayer.playeCard.erase(it);
    }
     //qDebug()<<"13"<< pushButtons[13].thisCard.getId()<<" "<< pushButtons[13].thisCard.getNumber()<<" "<<pushButtons[13].thisCard.getValue();
    server_card  = pushButtons[13];
    currentPlayer.set_selectedCard(server_card.thisCard);
    sendCard.push_back(pushButtons[13].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     mx.lock();
    socket->write(file_content);
    //socket->waitForBytesWritten(2000);
    socket->flush();
    mx.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[13]);
        ClientOrServer::delay(1000);
          pushButtons[13].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(client_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               endOfTheGame->show();
           }
           ////////////////////////////////////////
           else{for(auto& x:pushButtons)x.cards_button->setEnabled(false);
            continueTheGameButton->setEnabled(true);
           continueTheGameButton->show();}

       }
    }
}
//**************************************************************************************************
void Server::move_oneCards(buttons& cCards){
   // mx.lock();
    QPropertyAnimation *anim = new QPropertyAnimation(cCards.cards_button, "pos", this);
       anim->setDuration(500);
       anim->setEndValue(QPoint(290, 290));
       anim->start();
       connect(anim,&QAbstractAnimation::finished,this,[&](){cCards.cards_button->disconnect();cCards.cards_button->hide();set_picture(server_card);
           server_card.cards_button->setGeometry(290,290,101,141);
           client_card.cards_button->setGeometry(210,240,101,141);});
   // mx.unlock();
}
//**************************************************************************************************
void Server::move_twoCards(){
   // mx.lock();
    QPropertyAnimation *animServer = new QPropertyAnimation(server_card.cards_button, "pos", this);
    QPropertyAnimation *animClient = new QPropertyAnimation(client_card.cards_button, "pos", this);
       animServer->setDuration(500);
       animServer->setEndValue(QPoint(-10, 260));
       animServer->start();
       animClient->setDuration(500);
       animClient->setEndValue(QPoint(-10, 260));
       animClient->start();

       connect(animServer,&QAbstractAnimation::finished,this,[&](){server_card.cards_button->hide();server_card.clear();client_card.clear();});
       connect(animClient,&QAbstractAnimation::finished,this,[&](){client_card.cards_button->hide();server_card.clear();client_card.clear();});
       connect(animServer,SIGNAL(finished()),animServer,SLOT(deleteLater()));
       connect(animClient,SIGNAL(finished()),animClient,SLOT(deleteLater()));
       connect(animClient,SIGNAL(finished()),this,SLOT(change_geometry()));

      // mx.unlock();
}
//*************************************************************************************************
void Server ::play(){
         if(currentPlayer.get_countOfTurn()>1 && currentPlayer.get_countOfTurn()<7)caculateScore(0);
        // server pick cards first
        int turncount = currentPlayer.get_countOfTurn();
        sendCard = currentPlayer.creat_cards();
        currentPlayer.set_randomCards(sendCard,turncount);
        showCards(currentPlayer.playeCard);
        for(int i=0 ; i<pushButtons.size();i++){
            set_picture(pushButtons[i]);

        }
        writeToFileCards("sendCard.bin",sendCard);
        QFile file2("sendCard.bin");
        file2.open(QFile::ReadOnly | QFile::Text);
        QByteArray file_content2 = file2.readAll();
        mx.lock();
        socket->write(file_content2);
        socket->flush();
        mx.unlock();
        file2.close();
     /// making parrote cards
     ///
     whoShouldStartTheGameFirst();
     server_card.cards_button->setGeometry(290,290,101,141);
     client_card.cards_button->setGeometry(210,240,101,141);
     set_picture(server_card);
     set_picture(client_card);
     ClientOrServer::delay(1000);
     move_twoCards();
     //end
     ////guess label....
     ClientOrServer::delay(1000);
     guessLabel=new QLineEdit(this);
       guessLabel->setGeometry(120,310,391,51);
     QFont font_line("Algerian");
     guessLabel->setStyleSheet("font : 14pt;color: rgb(0,0,0);background:rgba(0,0,0,0);border:2px solid;border-color:#000;");
     guessLabel->setFont(font_line);
     guessLabel->setPlaceholderText("Enter your guess");
     for(auto& x:pushButtons)x.cards_button->setEnabled(false);
     guessLabel->show();
     connect(guessLabel,&QLineEdit::editingFinished,this,[&](){
     currentPlayer.set_guess(guessLabel->text().toInt());
     guessLabel->hide();
      for(auto& x:pushButtons)x.cards_button->setEnabled(true);
     //continue ro ham able kon
     });
     qDebug()<<currentPlayer.get_guess();

}
//***********************************************************************************************
void Server::caculateScore(int rivalScore){
    //age dorost gofte bashe....
    if(currentPlayer.get_guess()==currentPlayer.get_setWin()){
        //age gofte bashe 0 dast
        if(currentPlayer.get_guess()==0)currentPlayer.set_score(currentPlayer.get_score()+((currentPlayer.get_countOfTurn()-1)*10));
       else currentPlayer.set_score(currentPlayer.get_score()+(currentPlayer.get_guess()*10));
    }
    //age ghalat gofte bashe.....
    else{
        //age gofte bashe 0 dast
        if(currentPlayer.get_guess()==0)currentPlayer.set_score(currentPlayer.get_score()-((currentPlayer.get_countOfTurn()-1)*10));
        else currentPlayer.set_score(currentPlayer.get_score()-(abs(currentPlayer.get_guess()-currentPlayer.get_setWin())*10));
    }
    if(currentPlayer.get_countOfTurn()==7){
        if(rivalScore>currentPlayer.get_score()){
            cards server_order;
            server_order.setOrder("You Win");
            sendCard.push_back(server_order);
            writeToFileCards("sendCard.bin",sendCard);
            QFile file("sendCard.bin");
            file.open(QFile::ReadOnly | QFile::Text);
            QByteArray file_content = file.readAll();
            ////mutex bzar....
            socket->write(file_content);
            socket->flush();
            ////.........
            file.close();
            currentPlayer.set_lose(currentPlayer.get_lose()+1);
            endOfTheGame->setText("You lose");

        }

    else if(rivalScore<currentPlayer.get_score()){
        cards server_order;
        server_order.setOrder("You Lose");
        sendCard.push_back(server_order);
        writeToFileCards("sendCard.bin",sendCard);
        QFile file("sendCard.bin");
        file.open(QFile::ReadOnly | QFile::Text);
        QByteArray file_content = file.readAll();
        ////mutex bzar....
        socket->write(file_content);
        socket->flush();
        ////.........
        file.close();
        currentPlayer.set_win(currentPlayer.get_win()+1);
        endOfTheGame->setText("You win");

    }
        else endOfTheGame->setText("Equal");

        auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
        foundPlayer->set_lose(currentPlayer.get_lose());
        foundPlayer->set_win(currentPlayer.get_win());
        //foundPlayer->set_score(currentPlayer.get_score());
         writeToFile("myfile.bin");

    }
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    currentPlayer.set_setWin(0);

}
//**************************************************************************************************
void Server::on_continueTheGameButton_clicked(){
   currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);
   for(auto& x:pushButtons) delete x.cards_button;
   pushButtons.clear();
   continueTheGameButton->setEnabled(false);
   continueTheGameButton->hide();

   play();

}
//****************************************************************************************************
void Server::on_pushButton_9_clicked()
{

}
//****************************************************************************************************

void Server::on_pushButton_7_clicked()
{
    qDebug()<<"cos khar";
    cards server_order;
    server_order.setOrder("EXIT");
    sendCard.push_back(server_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    mx.lock();
    socket->write(file_content);
    socket->flush();
    mx.unlock();
    file.close();
    this->close();
    Skullking* newPage;
    newPage=new Skullking;
    newPage->show();
    Skullking::delay();
    newPage->Show_TextBrows();
}


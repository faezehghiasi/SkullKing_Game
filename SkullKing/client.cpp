#include"signup.h"
#include "client.h"
#include"signin.h"
#include "ui_client.h"
#include"clientorserver.h"
#include"menuselection.h"
#include <QDataStream>
#include<QFile>
#include<QDebug>
#include"skullking.h"
#include<QMutex>
QMutex socketLock;
QString Ip;
Client* cln;
Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    server_card.cards_button = new QPushButton(this);
    server_card.cards_button->setGeometry(210,240,101,141);
    client_card.cards_button = new QPushButton(this);
    client_card.cards_button->setGeometry(290,290,101,141);
    server_card.cards_button->hide();
    client_card.cards_button->hide();
    scoreLabel= new QLabel(this);
    scoreNumber= new QLabel(this);
    scoreLabel->setStyleSheet("font: 14pt Broadway;  color: rgb(255, 255, 255);background-color: rgb(7, 7, 7);");
    scoreNumber->setStyleSheet("font: 20pt Broadway;  color: rgb(13, 13, 13);");
    scoreLabel->setGeometry(10,50,81,31);
    scoreNumber->setGeometry(10,90,71,31);
    scoreLabel->setText("  score");
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    scoreLabel->show();
    scoreNumber->show();

    continueTheGameButton = new QPushButton("Wait for continue...",this);
    continueTheGameButton->setStyleSheet("background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;QPushButton#continueTheGameButton{background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;}QPushButton#continueTheGameButton:hover{ color:rgba(155,168,182,210) ;}QPushButton#continueTheGameButton:pressed{padding-left:5px; padding-top:5px;color:rgba(115 ,128,142,210);}");
    continueTheGameButton->setGeometry(150,300,301,141);
    continueTheGameButton->hide();


    endOfTheGame=new QPushButton("End the game",this);
    endOfTheGame->setStyleSheet("background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;QPushButton#continueTheGameButton{background-color:rgb(200, 129, 49); color: rgb(0, 0, 0); font: 15pt Stencil;border-color: rgb(85, 0, 0); border-radius:10px;}QPushButton#continueTheGameButton:hover{ color:rgba(155,168,182,210) ;}QPushButton#continueTheGameButton:pressed{padding-left:5px; padding-top:5px;color:rgba(115 ,128,142,210);}");
    endOfTheGame->setGeometry(150,300,301,141);
    endOfTheGame->hide();
    connect(endOfTheGame,&QPushButton::clicked,this,&Client::endOfGame);
    resume = new QPushButton("Resume",this);
    resume->setStyleSheet("background-color: rgb(101, 102, 104);color: rgb(0, 0, 0); font: 600 10pt Sitka Small Semibold;");
    resume->setGeometry(10,140,61,51);
    resume->setEnabled(false);
    resume->hide();
    connect(resume,SIGNAL(clicked()),this,SLOT(on_resumeButton_clicked()));
    gameStop = new QLabel(this);
    gameStop->setStyleSheet("color: rgb(0, 0, 0); font: 18pt Snap ITC;");
    gameStop->setText("Game Stoped");
    gameStop->setGeometry(200,270,221,51);
    gameStop->hide();

    returnButton=new QPushButton("Back to menu",this);
    returnButton->setGeometry(150,450,93,51);
    returnButton->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.0432692, y1:0.051, x2:0.952, y2:0.892045, stop:0.423077 rgba(152, 118, 63, 255), stop:1 rgba(255, 255, 255, 255));color: rgb(8, 8, 8);font: 900 16pt Bodoni MT Black;");
    returnButton->hide();
    returnButton->setEnabled(false);
    connect(returnButton,&QPushButton::clicked,this,&Client::on_returnButton);



}
//*****************************************************************************
void Client::creation(){
    socket  = new QTcpSocket;
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    QHostAddress add;
    add.setAddress(Ip);
    socket->connectToHost(add,9999);
    QMessageBox mb;
    if(!socket->waitForConnected(2000)){
     mb.critical(this,"Error","server not found");
    }
    else{
           ClientOrServer::changePage = true;
    }
}
//******************************************************************************
Client::~Client()
{
    delete ui;
}
//*****************************************************************************
void Client::connected() {
   qDebug()<<"client connected to server";
}
//*****************************************************************************
void Client::disconnected() {

}
//****************************************************************************
void Client::readyRead() {
       // reading file
       QByteArray file_content = socket->readAll();
       QFile file("recivedCard.bin");
       if(!file.open(QFile::WriteOnly | QFile::Text)){
           qDebug()<<"file can not open";
           return;
       }
       file.write(file_content);
       file .close();
       readFromFileCards("recivedCard.bin",recivedCard);
      /// end

       if(recivedCard.size()==1){
           if(recivedCard[0].getOrder()=="Start The Game"){
             emit startTheGame();
             return;
          }
           //****************************************
          else if(recivedCard[0].getOrder()=="EXIT"){

               currentPlayer.set_win(currentPlayer.get_win()+1);
               currentPlayer.set_coin(currentPlayer.get_coin()+100);
               auto it = find_if(listOfPlayer.begin(),listOfPlayer.end(),[&](auto p){
                   return(currentPlayer.get_username()==p.get_username());
               });
               it->set_win(currentPlayer.get_win());
               it->set_coin(currentPlayer.get_coin());
               writeToFile("myfile.bin");
               this->close();
               Skullking* newPage;
               newPage=new Skullking;
               newPage->show();
               Skullking::delay();
               newPage->Show_TextBrows();
           }
           //*******************************************
           else if(recivedCard[0].getOrder()=="You Win"){
                 currentPlayer.set_win(currentPlayer.get_win()+1);
                 currentPlayer.set_coin(currentPlayer.get_coin()+100);
                 auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
                 foundPlayer->set_win(currentPlayer.get_win());
                 foundPlayer->set_coin(currentPlayer.get_coin());
                 writeToFile("myfile.bin");
                 endOfTheGame->setText("You win");
           }
           //*********************************************
           else if(recivedCard[0].getOrder()=="You Lose"){
               currentPlayer.set_lose(currentPlayer.get_lose()+1);
               auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
               foundPlayer->set_lose(currentPlayer.get_lose());
               writeToFile("myfile.bin");
               endOfTheGame->setText("You lose");
           }
           //**********************************************
           else if(recivedCard[0].getOrder()=="STOP"){
               ui->pushButton_7->setEnabled(false);
               ui->pushButton_7->hide();
               resume->setEnabled(true);
               resume->show();
                gameStop->show();
                for(auto& x:pushButtons) x.cards_button->setEnabled(false);
           }
           //***********************************************
           else if(recivedCard[0].getOrder()=="RESUME"){
               resume->setEnabled(false);
               resume->hide();
               ui->pushButton_7->setEnabled(true);
               ui->pushButton_7->show();
                gameStop->hide();
               for(auto& x:pushButtons) x.cards_button->setEnabled(true);
           }
           //**********************************************
          else{
            //server send card
             server_card = recivedCard[0];
             if(!currentPlayer.get_starterOfEachRound()){
                 currentPlayer.set_turn(true);
                 availbleCards(server_card);
             }
             else {
                 buttons temp;
                 availbleCards(temp);
             }
             client_card.cards_button->setGeometry(290,290,101,141);
             server_card.cards_button->setGeometry(210,240,101,141);
             set_picture(server_card);
             ///calculateing the score
              if(!client_card.empty()&& !server_card.empty()){
                currentPlayer.calculate(server_card.thisCard);
                ClientOrServer::delay(2000);
                move_twoCards();
                if(currentPlayer.playeCard.size()==0){
                    if(currentPlayer.get_countOfTurn()==7){
                        for(auto& x:pushButtons) delete x.cards_button;
                        pushButtons.clear();
                        endOfTheGame->show();
                        returnButton->setEnabled(true);
                        returnButton->show();
                    }
                   else{ currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);
                   continueTheGameButton->show();}


                    //currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);
                    //continueTheGameButton->show();

                }
              }
             /// end
           }
           //***************************************************************
      }
  //************************************************************************************
      else if(recivedCard.size()==2){
           if(recivedCard[0].getNumber()<recivedCard[1].getNumber()) {
               currentPlayer.set_turn(true);
               currentPlayer.set_starterOfEachRound(true);
           }
           else{
               currentPlayer.set_turn(false);
               currentPlayer.set_starterOfEachRound(false);
           }
           server_card = recivedCard[0];
           client_card = recivedCard[1];
           client_card.cards_button->setGeometry(290,290,101,141);
           server_card.cards_button->setGeometry(210,240,101,141);
           continueTheGameButton->hide();
           set_picture(client_card);
           set_picture(server_card);
           ClientOrServer::delay(1000);
           move_twoCards();
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
           });
 //*************************************************************************************************
       }
      else{
           if (currentPlayer.get_countOfTurn()>1)calculateScore();
           for(auto& x:pushButtons) delete x.cards_button;
           pushButtons.clear();
           currentPlayer.set_randomCards(recivedCard,currentPlayer.get_countOfTurn());
           showCards(currentPlayer.playeCard);
           for(int i =0 ; i<pushButtons.size();i++){
              set_picture( pushButtons[i]);
           }
       }

}
//***************************************************************************
void Client::showCards(QList<cards> cCards){
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
       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
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
       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
       connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons4_clicked()));
       connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons5_clicked()));
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
       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
       connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons4_clicked()));
       connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons5_clicked()));
       connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons6_clicked()));
       connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons7_clicked()));
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

       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
       connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons4_clicked()));
       connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons5_clicked()));
       connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons6_clicked()));
       connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons7_clicked()));
       connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons8_clicked()));
       connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons9_clicked()));
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

       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
       connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons4_clicked()));
       connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons5_clicked()));
       connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons6_clicked()));
       connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons7_clicked()));
       connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons8_clicked()));
       connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons9_clicked()));
       connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons10_clicked()));
       connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons11_clicked()));
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
       connect(pushButtons[0].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons0_clicked()));
       connect(pushButtons[1].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons1_clicked()));
       connect(pushButtons[2].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons2_clicked()));
       connect(pushButtons[3].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons3_clicked()));
       connect(pushButtons[4].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons4_clicked()));
       connect(pushButtons[5].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons5_clicked()));
       connect(pushButtons[6].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons6_clicked()));
       connect(pushButtons[7].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons7_clicked()));
       connect(pushButtons[8].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons8_clicked()));
       connect(pushButtons[9].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons9_clicked()));
       connect(pushButtons[10].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons10_clicked()));
       connect(pushButtons[11].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons11_clicked()));
       connect(pushButtons[12].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons12_clicked()));
       connect(pushButtons[13].cards_button,SIGNAL(clicked()),this,SLOT(on_Buttons13_clicked()));

    }
              break;

  }




}
//***************************************************************************
void Client::availbleCards(buttons rivalCard){
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
//***********************************************************************************
void Client::on_Buttons0_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[0].thisCard);
    });
    if(it!=currentPlayer.playeCard.end()) currentPlayer.playeCard.erase(it);
    client_card = pushButtons[0];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[0].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    socketLock.lock();
    socket->write(file_content);
//    socket->waitForBytesWritten(2000);
    socket->flush();
    socketLock.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[0]);
    ClientOrServer::delay(1000);
    pushButtons[0].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
               continueTheGameButton->show();
          currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}
       }
    }

}
//**************************************************************************************************
void Client::on_Buttons1_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[1].thisCard);
    });
     if(it!=currentPlayer.playeCard.end()) currentPlayer.playeCard.erase(it);
     client_card = pushButtons[1];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[1].thisCard);
     writeToFileCards("sendCard.bin",sendCard);
     QFile file("sendCard.bin");
     file.open(QFile::ReadOnly | QFile::Text);
     QByteArray file_content = file.readAll();
     socketLock.lock();
     socket->write(file_content);
//      socket->waitForBytesWritten(2000);
     socket->flush();
    socketLock.unlock();
     file.close();
    currentPlayer.set_turn(false);
     move_oneCards(pushButtons[1]);
     ClientOrServer::delay(1000);
     pushButtons[1].clear();
     if(!client_card.empty()&& !server_card.empty()){
       currentPlayer.calculate(server_card.thisCard);
       ClientOrServer::delay(1000);
        move_twoCards();
        if(currentPlayer.playeCard.size()==0){
            ///////////////////////////////////////
            if(currentPlayer.get_countOfTurn()==7){
                for(auto& x:pushButtons) delete x.cards_button;
                pushButtons.clear();
                endOfTheGame->show();
                returnButton->setEnabled(true);
                returnButton->show();
            }
            ////////////////////////////////////////////////////
           else{
             continueTheGameButton->show();
             currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

        }
     }

}
//**************************************************************************************************
void Client::on_Buttons2_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[2].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
     client_card = pushButtons[2];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[2].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
//       socket->waitForBytesWritten(2000);
      socket->flush();
    socketLock.unlock();
      file.close();
      currentPlayer.set_turn(false);
      move_oneCards(pushButtons[2]);
      ClientOrServer::delay(1000);
      pushButtons[2].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{

              continueTheGameButton->show();
               currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }


}
//**************************************************************************************************
void Client::on_Buttons3_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[3].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[3];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[3].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     socketLock.lock();
    socket->write(file_content);
//     socket->waitForBytesWritten(2000);
    socket->flush();
    socketLock.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[3]);
    ClientOrServer::delay(1000);
    pushButtons[3].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
              continueTheGameButton->show();
               currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

       }
    }


}
//**************************************************************************************************
void Client::on_Buttons4_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[4].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[4];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[4].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     socketLock.lock();
    socket->write(file_content);
//     socket->waitForBytesWritten(2000);
    socket->flush();
    socketLock.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[4]);
    ClientOrServer::delay(1000);
    pushButtons[4].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
           continueTheGameButton->show();
            currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

       }
    }


}
//**************************************************************************************************
void Client::on_Buttons5_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[5].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[5];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[5].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     socketLock.lock();
    socket->write(file_content);
//     socket->waitForBytesWritten(2000);
    socket->flush();
    socketLock.unlock();
    file.close();
    currentPlayer.set_turn(false);
    move_oneCards(pushButtons[5]);
    ClientOrServer::delay(1000);
    pushButtons[5].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
         continueTheGameButton->show();
          currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

       }
    }


}
//**************************************************************************************************
void Client::on_Buttons6_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[6].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[6];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[6].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
//       socket->waitForBytesWritten(2000);
      socket->flush();
    socketLock.unlock();
      file.close();
      currentPlayer.set_turn(false);
      move_oneCards(pushButtons[6]);
      ClientOrServer::delay(1000);
      pushButtons[6].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
            continueTheGameButton->show();
             currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }


}
//**************************************************************************************************
void Client::on_Buttons7_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[7].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
     client_card = pushButtons[7];
     currentPlayer.set_selectedCard(client_card.thisCard);
     sendCard.push_back(pushButtons[7].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
//       socket->waitForBytesWritten(2000);
      socket->flush();
    socketLock.unlock();
      file.close();
      currentPlayer.set_turn(false);
      move_oneCards(pushButtons[7]);
      ClientOrServer::delay(1000);
      pushButtons[7].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
             continueTheGameButton->show();
              currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }


}
//**************************************************************************************************
void Client::on_Buttons8_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[8].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
      client_card = pushButtons[8];
      currentPlayer.set_selectedCard(client_card.thisCard);
      sendCard.push_back(pushButtons[8].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
//       socket->waitForBytesWritten(2000);
      socket->flush();
    socketLock.unlock();
      file.close();
     currentPlayer.set_turn(false);
      move_oneCards(pushButtons[8]);
      ClientOrServer::delay(1000);
      pushButtons[8].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
           continueTheGameButton->show();
            currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }


}
//**************************************************************************************************
void Client::on_Buttons9_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[9].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
      client_card = pushButtons[9];
      currentPlayer.set_selectedCard(client_card.thisCard);
      sendCard.push_back(pushButtons[9].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
//       socket->waitForBytesWritten(2000);
      socket->flush();
    socketLock.unlock();
      file.close();
     currentPlayer.set_turn(false);
      move_oneCards(pushButtons[9]);
      ClientOrServer::delay(1000);
      pushButtons[9].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
           continueTheGameButton->show();
            currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }


}
//**************************************************************************************************
void Client::on_Buttons10_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[10].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[10];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[10].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     socketLock.lock();
    socket->write(file_content);
//     socket->waitForBytesWritten(2000);
    socket->flush();
    socketLock.unlock();
    file.close();
     currentPlayer.set_turn(false);
    move_oneCards(pushButtons[10]);
    ClientOrServer::delay(1000);
    pushButtons[10].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
          continueTheGameButton->show();
           currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

       }
    }


}
//**************************************************************************************************
void Client::on_Buttons11_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[11].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[11];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[11].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
      socket->flush();
    socketLock.unlock();
      file.close();
       currentPlayer.set_turn(false);
      move_oneCards(pushButtons[11]);
      ClientOrServer::delay(1000);
      pushButtons[11].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
            continueTheGameButton->show();
             currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }

}
//**************************************************************************************************
void Client::on_Buttons12_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[12].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
    client_card = pushButtons[12];
    currentPlayer.set_selectedCard(client_card.thisCard);
    sendCard.push_back(pushButtons[12].thisCard);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
     socketLock.lock();
    socket->write(file_content);
    socket->flush();
    socketLock.unlock();
    file.close();
      currentPlayer.set_turn(false);
    move_oneCards(pushButtons[12]);
    ClientOrServer::delay(1000);
    pushButtons[12].clear();
    if(!client_card.empty()&& !server_card.empty()){
      currentPlayer.calculate(server_card.thisCard);
      ClientOrServer::delay(1000);
       move_twoCards();
       if(currentPlayer.playeCard.size()==0){
           ///////////////////////////////////////
           if(currentPlayer.get_countOfTurn()==7){
               for(auto& x:pushButtons) delete x.cards_button;
               pushButtons.clear();
               endOfTheGame->show();
               returnButton->setEnabled(true);
               returnButton->show();
           }
           ////////////////////////////////////////////////////
          else{
           continueTheGameButton->show();
            currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

       }
    }


}
//**************************************************************************************************
void Client::on_Buttons13_clicked(){

    if(!currentPlayer.get_turn()){
        QMessageBox MQ;
        MQ.warning(0,"","it's not you'r turn...");
        return;
    }
    auto it = find_if(currentPlayer.playeCard.begin(),currentPlayer.playeCard.end(),[&](auto p){
      return (p==pushButtons[13].thisCard);
    });
    if(it!=currentPlayer.playeCard.end())currentPlayer.playeCard.erase(it);
     client_card = pushButtons[13];
     currentPlayer.set_selectedCard(client_card.thisCard);
     sendCard.push_back(pushButtons[13].thisCard);
      writeToFileCards("sendCard.bin",sendCard);
      QFile file("sendCard.bin");
      file.open(QFile::ReadOnly | QFile::Text);
      QByteArray file_content = file.readAll();
     socketLock.lock();
      socket->write(file_content);
      socket->flush();
    socketLock.unlock();
      file.close();
        currentPlayer.set_turn(false);
      move_oneCards(pushButtons[13]);
      ClientOrServer::delay(1000);
      pushButtons[13].clear();
      if(!client_card.empty()&& !server_card.empty()){
        currentPlayer.calculate(server_card.thisCard);
        ClientOrServer::delay(1000);
         move_twoCards();
         if(currentPlayer.playeCard.size()==0){
             ///////////////////////////////////////
             if(currentPlayer.get_countOfTurn()==7){
                 for(auto& x:pushButtons) delete x.cards_button;
                 pushButtons.clear();
                 endOfTheGame->show();
                 returnButton->setEnabled(true);
                 returnButton->show();
             }
             ////////////////////////////////////////////////////
            else{
             continueTheGameButton->show();
             currentPlayer.set_countOfTurn(currentPlayer.get_countOfTurn()+1);}

         }
      }

}
//**************************************************************************************************
void Client :: set_client_card(struct buttons cCard){
    client_card =cCard;
}
//********************************************************************************************
void Client :: set_server_card(struct buttons sCard){
    server_card =sCard;
}
//********************************************************************************************
struct buttons Client::get_client_card(){
    return client_card;
}
//**********************************************************************************************
struct buttons Client::get_server_card(){
    return server_card;}
//**********************************************************************************************
void Client::set_picture(struct buttons crd){

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
//**********************************************************************************************************************
QVector<buttons> Client:: get_PushPuttons(){
    return pushButtons;
}
//**********************************************************************************************************************
void Client::move_oneCards(buttons& cCards){

    QPropertyAnimation *anim = new QPropertyAnimation(cCards.cards_button, "pos", this);
       anim->setDuration(500);
       anim->setEndValue(QPoint(290, 290));
       anim->start();
      connect(anim,&QAbstractAnimation::finished,this,[&](){cCards.cards_button->disconnect();cCards.cards_button->hide();set_picture(client_card);
          client_card.cards_button->setGeometry(290,290,101,141);
          server_card.cards_button->setGeometry(210,240,101,141);});

}
//**************************************************************************************************************************
void Client::move_twoCards(){

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



}
//*******************************************************************************************************************************************
void Client::calculateScore(){
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
    //auto foundPlayer=find_if(listOfPlayer.begin(),listOfPlayer.end(),[](auto x){return(x.get_username()==currentPlayer.get_username());});
   // foundPlayer->set_score(currentPlayer.get_score());
    //writeToFile("myfile.bin");
    scoreNumber->setText(QString::number(currentPlayer.get_score()));
    currentPlayer.set_setWin(0);
}
//*******************************************************************************************************************
void Client::on_pushButton_7_clicked()
{ if(currentPlayer.get_countOfStop()>1){
        QMessageBox MQ;
        MQ.warning(0,"","You are not allowed to do this");
        return;
    }
    currentPlayer.set_countOfStop(currentPlayer.get_countOfStop()+1);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_7->hide();
    resume->setEnabled(true);
    resume->show();
     gameStop->show();
    for(auto& x:pushButtons) x.cards_button->setEnabled(false);
     cards client_order;
     client_order.setOrder("STOP");
     sendCard.push_back(client_order);
     writeToFileCards("sendCard.bin",sendCard);
     QFile file("sendCard.bin");
     file.open(QFile::ReadOnly | QFile::Text);
     QByteArray file_content = file.readAll();
     socketLock.lock();
     socket->write(file_content);
     socket->flush();
      socketLock.unlock();
     file.close();

}
//********************************************************************************************************************
void Client::on_pushButton_8_clicked()
{
    cards client_order;
    client_order.setOrder("EXIT");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    socket->write(file_content);
    socket->flush();
    file.close();
    this->close();
    Skullking* newPage;
    newPage=new Skullking;
    newPage->show();
    Skullking::delay();
    newPage->Show_TextBrows();
}
//********************************************************************************************************************
void Client::endOfGame(){
    calculateScore();
    cards client_order;
    client_order.setNumber(currentPlayer.get_score());
    client_order.setOrder("SCORE");
    sendCard.push_back(client_order);
    writeToFileCards("sendCard.bin",sendCard);
    QFile file("sendCard.bin");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray file_content = file.readAll();
    /////mutex....
    socketLock.lock();
    socket->write(file_content);
    socket->flush();
    ////.......
    socketLock.unlock();
    file.close();
}
//*********************************************************************************************************************
void Client::on_resumeButton_clicked(){
    resume->setEnabled(false);
    resume->hide();
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_7->show();
     gameStop->hide();
    for(auto& x:pushButtons) x.cards_button->setEnabled(true);
     cards client_order;
     client_order.setOrder("RESUME");
     sendCard.push_back(client_order);
     writeToFileCards("sendCard.bin",sendCard);
     QFile file("sendCard.bin");
     file.open(QFile::ReadOnly | QFile::Text);
     QByteArray file_content = file.readAll();
     socketLock.lock();
     socket->write(file_content);
     socket->flush();
     socketLock.unlock();
     file.close();

}
//***********************************************************************************************
void Client::on_returnButton(){
    MenuSelection* newPage;
    newPage=new MenuSelection;
    this->close();
    newPage->show();
}

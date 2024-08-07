#include "player.h"
#include<QString>
#include<QRandomGenerator>
#include<QDebug>
#include<QFile>
#include<QMessageBox>
#include"signin.h"
#include"clientorserver.h"
using namespace std;
QVector<cards> recivedCard;
QVector<cards> sendCard;
QTextStream &operator<<(QTextStream& out ,cards p)
{   QString id = QString::number(p.getId());
    QString value = QString::number(p.getValue());
    QString number = QString::number(p.getNumber());
    out<<id<<"\n"<<value<<"\n"<<number<<"\n"<<p.getOrder()<<"\n";
    return out;
}
//*******************************************************
QTextStream& operator>>(QTextStream& in , cards& p){
     p.setId(in.readLine().toInt());
     p.setValue(in.readLine().toInt());
     p.setNumber(in.readLine().toInt());
     p.setOrder(in.readLine());
    return in;
}
//**********************************************************
void readFromFileCards(QString filename,QVector<cards>& card_val)
{
    card_val.clear();
    QFile myfile(filename);
    if(!myfile.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox Mb;
        Mb.critical(0,"File Situation","File didn't open!");
        return ;
    }
   QTextStream in(&myfile);
   cards newCard;
   for(int i=0 ; !in.atEnd();i++){
       in>>newCard;
       card_val.append(newCard);
   }
   myfile.close();
}
//*********************************************************
void writeToFileCards(QString filename,QVector<cards>& card_val)
{
     QFile myfile(filename);
    if(!myfile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"Not opened!";
        return ;
    }
   QTextStream out(&myfile);
   for(int i=0 ; i<card_val.size();i++)
   {
       out<<card_val[i];
  }
   myfile.flush();
   myfile.close();
   card_val.clear();
}
//************************************************************
Player::Player(QString name_val, QString username_val, QString password_val, QString address_val,
    QString phoneNumber_val, int win_val, int lose_val,int coin_val, bool score_val, bool turn_val,bool server_val,int countturn,int geuss_val,int setWin_val,bool starter_val) {
    name = name_val;
    username = username_val;
    password = password_val;
    address = address_val;
    phoneNumber = phoneNumber_val;
    win = win_val;
    lose = lose_val;
    coin = coin_val;
    score = score_val;
    turn = turn_val;
    server=server_val;
    countOfTurn=countturn;
    guess=geuss_val;
    setWin=setWin_val;
    starterOfEachRound = starter_val;


}
//***********************************************
int Player::get_win() {
    return win;
}
//***********************************************
int Player::get_lose() {
    return lose;
}
//************************************************
QString Player::get_name() {
    return name;
}
//************************************************
QString Player::get_username() {
    return username;
}
//***********************************************
QString Player::get_password() {
    return password;
}
//************************************************
QString Player:: get_address() {
    return address;
}
//************************************************
QString Player::get_phoneNumber() {
    return phoneNumber;
}
//**********************************************
int Player::get_countOfTurn(){return countOfTurn;}
//*****************************************************
void Player::set_countOfTurn(int n){countOfTurn=n;}
//************************************************
void Player::set_win(int win_val) {
    win = win_val;
}
//*************************************************
void Player::set_lose(int lose_val) {
    lose = lose_val;
}
//*************************************************
void Player::set_name(QString name_val) {
    name = name_val;
}
//**************************************************
void Player::set_username(QString usename_val) {
    username = usename_val;
}
//**************************************************
void Player :: set_password(QString password_val) {
    password = password_val;
}
//***************************************************
void Player:: set_address(QString address_val) {
    address = address_val;
}
//*****************************************************
void Player:: set_phoneNumber(QString phoneNumber_val) {
    phoneNumber = phoneNumber_val;
}
//*****************************************************
int Player::get_coin(){
    return coin;
}
//******************************************************
void Player::set_coin(int coin_val){
    coin = coin_val;
}
//*******************************************************
bool Player::get_server(){
  return server;
}
//********************************************************
void Player::set_server(bool server_val){
    server =server_val;
}
//*********************************************************
bool Player::get_starterOfEachRound(){
    return starterOfEachRound;
}
//**********************************************************
void Player::set_starterOfEachRound(bool starter_val){
    starterOfEachRound = starter_val;
}
//***********************************************************
QVector<cards> Player::set_randomCards(QVector<cards>& exitCards, int countOfTurn){

    for(int i =0;i<countOfTurn*2;i++){
        int index = QRandomGenerator::global()->generate()%exitCards.size();
        auto it = exitCards.begin();
        it+=index;
        playeCard.push_back(exitCards[index]);
        exitCards.erase(it);
    }
     sort(playeCard.begin(),playeCard.end(),[](auto a ,auto b){return a.getId()<b.getId();});
    return exitCards;
}
//******************************************************************
QVector<cards> Player::creat_cards(){
    QVector<cards> allOfCards;
    for(int i=1 ; i<5;i++)
    {
        for(int j=1;j<9;j++)
        {
            commonCards tempCard(i,j);
            allOfCards.append(tempCard);
        }
    }
    for(int i=5 ;i<8;i++)
    {
        if(i==7){
          for(int j=0 ;j<4 ;j++){
              importantCards tempCard(i);
              allOfCards.append(tempCard);
          }
        }
        else{
            for(int j=0 ;j<3 ;j++){
                importantCards tempCard(i);
                allOfCards.append(tempCard);
            }
        }
    }
    return allOfCards;

}
//*******************************************************************
int Player::get_setWin(){return setWin;}
//******************************************************************
int Player::get_score(){return score;}
//*******************************************************************
int Player::get_guess(){return guess;}
//********************************************************************
bool Player::get_turn(){return turn;}
//***********************************************************************
cards Player::get_selectedCard(){return selectedCard;}
//**********************************************************************
void Player::set_selectedCard(cards c){selectedCard=c;}
//**********************************************************************
void Player::set_setWin(int sw){setWin=sw;}
//******************************************************************
void Player::set_score(int s){score=s;}
//*******************************************************************
void Player::set_guess(int g){guess=g;}
//*******************************************************************
void Player::set_turn(bool b){turn=b;}
//********************************************************************
void Player::calculate(cards rivalCard){

    if(get_server())for(auto& x:srv->get_buttons())x.cards_button->setEnabled(true);
    else for(auto& x:cln->get_PushPuttons())x.cards_button->setEnabled(true);

    if((selectedCard.getId()==1||selectedCard.getId()==2||selectedCard.getId()==3)&&(rivalCard.getId()==1||rivalCard.getId()==2||rivalCard.getId()==3))
    {
        if(selectedCard.getId()==rivalCard.getId()){

            ////************************************************
            if(selectedCard.getNumber()>rivalCard.getNumber()){

                currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                currentPlayer.set_turn(true);
                currentPlayer.set_starterOfEachRound(true);
                if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                else cln->scoreNumber->setText(QString::number(get_score()));
                return;
            }
        }

        else{
            ////*************************************************
            if(currentPlayer.get_starterOfEachRound()){

                currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                currentPlayer.set_turn(true);
                currentPlayer.set_starterOfEachRound(true);
                if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                else cln->scoreNumber->setText(QString::number(get_score()));
                return;
            }
        }


        if(get_server()){
            srv->get_server_card().clear();
            srv->get_client_card().clear();
        }
        else{
            cln->get_server_card().clear();
            cln->get_client_card().clear();
        }

    }
    ////********************************************************************************************************

    else if(selectedCard.getId()==4){
        ////*****************************************************
        if(rivalCard.getId()==1||rivalCard.getId()==2||rivalCard.getId()==3){
            currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
            currentPlayer.set_turn(true);
            currentPlayer.set_starterOfEachRound(true);
            if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
            else cln->scoreNumber->setText(QString::number(get_score()));
            return;
        }
        ////**********************************************************
        else if(rivalCard.getId()==4){
            if(selectedCard.getNumber()>rivalCard.getNumber()){
                currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                currentPlayer.set_turn(true);
                currentPlayer.set_starterOfEachRound(true);
                if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                else cln->scoreNumber->setText(QString::number(get_score()));
                return;
            }

        }


        ////************************************************************
        if(get_server()){
            srv->get_server_card().clear();
            srv->get_client_card().clear();
        }
        else{
            cln->get_server_card().clear();
            cln->get_client_card().clear();
        }

    }
    ////***************************************************************************************************
    else if(selectedCard.getId()==5||selectedCard.getId()==6||selectedCard.getId()==7){
        if(rivalCard.getId()==1||rivalCard.getId()==2||rivalCard.getId()==3||rivalCard.getId()==4){
            currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
            currentPlayer.set_turn(true);
            currentPlayer.set_starterOfEachRound(true);
            if(selectedCard.getId()==5)currentPlayer.set_score(currentPlayer.get_score()+15);
            else if(selectedCard.getId()==6)currentPlayer.set_score(currentPlayer.get_score()+20);
            else if(selectedCard.getId()==7)currentPlayer.set_score(currentPlayer.get_score()+10);
            if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
            else cln->scoreNumber->setText(QString::number(get_score()));
            return;
        }
        else {
            if(selectedCard.getId()==5){
                switch(rivalCard.getId()){
                case 5:if(get_starterOfEachRound()){
                        currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                        currentPlayer.set_score(currentPlayer.get_score()+30);
                        currentPlayer.set_turn(true);
                        currentPlayer.set_starterOfEachRound(true);
                        if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                        else cln->scoreNumber->setText(QString::number(get_score()));
                        return;
                    }
                    break;
                case 7:{
                    currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                    currentPlayer.set_score(currentPlayer.get_score()+25);
                    currentPlayer.set_turn(true);
                    currentPlayer.set_starterOfEachRound(true);
                    if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                    else cln->scoreNumber->setText(QString::number(get_score()));
                    return;
                }
                break;
                }
            }
            else if(selectedCard.getId()==6){
                switch(rivalCard.getId()){
                case 5:{
                    currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                    currentPlayer.set_score(currentPlayer.get_score()+35);
                    currentPlayer.set_turn(true);
                    currentPlayer.set_starterOfEachRound(true);
                    if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                    else cln->scoreNumber->setText(QString::number(get_score()));
                    return;
                }

                break;
                case 6:if(get_starterOfEachRound()){
                        currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                        currentPlayer.set_score(currentPlayer.get_score()+40);
                        currentPlayer.set_turn(true);
                        currentPlayer.set_starterOfEachRound(true);
                        if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                        else cln->scoreNumber->setText(QString::number(get_score()));
                        return;
                    }

                    break;


                }
            }
            else{
                switch(rivalCard.getId()){

                case 6:{
                    currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                    currentPlayer.set_score(currentPlayer.get_score()+30);
                    currentPlayer.set_turn(true);
                    currentPlayer.set_starterOfEachRound(true);
                    if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                    else cln->scoreNumber->setText(QString::number(get_score()));
                    return;
                }
                break;
                case 7:
                    if(get_starterOfEachRound())
                    {currentPlayer.set_setWin(currentPlayer.get_setWin()+1);
                        currentPlayer.set_score(currentPlayer.get_score()+20);
                        currentPlayer.set_turn(true);
                        currentPlayer.set_starterOfEachRound(true);
                        if(get_server())srv->scoreNumber->setText(QString::number(get_score()));
                        else cln->scoreNumber->setText(QString::number(get_score()));
                        return;
                    }

                    break;
                }
            }
        }
        if(get_server()){
            srv->get_server_card().clear();
            srv->get_client_card().clear();
        }
        else{
            cln->get_server_card().clear();
            cln->get_client_card().clear();
        }

    }
    ////************************************************
    currentPlayer.set_turn(false);
    currentPlayer.set_starterOfEachRound(false);

}
//************************************************************************************
Player& Player::operator=(Player p){
    username=p.get_username();
    password=p.get_password();
    name=p.get_name();
    address=p.get_address();
    phoneNumber=p.get_phoneNumber();
    win=p.get_win();
    lose=p.get_lose();
    coin=p.get_coin();
    server=p.get_server();
    score=p.get_score();
    turn=p.get_turn();
    countOfTurn=p.get_countOfTurn();
    guess=p.get_guess();
    setWin=p.get_setWin();
    return *this;
}
//************************************************************************************
void Player::clearDataOfGame(){
    server =false;
    score = 0;
    turn = false;
    countOfTurn = 1;
    guess = 0;
    setWin = 0;
    playeCard.clear();
    starterOfEachRound = false;
}


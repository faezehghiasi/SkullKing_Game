#ifndef PLAYER_H
#define PLAYER_H
#include<QString>
#include<QVector>
#include"cards.h"
#include"common.h"
#include"importantcards.h"
#include<QList>
#include<QTextStream>
#include<QPushButton>
extern QVector<cards> recivedCard;
extern QVector<cards> sendCard;
struct buttons{
    QPushButton* cards_button;
    cards thisCard;
    //overload =
    buttons& operator =(cards c){
        this->thisCard = c;
        return *this;
    }
    void clear(){
        thisCard.setNumber(0);
        thisCard.setId(0);
        thisCard.setValue(0);
        thisCard.setOrder("");
    }
    bool empty(){
        return(thisCard.getId()==0&&thisCard.getNumber()==0&&thisCard.getValue()==0&&thisCard.getOrder()=="");
    }

};
QTextStream &operator<<(QTextStream& out , cards p);
QTextStream& operator>>(QTextStream& in , cards& p);
void readFromFileCards(QString filename,QVector<cards>&);
void writeToFileCards(QString filename,QVector<cards>&);
using namespace std;
class Player
{
private:
    QString name;
    QString username;
    QString password;
    QString address;
    QString phoneNumber;
    int win;
    int lose;
    int coin;

    bool server;
    int score;
    bool turn;
    int countOfTurn;
    int guess;
    int setWin;
    cards selectedCard;
public:
     QList<cards> playeCard;
    Player(QString name_val = "", QString username_val = "", QString password_val = "", QString address_val = "",
        QString phoneNumber_val = "", int win_val = 0, int lose_val = 0,int coin_val = 0,bool score_val = false, bool turn_val = false,bool server_val=false,int countturn=1,int geuss_val=0,int setWin_val=0);
    int get_win();
    //geter seter vase selectedcard
    cards get_selectedCard();
    int get_lose();
    int get_score();
    QString get_name();
    QString get_username();
    QString get_password();
    QString get_address();
    QString get_phoneNumber();
    bool get_server();
    bool get_turn();
    int get_guess();
    int get_setWin();
    int get_coin();
    int get_countOfTurn();
    void set_win(int win_val);
    void set_lose(int lose_val);
    void set_name(QString name_val);
    void set_username(QString usename_val);
    void set_password(QString password_val);
    void set_address(QString address_val);
    void set_phoneNumber(QString phoneNumber_val);
    void set_coin(int coin_val );
    void set_server(bool server_val);
    void set_countOfTurn(int cturn);
    void set_guess(int g);
    void set_setWin(int sw);
    void set_turn(bool);
    void set_selectedCard(cards c);
    void set_score(int s);
    QVector<cards> set_randomCards(QVector<cards>& exitCards,int coutOfTurn);
    QVector<cards> creat_cards();
    void calculate(cards);
};

#endif // PLAYER_H

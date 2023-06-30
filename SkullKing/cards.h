#ifndef CARDS_H
#define CARDS_H
#include<QString>
class cards
{
protected:
    int id;
    int value;
    int number;
    QString order;
public:
    cards();
    cards(int Id, int Numebr);
    //show function //i think it will be pure virtual
    //move function //i think it will be pure virtual
    int getId();
    int getNumber();
    int getValue();
    QString getOrder();
    void setId(int);
    void setNumber(int);
    void setValue(int);
    void setOrder(QString order_val);
    cards& operator=(cards c);
};

#endif // CARDS_H

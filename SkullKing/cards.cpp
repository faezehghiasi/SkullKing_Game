#include "cards.h"
//******************************************************************************************
cards::cards() { id = 0; value = 0; number = 0; }
//******************************************************************************************
cards::cards(int Id, int Number) {
    id = Id;
    number = Number;
    value = 0;
}
//******************************************************************************************
int cards::getId() { return id; }
//******************************************************************************************
int cards::getNumber() { return number; }
//******************************************************************************************
int cards::getValue() { return value; }
//******************************************************************************************
void cards::setId(int id_val) {id=id_val;  }
//******************************************************************************************
void cards::setNumber(int number_val) { number=number_val; }
//******************************************************************************************
void cards::setValue(int value_val) { value=value_val; }
//******************************************************************************************
void cards::setOrder(QString order_val){order = order_val;}
//******************************************************************************************
QString cards::getOrder(){return order;}
cards& cards::operator=(cards c){
    id=c.getId();
    number=c.getNumber();
    value = c.getValue();
    order = c.getOrder();
    return *this;
}
//******************************************************************************************
bool cards::operator==(cards c){
    return(id==c.getId()&&number==c.getNumber()&&value==c.getValue()&&order==c.getOrder());
}

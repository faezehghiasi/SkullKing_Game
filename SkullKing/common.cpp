#include "common.h"
//******************************************************************************************
commonCards::commonCards(int Id, int Number) :cards(Id, Number) {
    if (id == 4) value = 9;//value of hokm
    else value = number;
    //id=1 is treasure
    //id=2 is map
    //id=3 is parrot
    //id=4 is hokm
}

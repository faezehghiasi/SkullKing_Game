#include "importantcards.h"

importantCards::importantCards(int Id) :cards(Id, 0) {
    switch (id) {
    //king
    case 5:value = 15; break;
    //queen
    case 6:value = 20; break;
    //pirate
    case 7:value = 10; break;
    }
}

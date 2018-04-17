
#include "classification.h"


Classification::Classification(unsigned int type,//
                                unsigned int amount,//
                                unsigned int width){ //
    this-> type = type;
    this-> amount = amount;
    this-> width = width;
}

int Classification::get_type() const{
    return this->type;
}

unsigned int Classification::get_amount() const{
    return this-> amount;
}

   
unsigned int Classification::get_width() const{
    return this-> width;
}

void Classification::take(unsigned int amount_taken){
    if (amount_taken > this->amount){
        throw std::runtime_error( //
            "Impossible to take more scrows than classification has\n");
    }
    this-> amount = this->amount -amount_taken;
}


std::ostream& operator<< (std::ostream &out, const Classification &cl){
    out << "\nClassification = type: " << cl.type << " amount:  " //
    << cl.amount << " widt: " << cl.width << endl;
    return out;
}

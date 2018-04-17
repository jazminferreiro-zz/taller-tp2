#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <iostream>
using std::ios;
using std::endl;


class Classification{
    int type;
    unsigned int amount;
    unsigned int width;

    public:
    Classification(unsigned int type, unsigned int amount, unsigned int width);

    int get_type() const;

    unsigned int get_amount() const;

   
    unsigned int get_width() const;

    void take(unsigned int amount_taken);

    friend std::ostream& operator<<(std::ostream &out,const Classification &cl);
};

std::ostream& operator<< (std::ostream &out, const Classification &cl);

#endif
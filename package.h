
#ifndef PACKAGE_H_
#define PACKAGE_H

#include <string.h>
#include <iostream>

using std::string;
using std::cout;



#define ERROR 1
#define SUCCESS 0


#include "classification.h"


class Package {
private:
    int type_id;
    string name;
    unsigned int limit;
    unsigned int current_screw_amount;
    unsigned int * width_list;

    bool is_par(unsigned int n );
    static int comp_unsigned_int(const void *a, const void *b);
    unsigned int get_median_value();
    void init();
   

public:
    Package(int type_id, string name, int limit);

    //Constructor por movimiento
    Package(Package&& other);

    //Constructor por copia
    Package(const Package &other);

    //Asignacion por movimiento
    Package& operator=(Package&& other);

    //Asigmacion por copia
    Package& operator=(const Package &other);


    unsigned int get_limit()const;

    int get_type_id() const;

     unsigned int get_current_screw_amount() const;

    bool operator <(const Package &other) const;


    bool can_be_added(Classification & cl) const;

    int add_screws(Classification & cl, unsigned int amount_to_add);

    void print_as_remnant();


    ~Package();

    friend std::ostream& operator<< (std::ostream &out, const Package &pac);
};

std::ostream& operator<< (std::ostream &out, const Package &pac);

#endif

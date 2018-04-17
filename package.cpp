#include <string.h>
#include <string>
#include "package.h"
bool Package::is_par(unsigned int n ){
    return (n%2 == 0);
}

int Package::comp_unsigned_int(const void *a, const void *b){
    int x = *(unsigned int *)a;
    int y = *(unsigned int *)b;
    if (x > y)
        return 1;
    else if (x == y)
        return 0;
    else
        return -1;
}

unsigned int Package::get_median_value(){
        if (is_par(this->limit)){
            int pos = (this-> limit)/2;
            unsigned int num1 = this->width_list[pos -1];
            unsigned int num2 = this->width_list[pos];
            return (num1 +num2)/2;  
        }
        int pos = (this->limit)/2;
        return this->width_list[pos];
    }
    
void Package::init(){
    this->current_screw_amount = 0;
    memset(this->width_list, 0, limit*sizeof(unsigned int));
}
   

Package::Package(int type_id, string name, int limit){
        this->type_id = type_id;
        this->name = name;
        this->limit = limit;
        this->width_list = (unsigned int*)malloc(limit*sizeof(unsigned int));
        init();
    }

    //Constructor por movimiento
Package::Package(Package&& other) {  
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = other.width_list;

        other.type_id = -1;
        other.name = "null";
        other.limit = 0;
        other.current_screw_amount = 0;
        other.width_list = nullptr;
}

//Constructor por copia
Package::Package(const Package &other) {
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list =(unsigned int*)malloc(//
            other.limit*sizeof(unsigned int));
        memcpy(this->width_list, other.width_list,//
            other.limit*sizeof(unsigned int));
}

//Asignacion por movimiento
Package& Package::operator=(Package&& other) {
        if (this == &other) {
            return *this; // other is myself!
        }
        if (this->width_list){
            free(this->width_list);
        }
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = other.width_list;

        other.type_id = -1;
        other.name = "null";
        other.limit = 0;
        other.current_screw_amount = 0;
        other.width_list = nullptr;
        return *this;
}

//Asigmacion por copia
Package& Package::operator=(const Package &other) {
        if (this == &other) {
            return *this; // other is myself!
        }
        if (this->width_list){
            free(this->width_list);
        }
        
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = (unsigned int*)//
        malloc(other.limit*sizeof(unsigned int));
        memcpy(this->width_list, //
        other.width_list,other.limit*sizeof(unsigned int) );
    return *this;
}


unsigned int Package::get_limit() const{
        return limit;
    }

    int Package::get_type_id() const{
        return this->type_id;
    }

     unsigned int Package::get_current_screw_amount() const {
        return current_screw_amount;
    }

    bool Package::operator <(const Package &other) const{
        return this->type_id < other.get_type_id();
    }


    bool Package::can_be_added(Classification & cl) const{
        return this->type_id == cl.get_type();
    }


    int Package::add_screws(Classification & cl, unsigned int amount_to_add){
        cl.take(amount_to_add);

        for (unsigned int i = 0; i < amount_to_add; i++){
            //cout << "agregando "<< amount_to_add<<"\n";
            this->width_list[this->current_screw_amount] = cl.get_width();
            this->current_screw_amount++;
        }

        qsort(this->width_list, this->current_screw_amount, 
        sizeof(unsigned int), comp_unsigned_int);

        //print_with();

        if (this->current_screw_amount >= this->limit){
            cout << "Paquete listo: "
            << current_screw_amount 
            <<" tornillos de tipo " << name 
            << " (mediana: " << get_median_value()<< ")"
            << endl;
            init();
        }
        return SUCCESS;
    }

  
void Package:: print_as_remnant(){
        cout << "* " << this-> current_screw_amount
         << " tornillos de tipo " << this->name << endl; 
}



Package::~Package(){
    if (this->width_list){
        free(this->width_list);
    }
}

std::ostream& operator<< (std::ostream &out, const Package &pac)
{
    cout << " package {  id: " <<pac.type_id 
    << " name: " << pac.name 
    << " current_amount: " <<pac.current_screw_amount 
    << " limit: " <<pac.limit << " }\n";
    return out;
}



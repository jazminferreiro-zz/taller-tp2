
#include "package_protector.h"

Package_protector::Package_protector(std::list<Package>* packages){
    this->packages = packages;
}

void Package_protector::prepare_package(Classification cl){
    for (std::list<Package>::iterator pac_iter = (*packages).begin(); 
        pac_iter != (*packages).end(); ++pac_iter){
        if ((*pac_iter).can_be_added(cl)){
            try_to_add_screws(*pac_iter, cl);
            return;
        }   
    }
    cerr << "Tipo de tornillo invalido: " << cl.get_type() << endl; 
}

int Package_protector::try_to_add_screws(Package &pac, Classification & cl){
  
    m.lock();

    if (( !pac.can_be_added(cl) ) || //
        (pac.get_current_screw_amount() >= pac.get_limit()) ){
    return ERROR;
    }


    unsigned int amount_to_add = 0;

    if ((pac.get_current_screw_amount() + cl.get_amount())//
        <= pac.get_limit() ) { 
        amount_to_add = cl.get_amount();
    } else{ 
        amount_to_add = pac.get_limit() - pac.get_current_screw_amount();
    }

    pac.add_screws(cl, amount_to_add);


    if ( cl.get_amount() > 0 ){
        m.unlock();
        try_to_add_screws(pac,cl);
    }

    m.unlock();
    return SUCCESS;
}


#ifndef PACKAGE_PROTECTOR_H_
#define PACKAGE_PROTECTOR_H_

#include <iostream>

using std::cerr;
using std::endl;
using std::ios;



#include <list>
#include <mutex>

#include "configuration.h"
#include "classification.h"



#define ERROR 1
#define SUCCESS 0

class Package_protector { 
private:
    std::mutex m; 
    std::list<Package> * packages;

public:
    Package_protector(std::list<Package>* packages);
    void prepare_package(Classification cl);
    int try_to_add_screws(Package &pac, Classification & cl);
};
#endif
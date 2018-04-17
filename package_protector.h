#ifndef PACKAGE_PROTECTOR_H_
#define PACKAGE_PROTECTOR_H_

#include "configuration.h"
#include "classification.h"
#include <list>
#include <mutex>
#include <iostream>

using std::cerr;
using std::endl;
using std::ios;

#define ERROR 1
#define SUCCESS 0

class Package_protector { 
private:
    std::mutex m; 
    std::list<Package> * packages;

public:
explicit Package_protector(std::list<Package>* packages);
    void prepare_package(Classification cl);
    int try_to_add_screws(Package &pac, Classification & cl);
};
#endif

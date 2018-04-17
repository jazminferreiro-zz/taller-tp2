
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H

#include "package.h"
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
using std::ios;
using std::ifstream;
using std::streampos;



class Configuration{
    char * config_file_name;
    ifstream config_file;

public:
    explicit Configuration(char config_file_name[]);
    std::list<Package> get_packages();
};

#endif

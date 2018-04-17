#include <string>
#include <list>
#include "configuration.h"


Configuration::Configuration(char config_file_name[]){
    this->config_file_name = config_file_name;
}


std::list<Package> Configuration::get_packages(){
    std::list<Package> packages = {};

    int id, limit;
    string name;
    
    ifstream config_file(this->config_file_name);
    std::string line;

    while (std::getline(config_file, line)){
        std::istringstream linereader(line, std::ios::binary);

        linereader >> id;

        linereader.ignore(line.size(), '=');
        
        std::getline(linereader, name, ',');

        linereader >> limit;

        Package pac(id,name,limit);

        packages.push_back(pac);
    }
    
    return std::move(packages);
}

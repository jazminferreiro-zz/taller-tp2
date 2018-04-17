#include "classification.h"
#include "classifier_device.h"
#include "package_protector.h"
#include "screw_packager.h"
#include <string>
#include <list>
#include <vector>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

#define ERROR 1
#define SUCCESS 0
#define CONFIG_FILE_NAME 1
#define START_INDEX 2
#define EXPECTED_OPERANDS_NUM 3


int main(int argc, char * argv[] ){
    if (argc < EXPECTED_OPERANDS_NUM){
        cerr << "invalid number of operations\n";
        return ERROR;
    }

    Configuration configuration(argv[CONFIG_FILE_NAME]);
    std::list<Package> packages = configuration.get_packages();


    Package_protector protector(&packages);
 
    std::vector<Thread*> threads;

    for (int i = START_INDEX; i < argc; ++i) {
        threads.push_back(new Screw_packager(argv[i],protector));
    }

    for (int i = 0; i < argc-START_INDEX; ++i) {
        threads[i]->start();
    }

    for (int i = 0; i < argc-START_INDEX; ++i) {
        threads[i]->join();
        delete threads[i];
    }

    packages.sort();

    
    cout << "# Informe de remanentes"<< endl;

    for (std::list<Package>::iterator pac_iter = packages.begin(); 
        pac_iter != packages.end(); ++pac_iter){
        (*pac_iter).print_as_remnant(); 
    }
    return 0;
}



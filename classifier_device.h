
#ifndef CLASSIFIER_DEVICE_H
#define CLASSIFIER_DEVICE_H

#include "classification.h"
#include <string>
#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::streampos;


#define ERROR 1
#define SUCCESS 0
class Classifier_device{
private:
    char * classifier_file_name;
    std::ifstream classifier_ifstream;
    std::streampos size;
    std::string classifier_device;
    bool is_connected;
    bool has_finished;
    

    bool are_all_bits_one(char * buffer);


public:
    explicit Classifier_device(char * classifier_file_name);

    bool has_reach_end();

    bool is_still_connect();

    int connect();

    Classification get_classification();
};

#endif


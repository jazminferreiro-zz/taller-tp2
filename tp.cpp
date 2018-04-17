

#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;



#include <fstream>
#include <typeinfo>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <string.h>
#include <thread>
#include <mutex>




#define ERROR 1
#define SUCCESS 0
#define CONFIG_FILE_NAME 1
#define START_INDEX 2
#define EXPECTED_OPERANDS_NUM 3


#include "classification.h"
#include "classifier_device.h"
#include "package_protector.h"





//////////////////////////////////////////////////////////////////////
class Thread {
    private:
        std::thread thread;
 
    public:
        Thread() {}

        void start() {
            thread = std::thread(&Thread::run, this);
        }

        void join() {
            thread.join();
        }

        virtual void run() = 0;
        virtual ~Thread() {}

        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        Thread(Thread&& other) {
            this->thread = std::move(other.thread);
        }

        Thread& operator=(Thread&& other) {
            this->thread = std::move(other.thread);
            return *this;
        }
};




class Screw_packager : public Thread {
    private:
        Classifier_device classifier_device;
        Package_protector &protector;

    public:
        Screw_packager(char * classifier_name, Package_protector & protector) :
            classifier_device(classifier_name), protector(protector) {
                this->classifier_device.connect();
            }

        virtual void run() {
            while ( classifier_device.is_still_connect()//
                && !classifier_device.has_reach_end() ){
                try{
                    Classification cl = classifier_device.get_classification();
                    protector.prepare_package(cl);
                } catch(int e){
                    continue;
                }
            }
        }
};


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
        threads.push_back(new Screw_packager(argv[i],
                                    protector));
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



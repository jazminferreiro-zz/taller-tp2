#include "screw_packager.h"

void Thread::start() {
    thread = std::thread(&Thread::run, this);
}

void Thread::join() {
    thread.join();
}

Screw_packager::Screw_packager(char * classifier_name, Package_protector & protector) :
    classifier_device(classifier_name), protector(protector) {
    this->classifier_device.connect();
}

void Screw_packager::run() {
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




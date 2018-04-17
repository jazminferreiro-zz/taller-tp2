#ifndef THREAD_H_
#define THREAD_H_

#include<thread>
#include "package_protector.h"
#include "classifier_device.h"

class Thread {
    private:
        std::thread thread;
 
    public:
        Thread() {}

        void start();

        void join();

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
        Screw_packager(char * classifier_name, Package_protector & protector);
        virtual void run();
};
#endif

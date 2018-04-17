

#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;
using std::streampos;


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




///////////////////////////////////////////////////////////////////////////////

class Package {
    int type_id;
    string name;
    unsigned int limit;
    unsigned int current_screw_amount;
    unsigned int * width_list;

    bool is_par(unsigned int n ){
        return (n%2 == 0);
    }


    static int comp_unsigned_int(const void *a, const void *b){
        int x = *(unsigned int *)a;
        int y = *(unsigned int *)b;
        if (x > y)
            return 1;
        else if (x == y)
            return 0;
        else
            return -1;
    }

    unsigned int get_median_value(){
        if (is_par(this->limit)){
            int pos = (this-> limit)/2;
            unsigned int num1 = this->width_list[pos -1];
            unsigned int num2 = this->width_list[pos];
            return (num1 +num2)/2;  
        }
        int pos = (this->limit)/2;
        return this->width_list[pos];
    }
    
    void init(){
        this-> current_screw_amount = 0;
        memset(this->width_list, 0, limit*sizeof(unsigned int));
    }

   


    public:
    Package(int type_id, string name, int limit){
        this->type_id = type_id;
        this->name = name;
        this->limit = limit;
        this->width_list = (unsigned int*)malloc(limit*sizeof(unsigned int));
        init();
    }

    unsigned int get_limit(){
        return limit;
    }

    int get_type_id() const{
        return this->type_id;
    }

    bool operator <(const Package &other) const{
        return this->type_id < other.get_type_id();
    }


    unsigned int get_current_screw_amount(){
        return current_screw_amount;
    }

    bool can_be_added(Classification & cl){
        return this->type_id == cl.get_type();
    }


    int add_screws(Classification & cl, unsigned int amount_to_add){
        cl.take(amount_to_add);

        for (unsigned int i = 0; i < amount_to_add; i++){
            //cout << "agregando "<< amount_to_add<<"\n";
            this->width_list[this->current_screw_amount] = cl.get_width();
            this->current_screw_amount++;
        }

        qsort(this->width_list, this->current_screw_amount, 
        sizeof(unsigned int), comp_unsigned_int);

        //print_with();

        if (this->current_screw_amount >= this->limit){
            cout << "Paquete listo: "
            << current_screw_amount 
            <<" tornillos de tipo " << name 
            << " (mediana: " << get_median_value()<< ")"
            << endl;
            init();
        }
        return SUCCESS;
    }

    void print_with(){
        cout << endl;
        for (unsigned int i = 0; i< this-> limit; i++){
            if (i %10 == 0 && i!=0){
                cout << endl;
            }
            cout << "" <<this-> width_list[i] << ",";
        }
        cout << "]"<< endl;
    }

    void print_as_remnant(){
        cout << "* " << this-> current_screw_amount
         << " tornillos de tipo " << this->name << endl; 
    }

    //Constructor por movimiento
    Package(Package&& other) {  
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = other.width_list;

        other.type_id = -1;
        other.name = "null";
        other.limit = 0;
        other.current_screw_amount = 0;
        other.width_list = nullptr;
    }

    //Constructor por copia
    Package(const Package &other) {
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list =(unsigned int*)malloc(//
            other.limit*sizeof(unsigned int));
        memcpy(this->width_list, other.width_list,//
            other.limit*sizeof(unsigned int));
    }

    //Asignacion por movimiento
    Package& operator=(Package&& other) {
        if (this == &other) {
            return *this; // other is myself!
        }
        if (this->width_list){
            free(this->width_list);
        }
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = other.width_list;

        other.type_id = -1;
        other.name = "null";
        other.limit = 0;
        other.current_screw_amount = 0;
        other.width_list = nullptr;
        return *this;
    }

    //Asigmacion por copia
    Package& operator=(const Package &other) {
        if (this == &other) {
            return *this; // other is myself!
        }
        if (this->width_list){
            free(this->width_list);
        }
        
        this->type_id = other.type_id;
        this->name = other.name;
        this->limit = other.limit;
        this->current_screw_amount = other.current_screw_amount;
        this->width_list = (unsigned int*)//
        malloc(other.limit*sizeof(unsigned int));
        memcpy(this->width_list, //
            other.width_list,other.limit*sizeof(unsigned int) );
        return *this;
    }

    ~Package(){
        if (this->width_list){
            free(this->width_list);
        }
    }

    friend std::ostream& operator<< (std::ostream &out, const Package &pac);
};

std::ostream& operator<< (std::ostream &out, const Package &pac)
{
    cout << " package {  id: " <<pac.type_id 
    << " name: " << pac.name 
    << " current_amount: " <<pac.current_screw_amount 
    << " limit: " <<pac.limit << " }\n";
    return out;
}




////////////////////////////////////////////////////////////////////////////////

/*
class Lock {
private:
    std::mutex &m;

public:
    explicit Lock(std::mutex &m) : m(m) {
        m.lock();
    }

    ~Lock() {
        m.unlock();    
    }

private:
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;
    Lock(Lock&&) = delete;
    Lock& operator=(Lock&&) = delete;
};*/
////////////////////////////////////////////////////////////////////////////////

class Package_protector { // aka monitor
    private:
    std::mutex m; 
    std::list<Package> * packages;

    public:
        explicit Package_protector(std::list<Package>* packages){
            this->packages = packages;
        }

        void prepare_package(Classification cl){
            for (std::list<Package>::iterator pac_iter = (*packages).begin(); //
                    pac_iter != (*packages).end(); ++pac_iter){
                    if ((*pac_iter).can_be_added(cl)){
                        // m.lock();
                       // cout <<"se quiere agregar  "<< cl << endl;
                        try_to_add_screws(*pac_iter, cl);
                        //cout <<" queda  " <<(*pac_iter)  <<endl;
                        //m.unlock();
                        return;
                }   
            }
            cerr << "Tipo de tornillo invalido: " << cl.get_type() << endl; 
        }

        int try_to_add_screws(Package &pac, Classification & cl){
            //cout << " al comienzo\n ";
            //cout << pac << endl;
            //count << cl << endl;
      
        m.lock();

        if (( !pac.can_be_added(cl) ) || //
            (pac.get_current_screw_amount() >= pac.get_limit()) ){
            return ERROR;
        }

        
        unsigned int amount_to_add = 0;
        
        if ((pac.get_current_screw_amount() + cl.get_amount())//
            <= pac.get_limit() ) { 
        // pongo todos los del clasificador total no llenan el paquete
            amount_to_add = cl.get_amount();
        } else{ 
        // pongo solamente la cantidad que puedo xq sino sobrepaso el paquete
            amount_to_add = pac.get_limit() - pac.get_current_screw_amount();
        }
        
        pac.add_screws(cl, amount_to_add);
        
        
        if ( cl.get_amount() > 0 ){
            //cout << " despues de agregar\n ";
            //cout << pac << endl;
            //xcout << cl << endl;
             m.unlock();
            try_to_add_screws(pac,cl);
        }
        

         m.unlock();
        return SUCCESS;
    }
};

///////////////////////////////////////////////////////////////////////////////
/*

class Classifier_device{
    char * classifier_file_name;
    bool is_connected;

    ifstream classifier_ifstream;
    streampos size;
    string classifier_device;
    bool has_finished;
    std::mutex m; 

    bool are_all_bits_one(char * buffer){
        for (int byte = 0; byte < 4; byte ++){
            char c = buffer[byte];

            for (int i = 7; i >= 0; i--){
                if ( ((c >> i) & 1) != 1 ){
                    return false;
                }
            }
        }
        return true; 
    }


    public:
    explicit Classifier_device(char * classifier_file_name){
        this->classifier_file_name = classifier_file_name;
        this->is_connected = false;
        this->has_finished = false;
    }

    bool has_reach_end(){
        return std::move(this->has_finished);
    }

    bool is_still_connect(){
        return std::move(this-> is_connected);
    }


    int connect(){
        classifier_ifstream.open(this->classifier_file_name,//
        ios::in|ios::binary|ios::ate);
        if (classifier_ifstream.is_open()){ 
            this->is_connected = true;
            this->has_finished = false;
            this->size = classifier_ifstream.tellg();

            classifier_ifstream.seekg(0, ios::beg);
            char c;
            string name;
            while (classifier_ifstream.tellg() <size){
                classifier_ifstream.get(c);
                if (c == 0){
                    break;
                }
                name += c;
            }
            this->classifier_device = name;
            cout <<classifier_file_name 
            <<": se establece conexion con el dispositivo " 
            << classifier_device <<"\n";
            return SUCCESS;
        } else {
            cerr << classifier_file_name 
            <<": no se pudo conectar con el dispositivo"<< endl;
            return ERROR;
        }
    }


    Classification get_classification(){
        if (this->is_connected == false){
            throw std::runtime_error(//
                "Impossible to get classification, device disconnected\n");
        }

        char  buffer[4];
        
        classifier_ifstream.read(buffer, 4);


        if (are_all_bits_one(buffer)){
            cerr << this->classifier_device << " atascado" << endl;
            throw ERROR;
        }

        unsigned int type =  (unsigned char)(buffer[0]) >> 3;

        unsigned int amount =  int(((unsigned char)(buffer[0]) << 29) >> 5|
        (unsigned char)(buffer[1]) << 11 |
        (unsigned char)(buffer[2]) << 3 |
        (unsigned char)(buffer[3]) >> 5);
    

        unsigned int width =  (unsigned char)(buffer[3]) << 27;
        width = (width >> 27);
            
        if (classifier_ifstream.tellg() >= this->size){
            this->has_finished = true;
            classifier_ifstream.close();
            this->is_connected = false;
        }

        Classification cl(type,amount,width);

        return std::move(cl);
    }
};*/

////////////////////////////////////////////////////////////////////////
class Configuration{
    char * config_file_name;
    ifstream config_file;

public:
    explicit Configuration(char config_file_name[]){
        this->config_file_name = config_file_name;
    }


    std::list<Package> get_packages(){
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
};




///////////////////////////////////////////////////////////////////////////////





void remnants(std::list<Package> & packages){
    cout << "# Informe de remanentes"<< endl;

    for (std::list<Package>::iterator pac_iter = packages.begin(); 
        pac_iter != packages.end(); ++pac_iter){
        (*pac_iter).print_as_remnant(); 
    }
}



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



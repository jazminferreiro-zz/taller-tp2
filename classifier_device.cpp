#include <string>
#include "classifier_device.h"

bool Classifier_device:: are_all_bits_one(char * buffer){
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


Classifier_device::Classifier_device(char * classifier_file_name){
    this->classifier_file_name = classifier_file_name;
    this->is_connected = false;
    this->has_finished = false;
}

bool Classifier_device::has_reach_end(){
    return std::move(this->has_finished);
}

bool Classifier_device::is_still_connect(){
    return std::move(this-> is_connected);
}


int Classifier_device::connect(){
    classifier_ifstream.open(this->classifier_file_name,//
    ios::in|ios::binary|ios::ate);
    if (classifier_ifstream.is_open()){ 
        this->is_connected = true;
        this->has_finished = false;
        this->size = classifier_ifstream.tellg();

        classifier_ifstream.seekg(0, ios::beg);
        char c;
        std::string name;
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
    }
    cerr << classifier_file_name 
    <<": no se pudo conectar con el dispositivo"<< endl;
    return ERROR;
}


Classification Classifier_device::get_classification(){
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

/* 
 * File:   Pivote.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 05:43 PM
 */

#include "Pivote.h"
#include "Objeto.h"

Pivote::Pivote() {
}

Pivote::Pivote(const Pivote& orig) {
}

Pivote::~Pivote() {
}

Pivote::Pivote(Objeto& ob, double r){
    this->centro = &ob;
    this->radio = r;
}

Pivote::Pivote(Objeto& c){
    this->centro = &c;
}

/* 
 * File:   Nodo.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:21 PM
 */

#include "Nodo.h"

Nodo::Nodo() {
}

Nodo::Nodo(const Nodo& orig) {
}

Nodo::Nodo(Bloque& bloq){
    this->block = bloq;
}

Nodo::~Nodo() {
}

Nodo::Nodo(Bloque& bloq, Objeto& ob){
    this->block = bloq;
    this->centro = ob;
}


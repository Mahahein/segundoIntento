/* 
 * File:   Objeto.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 12:47 PM
 */

#include "Objeto.h"
#include <vector>
using namespace std;

Objeto::Objeto() {
}

Objeto::Objeto(const Objeto& orig) {
    
}
Objeto::Objeto(int pezo){
    peso = pezo;
}

Objeto::~Objeto() {
}

Objeto::poneValor(double val){
    valores.push_back(val);
}

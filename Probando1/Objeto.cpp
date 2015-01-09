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

void Objeto::poneValor(double val){
    valores.push_back(val);
}

void Objeto::calculaPeso(){
    if(valores.size()>0){
        double peso=0;
        for(vector<double>::iterator i = valores.begin(); i!= valores.end(); ++i)
            peso+=*i;
        this->peso = peso;
    }
    else
        this->peso = 0;
}


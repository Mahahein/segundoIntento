/* 
 * File:   Objeto.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 12:47 PM
 */

#include "Objeto.h"
#include <vector>
#include <iostream>
using namespace std;

Objeto::Objeto() {
    distanciaAcumulada = 0;
    esPivote = false;
}

Objeto::Objeto(const Objeto& orig) {
    distanciaAcumulada = 0;
    esPivote = false;
}
Objeto::Objeto(int dist){
    distanciaAcumulada = dist;
    esPivote = false;
}

Objeto::~Objeto() {
}

void Objeto::poneValor(double val){
    valores.push_back(val);
}

void Objeto::aumentaAcumulado(double k){
    distanciaAcumulada += k;
}

bool Objeto::operator <(const Objeto& ob) const{
    return (distancias.at(comparando) < ob.distancias.at(comparando));
}

bool Objeto::operator >(const Objeto& ob) const{
    return (distancias.at(comparando) > ob.distancias.at(comparando));
}

bool Objeto::operator ==(const Objeto& ob) const{
    return (id == ob.id);
}

ostream& Objeto::operator <<(ostream& out){
    for(int i = 0; i < valores.size(); i++){
        out << valores.at(i) << " ";
    }
    out << "\n";
    return out;
}

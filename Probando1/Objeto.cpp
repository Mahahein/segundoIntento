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
    distanciaAcumulada = 0;
}

Objeto::Objeto(const Objeto& orig) {
    distanciaAcumulada = 0;
}
Objeto::Objeto(int dist){
    distanciaAcumulada = dist;
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

ostream& Objeto::operator <<(ostream& out, const Objeto& ob){
    for(vector<double>::iterator i = valores.begin(); i != valores.end(); ++i){
        out << (*i) << " ";
    }
    out << "\n";
    return out;
}
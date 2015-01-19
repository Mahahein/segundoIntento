/* 
 * File:   Pivote.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 05:43 PM
 */

#include "Pivote.h"
#include "Objeto.h"
#include <vector>
#include <algorithm>

Pivote::Pivote() {
    this->cercanos = new vector<Objeto*>(31);
    this->reemplazos = new vector<Objeto*>(31);
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
    this->cercanos = new vector<Objeto*>(31);
    this->reemplazos = new vector<Objeto*>(31);
}

bool Pivote::operator <(const Pivote& piv) const{
    return (radio < piv.radio);
}

bool Pivote::operator >(const Pivote& piv) const{
    return (radio > piv.radio);
}

void Pivote::actualizaRadio(){
    vector<Objeto*>::iterator mayor = cercanos.begin();
    for(vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
	if( (*mayor)->distancias.at(pos) < (*i)->distancias.at(pos) )
	    mayor = i;
    }
    radio = (*mayor)->distancias.at(pos);
}

void Pivote::actualizaRadio2(){
    vector<Objeto*>::iterator mayor = reemplazos.begin();
    for(vector<Objeto*>::iterator i = reemplazos.begin(); i != reemplazos.end(); ++i){
        if( (*mayor)->distancias.at(pos) < (*i)->distancias.at(pos) )
            mayor = i;
    }
    radio2 = (*mayor)->distancias.at(pos);
}

void Pivote::ordenaCercanos(int pos){
    for(vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
	(*i)->comparando = pos;
    }
    sort( cercanos.begin(), cercanos.end());
}

void Pivote::ordenaReemplazos(int pos, int modo){
    for(vector<Objeto*>::iterator i = reemplazos.begin(); i != reemplazos.end(); ++i){
        (*i)->comparando = pos;
    }
    if(modo==1)
        sort( reemplazos.begin(), reemplazos.end());
    else
        sort( reemplazos.begin(), reemplazos.end(), Objeto.operator >() );
}



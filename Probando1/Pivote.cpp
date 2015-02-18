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

bool Pivote::operator <(const Pivote& piv) const{
    return (radio < piv.radio);
}

bool Pivote::operator >(const Pivote& piv) const{
    return (radio > piv.radio);
}

void Pivote::actualizaRadio(){
    int mayor = 0;
    for(int i = 0; i < cercanos.size(); i++){
	if( cercanos[mayor]->distancias[pos] < cercanos[i]->distancias[pos] )
	    mayor = i;
    }
    radio = cercanos[mayor]->distancias[pos];
}

void Pivote::ordenaCercanos(int pos){
    for(vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
	   (*i)->comparando = pos;
    }
    sort( cercanos.begin(), cercanos.end());
}

void Pivote::actualizaMasLejano(){
    int lejano = 0;
    for( int i = 0; i < cercanos.size(); i++){
        if( cercanos[i]->distancias[this->pos] >= cercanos[lejano]->distancias[this->pos] ){
            lejano = i;
        }
    }
    this->posMasLejano = lejano;
}
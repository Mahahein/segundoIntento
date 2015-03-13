/* 
 * File:   Pivote.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 05:43 PM
 */

#include "Pivote.h"
#include "Objeto.h"
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

Pivote::Pivote() {
    numCluster = -1;
}

Pivote::Pivote(const Pivote& orig) {
    numCluster = -1;
}

Pivote::~Pivote() {
}

Pivote::Pivote(Objeto& ob, double r){
    this->centro = &ob;
    this->radio = r;
    numCluster = -1;
}

Pivote::Pivote(Objeto& c){
    this->centro = &c;
    numCluster = -1;
}

bool Pivote::operator <(const Pivote& piv) const{
    return (numCluster < piv.numCluster);
}

bool Pivote::operator >(const Pivote& piv) const{
    return (numCluster > piv.numCluster);
}

void Pivote::actualizaRadio(){
    int mayor = 0;
    vector<Objeto*>::iterator itMay = cercanos.begin();
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i ){
    	if( (*itMay)->distancias[pos] < (*i)->distancias[pos] )
    	    itMay = i;
    }
    radio = (*itMay)->distancias[pos];
}

void Pivote::actualizaRadioCentro(){
    int mayor = 0;
    double dist;
    vector<Objeto*>::iterator itMay = cercanos.begin();
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i ){
        for(int j = 0; j < (*i)->valores.size(); j++ )
            dist += pow( centro->valores[j] + (*i)->valores[j] , 2 );
        dist = sqrt( dist );
        (*i)->distanciaACentro = dist;
    }
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
        if( (*itMay)->distanciaACentro < (*i)->distanciaACentro )
            itMay = i;
    }
    radio = (*itMay)->distanciaACentro;
}

void Pivote::ordenaCercanos(int pos){
    for(vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
	   (*i)->comparando = pos;
    }
    //sort( cercanos.begin(), cercanos.end());
}

void Pivote::actualizaMasLejano(){
    vector<Objeto*>::iterator lejano = cercanos.begin();
    //cout << "EN PIVOTE.CPP ";
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i ){
        //cout << "cercanos[i]->distancias.size(): " << cercanos[i]->distancias.size() << "  this->pos: " << this->pos << endl;
        if( (*i)->distancias[this->pos] >= (*lejano)->distancias[this->pos] ){
            lejano = i;
        }
    }
    this->posMasLejano = lejano;
}
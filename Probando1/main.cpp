/* 
 * File:   main.cpp
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 10:42 AM
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include "Cargador.h"
#include "Lector.h"
#include "Objeto.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    Cargador s;
    //s.leer("vector.txt");
    //cout << "Hola Mundo" << endl;
    Lector* l = new Lector(2);
    vector<Objeto*> obs = l->leerDesdeArchivo("salida.txt");
    if(!obs.empty()){
    	for(int i = 0; i < obs.size(); i++){
    		for(int k = 0; k < obs[i]->valores.size(); k++){
    			cout << obs[i]->valores[k] << " ";
    		}
    		cout << obs[i]->distanciaAcumulada << endl;
    	}
    }
    
    return 0;
}


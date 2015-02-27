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
#include "Consultor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	int i, k, j;
    Cargador* s = new Cargador();
    s->leer("vector.txt");

    Consultor* c = new Consultor();
    c->pivotes = s->pivotesProvisorios;
    c->clusters = s->pivotesEnMemoria;
    c->saco = s->bolsa;

    //cout << "Hola Mundo" << endl;
    Lector* l = new Lector(2);
    vector<Objeto*> obs = l->leerDesdeArchivo("salida.txt");
    if(!obs.empty()){
    	for( i = 0; i < obs.size(); i++){
    		cout << "la respuesta a la consulta :";
    		for( k = 0; k < obs[i]->valores.size(); k++){
    			cout << obs[i]->valores[k] << " ";
    		}
    		cout << obs[i]->distanciaAcumulada << " es :"<< endl;
    		vector<Objeto*> resp = c->consultar(obs[i], obs[i]->distanciaAcumulada);
    		if(!resp.empty()){
    			for( k = 0; k < resp.size(); k++){
    				for( j = 0; j < resp[k]->valores.size(); j++ ){
    					cout << resp[k]->valores[j] << " ";
    				}
    				cout << endl;
    			}
    		}
    		else{
    			cout << "vacio" << endl;
    		}
    	}
    }
    
    return 0;
}


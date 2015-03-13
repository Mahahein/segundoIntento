/* 
 * File:   main.cpp
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 10:42 AM
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include "Cargador.h"
#include "Lector.h"
#include "Objeto.h"
#include "Consultor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	int j;
    Cargador* s = new Cargador();
    s->leer("vector.txt");

    Consultor* c = new Consultor();
    c->pivotes = s->pivotesProvisorios;
    c->clusters = s->pivotesEnMemoria;
    c->saco = s->bolsa;
    //cout << "en el main" << endl;
    //cout << c->clusters.size() << "  " << c->pivotes.size() << "  " << c->saco.size() << endl;
    //cout << "tamaño double: " << sizeof(double) << endl;
    //cout << "Hola Mundo" << endl;
    Lector* l = new Lector(2);
    vector<Objeto*> obs = l->leerDesdeArchivo("salida.txt");
    if(!obs.empty()){
    	for(vector<Objeto*>::iterator i = obs.begin(); i != obs.end(); ++i){
    		printf("la respuesta a la consulta :");
            for( int k = 0; k < (*i)->valores.size(); k++){
                printf("%8.6f ", (*i)->valores[k]);
            }
            vector<Objeto*> resp = c->consultarBusquedaRango((*i), (*i)->distanciaAcumulada);
            printf( "%f es de tamaño %ld\n", (*i)->distanciaAcumulada, resp.size() );
            if(!resp.empty()){
                for( vector<Objeto*>::iterator k = resp.begin(); k != resp.end(); ++k ){
                    for( j = 0; j < (*k)->valores.size(); j++ ){
                        printf("%8.6f ", (*k)->valores[j]);
                    }
                    printf("\n");
                }
            }
            else{
                printf("vacio\n");
            }
    	}
    }
    
    return 0;
}


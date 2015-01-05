/* 
 * File:   Saco.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:30 PM
 */

#include "Saco.h"
#include "Objeto.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>
using namespace std;

Saco::Saco() {
}

Saco::Saco(const Saco& orig) {
}

Saco::~Saco() {
}

Saco::leer(string archivo){
    ifstream entrada(archivo);
    string buf;
    for( string linea; getline(entrada, linea);){
        stringstream ss(linea);
        Objeto ob();
        while( ss >> buf ){
            ob.poneValor(std::strtod(buf));
        }
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        
        //AGREGAR A LA BOLSA
        bolsa.push_back(ob);
        
        //VERIIFCAR CUANTOS OBJETOS HAY EN LA BOLSA
        if(bolsa.size()*sizeof(double)>= 1024){
            //VACIAR LA BOLSA Y PASAR A UN BLOQUE
            //PARA VACIAR:
            ////CALCULAR EL CENTRO DE LOS QUE ESTAN EN LA BOLSA
            ////ESCRIBIR LOS DATOS DE LOS OBJETOS EN MEMORIA SECUNDARIA
            ////AGREGAR EL CENTRO A LA LISTA DE PIVOTES EN RAM
        }
    }
        
    
    
    
}


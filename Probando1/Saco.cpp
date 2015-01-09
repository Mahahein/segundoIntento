/* 
 * File:   Saco.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:30 PM
 */

#include "Saco.h"
#include "Objeto.h"
#include "Pivote.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

Saco::Saco() {
}

Saco::Saco(const Saco& orig) {
}

Saco::~Saco() {
}

void Saco::leer(string archivo){
    
    ifstream entrada;
    entrada.open("vector.txt");
    vector<Pivote*> pivotes;
    string buf;
    long cluster = 1;
    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        
        Objeto* ob = new Objeto();
        char* end;
        while( ss >> buf ){
            //cout << "Hola" << endl;
            //cout << buf << endl;
            const char* buf2 = buf.data();
            if(isdigit(buf2[0])){
                //cout << buf2 << endl;
                ob->poneValor(strtod(buf2, &end));
            }
        }
        distancias(ob, pivotes);
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        
        //AGREGAR A LA BOLSA
        bolsa.push_back(ob);
        //cout << ob->valores.size() << endl;
        //VERIIFCAR CUANTOS OBJETOS HAY EN LA BOLSA
        if(bolsa.size()*sizeof(double)>= 30720 && bolsa.size()*sizeof(double)<=40960){
            //VACIAR LA BOLSA Y PASAR A UN BLOQUE
            //PARA VACIAR:
            ////CALCULAR EL CENTRO DE LOS QUE ESTAN EN LA BOLSA
            ofstream file;
            //cout << "sacando " << bolsa.size()*sizeof(double) << endl;
            string salida;
            stringstream ss;
            ss << "cluster";
            ss << cluster;
            ss << ".txt";
            //cout << cluster << endl;
            salida = ss.str();
            //cout << salida << cluster << endl;
            file.open(salida.c_str());
            
            for(vector<Objeto*>::iterator i = bolsa.begin(); i!= bolsa.end(); ++i){
                //Objeto o = bolsa.at(i);
                //cout << bolsa.at(i)->valores.size() << endl;
                for(vector<double>::iterator k = (*i)->valores.begin(); k!= (*i)->valores.end() ; ++k){
                    //cout << (bolsa.at(i))->valores.at(k) << endl;
                    file << *k << " ";
                }
                file << "\n";
            }
            file.close();
            bolsa.clear();
            cluster++;
            ////ESCRIBIR LOS DATOS DE LOS OBJETOS EN MEMORIA SECUNDARIA
            ////AGREGAR EL CENTRO A LA LISTA DE PIVOTES EN RAM
        }      
    }
    if(bolsa.size() > 0){
        ofstream file;
        //cout << "sacando " << bolsa.size()*sizeof(double) << endl;
        string salida;
        stringstream ss;
        ss << "cluster";
        ss << cluster;
        ss << ".txt";
        //cout << cluster << endl;
        salida = ss.str();
        cout << salida << cluster << endl;
        file.open(salida.c_str());
        for(vector<Objeto*>::iterator i = bolsa.begin(); i!= bolsa.end(); ++i){
            //cout << bolsa.at(i)->valores.size() << endl;
            for(vector<double>::iterator k = (*i)->valores.begin(); k!= (*i)->valores.end() ; ++k){
                //cout << (bolsa.at(i))->valores.at(k) << endl;
                file << *k << " ";
            }
            file << "\n";
        }
        file.close();   
        bolsa.clear();
        cluster++;
    }
    entrada.close();  

}

void Saco::peso(Objeto& ob){
    ob.calculaPeso();
}

void Saco::distancias(Objeto* ob, vector<Pivote*> & pivots){
    if(pivots.size()>0){
        for(vector<Pivote*>::iterator i = pivots.begin(); i!= pivots.end(); ++i){
            double dist = 0;
            vector<double>::iterator k = (*i)->centro->valores.begin();
            vector<double>::iterator l = (ob)->valores.begin();
            for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                dist += ((*k+*l)*(*k+*l));
            }
            ob->distancias.push_back(dist);
        }
            
    }
}

void Saco::nuevaDistancia(Objeto* ob, Pivote* piv){
    
}

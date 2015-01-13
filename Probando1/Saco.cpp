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
#include <float.h>
#include <algorithm>
#define N 2048
#define n 32
#define B 32

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
    string buf;
    long cluster = 1;
    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        
        Objeto* ob = new Objeto();
        char* end;
        int cont = 0;
        while( ss >> buf ){
            //cout << "Hola" << endl;
            //cout << buf << endl;
            const char* buf2 = buf.data();
            if(isdigit(buf2[0])){
                if(cont==0)
                    ob->id = strtod(buf2, &end);
                //cout << buf2 << endl;
                else if (cont == 1)
                    continue;
                else
                    ob->poneValor(strtod(buf2, &end));
                cont++;
            }
        }
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        distanciasAPivotes(ob);
        //AGREGAR A LA BOLSA
        bolsa.push_back(ob);
        
        //VERIIFCAR CUANTOS OBJETOS HAY EN LA BOLSA
        //SI HAY B+1 ELEMENTOS (O 1), TIENE QUE ELEGIR UN PIVOTE PROVISORIO
        if(bolsa.size()%B == 1){
            //ELEGIR PIVOTE PROVISORIO
            //CREAMOS LOS ITERADORES PARA RECORRER LA BOLSA Y MARCAR EL QUE TENGA MAYOR DISTANCIA ACUMULADA
            vector<Objeto*>::iterator i = bolsa.begin();
            vector<Objeto*>::iterator mayObj = i;
            //RECORREMOS LA BOLSA BUSCANDO AL CON MAYOR DISTANCIA ACUMULADA
            for(; i != bolsa.end(); ++i){
                if((*i)->distanciaAcumulada>(*mayObj)->distanciaAcumulada)
                    mayObj = i;
            }
            //LUEGO, SI O SI TENDREMOS UN OBJETO SELECCIONADO
            //Y CREAMOS UN PIVOTE CON ÉL
            Pivote* p = new Pivote(*(*mayObj));
            //LO AGREGAMOS A LOS PIVOTES PROVISORIOS
            pivotesProvisorios.push_back(p);
            //LO ELIMINAMOS DE LA BOLSA DE OBJETOS ENTRANTES
            bolsa.erase(mayObj);
            //Y AGREGAMOS A LOS DEMAS OBJETOS LA DISTANCIA AL NUEVO PIVOTE CREADO
            for(vector<Objeto*>::iterator j = bolsa.begin(); j != bolsa.end(); ++j){
                distanciaPivoteNuevo((*j), p);
            }
        }
        
        
        //SI ALCANZAMOS EL TAMANIO ASIGNADO A LA BOLSA
        //TENEMOS QUE VACIAR UN POCO, CREANDO UN CLUSTER
        if(bolsa.size()==N){
            //ELEGIR UN PIVOTE PARA CENTRO DE CLUSTER
            //PRIMERO DETERMINAR RADIO DE COBERTURA
            //PARA ESTO SE DEBE BUSCAR LOS n-1 VECINOS MAS CERCANOS DE CADA PIVOTE PROVISORIO
            //CREAMOS UNA LISTA AUXILIAR PARA GUARDAR LOS VECTORES CREADOS CON LOS VECINOS DE CADA PIVOTE
            vector< vector<Objeto*> > listAux;
            listAux.clear();
            int pos = 0;
            //RECORREMOS EL VECTOR DE PIVOTES PROVISORIOS
            for(vector<Pivote*>::iterator vi = pivotesProvisorios.begin(); vi != pivotesProvisorios.end() ; ++vi){
                //POR CADA PIVOTE, OBTENEMOS SUS VECINOS Y LOS GUARDAMOS EN LISTAUX
                listAux.push_back(obtieneCercanos(pos));
                //LUEGO, A CADA PIVOTE LE CALCULAMOS SU RADIO DE COBERTURA
                (*vi)->radio = obtieneRadio(listAux.back(), pos);
            }
            //CREAMOS LOS ITERADORES PARA VOLVER A RECORRER EL VECTRO DE PIVOTES PROVISORIOS
            //Y BUSCAR QUE QUE TENGA MENOR RADIO DE COBERTURA
            //Y TAMBIÉN UNA REFERENCIA AL VECTOR DE VECINOS QUE TIENE
            vector<Pivote*>::iterator vi = pivotesProvisorios.begin();
            vector< vector<Objeto*> >::iterator oi = listAux.begin();
            vector<Pivote*>::iterator vi_mayor = vi;
            vector< vector<Objeto*> >::iterator oi_mayor = oi;
            
            //RECORRERMOS EL VECTOR BUSCANDO EL CON MENOR RADIO
            for(; vi != pivotesProvisorios.end() && oi != listAux.end(); ++vi, ++oi){
                if((*vi)->radio < (*vi_mayor)->radio){
                    vi_mayor = vi;
                    oi_mayor = oi;
                }
            }
            
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
            
            for(vector<Objeto*>::iterator i = (*oi_mayor).begin(); i!= (*oi_mayor).end(); ++i){
                //Objeto o = bolsa.at(i);
                //cout << bolsa.at(i)->valores.size() << endl;
                for(vector<double>::iterator k = (*i)->valores.begin(); k!= (*i)->valores.end() ; ++k){
                    //cout << (bolsa.at(i))->valores.at(k) << endl;
                    file << *k << " ";
                }
                file << "\n";
                bolsa.erase(bolsa.begin()+(*i)->pos);
            }
            file.close();
            (*oi_mayor).clear();
            listAux.clear();
            cluster++;
            ofstream pivotesDisco;
            pivotesDisco.open("pivotes.txt", ofstream::app);
            
            pivotesDisco << (*vi_mayor)->centro->id << " " << (*vi_mayor)->radio << "\n";
            
            pivotesDisco.close();
            ////ESCRIBIR LOS DATOS DE LOS OBJETOS EN MEMORIA SECUNDARIA
            ////AGREGAR EL CENTRO A LA LISTA DE PIVOTES EN RAM
        }      
    }
    /*if(bolsa.size() > 0){
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
    }*/
    entrada.close();  

}


void Saco::distanciasAPivotes(Objeto* ob){
    if(pivotesProvisorios.size()>0){
        for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i!= pivotesProvisorios.end(); ++i){
            double dist = 0;
            vector<double>::iterator k = (*i)->centro->valores.begin();
            vector<double>::iterator l = (ob)->valores.begin();
            for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                dist += ((*k+*l)*(*k+*l));
            }
            ob->distancias.push_back(dist);
            ob->aumentaAcumulado(dist);
        }
            
    }
    if(pivotesEnMemoria.size()>0){
        for(vector<Pivote*>::iterator i = pivotesEnMemoria.begin(); i!= pivotesEnMemoria.end(); ++i){
            double dist = 0;
            vector<double>::iterator k = (*i)->centro->valores.begin();
            vector<double>::iterator l = (ob)->valores.begin();
            for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                dist += ((*k+*l)*(*k+*l));
            }
            ob->distancias.push_back(dist);
            ob->aumentaAcumulado(dist);
        }
            
    }
    
}

void Saco::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
    double dist = 0;
    vector<double>::iterator k = (piv)->centro->valores.begin();
    vector<double>::iterator l = (ob)->valores.begin();
    for(; k != (piv)->centro->valores.end() && l != (ob)->valores.end(); ++k, ++l){
        dist += ((*k+*l)*(*k+*l));
    }
    ob->distancias.push_back(dist);
    ob->aumentaAcumulado(dist);
}

vector<Objeto*> Saco::obtieneCercanos(int pos){
    vector<Objeto*> cercanos;
    int p = 0;
    for(vector<Objeto*>::iterator k = bolsa.begin(); k != bolsa.end(); ++k){
        (*k)->comparando = pos;
        (*k)->pos = p;
        if(!estaEnPivotes((*k)->id)){
            if(cercanos.size()<(n-1)){
                cercanos.push_back((*k));
            }
            else{
                sort(cercanos.begin(), cercanos.end());
                if((*k) < cercanos.back()){
                    cercanos.pop_back();
                    cercanos.push_back((*k));
                }
            }
        }
        p++;
    }
    return cercanos;
}

bool Saco::estaEnPivotes(int id){
    for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i){
        if(id == (*i)->centro->id){
            return true;
        }
    }
    for(vector<Pivote*>::iterator k = pivotesEnMemoria.begin(); k != pivotesEnMemoria.end(); ++k){
        if(id == (*k)->centro->id){
            return true;
        }
    }
    return false;
}

double Saco::obtieneRadio(vector<Objeto*>& obs, int pos){
    double mayor = -DBL_MAX;
    for(vector<Objeto*>::iterator i = obs.begin(); i != obs.end(); ++i){
        if((*i)->distancias.at(pos) > mayor)
            mayor = (*i)->distancias.at(pos);
    }
    return mayor;
}
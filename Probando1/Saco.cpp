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
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <float.h>
#include <algorithm>
#include <math.h>
#define N 40
#define n 5
#define B 5

using namespace std;

Saco::Saco() {
}

Saco::Saco(const Saco& orig) {
}

Saco::~Saco() {
}

void Saco::leer(string archivo){
    ifstream entrada;
    ofstream pivotesDisco;
    entrada.open("vector.txt");
    string buf, salida;
    const char* buf2;
    long cluster = 1;
    int nPivotes = 0;
    yaHayUno = false;
    yaHayOtro = false;
    char* end;
    int cont, mayObj, k, l, minPiv;
    vector<Objeto*>::iterator i;
    vector<Pivote*>::iterator menor, vi;
    vector<double>::iterator j;
    ofstream file;
    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        //cout << "creando objeto" << endl;
        Objeto* ob = new Objeto();
        
        cont = 0;
        //cout << "leyendo objeto" << endl;
        while( ss >> buf ){
            //cout << "Hola" << endl;
            //cout << buf << endl;
            buf2 = buf.data();
            if(isdigit(buf2[0])){
                if(cont == 0)
                    ob->id = strtod(buf2, &end);
                //cout << buf2 << endl;
                else if (cont > 2){
                    ob->poneValor(strtod(buf2, &end));
                    //cout << ob->valores.back() << " ";
                }
            }
            cont++;
        }
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        distanciasAPivotes(ob);
        bolsa.push_back(ob);

        //ELEGIR UN PIVOTE PROVISORIO
        if( (bolsa.size()%B) == 1 ){
            cout << "sacando pivote provisorio" << endl;
            mayObj = 0;
            for( k = 0; k < bolsa.size(); k++){
                if( bolsa[k]->distanciaAcumulada >= bolsa[mayObj]->distanciaAcumulada && !bolsa[k]->esPivote )
                    mayObj = k;
            }

            cout << "creando pivote provisorio" << endl;
            Pivote* piv = new Pivote();
            piv->centro = bolsa[mayObj];
            piv->centro->id = bolsa[mayObj]->id;
            piv->pos = nPivotes;
            bolsa[mayObj]->esPivote = true;
            pivotesProvisorios.push_back(piv);
            yaHayUno = true;
            nPivotes++;

            for( k = 0; k < bolsa.size(); k++ ){
                distanciaPivoteNuevo(bolsa[k], piv);
            }

        }
        //PASAR UN PIVOTE A MEMORIA SECUNDARIA
        if( bolsa.size() == N ){
            cout << " antes bolsa.size() : " << bolsa.size() << endl;
            cout << "sacando pivote a memoria" << endl;
            cout << "pivotesProvisorios.size() :" << pivotesProvisorios.size() << endl;
            minPiv = 0;
            
            for( k = 0; k < pivotesProvisorios.size(); k++){
                
                //cout << "buscando cercanos" << endl;
                obtieneCercanos(pivotesProvisorios[k]);
                pivotesProvisorios[k]->actualizaRadio();
                //cout << "paso el actualizaRadio" << endl;
                if( pivotesProvisorios[k]->radio <= pivotesProvisorios[minPiv]->radio )
                    minPiv = k;
                //cout << "paso el if" << endl;
            }

            

            stringstream ss;
            ss << "cluster";
            ss << cluster;
            ss << ".txt";
            salida = ss.str();

            file.open(salida.c_str());
            
            for( k = 0; k < pivotesProvisorios[minPiv]->cercanos.size(); k++ ){
                for( l = 0; l < pivotesProvisorios[minPiv]->cercanos[k]->valores.size(); l++ ){
                    file << pivotesProvisorios[minPiv]->cercanos[k]->valores[l] << " ";
                }
                file << "\n";
            }
            file.close();

            for( k = 0; k < pivotesProvisorios[minPiv]->cercanos.size(); k++){
                //iter_swap( bolsa.begin() + pivotesProvisorios[minPiv]->cercanos[k]->pos , bolsa.begin() + (bolsa.size() - 1) );
                cout << pivotesProvisorios[minPiv]->cercanos[k]->id << " ";
                bolsa[pivotesProvisorios[minPiv]->cercanos[k]->pos] = bolsa[bolsa.size()-1];
                bolsa.pop_back();
                bolsa[pivotesProvisorios[minPiv]->cercanos[k]->pos]->pos = pivotesProvisorios[minPiv]->cercanos[k]->pos;
            }
            cout << endl;

            for( k = 0; k < bolsa.size(); k++){
                cout << bolsa[k]->id << " ";
            }
            cout << endl;
            for( k = 0; k < pivotesProvisorios.size(); k++ ){
                cout << pivotesProvisorios[k]->centro->id << " ";
            }
            cout << endl;

            sacaPivoteDelSaco( pivotesProvisorios[minPiv]->centro->id );
            //bolsa.erase(bolsa.begin() + pivotesProvisorios[minPiv]->centro->pos );

            pivotesEnMemoria.push_back(pivotesProvisorios[minPiv]);
            pivotesProvisorios.erase(pivotesProvisorios.begin() + minPiv);
            //iter_swap(pivotesProvisorios.begin() + minPiv , pivotesProvisorios.begin() + (pivotesProvisorios.size() -1) );
            //pivotesProvisorios[minPiv] = pivotesProvisorios[pivotesProvisorios.size()-1];
            //pivotesProvisorios.pop_back();
            //pivotesProvisorios.erase(pivotesProvisorios.begin() + pivotesProvisorios[minPiv]->pos);
            yaHayOtro = true;
            cluster++;
            cout << "despues bolsa.size() : " << bolsa.size() << endl;
        }
        

    }
        
    
    entrada.close();  

}


void Saco::distanciasAPivotes(Objeto* ob){
    vector<double>::iterator k;
    vector<double>::iterator l;
    if(!estaEnPivotes((*ob).id)){
        if(pivotesProvisorios.size()>0){
            for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i!= pivotesProvisorios.end(); ++i){
                double dist = 0;
                k = (*i)->centro->valores.begin();
                l = (ob)->valores.begin();
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
                k = (*i)->centro->valores.begin();
                l = (ob)->valores.begin();
                for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                    dist += ((*k+*l)*(*k+*l));
                }
                ob->distancias.push_back(dist);
                ob->aumentaAcumulado(dist);
            }

        }
    }
    else{
        (*ob).distanciaAcumulada = - DBL_MAX;
    }
    
}

void Saco::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
    vector<double>::iterator k;
    vector<double>::iterator l;
    if(!(ob)->esPivote){
        double dist = 0;
        k = (piv)->centro->valores.begin();
        l = (ob)->valores.begin();
        for(; k != (piv)->centro->valores.end() && l != (ob)->valores.end(); ++k, ++l){
            dist += ((*k+*l)*(*k+*l));
        }
        ob->distancias.push_back(sqrt(dist));
        ob->aumentaAcumulado(sqrt(dist));
    }
}

void Saco::obtieneCercanos(Pivote* p){
    p->cercanos.clear();
    for( int k = 0; k < bolsa.size(); k++){
        bolsa[k]->comparando = p->pos;
        bolsa[k]->pos = k;

        if(!bolsa[k]->esPivote){
            if(p->cercanos.size()<(n-1)){
                p->cercanos.push_back(bolsa[k]);
                p->actualizaMasLejano();
            }
            else{
                if( bolsa[k]->distancias[p->pos] < p->radio ){
                    p->cercanos[p->posMasLejano] = (bolsa[k]);
                    p->actualizaMasLejano();
                }
            }
        }
    }
    //cout << "saliendo de buscar los cercanos" << endl;
}

bool Saco::estaEnPivotes(int id){
    if(yaHayUno){
        for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i){
            if(id == (*i)->centro->id){
                return true;
            }
        }
    }
    if(yaHayOtro){
        for(vector<Pivote*>::iterator k = pivotesEnMemoria.begin(); k != pivotesEnMemoria.end(); ++k){
            if(id == (*k)->centro->id){
                return true;
            }
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

void Saco::sacaPivoteDelSaco(int id){
    cout << "id pivote : " << id << endl;
    for(int i = 0; i < bolsa.size(); i++){
	    if( bolsa[i]->id == id){
            cout << "pillo el pivote en la bolsa" << endl;
	        bolsa.erase(bolsa.begin() + i);
	        return;
	    }
    }
}

/*void Saco::agregarACercanosDePivotes(Objeto* ob){
    if(yaHayUno){
	for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i){
	    if( (*i)->cercanos.size() < (B-1)){
	    	(*i)->cercanos.push_back(ob);
		(*i)->actualizaRadio();
	    }
	    else if( (*ob).distancias.at((*i)->pos) < (*i)->radio){
		(*i)->ordenaCercanos((*i)->pos);
		(*i)->cercanos.pop_back();
		(*i)->cercanos.push_back(ob);
		(*i)->actualizaRadio();
	    }
	    else if( (*i)->reemplazos.size() < 2*(B-1)){
		(*i)->reemplazos.push_back(ob);
	    	(*i)->actualizaRadio2();
	    }
	    else if( (*ob).distancias.at((*i)->pos) < (*i)->radio2){
		(*i)->ordenaReemplazos((*i)->pos, 1);
		(*i)->reemplazos.pop_back();
		(*i)->reemplazos.push_back(ob);
		(*i)->actualizaRadio2();
	    }
	}
    }
}*/

/*void Saco::eliminarObjetoDeCercanos(int idPiv, int idObj){
    vector<Pivote*>::iterator i;
    vector<Objeto*>::iterator j;
    for(i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i){
        if( (*i)->centro->id != idPiv ){
            for( j = (*i)->cercanos.begin(); j != (*i)->cercanos.end(); ++j ){
                if( (*j)->id == idObj ){
                    iter_swap( j , (*i)->cercanos.begin()+((*i)->cercanos.size -1) );
                    (*i)->cercanos.pop_back();
                    if( (*i)->reemplazos.size > 0 ){
                        (*i)->ordenaReemplazos((*i)->pos, 2);
                        (*i)->cercanos.push_back((*i)->reemplazos.back());
                        (*i)->reemplazos.pop_back();
                        (*i)->actualizaRadio2();
                    }
                    (*i)->actualizaRadio();
                }
            }
        }
    }
}*/

/* 
 * File:   Saco.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:30 PM
 */

#include "Cargador.h"
#include "Objeto.h"
#include "Pivote.h"
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <stdlib.h>
#include <float.h>
#include <algorithm>
#include <math.h>
#define N 8192
#define n 256
#define B 256

using namespace std;

Cargador::Cargador() {
}

Cargador::Cargador(const Cargador& orig) {
}

Cargador::~Cargador() {
}

void Cargador::leer(string archivo){
    ifstream entrada;
    ofstream pivotesDisco;
    entrada.open("vector.txt");
    string buf;
    const char* buf2;
    long cluster = 0;
    int nPivotes = 0;
    char* end;
    int cont, mayObj, k, l, minPiv, objs;
    vector<Objeto*>::iterator i;
    vector<Pivote*>::iterator menor, vi;
    vector<double>::iterator j;
    
    string salida = "clusters.bin";
    file.open(salida.c_str(), ios::out | ios::binary);
    objs = 0;
    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        //cout << "creando objeto" << endl;
        Objeto* ob = new Objeto();
        ob->distancias.resize(0);
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
                else if (cont > 2 && cont < 5){
                    ob->poneValor(strtod(buf2, &end));
                    //cout << ob->valores.back() << " ";
                }
            }
            cont++;
        }
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        //cout << "calculando la distanca a los pivotes" << endl;
        distanciasAPivotes(ob);
        ob->pos = bolsa.size();
        //cout << "agregando a la bolsa" << endl;
        bolsa.push_back(ob);
        objs++;
        //cout << "objeto: " << objs << endl;
        //ELEGIR UN PIVOTE PROVISORIO
        if( (bolsa.size()%B) == 1 ){
            //cout << "sacando pivote provisorio" << endl;
            elegirPivoteProvisorio(nPivotes);
            //nPivotes++;
        }
        //PASAR UN PIVOTE A MEMORIA SECUNDARIA
        if( bolsa.size() == N ){
            //cout << "pasando a memoria secundaria" << endl;
            pasarAMemoria(cluster, &nPivotes);
            cluster++;
            //cout << "en la func leer nPivotes: " << nPivotes << endl;
        }
        

    }
    //cout << "termino de leer" << endl;
    //cout << pivotesEnMemoria.size() << "  " << pivotesProvisorios.size() << "  " << bolsa.size() << endl;
    file.close();
    entrada.close();  

}


void Cargador::distanciasAPivotes(Objeto* ob){
    vector<double>::iterator k;
    vector<double>::iterator l;
    if(pivotesProvisorios.size()>0){
        for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i!= pivotesProvisorios.end(); ++i){
            double dist = 0;
            k = (*i)->centro->valores.begin();
            l = (ob)->valores.begin();
            for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                dist += pow( *k + *l, 2 );
            }
            ob->poneDistancia(sqrt(dist));
            ob->aumentaAcumulado(sqrt(dist));
        }

    }    
}

double Cargador::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
    //cout <<"-->llega a la funcion " << ob->id << " " << piv->centro->id << endl;
    vector<double>::iterator k;
    vector<double>::iterator l;
    double dist = 0;
    k = (piv)->centro->valores.begin();
    l = (ob)->valores.begin();
    //cout << "-->empieza el for" << endl;
    for(; k != (piv)->centro->valores.end() && l != (ob)->valores.end(); ++k, ++l){
        dist += pow( *k + *l , 2);
    }
    //cout << "-->termina el for" << endl;
    double raiz = sqrt(dist);
    //cout << "-->aumentando el acumulado" << endl;
    ob->aumentaAcumulado(raiz);
    //cout << "-->agregando a las distancias " << ob->distancias.capacity() << endl;

    //(ob)->poneDistancia( raiz );
    
    
    //cout << "-->saiendo de la funcion" << endl;
    return raiz;
}

void Cargador::obtieneCercanos(Pivote* p){
    p->cercanos.clear();
    int k = 0;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i){
        //cout << "aqui" << endl;
        (*i)->comparando = p->pos;
        (*i)->pos = k;
        //cout << "aca" << endl;
        if( (*i)->id != p->centro->id ){
            //cout << "aculla" << endl;
            if(p->cercanos.size()<(n-1)){
                //cout << "metiendo a la mala" << endl;
                p->cercanos.push_back((*i));
                //cout << "actualizando el mas lejano" << endl;
                p->actualizaMasLejano();
                //cout << "sacando a la wena" << endl;
            }
            else{
                //cout << "aquino" << endl;
                if( (*i)->distancias[p->pos] < p->radio ){
                    p->cercanos.erase( p->posMasLejano );
                    p->cercanos.push_back((*i));
                    p->actualizaMasLejano();
                }
                //cout << "acalla" << endl;
            }
            //cout << "per cua" << endl;
        }
        k++;
    }
    //cout << "saliendo de buscar los cercanos" << endl;
}


/*double Cargador::obtieneRadio(vector<Objeto*>& obs, int pos){
    double mayor = -DBL_MAX;
    for(vector<Objeto*>::iterator i = obs.begin(); i != obs.end(); ++i){
        if((*i)->distancias.at(pos) > mayor)
            mayor = (*i)->distancias.at(pos);
    }
    return mayor;
}*/

void Cargador::sacaPivoteDelSaco(int id){
    //cout << "id pivote : " << id << endl;
    for(vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
	    if( (*i)->id == id){
            //cout << "pillo el pivote en la bolsa" << endl;
	        bolsa.erase(i);
	        return;
	    }
    }
}


void Cargador::elegirPivoteProvisorio(int nPivotes){
    //cout << "entro a la busqueda de pivote" << endl;
    vector<Objeto*>::iterator mayObj;
    vector<Objeto*>::iterator i;
    double disAc = -DBL_MAX;
    for( i = bolsa.begin(); i != bolsa.end(); ++i ){
        if( (*i)->distanciaAcumulada >= disAc && !(*i)->esPivote ){
            mayObj = i;
            disAc = (*i)->distanciaAcumulada;
        }
    }

    //cout << "creando pivote provisorio" << endl;
    Pivote* piv = new Pivote();
    piv->centro = (*mayObj);
    piv->centro->id = (*mayObj)->id;
    piv->pos = pivotesProvisorios.size();
    (*mayObj)->esPivote = true;
    pivotesProvisorios.push_back(piv);
    //cout << "pivote Provisorio nuevo id : " << (*mayObj)->id << " bolsa[]->esPivote : " << (*mayObj)->esPivote << " piv->centro->esPivote :" << piv->centro->esPivote << " piv->pos :" << piv->pos <<  endl;
    //cout << bolsa.size() << endl;
    int k = 0;
    double distancia;
    for( i = bolsa.begin(); i != bolsa.end(); ++i ){
        //cout << "se mete aca por lo menos " << k << " viendo las distancias " << (*i)->distancias.size() << endl;
        distancia = distanciaPivoteNuevo((*i), piv);
        (*i)->poneDistancia(distancia);
        //(*i)->distancias.resize( (*i)->distancias.size()+1, distancia );
        k++;
    }
    //cout << "saliendo de elegir pivote" << endl;
}

void Cargador::pasarAMemoria(int nCluster, int* nPivotes){
    
    vector<Pivote*>::iterator minPiv = pivotesProvisorios.begin();
    vector<Pivote*>::iterator j;
    vector<Objeto*>::iterator i;
    int k, position;
    //cout << "buscando el de menor radio de cobertura" << endl;
    for( j = pivotesProvisorios.begin(); j != pivotesProvisorios.end(); ++j ){
        //cout << "buscando cercanos" << endl;
        obtieneCercanos((*j));
        (*j)->actualizaRadio();
        //cout << "paso el actualizaRadio" << endl;
        if( (*j)->radio <= (*minPiv)->radio )
            minPiv = j;
        //cout << "paso el if" << endl;
    }
    //cout << "pasando a memoria el pivote : " << pivotesProvisorios[minPiv]->centro->id << " en la posicion : " << minPiv << " de los pivotes provisorios" << endl;
    //cout << "preparando para escribir" << endl;
    (*minPiv)->numCluster = nCluster;
    //cout << "Pasando a memoria el pivote " << pivotesProvisorios[minPiv]->centro->id << " en la posicion " << minPiv << endl; 
    const char* pointer = reinterpret_cast<const char*>(&(*minPiv)->centro->valores[0]);

    file.write( pointer, sizeof(double)*2 );
    //cout << "escribiendo" << endl;
    for( i = (*minPiv)->cercanos.begin(); i != (*minPiv)->cercanos.end(); ++i ){
        const char* pointer = reinterpret_cast<const char*>(&(*i)->valores[0]);
        file.write( pointer, sizeof(double)*(*i)->valores.size() );
    }
    //cout << "dejando de escribir, sacando del saco a los locos" << endl;
    int pivotesLlevados = 0;
    for( i = (*minPiv)->cercanos.begin(); i != (*minPiv)->cercanos.end(); ++i ){
        if( (*i)->esPivote ){
            sacaPivoteDeProvisorios( (*i)->id );
            elegirPivoteProvisorio(*nPivotes);
        }
        bolsa.erase(bolsa.begin()+(*i)->pos);
        //cout << "borro uno" << endl;
        ajustaPosiciones();
    }
    
    //cout << "terminando de sacar los pivotes que se fueron" << endl;
    sacaPivoteDelSaco( (*minPiv)->centro->id );
    //cout << "sacando el centro de la lista de pivotes" << endl;
    pivotesEnMemoria.push_back( (*minPiv) );
    position = (*minPiv)->pos;
    (*minPiv)->cercanos.clear();
    pivotesProvisorios.erase( minPiv );
    //cout << "ajustando posiciones de la lista de pivotes. minPiv " << minPiv << endl;
    ajustaPosicionesPivotes(position);
    //cout << "terminando de pasar a memoria " << endl;
}

void Cargador::sacaPivoteDeProvisorios(int id){
    int k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i ){
        if( (*i)->centro->id == id ){
            pivotesProvisorios.erase( i );
            //cout << "ajustando posiciones" << endl;
            ajustaPosicionesPivotes(k);
            //cout << "deje de ajustar" << endl;
            return;
        }
        k++;
    }
}

void Cargador::ajustaPosicionesPivotes(int posElim){
    int k;
    //cout << "-> eliminando la distancia en los objetos de la bolsa" << endl;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
        (*i)->distancias.erase( (*i)->distancias.begin() + posElim );
    }
    //cout << "-> ajustando las posiciones" << endl;
    k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i  ){
        (*i)->pos = k;
        k++;
    }
    //cout << "-> fin" << endl;
}

void Cargador::ajustaPosiciones(){
    int k = 0;
    for(vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
        (*i)->pos = k;
        k++;
    }
}
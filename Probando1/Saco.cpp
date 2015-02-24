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
#define N 16384
#define n 512
#define B 512

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
    string buf;
    const char* buf2;
    long cluster = 1;
    int nPivotes = 0;
    char* end;
    int cont, mayObj, k, l, minPiv;
    vector<Objeto*>::iterator i;
    vector<Pivote*>::iterator menor, vi;
    vector<double>::iterator j;
    
    string salida = "clusters.bin";
    file.open(salida.c_str(), ios::out | ios::binary);

    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        //cout << "creando objeto" << endl;
        Objeto* ob = new Objeto();
        
        cont = 0;
        cout << "leyendo objeto" << endl;
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
        distanciasAPivotes(ob);
        bolsa.push_back(ob);

        //ELEGIR UN PIVOTE PROVISORIO
        if( (bolsa.size()%B) == 1 ){
            cout << "sacando pivote provisorio" << endl;
            elegirPivoteProvisorio(nPivotes);
            //nPivotes++;
        }
        //PASAR UN PIVOTE A MEMORIA SECUNDARIA
        if( bolsa.size() == N ){
            cout << "pasando a memoria secundaria" << endl;
            pasarAMemoria(cluster, &nPivotes);
            cluster++;
            //cout << "en la func leer nPivotes: " << nPivotes << endl;
        }
        

    }
        
    file.close();
    entrada.close();  

}


void Saco::distanciasAPivotes(Objeto* ob){
    vector<double>::iterator k;
    vector<double>::iterator l;
    if( !ob->esPivote ){
        if(pivotesProvisorios.size()>0){
            for(vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i!= pivotesProvisorios.end(); ++i){
                double dist = 0;
                k = (*i)->centro->valores.begin();
                l = (ob)->valores.begin();
                for(; k!=(*i)->centro->valores.end() &&  l!=(ob)->valores.end(); ++k, ++l){
                    dist += ((*k+*l)*(*k+*l));
                }
                ob->distancias.push_back(sqrt(dist));
                ob->aumentaAcumulado(sqrt(dist));
            }

        }
        /*
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
        */
    }
    else{
        (*ob).distanciaAcumulada = - DBL_MAX;
    }
    
}

void Saco::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
    vector<double>::iterator k;
    vector<double>::iterator l;
    double dist = 0;
    k = (piv)->centro->valores.begin();
    l = (ob)->valores.begin();
    for(; k != (piv)->centro->valores.end() && l != (ob)->valores.end(); ++k, ++l){
        dist += ((*k+*l)*(*k+*l));
    }
    ob->distancias.push_back(sqrt(dist));
    ob->aumentaAcumulado(sqrt(dist));

}

void Saco::obtieneCercanos(Pivote* p){
    p->cercanos.clear();
    for( int k = 0; k < bolsa.size(); k++){
        //cout << "aqui" << endl;
        bolsa[k]->comparando = p->pos;
        bolsa[k]->pos = k;
        //cout << "aca" << endl;
        if( bolsa[k]->id != p->centro->id ){
            //cout << "aculla" << endl;
            if(p->cercanos.size()<(n-1)){
                //cout << "metiendo a la mala" << endl;
                p->cercanos.push_back(bolsa[k]);
                //cout << "actualizando el mas lejano" << endl;
                p->actualizaMasLejano();
                //cout << "sacando a la wena" << endl;
            }
            else{
                //cout << "aquino" << endl;
                if( bolsa[k]->distancias[p->pos] < p->radio ){
                    p->cercanos[p->posMasLejano] = (bolsa[k]);
                    p->actualizaMasLejano();
                }
                //cout << "acalla" << endl;
            }
            //cout << "per cua" << endl;
        }
    }
    //cout << "saliendo de buscar los cercanos" << endl;
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
    //cout << "id pivote : " << id << endl;
    for(int i = 0; i < bolsa.size(); i++){
	    if( bolsa[i]->id == id){
            //cout << "pillo el pivote en la bolsa" << endl;
	        bolsa.erase(bolsa.begin() + i);
	        return;
	    }
    }
}

void Saco::elegirPivoteProvisorio(int nPivotes){
    int mayObj = 0;
    int k;
    double disAc = -DBL_MAX;
    for( k = 0; k < bolsa.size(); k++){
        if( bolsa[k]->distanciaAcumulada >= disAc && !bolsa[k]->esPivote ){
            mayObj = k;
            disAc = bolsa[k]->distanciaAcumulada;
        }
    }

    //cout << "creando pivote provisorio" << endl;
    Pivote* piv = new Pivote();
    piv->centro = bolsa[mayObj];
    piv->centro->id = bolsa[mayObj]->id;
    piv->pos = pivotesProvisorios.size();
    bolsa[mayObj]->esPivote = true;
    pivotesProvisorios.push_back(piv);
    cout << "pivote Provisorio nuevo id : " << bolsa[mayObj]->id << " bolsa[]->esPivote : " << bolsa[mayObj]->esPivote << " piv->centro->esPivote :" << piv->centro->esPivote << " piv->pos :" << piv->pos <<  endl;
    for( k = 0; k < bolsa.size(); k++ ){
        distanciaPivoteNuevo(bolsa[k], piv);
    }
    cout << "saliendo de elegir pivote" << endl;
}

void Saco::pasarAMemoria(int nCluster, int* nPivotes){
    
    int minPiv = 0;
    int k;
    cout << "buscando el de menor radio de cobertura" << endl;
    for( k = 0; k < pivotesProvisorios.size(); k++){
        //cout << "buscando cercanos" << endl;
        obtieneCercanos(pivotesProvisorios[k]);
        pivotesProvisorios[k]->actualizaRadio();
        //cout << "paso el actualizaRadio" << endl;
        if( pivotesProvisorios[k]->radio <= pivotesProvisorios[minPiv]->radio )
            minPiv = k;
        //cout << "paso el if" << endl;
    }
    cout << "pasando a memoria el pivote : " << pivotesProvisorios[minPiv]->centro->id << " en la posicion : " << minPiv << " de los pivotes provisorios" << endl;
    //cout << "preparando para escribir" << endl;
    pivotesProvisorios[minPiv]->numCluster = nCluster;

    const char* pointer = reinterpret_cast<const char*>(&pivotesProvisorios[minPiv]->centro->valores[0]);

    file.write( pointer, sizeof(double)*2 );
    //cout << "escribiendo" << endl;
    for( k = 0; k < pivotesProvisorios[minPiv]->cercanos.size(); k++ ){
        /*for( int l = 0; l < pivotesProvisorios[minPiv]->cercanos[k]->valores.size(); l++ ){
            file << pivotesProvisorios[minPiv]->cercanos[k]->valores[l] << " ";
        }
        file << "\n";*/
        const char* pointer = reinterpret_cast<const char*>(&pivotesProvisorios[minPiv]->cercanos[k]->valores[0]);
        file.write( pointer, sizeof(double)*2 );
    }
    //cout << "dejando de escribir, sacando del saco a los locos" << endl;
    int pivotesLlevados = 0;
    for( k = 0; k < pivotesProvisorios[minPiv]->cercanos.size(); k++){
        //iter_swap( bolsa.begin() + pivotesProvisorios[minPiv]->cercanos[k]->pos , bolsa.begin() + (bolsa.size() - 1) );
        //cout << pivotesProvisorios[minPiv]->cercanos[k]->id << " ";
        //cout << k << " ";
        if( pivotesProvisorios[minPiv]->cercanos[k]->esPivote ){
            pivotesLlevados++;
        }
        
        bolsa[pivotesProvisorios[minPiv]->cercanos[k]->pos] = bolsa[bolsa.size()-1];
        bolsa.pop_back();
        bolsa[pivotesProvisorios[minPiv]->cercanos[k]->pos]->pos = pivotesProvisorios[minPiv]->cercanos[k]->pos;
        
    }
    //cout << " fin" << endl;

    /*for( k = 0; k < bolsa.size(); k++){
        cout << bolsa[k]->id << " ";
    }
    cout << endl;
    for( k = 0; k < pivotesProvisorios.size(); k++ ){
        cout << pivotesProvisorios[k]->centro->id << " ";
    }
    cout << endl;
    */
    //cout << "sacando los pivotes que se fueron" << endl;
    if( pivotesLlevados > 0 ){
        //cout << "se lleva " << pivotesLlevados << " pivotes" << endl;
        Pivote* aux = pivotesProvisorios[minPiv];
        for( k = 0; k < aux->cercanos.size(); k++ ){
            if( aux->cercanos[k]->esPivote ){
                //cout << "sacando el " << aux->cercanos[k]->id << endl;
                sacaPivoteDeProvisorios( aux->cercanos[k]->id );
                elegirPivoteProvisorio(*nPivotes);
                //cout << *nPivotes << endl;
                //*nPivotes += 1;
                //cout << *nPivotes << endl;
            }
        }

    }
    //cout << "terminando de sacar los pivotes que se fueron" << endl;

    sacaPivoteDelSaco( pivotesProvisorios[minPiv]->centro->id );
    //cout << "sacando el centro de la lista de pivotes" << endl;
    pivotesEnMemoria.push_back(pivotesProvisorios[minPiv]);
    pivotesProvisorios.erase(pivotesProvisorios.begin() + minPiv);
    //cout << "ajustando posiciones de la lista de pivotes. minPiv " << minPiv << endl;
    ajustaPosicionesPivotes(minPiv);
    //cout << "terminando de pasar a memoria " << endl;
}

void Saco::sacaPivoteDeProvisorios(int id){
    int k;
    for( k = 0; k < pivotesProvisorios.size(); k++){
        if( pivotesProvisorios[k]->centro->id == id ){
            pivotesProvisorios.erase( pivotesProvisorios.begin() + k );
            cout << "ajustando posiciones" << endl;
            ajustaPosicionesPivotes(k);
            cout << "deje de ajustar" << endl;
            return;
        }
    }
}

void Saco::ajustaPosicionesPivotes(int posElim){
    int k;
    //cout << "-> eliminando la distancia en los objetos de la bolsa" << endl;
    for( k = 0; k < bolsa.size(); k++){
        bolsa[k]->distancias.erase( bolsa[k]->distancias.begin() + posElim );
    }
    //cout << "-> ajustando las posiciones" << endl;
    for( k = 0; k < pivotesProvisorios.size(); k++ ){
        pivotesProvisorios[k]->pos = k;
    }
    //cout << "-> fin" << endl;
}

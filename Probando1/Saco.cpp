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
#include <math.h>
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
    ofstream pivotesDisco;
    entrada.open("vector.txt");
    string buf, salida;
    const char* buf2;
    long cluster = 1;
    int nPivotes = 0;
    yaHayUno = false;
    yaHayOtro = false;
    char* end;
    int cont, mayObj, k;
    vector<Objeto*>::iterator i;
    vector<Pivote*>::iterator menor, vi;
    vector<double>::iterator j;
    ofstream file;
    for( string linea; getline(entrada, linea);){
        
        stringstream ss(linea);
        
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
        //cout << endl;
        //TIENE QUE CALCULAR LA DISTANCIA A LOS CENTROS Y LOS PIVOTES TEMPORALES
        distanciasAPivotes(ob);
	//cout << "agregando a los pivotes cercanos" << endl;
        agregarACercanosDePivotes(ob);
        //AGREGAR A LA BOLSA
        //cout << bolsa.size() << endl;
        bolsa.push_back(ob);
        //cout << ob->id << " " << ob->distanciaAcumulada << endl;
        //VERIIFCAR CUANTOS OBJETOS HAY EN LA BOLSA
        //SI HAY B+1 ELEMENTOS (O 1), TIENE QUE ELEGIR UN PIVOTE PROVISORIO
        
        if( (bolsa.size() % B) == 1 ){
            
            //cout << "sacando pivote provisorio" << endl;
            //ELEGIR PIVOTE PROVISORIO
            //CREAMOS LOS ITERADORES PARA RECORRER LA BOLSA Y MARCAR EL QUE TENGA MAYOR DISTANCIA ACUMULADA
            
            mayObj = 0;
            k = 0;
            //RECORREMOS LA BOLSA BUSCANDO AL CON MAYOR DISTANCIA ACUMULADA
            for( i = bolsa.begin(); i != bolsa.end(); ++i){
		(*i)->pos = k;
                if(!estaEnPivotes((*i)->id) && (*i)->distanciaAcumulada >= bolsa.at(mayObj)->distanciaAcumulada)
                    mayObj = k;
                k++;
            }
            //LUEGO, SI O SI TENDREMOS UN OBJETO SELECCIONADO
            //Y CREAMOS UN PIVOTE CON ÉL
            Pivote* p = new Pivote(*bolsa.at(mayObj));
	    (*p).pos = nPivotes;
	    
            //LO AGREGAMOS A LOS PIVOTES PROVISORIOS
            pivotesProvisorios.push_back(p);
            //LO ELIMINAMOS DE LA BOLSA DE OBJETOS ENTRANTES
            //bolsa.erase(mayObj);
            //Y AGREGAMOS A LOS DEMAS OBJETOS LA DISTANCIA AL NUEVO PIVOTE CREADO
            for( i = bolsa.begin(); i != bolsa.end(); ++i){
                distanciaPivoteNuevo((*i), p);
            }
	    //cout << "obteniendo sus cercanos" << endl;
	    (*p).cercanos = obtieneCercanos(p->pos);
	    nPivotes++;
            //cout << (*p).centro->id << endl;
            yaHayUno=true;
        }
        

        //SI ALCANZAMOS EL TAMANIO ASIGNADO A LA BOLSA
        //TENEMOS QUE VACIAR UN POCO, CREANDO UN CLUSTER
        if(bolsa.size()==N){
	    //cout << "pasando a memoria" << endl;
            //cout << "hola 2" << endl;
            //ELEGIR UN PIVOTE PARA CENTRO DE CLUSTER
            //PRIMERO DETERMINAR RADIO DE COBERTURA
            //PARA ESTO SE DEBE BUSCAR LOS n-1 VECINOS MAS CERCANOS DE CADA PIVOTE PROVISORIO
            //CREAMOS UNA LISTA AUXILIAR PARA GUARDAR LOS VECTORES CREADOS CON LOS VECINOS DE CADA PIVOTE
            //vector< vector<Objeto*> >* listAux = new vector< vector<Objeto*> >();
            //(*listAux).clear();
            //int pos = 0;
            //RECORREMOS EL VECTOR DE PIVOTES PROVISORIOS
            //cout << "obteniendo el de menor radio" << endl;
	    menor = pivotesProvisorios.begin();
            for( vi = pivotesProvisorios.begin(); vi != pivotesProvisorios.end() ; ++vi ){
                //POR CADA PIVOTE, OBTENEMOS SUS VECINOS Y LOS GUARDAMOS EN LISTAUX
                //(*listAux).push_back(obtieneCercanos((*vi)->pos));
                //LUEGO, A CADA PIVOTE LE CALCULAMOS SU RADIO DE COBERTURA
                //(*vi)->radio = obtieneRadio((*listAux).back(),(*vi)-> pos);
		if((*menor)->radio > (*vi)->radio)
		    menor = vi;
		
            }
            //CREAMOS LOS ITERADORES PARA VOLVER A RECORRER EL VECTRO DE PIVOTES PROVISORIOS
            //Y BUSCAR QUE QUE TENGA MENOR RADIO DE COBERTURA
            //Y TAMBIÉN UNA REFERENCIA AL VECTOR DE VECINOS QUE TIENE
            /*vector<Pivote*>::iterator vi = pivotesProvisorios.begin();
            //vector< vector<Objeto*> >::iterator oi = (*listAux).begin();
            vector<Pivote*>::iterator vi_mayor = vi;
            vector< vector<Objeto*> >::iterator oi_mayor = oi;
            
            //RECORRERMOS EL VECTOR BUSCANDO EL CON MENOR RADIO
	    //cout << "buscando el con menor radio" << endl;
	    //cout << pivotesProvisorios.size() << endl;
            for(; vi != pivotesProvisorios.end() && oi != (*listAux).end(); ++vi, ++oi){
                if((*vi)->radio <= (*vi_mayor)->radio){
                    vi_mayor = vi;
                    oi_mayor = oi;
                }
            }*/
            //cout << (*vi_mayor)->centro->id << endl;
            //PARA VACIAR:
            ////CALCULAR EL CENTRO DE LOS QUE ESTAN EN LA BOLSA
            
            //cout << "sacando " << bolsa.size()*sizeof(double) << endl;
            
            stringstream ss;
            ss << "cluster";
            ss << cluster;
            ss << ".txt";
            //cout << cluster << endl;
            salida = ss.str();
            //cout << salida << cluster << endl;
            file.open(salida.c_str());
            //cout << "escribiendo archivo de cluster" << endl;
	    //cout << (*oi_mayor).size() << endl;
            for( i = (*menor)->cercanos.begin(); i!= (*menor)->cercanos.end(); ++i){
                //Objeto o = bolsa.at(i);
                //cout << (*i)->valores.size() << endl;
                file << (*i)->id << " ";
                for( j = (*i)->valores.begin(); j!= (*i)->valores.end() ; ++j){
                    //cout << (*k) << " ";
	    	    file << *j << " ";
                }
                //file << (*i);
                file << "\n";
		//cout << endl;
                //bolsa.erase(bolsa.begin()+(*i)->pos);
            }
	    //cout << "eliminando de la bolsa" << endl;
	    for( i = (*menor)->cercanos.begin(); i!= (*menor)->cercanos.end(); ++i){
		iter_swap(bolsa.begin()+(*i)->pos, bolsa.begin()+(bolsa.size()-1));
                eliminarObjetoDeCercanos((*menor)->centro->id, (*i)->id);
		bolsa.pop_back();
		//bolsa.erase(bolsa.begin()+(*i)->pos);
	    }
            file.close();
            //(*oi_mayor).clear();
            //(*listAux).clear();
            cluster++;
	    //cout << "agregando el pivote en vector de pivotesEnMemoria" << endl;
            pivotesDisco.open("pivotes.txt", ofstream::app);
            
            pivotesDisco << (*menor)->centro->id << " " << (*menor)->radio << "\n";
            pivotesEnMemoria.push_back(*menor);
	    //cout << "sacando el pivote de los provisorios" << endl;
	    sacaPivoteDelSaco((*menor)->centro->id);
            pivotesProvisorios.erase(menor);
            //bolsa.erase(vi_mayor);
            pivotesDisco.close();
            yaHayOtro = true;
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
    if(!estaEnPivotes((*ob).id)){
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
    for(vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i){
	if((*i)->id == id){
	    bolsa.erase(i);
	    return;
	}
    }
}

void Saco::agregarACercanosDePivotes(Objeto* ob){
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
}

void Saco::eliminarObjetoDeCercanos(int idPiv, int idObj){
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
}

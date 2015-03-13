
#include "Consultor.h"
#include "Objeto.h"
#include "Pivote.h"
#include <vector>
#include <set>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace std;

Consultor::Consultor(){
	this->archivo = fopen("clusters.bin", "rb");
};

Consultor::Consultor(int dim){
	this->archivo = fopen("clusters.bin", "rb");
	this->dimension = dim;
};

Consultor::Consultor(const Consultor& orig){
	this->archivo = fopen("clusters.bin", "rb");
};

Consultor::~Consultor(){
	fclose(this->archivo);
};

vector<Objeto*> Consultor::consultarBusquedaRango(Objeto* ob, double r){
	vector<Objeto*> cercanos;
	vector<Pivote*> candidatos;
	vector<Pivote*>::iterator l;
	vector<Objeto*>::iterator m;
	int i, j, k;
	double dist;
	bool esta;

	//BUSCANDO EN LOS CLUSTER

	for( l = clusters.begin(); l != clusters.end(); ++l ){
		dist = 0;
		for( j = 0; j < (*l)->centro->valores.size(); j++ )
			dist += pow( (*l)->centro->valores[j] + ob->valores[j] , 2 );
		dist = sqrt( dist );
		if( dist <= (r + (*l)->radio )){
			candidatos.push_back((*l));
		}
	}

	if(!candidatos.empty()){

		for( l = candidatos.begin(); l != candidatos.end(); ++l ){

			vector<Objeto*> objetosBloque = extraerCluster( (*l)->numCluster );

			for( m = objetosBloque.begin(); m != objetosBloque.end(); ++m ){
				dist = 0;
				for( k = 0; k < (*m)->valores.size(); k++ ){
					dist += pow( (*m)->valores[k] + ob->valores[k] , 2 );
				}
				dist = sqrt( dist );
				if( dist <= r ){
					cercanos.push_back((*m));
				}
			}
			objetosBloque.clear();
		}
	}

	vector<double> distPivs;
	for( l = pivotes.begin(); l != pivotes.end(); ++l ){
		dist=0;
		for( i = 0; i < (*l)->centro->valores.size(); i++ ){
			dist += pow( (*l)->centro->valores[i] + ob->valores[i], 2 );
		}
		dist = sqrt( dist );
		distPivs.push_back( dist );
	}

	//ELIGIENDO POR DESIGUALDAD TRIANGULAR

	for( m = saco.begin(); m != saco.end(); ++m ){
		for( j = 0; j < (*m)->distancias.size(); j++ ){
			if( fabs( distPivs[j] - (*m)->distancias[j] ) <= r ){ // DESIGUALDAD TRIANGULAR
				dist = 0;
				for( k = 0; k < (*m)->valores.size(); k++ ){
					dist += pow( (*m)->valores[k] + ob->valores[k], 2 );
				}
				dist = sqrt( dist );
				if( dist <= r){
					cercanos.push_back( (*m) );
					break;
				}

			}
		}
	}
	//cout << "objetos sacados de ram: " << cercanos.size()-disco << endl;
	
	return cercanos;

};

bool Consultor::eliminarObjeto(Objeto* ob){
	bool eliminado = false;
	bool igual;

	vector<double> distCentros = distanciaACentros( ob );
	vector<double> distPivotes = distanciaAPivotes( ob );

	vector<Pivote*> candidatos;
	vector<Pivote*>::iterator k;

	vector<Objeto*>::iterator i;

	double dist;
	int j;
	//Buscando en RAM
	for( i = saco.begin(); i != saco.end() && !eliminado; ++i ){
		igual = true;
		for( j = 0; j < (*i)->valores.size() && igual; j++ ){
			if( ob->valores[j] !=  (*i)->valores[j] ){
				igual = false;
			}
		}
		if( igual ){
			if( (*i)->esPivote ){
				eliminaPivote( (*i)->id );
				elegirPivote();
			}
			saco.erase(i);
			ajustaPosicionesSaco();
			eliminado = true;
			return eliminado;
		}
	}

	//BUSCAR EN CLUSTERS

	for( k = clusters.begin(); k != clusters.end(); ++k ){
		dist = 0;
		for( j = 0; j < (*k)->centro->valores.size(); j++ ){
			dist += ( (*k)->centro->valores[j] + ob->valores[j] , 2 );
		}
		dist = sqrt( dist );
		if( dist <= (*k)->radio ){
			vector<Objeto*> bloque = extraerCluster( (*k)->numCluster );
			for(i = bloque.begin(); i != bloque.end(); ++i ){
				igual = true;
				for( j = 0; j < (*i)->valores.size() && igual; j++ ){
					if( ob->valores[j] != (*i)->valores[j] )
						igual = false;
				}

				if( igual ){
					
					if( i - bloque.begin() == 0 ){
						//ES EL CENTROOOOO!!!! AAAAAHHHHH!!!	
						cout << "se topo con el centro de un cluster, nada que hacer aun" << endl;
					}
					else{
						//BUSCA UNO EN LOS DE LA RAM
						Objeto* nuevo = cercanoA(bloque[0]);
						if( nuevo->esPivote ){
							eliminaPivote( nuevo->id );
							elegirPivote();
						}
						bloque.erase(i);
						bloque.push_back(nuevo);
						saco.erase( saco.begin() + nuevo->pos );
						ajustaPosicionesSaco();
						(*k)->cercanos = bloque;
						(*k)->centro = bloque[0];
						(*k)->actualizaRadioCentro();
						reescribeCluster((*k));
						eliminado = true;
						return eliminado;
					}
				}


			}

		}

	}
	return eliminado;

};

vector<double> Consultor::distanciaACentros(Objeto* ob){
	vector<double> distancias;
	double dist;
	for( vector<Pivote*>::iterator i = clusters.begin(); i != clusters.end(); ++i ){
		dist = 0;
		for( int j = 0; j < (*i)->centro->valores.size(); j++ ){
			dist += pow( (*i)->centro->valores[j] + ob->valores[j] , 2 );
		}
		dist = sqrt(dist);
		distancias.push_back(dist);
	}

	return distancias;

};

vector<double> Consultor::distanciaAPivotes(Objeto* ob){
	vector<double> distancias;
	double dist;
	for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i ){
		dist = 0;
		for( int j = 0; j < (*i)->centro->valores.size(); j++ ){
			dist += pow( (*i)->centro->valores[j] + ob->valores[j] , 2 );
		}
		dist = sqrt(dist);
		distancias.push_back(dist);
	}

	return distancias;
};

vector<Objeto*> Consultor::extraerCluster(int numCluster){
	vector<Objeto*> objetos;
	int j;
	fseek( archivo, 4096*numCluster, SEEK_SET );
	double block[512];
	fread( block, sizeof(double), 512, archivo );

	for( j = 0; j < 512; j+=2 ){
		Objeto* objMem = new Objeto();
		objMem->poneValor( block[j] );
		objMem->poneValor( block[j+1] );
		objetos.push_back(objMem);
	}

	return objetos;
};

void Consultor::eliminaPivote(int id){
    int k = 0;
    for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i ){
        if( (*i)->centro->id == id ){
            pivotes.erase( i );
            ajustaPosicionesPivotes(k);
            return;
        }
        k++;
    }
};

void Consultor::elegirPivote(){

    vector<Objeto*>::iterator mayObj;
    vector<Objeto*>::iterator i;
    double disAc = -DBL_MAX;
    for( i = saco.begin(); i != saco.end(); ++i ){
        if( (*i)->distanciaAcumulada >= disAc && !(*i)->esPivote ){
            mayObj = i;
            disAc = (*i)->distanciaAcumulada;
        }
    }

    Pivote* piv = new Pivote();
    piv->centro = (*mayObj);
    piv->centro->id = (*mayObj)->id;
    piv->pos = pivotes.size();
    (*mayObj)->esPivote = true;
    pivotes.push_back(piv);
    
    double distancia;
    for( i = saco.begin(); i != saco.end(); ++i ){
        distancia = distanciaPivoteNuevo((*i), piv);
        (*i)->poneDistancia(distancia);
    }

};

void Consultor::ajustaPosicionesPivotes(int posElim){
    int k;
    for( vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
        (*i)->distancias.erase( (*i)->distancias.begin() + posElim );
    }
    k = 0;
    for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i  ){
        (*i)->pos = k;
        k++;
    }
};

void Consultor::ajustaPosicionesSaco(){
    int k = 0;
    for(vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
        (*i)->pos = k;
        k++;
    }
};

double Consultor::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
	vector<double>::iterator k;
    vector<double>::iterator l;
    double dist = 0;
    k = (piv)->centro->valores.begin();
    l = (ob)->valores.begin();

    for(; k != (piv)->centro->valores.end() && l != (ob)->valores.end(); ++k, ++l){
        dist += pow( *k + *l , 2);
    }

    double raiz = sqrt(dist);
    ob->aumentaAcumulado(raiz);

    return raiz;
};

Objeto* Consultor::cercanoA(Objeto* centro){
	double dist, distMin;
	distMin = DBL_MAX;
	Objeto* cercano;
	for( vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
		dist = 0;
		for( int j =0; j < (*i)->valores.size(); j++ ){
			dist += pow( (*i)->valores[j] + centro->valores[j] , 2 );
		}
		dist = sqrt( dist );
		if( distMin > dist ){
			distMin = dist;
			cercano = (*i);
		}
	}

	return cercano;
};

void Consultor::reescribeCluster(Pivote* centro){
	vector<Objeto*>::iterator i;
	fseek( archivo, 4096*centro->numCluster, SEEK_SET );
    //cout << "escribiendo" << endl;
    for( i = centro->cercanos.begin(); i != centro->cercanos.end(); ++i ){
        const char* pointer = reinterpret_cast<const char*>(&(*i)->valores[0]);
        fwrite( pointer, sizeof(double), (*i)->valores.size(), archivo );
    }

}
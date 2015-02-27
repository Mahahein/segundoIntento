
#include "Consultor.h"
#include "Objeto.h"
#include "Pivote.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

Consultor::Consultor(){
	this->archivo = fopen("clusters.bin", "rb");
};

Consultor::Consultor(const Consultor& orig){
	this->archivo = fopen("clusters.bin", "rb");
};

Consultor::~Consultor(){
	fclose(this->archivo);
};

vector<Objeto*> Consultor::consultar(Objeto* ob, double r){
	vector<Objeto*> cercanos;
	vector<Pivote*> candidatos;
	int i, j, k;
	double dist;
	for( i = 0; i < clusters.size(); i++ ){
		dist = 0;
		for( j = 0; j < clusters[i]->centro->valores.size(); j++ )
			dist += pow( clusters[i]->centro->valores[j] + ob->valores[j] , 2 );
		dist = sqrt( dist );
		if( dist <= (r + clusters[i]->radio ) ){
			candidatos.push_back(clusters[i]);
		}
	}
	for(i = 0; i < pivotes.size(); i++){
		dist = 0;
		for(j = 0; j < pivotes[i]->centro->valores.size(); j++)
			dist += pow( pivotes[i]->centro->valores[j] + ob->valores[j] , 2 );
		dist = sqrt( dist );
		if( dist <= (r + pivotes[i]->radio ) ){
			candidatos.push_back(pivotes[i]);
		}
	}
	if(!candidatos.empty()){
		for( i = 0; i < candidatos.size(); i++ ){
			if( candidatos[i]->numCluster != -1 ){
				//EN MEMORIA
				fseek( archivo, 512*candidatos[i]->numCluster, SEEK_SET );
				double block[512];
				fread( block, sizeof(double), 512, archivo );
				vector<Objeto*> objetosBloque;
				for( j = 0; j < 512; j+=2 ){
					Objeto* objMem = new Objeto();
					objMem->poneValor( block[j] );
					objMem->poneValor( block[j+1] );
					objetosBloque.push_back(objMem);
				}
				for( j = 0; j < objetosBloque.size(); j++ ){
					dist = 0;
					for( k = 0; k < objetosBloque[j]->valores.size(); k++ ){
						dist += pow( objetosBloque[j]->valores[k] + ob->valores[k] , 2 );
					}
					dist = sqrt( dist );
					if( dist <= r ){
						cercanos.push_back(objetosBloque[j]);
					}
				}
			}
			else{
				//EN RAM
				for( j = 0; j < candidatos[i]->cercanos.size(); j++ ){
					dist = 0;
					for( k = 0; k < candidatos[i]->cercanos[j]->valores.size(); k++ ){
						dist += pow( candidatos[i]->cercanos[j]->valores[k] + ob->valores[k], 2 );
					}
					dist = sqrt( dist );
					if( dist <= r ){
						cercanos.push_back( candidatos[i]->cercanos[j] );
					}
				}
				
			}
		}
	}
	return cercanos;

};
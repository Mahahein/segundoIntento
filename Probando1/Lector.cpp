

#include <iostream>
#include "Lector.h"
#include "Objeto.h"
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

Lector::Lector(){

}

Lector::Lector(int dimen){
	this->dimension = dimen;
}

Lector::Lector(const Lector& orig){

}

Lector::~Lector(){

}

vector<Objeto*> Lector::leerDesdeArchivo(string nombreArchivo){
	//CONVENCION MIA :P
	//-- PRIMERA LINEA: NUMERO DE CONSULTAS
	//-- SEGUNDA LINEA EN ADELANTE: VECTOR DEL OBJETO - RADIO
	cout << "Leyendo consultas desde " << nombreArchivo << endl;
	vector<Objeto*> consultas;
	FILE* archivo = fopen(nombreArchivo.c_str(), "r");
	int ncon;
	fscanf(archivo, "%d", &ncon);
	int cont = 0;
	int cont2;
	double e, r;
	while( cont < ncon ){
		Objeto* o = new Objeto();
		cont2 = 0;
		while( cont2 < this->dimension ){
			fscanf(archivo, "%lf", &e);
			o->poneValor(e);
			cont2++;
		}
		fscanf(archivo, "%lf", &r);
		o->distanciaAcumulada = r;
		consultas.push_back(o);
		cont++;
	}
	return consultas;
}

Objeto* Lector::leerDesdeEE(){
	Objeto* o = new Objeto();
	int cont = 0;
	double e;
	while( cont < this->dimension ){
		printf( "Ingrese elemento %d del vector: ", cont);
		scanf( "%lf", &e );
		cont++;
		o->poneValor(e);
	}
	printf( "Ingrese el radio de consulta: ");
	scanf( "%lf", &e );
	o->distanciaAcumulada = e;
	return o;
}
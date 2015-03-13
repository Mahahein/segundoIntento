
#ifndef CONSULTOR_H
#define	CONSULTOR_H
#include "Objeto.h"
#include "Pivote.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>


using namespace std;

class Consultor{
	public:
		Consultor();
		Consultor(int dim);
		Consultor(const Consultor& orig);
		~Consultor();
		FILE* archivo;
		vector<Objeto*> saco;
		vector<Pivote*> clusters;
		vector<Pivote*> pivotes;
		int dimension;
		vector<Objeto*> consultarBusquedaRango(Objeto* ob, double r);
		bool eliminarObjeto(Objeto* ob);
		vector<double> distanciaACentros(Objeto* ob);
		vector<double> distanciaAPivotes(Objeto* ob);
		vector<Objeto*> extraerCluster(int numCluster);
		void eliminaPivote(int id);
    	void elegirPivote();
    	void ajustaPosicionesPivotes(int posElim);
    	void ajustaPosicionesSaco();
    	double distanciaPivoteNuevo(Objeto* ob, Pivote* piv);
    	Objeto* cercanoA(Objeto* centro);
    	void reescribeCluster(Pivote* centro);
	private:

};

#endif	/* CONSULTOR_H */
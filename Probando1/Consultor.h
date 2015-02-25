
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
		Consultor(const Consultor& orig);
		~Consultor();
		FILE* archivo;
		vector<Objeto*> saco;
		vector<Pivote*> clusters;
		vector<Pivote*> pivotes;
		vector<Objeto*> consultar(Objeto* ob, double r);
	private:

};

#endif	/* CONSULTOR_H */
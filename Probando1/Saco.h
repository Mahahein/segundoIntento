/* 
 * File:   Saco.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 03:30 PM
 */

#ifndef SACO_H
#define	SACO_H
#include <vector>
#include <string>
#include "Objeto.h"
#include "Pivote.h"
using namespace std;

class Saco {
public:
    vector<Objeto*> bolsa;
    vector<Pivote*> pivotesProvisorios;
    vector<Pivote*> pivotesEnMemoria;
    Saco();
    Saco(const Saco& orig);
    virtual ~Saco();
    void leer(string archivo);
    void distanciasAPivotes(Objeto* ob);
    void distanciaPivoteNuevo(Objeto* ob, Pivote* piv);
    vector<Objeto*> obtieneCercanos(int id);
    bool estaEnPivotes(int id);
private:

};

#endif	/* SACO_H */


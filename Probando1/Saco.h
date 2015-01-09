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
    Saco();
    Saco(const Saco& orig);
    virtual ~Saco();
    void leer(string archivo);
    void peso(Objeto& ob);
    void distancias(Objeto* ob, vector<Pivote*> & pivots);
    void nuevaDistancia(Objeto* ob, Pivote* piv);
    
private:

};

#endif	/* SACO_H */


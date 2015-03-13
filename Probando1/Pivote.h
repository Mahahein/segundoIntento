/* 
 * File:   Pivote.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 05:42 PM
 */

#ifndef PIVOTE_H
#define	PIVOTE_H
#include "Objeto.h"
#include <set>

class Pivote {
public:
    double radio;
    double radio2;
    Objeto *centro;
    int pos;
    vector<Objeto*>::iterator posMasLejano;
    int numCluster;
    vector<Objeto*> cercanos;
    Pivote();
    Pivote(const Pivote& orig);
    Pivote(Objeto &c);
    Pivote(Objeto &ob, double r);
    bool operator<(const Pivote& piv) const;
    bool operator>(const Pivote& piv) const;
    virtual ~Pivote();
    void actualizaRadio();
    void actualizaRadioCentro();
    void ordenaCercanos(int pos);
    void actualizaMasLejano();
private:

};

#endif	/* PIVOTE_H */


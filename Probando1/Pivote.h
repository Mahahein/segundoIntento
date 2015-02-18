/* 
 * File:   Pivote.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 05:42 PM
 */

#ifndef PIVOTE_H
#define	PIVOTE_H
#include "Objeto.h"
#include <vector>

class Pivote {
public:
    double radio;
    double radio2;
    Objeto *centro;
    int pos;
    vector<Objeto*> cercanos;
    //vector<Objeto*> reemplazos;
    Pivote();
    Pivote(const Pivote& orig);
    Pivote(Objeto &c);
    Pivote(Objeto &ob, double r);
    bool operator<(const Pivote& piv) const;
    bool operator>(const Pivote& piv) const;
    virtual ~Pivote();
    void actualizaRadio();
    void actualizaRadio2();
    void ordenaCercanos(int pos);
    void ordenaReemplazos(int pos, int modo);
private:

};

#endif	/* PIVOTE_H */


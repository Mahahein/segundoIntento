/* 
 * File:   Pivote.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 05:42 PM
 */

#ifndef PIVOTE_H
#define	PIVOTE_H
#include "Objeto.h"

class Pivote {
public:
    double radio;
    Objeto *centro;
    Pivote();
    Pivote(const Pivote& orig);
    Pivote(Objeto &c);
    Pivote(Objeto &ob, double r);
    virtual ~Pivote();
private:

};

#endif	/* PIVOTE_H */


/* 
 * File:   Objeto.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 12:47 PM
 */

#ifndef OBJETO_H
#define	OBJETO_H
#include <vector>
using namespace std;

class Objeto {
public:
    vector<double> valores;
    vector<double> distancias;
    double peso;
    Objeto();
    Objeto(const Objeto& orig);
    Objeto(int pezo);
    virtual ~Objeto();
    poneValor(double val);
private:

};

#endif	/* OBJETO_H */


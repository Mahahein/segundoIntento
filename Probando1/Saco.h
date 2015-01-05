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
using namespace std;

class Saco {
public:
    vector<Objeto> bolsa;
    Saco();
    Saco(const Saco& orig);
    virtual ~Saco();
    leer(string archivo);
private:

};

#endif	/* SACO_H */


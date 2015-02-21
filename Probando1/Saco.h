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
 #include <fstream>
#include "Objeto.h"
#include "Pivote.h"
using namespace std;

class Saco {
public:
    vector<Objeto*> bolsa;
    vector<Pivote*> pivotesProvisorios;
    vector<Pivote*> pivotesEnMemoria;
    ofstream file;
    Saco();
    Saco(const Saco& orig);
    virtual ~Saco();
    void leer(string archivo);
    void distanciasAPivotes(Objeto* ob);
    void distanciaPivoteNuevo(Objeto* ob, Pivote* piv);
    void obtieneCercanos(Pivote* p);
    double obtieneRadio(vector<Objeto*> & obs, int pos);
    void sacaPivoteDelSaco(int id);
    void sacaPivoteDeProvisorios(int id);
    void elegirPivoteProvisorio(int nPivotes);
    void pasarAMemoria(int nCluster, int* nPivotes);
    //void agregarACercanosDePivotes(Objeto* ob);
    //void eliminarObjetoDeCercanos(int idPiv, int idObj);
private:

};

#endif	/* SACO_H */


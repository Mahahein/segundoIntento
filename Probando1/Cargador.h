/* 
 * File:   Saco.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 03:30 PM
 */

#ifndef CARGADOR_H
#define	CARGADOR_H
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include "Objeto.h"
#include "Pivote.h"
using namespace std;

class Cargador {
public:
    vector<Objeto*> bolsa;
    vector<Pivote*> pivotesProvisorios;
    vector<Pivote*> pivotesEnMemoria;
    ofstream file;
    Cargador();
    Cargador(const Cargador& orig);
    virtual ~Cargador();
    void leer(string archivo);
    void distanciasAPivotes(Objeto* ob);
    double distanciaPivoteNuevo(Objeto* ob, Pivote* piv);
    void obtieneCercanos(Pivote* p);
    //double obtieneRadio(set<Objeto*> & obs, int pos);
    void sacaPivoteDelSaco(int id);
    void sacaPivoteDeProvisorios(int id);
    void elegirPivoteProvisorio(int nPivotes);
    void pasarAMemoria(int nCluster, int* nPivotes);
    void ajustaPosicionesPivotes(int posElim);
    void ajustaPosiciones();
    //void agregarACercanosDePivotes(Objeto* ob);
    //void eliminarObjetoDeCercanos(int idPiv, int idObj);
private:

};

#endif	/* CARGADOR_H */


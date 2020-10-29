#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <list>
#include <vector>
#include <set>
#include "Tipos.h"

using namespace std;

class Registro {
public:
    
    Registro();

    const list<NombreCampo> campos();

    Valor& operator[](const NombreCampo& campo);

    const NombreCampo devolverCampo(unsigned int i) const;

    void definir(const NombreCampo& nc, const Valor& v);

    const bool operator == (const Registro& r2) const;

    Valor dameValor(unsigned int i) ;

    const unsigned int size() const;

    void borrarCampoPorPos(unsigned int i);

    Registro concatenar(const Registro &r) const;

    void borrarPorCampo(const NombreCampo campo);

    const bool definido (const NombreCampo&);

    //Pre: el campo se encuenta definido.
    void borrarCampo(const NombreCampo&);

    void cProjReg (const set<NombreCampo> cc);

    const Valor dameValorConst(unsigned int i) const;

    void ReemplazarCampo(const NombreCampo &nc, NombreCampo &aReemplazar, Valor &valor);





private:

    struct nodo {
        NombreCampo nom_campo;
        Valor valor;
    };
    vector<nodo> _registro;
  
};



#endif /*__REGISTRO_H__*/

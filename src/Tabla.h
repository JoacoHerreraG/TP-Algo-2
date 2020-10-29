#ifndef __TABLA_H__
#define __TABLA_H__

#include <fstream>
#include <vector>

#include "Registro.h"
#include "Consulta.h"
#include "string_map.h"
#include "string_map.h"
#include "Tipos.h"



class Tabla {
public:
    

    Tabla();

    Tabla(NombreTabla tabla, vector<NombreCampo> campos, NombreCampo clave);

    void insertar(Registro* r);

    void borrar(Valor &v);

    list<NombreCampo> campos();

    const NombreCampo clave() const;

    list<Registro> registros();

    list<Registro> selectConClaveAux(Valor &v);

    list<Registro> selectSinClaveAux(const NombreCampo &c, Valor &v);

    list<Registro> selectCSClaveAux(Valor &v);

    friend list<Registro> interDosTablasAux(Tabla t1, Tabla t2);

    friend list<Registro> joinConClavesAux(Tabla &t1, Tabla &t2);

    const string_map<Registro*> TrieValoresClave() const;

    const unsigned int cantReg() const;

    const unsigned int numClave() const;

   const string_map<unsigned int> Triecampos() const;

    Tabla& operator=(Tabla &d);



private:

        string                    _nomClave;
        unsigned int              _numClave;
        unsigned int              _cantReg;
        string_map<Registro*>     _TrieValoresClave;
        string_map<unsigned int>  _Triecampos;
        list<Registro>            _Registros;


};


/**
Para usar la lista como conjunto
 */


#endif /*__TABLA_H__*/
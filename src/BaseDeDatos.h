#ifndef __BASEDEDATOS_H__
#define __BASEDEDATOS_H__

#include <fstream>
#include <vector>
#include <list>
#include <tuple>

#include "Tabla.h"
#include "Registro.h"
#include "Consulta.h"
#include "string_map.h"
#include "modulos_basicos/linear_set.h"
#include "modulos_basicos/linear_set_iterators.h"



class BaseDeDatos {
public:
    BaseDeDatos();
    const list<NombreTabla> &tablas();
    void AgregarTabla(NombreTabla &nt, Tabla &t);
    void EliminarTabla(NombreTabla &nt);
    void AgregarRegistro(NombreTabla nt, Registro r);
    void eliminarRegistro(NombreTabla &nt, Registro &r);
    list<Registro> consultar(const Consulta& q);
    Tabla& darTabla(NombreTabla nt);


private:
    string_map<Tabla> _diccionario;
    list<Registro> cFrom(NombreTabla nt);
    list<Registro> cSelect(list<Registro> q, NombreCampo c, Valor v);
    list<Registro> cInter(list<Registro> q1, list<Registro> q2);
    list<Registro> cUnion(list<Registro> q1, list<Registro> q2);
    list<Registro> cMatch (list<Registro> cr, NombreCampo c1, NombreCampo c2);
    list<Registro> cProj (list<Registro> cr, set<NombreCampo> cc);
    list<Registro> cRename (list<Registro> cr, NombreCampo c1, NombreCampo c2);
    list<Registro> cProduct (list<Registro> cr1, list<Registro> cr2);
    list<Registro> selectConClave(Tabla t, Valor v);
    list<Registro> selectSinClave(Tabla t, const NombreCampo c, Valor v);
    list<Registro> selectCSclave(Tabla t,Valor v);
    list<Registro> interDosTablas(NombreTabla nt1, NombreTabla nt2);
    list<Registro> joinConClaves(NombreTabla nt1, NombreTabla nt2);
};

#endif //__BASEDEDATOS_H__
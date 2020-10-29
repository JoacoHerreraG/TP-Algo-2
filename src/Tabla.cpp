#include <fstream>
#include <vector>
#include "Registro.h"
#include "Consulta.h"
#include "modulos_basicos/linear_set.h"
#include <list>
#include "Tabla.h"

//Constructor vacio que utiliza el Trie
Tabla::Tabla() {
    _nomClave = "random(string)";
    _cantReg = 0;
    _TrieValoresClave = string_map<Registro *>();
    _Triecampos = string_map<unsigned int>();
    _Registros = list<Registro>();
    vector<NombreCampo> campos;
    campos.push_back("1");

    NombreCampo clave = "hola";


    for (unsigned int i = 0; i < campos.size(); i++) {
        if (campos[i] == clave) {
            _numClave = i;
        } else {
            _Triecampos[campos[i]] = i;
        }
    }
}

Tabla::Tabla(NombreTabla tabla,
             vector<NombreCampo> campos,
             NombreCampo clave) {
    _nomClave = clave;
    _cantReg = 0;
    _TrieValoresClave = string_map<Registro *>();
    _Triecampos = string_map<unsigned int>();
    _Registros = list<Registro>();

    for (unsigned int i = 0; i < campos.size(); i++) {
        if (campos[i] == clave) {
            _numClave = i;
        } else {
            _Triecampos[campos[i]] = i;
        }
    }
}


void Tabla::insertar(Registro* r) {
    _cantReg++;
    _Registros.push_back(*r);
    Valor valor = (*r)[_nomClave];
    const pair<string, Registro*> tupla = make_pair(valor, &_Registros.back());
    _TrieValoresClave.insert(tupla);

}

void Tabla::borrar(Valor &v) {
//acordarse de agregar que chequee si está definido v
    if (_TrieValoresClave.count(v) == 1) {


        Registro registroBorrar = *(_TrieValoresClave.at(v));
        _TrieValoresClave.erase(v);
        _cantReg--;
        for (auto it = _Registros.begin(); it != _Registros.end(); it++) {
            if (registroBorrar == *it) {
                _Registros.erase(it);
                break;
        }
    }
    }
}


list<NombreCampo> Tabla::campos() {
    list<NombreCampo> res;
    res = _Triecampos.claves();
    res.push_back(_nomClave);
    return res;
}

const NombreCampo Tabla::clave() const {
    return _nomClave;
}

list<Registro> Tabla::registros() {
    list<Registro> registros = _Registros;
    return registros;
}

list<Registro> Tabla::selectConClaveAux(Valor &v) {
    list<Registro> res = list<Registro>();
    if (_TrieValoresClave.count(v) == 1) {
        res.push_back(*(_TrieValoresClave.at(v)));
    }
    return res;
}

list<Registro> Tabla::selectSinClaveAux(const NombreCampo &c, Valor &v) {
    list<Registro> res = list<Registro>();
    if (_Triecampos.count(c) == 1) {
        for (auto it = _Registros.begin(); it != _Registros.end(); it++) {
            if (((*it)[c]) == v) {
                res.push_back(*it);
            }
        }
    }
    return res;
}

list<Registro> Tabla::selectCSClaveAux(Valor &v) {

    Registro reg = *((_TrieValoresClave).at(v));
    list<Registro> res = list<Registro>();
    if (reg.dameValor(_numClave) == v) {
        res.push_back(reg);
    }

    return res;
}

list<Registro> interDosTablasAux(Tabla t1, Tabla t2) {
    list<Registro> res = list<Registro>();
    Tabla *tablaEspejo1;
    Tabla *tablaEspejo2;
    if (t1._cantReg >= t2._cantReg) {
        tablaEspejo1 = &t1;
        tablaEspejo2 = &t2;
    } else {
        tablaEspejo1 = &t2;
        tablaEspejo2 = &t1;
    }
    for (auto it = (tablaEspejo1->_Registros).begin(); it != (tablaEspejo1->_Registros).end(); it++) {

        Valor valor = (it)->dameValor(tablaEspejo1->_numClave);

        if (tablaEspejo2->_TrieValoresClave.count(valor) == 1) {
            Registro reg = *(tablaEspejo2->_TrieValoresClave.at(valor));
            if (reg == *it) {
                res.push_back(reg);
            }
        }
    }
    return res;
}


list<Registro> joinConClavesAux(Tabla &t1, Tabla &t2) {
    list<Registro> res = list<Registro>();
    list<Valor> claves = list<Valor>();
    Tabla *tablaEspejo2 = NULL;
    Tabla *tablaEspejo1 = NULL;
    if (t1._cantReg <= t2._cantReg) {
        claves = t1._TrieValoresClave.claves();
        tablaEspejo2 = &t2;
        tablaEspejo1 = &t1;
    } else {
        claves = t2._TrieValoresClave.claves();
        tablaEspejo1 = &t2;
        tablaEspejo2 = &t1;

    }
    res = list<Registro>();
    for (auto it = claves.begin(); it != claves.end(); it++) {
        const string claveI = *it;
        if (tablaEspejo2->_TrieValoresClave.count(claveI)) {
            Registro nuevo = t1._TrieValoresClave.at(claveI)->concatenar(*(t2._TrieValoresClave.at(claveI)));
            res.push_back(nuevo);                            /**No se agregan repetidos. **/
        }
    }
    return res;
}

const unsigned int Tabla::numClave() const {
    return _numClave;
}

const unsigned int Tabla::cantReg() const {
    return _cantReg;
}

const string_map<Registro *> Tabla::TrieValoresClave() const {
    return _TrieValoresClave;
}

const string_map<unsigned int> Tabla::Triecampos() const {
    return _Triecampos;
}


Tabla &Tabla::operator=(Tabla &t) {
    _nomClave = t.clave();
    _numClave = t.numClave();
    _cantReg = t.cantReg();
    _TrieValoresClave = t.TrieValoresClave();
    _Triecampos = t.Triecampos();
    list<Registro> r = t.registros();
    for (auto it = r.begin(); it != r.end(); it++) {
        _Registros.push_back(*it);
    }


}
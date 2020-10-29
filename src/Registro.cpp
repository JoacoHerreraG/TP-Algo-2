#include <set>
#include "Registro.h"


Registro::Registro() : _registro({}){
}


const list<NombreCampo> Registro::campos() {
    //Creo una lista vacia
    list<NombreCampo> conjunto_campos = list<NombreCampo>();
    //Recorro cad posicion del registro para acceder a sus nombres de campos
    for (unsigned int i = 0; i < _registro.size(); i++) {
        //Fast_insert en linear list
        conjunto_campos.push_back(_registro[i].nom_campo);
    }
    return conjunto_campos;
}


const unsigned int Registro::size() const {
    return _registro.size();
}


Valor &Registro::operator[](const NombreCampo &campo) {
    //Recorre el registro hasta que encuentra el campo correspondiente, devuelve un puntero a la posicion del registro
    //Preguntar que hacer bien aca
    unsigned int i;
    for ( i = 0; i < _registro.size(); i++) {
        if (campo == _registro[i].nom_campo) {
            return (_registro[i].valor);
        }
    }
    return (_registro[i].valor);
}


const NombreCampo Registro::devolverCampo(unsigned int i) const {
    //Recorre el registro hasta que encuentra el campo correspondiente, devuelve un puntero a la posicion del registro
    //Preguntar que hacer bien aca
    return ((_registro[i]).nom_campo);
}


void Registro::definir(const NombreCampo &nc, const Valor &v) {
    // accedemos a esta funcion con registro.definir(nc, v)
    unsigned int i = 0;
    while (i < _registro.size() && _registro[i].nom_campo != nc) {
        i++;
    }
    if (i == _registro.size()) {
        nodo Nodo;
        Nodo.nom_campo = nc;
        Nodo.valor = v;
        _registro.push_back(Nodo);
    } else {
        _registro[i].valor = v;
    }
}

Valor Registro::dameValor(unsigned int i) {

    return (_registro[i].valor);
}

const Valor Registro::dameValorConst(unsigned int i) const {
    return (_registro[i].valor);
}


const bool Registro::operator==(const Registro &r2) const {
    if ((*this).size() != r2.size()) {
        return false;
    }
    for (unsigned int i = 0; i < this->size(); i++) {
        if (this->devolverCampo(i) != r2.devolverCampo(i) || this->dameValorConst(i) != r2.dameValorConst(i)) {
            return false;
        }
    }
    return true;
}


void Registro::borrarCampoPorPos(unsigned int i) {
    _registro.erase(_registro.begin() + i);
}


//Pre: el campo se encuenta definido.
void Registro::borrarCampo(const NombreCampo &nc){
    unsigned int i;
    for (i = 0; i <_registro.size() && _registro[i].nom_campo!=nc ; ++i) {


    }
    if (i != _registro.size()){
        _registro.erase(_registro.begin() + i);
    }
}


void Registro::borrarPorCampo(const NombreCampo campo){
    unsigned int i = 0;
    while (i < _registro.size()) {
        if (_registro[i].nom_campo == campo) {
            _registro.erase(_registro.begin()+i);
        }
        i++;
    }
}

void Registro::ReemplazarCampo(const NombreCampo &nc, NombreCampo &nuevoNombre, Valor &valor){
    unsigned int i;
    for (i = 0; i <_registro.size() && _registro[i].nom_campo!=nc ; ++i) {


    }
    if (i != _registro.size()){
    nodo nodoCambiado;
    nodoCambiado.nom_campo = nuevoNombre;
    nodoCambiado.valor = valor;
        _registro[i] = nodoCambiado;
    }
}

Registro Registro::concatenar(const Registro &r) const{
    Registro res = *this;
    unsigned int i = 0;
    while (i < r.size()) {
        res.definir(r.devolverCampo(i), r.dameValorConst(i));
        i++;
    }
    return res;
}

const bool Registro::definido(const NombreCampo &nc){
    unsigned int i=0;
    for (i = 0; i <_registro.size() && _registro[i].nom_campo!= nc; ++i) {

    }
    return (i !=_registro.size());
}


void Registro::cProjReg(const set<NombreCampo> cc){
    unsigned int i = 0;
    unsigned int proyectados = 0;

    while (i < this->size() && proyectados < this->size()) {
        auto it = cc.begin();

        while (it != cc.end() && (this->devolverCampo(i) != (*it))) {
            it++;
            if (it == cc.end()) {
                this->borrarCampoPorPos(i);
            } else {
                proyectados++;
            }
        }
        i++;
    }
}

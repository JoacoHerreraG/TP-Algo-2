
#include <cassert>
#include <cstdlib>
#include <sstream>


#include "Driver.h"

#include "string_map.h"

void Driver::crearTabla(NombreTabla tabla, vector<NombreCampo> campos,
                        NombreCampo clave) {
    Tabla tablita = Tabla(tabla, campos, clave);
    _base.AgregarTabla(tabla,tablita);

}


void Driver::insertarRegistro(NombreTabla t, Registro r) {

    (_base.darTabla(t)).insertar(&r);
   // _base.AgregarTabla(t,tabla);
   // Tabla prueba = this->_base.darTabla(t);
   // int i = false;

}

Tabla& Driver::darTabla(NombreTabla nt){
   return (_base.darTabla(nt));
}

void Driver::eliminarRegistro(NombreTabla nt, Registro &r){
    _base.eliminarRegistro(nt, r);
}

unsigned int Driver::getCantTabla(){
    return _base.tablas().size();
}

void Driver::borrarTabla(NombreTabla nt){
    _base.EliminarTabla(nt);
}

NombreCampo Driver::clave(Tabla t){
    return t.clave();
}

Respuesta Driver::consultar(const Consulta& q){
    list<Registro> l = _base.consultar(q) ;
    Respuesta respuesta = convertir(l);
    return respuesta;
}
//Esto es por culpa de March!!!
Respuesta Driver::convertir(list<Registro> l){
    Respuesta respuesta;
    for (auto it= l.begin();it != l.end(); it++){
        respuesta.push_back(*it);
    }
    return respuesta;
}


void Driver::leerDataset(string dataset) {
    ifstream is("datos/" + dataset + ".txt");




    vector<string> campos;
    _leerLinea(is, campos);
    crearTabla(dataset, campos, campos[0]);


    int linea = 1;
    while (!is.eof()) {
        vector<string> valores;
        if (!_leerLinea(is, valores)) {
            break;
        }
        if (campos.size() != valores.size()) {
            cerr << "Error leyendo dataset: " << dataset << endl;
            cerr << "El registro en la linea " << linea
                 << " tiene " << valores.size() << " campos"
                 << " pero la tabla declara " << campos.size() << " campos."
                 << endl;
            exit(1);
        }
        Registro r;

        for (size_t i = 0; i < campos.size(); i++) {            //Creo el registro.
            r.definir(campos[i], valores[i]);
        }

        insertarRegistro(dataset, r);


    }
    }


bool Driver::_leerLinea(ifstream& is, vector<string>& valores) const {
    string linea;
    if (!getline(is, linea)) {
        return false;
    }

    istringstream isl(linea);
    string valor;
    while (getline(isl, valor, ',')) {
        valores.push_back(valor);
    }
    return true;
}


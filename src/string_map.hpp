#include "string_map.h"


template<typename T>
string_map<T>::Nodo::Nodo() {
    definicion = nullptr;
    vector<Nodo *> v;
    for (int i = 0; i < 256; i++) {
        v.push_back(nullptr);
    }
    siguientes = v;
}

template<typename T>
string_map<T>::Nodo::Nodo(string_map<T>::Nodo *v) {
    definicion = nullptr;
    vector<Nodo *> n;
    for (int i = 0; i < 256; ++i) {
        n.push_back(nullptr);
    }
    siguientes = n;

    if (v != nullptr) {
        if (v->definicion != nullptr) {
            definicion = new T(*v->definicion);
        }

        for (unsigned int j = 0; j < v->siguientes.size(); ++j) {
            if (v->siguientes[j] != nullptr) {
                siguientes[j] = new Nodo(v->siguientes[j]);
            }
        }
    }
}

template<typename T>
string_map<T>::string_map() {
    _size = 0;
    _raiz = new Nodo();
}

template<typename T>
string_map<T>::string_map(const string_map<T> &aCopiar) : string_map() {
    *this = aCopiar;
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &d) {
    eraseAux(_raiz);
    _raiz = new Nodo(d._raiz);
    _size = d.size();
    _claves = d._claves;
    return *this;
}

template<typename T>
string_map<T>::~string_map() {
    if (_raiz != nullptr) {

    }
    eraseAux(_raiz);
    _raiz = nullptr;
    _size = 0;
    _claves.clear();
}


template<typename T>
T &string_map<T>::operator[](const string &clave) {
    Nodo *nodo = _raiz;
    int letra;
    for (unsigned int i = 0; i < clave.size(); i++) {
        letra = clave[i];
        if (nodo->siguientes[letra] == nullptr) {
            nodo->siguientes[letra] = new Nodo();
        }
        nodo = nodo->siguientes[letra];
    }
    if (nodo->definicion == nullptr) {
        nodo->definicion = new T();
        _size++;
        _claves.push_back(clave);
    }
    return (*nodo->definicion);
}

template<typename T>
void string_map<T>::insert(const pair<string, T>& tupla) {
    Nodo *nodo = _raiz;
    int letra;
    const string clave = get<0>(tupla);
    if (_raiz == nullptr) {
        Nodo* nodoRaiz = new Nodo();
        _raiz = nodoRaiz;
        nodo = _raiz;
    }

    for (unsigned int i = 0; i < clave.size(); i++) {
        letra = clave[i];
        if (nodo->siguientes[letra] == nullptr) {
            nodo->siguientes[letra] = new Nodo();
        }
        nodo = nodo->siguientes[letra];
    }
    if (nodo->definicion == nullptr) {
        nodo->definicion = new T(get<1>(tupla));
        _size++;
        _claves.push_back(clave);
    } else {
        nodo->definicion = new T(get<1>(tupla));
    }

}



template<typename T>
int string_map<T>::count(const string &clave) const {
    Nodo *nodo = _raiz;
    int count = 2;
    int i = 0;
    int letra;
    while (i < clave.size() && nodo != nullptr) {
        letra = int(clave[i]);
        if (nodo->siguientes[letra] == nullptr) {
            count = 0;
        }
        i++;
        nodo = nodo->siguientes[letra];
    }

    if (nodo != nullptr && nodo->definicion != nullptr) {
        count = 1;
    }

    return count;
}

template<typename T>
const T &string_map<T>::at(const string &clave) const {
    Nodo *nodo = _raiz;
    int letra;
    for (int i = 0; i < clave.size(); i++) {
        letra = clave[i];
        nodo = nodo->siguientes[letra];
    }

    return (*nodo->definicion);
}

template<typename T>
T &string_map<T>::at(const string &clave) {
    Nodo *nodo = _raiz;
    int letra;
    for (unsigned int i = 0; i < clave.size(); i++) {
        letra = clave[i];
        nodo = nodo->siguientes[letra];
    }

    return (*nodo->definicion);
}

template<typename T>
    void string_map<T>::erase(const string &clave) {
        if (_size == 1) {
            eraseAux(_raiz);

            _raiz = nullptr;
            _size = 0;
            _claves.clear();
        } else if (clave == "") {
            delete _raiz->definicion;
            _raiz->definicion = nullptr;
            _claves.remove("");
        } else {
            Nodo *buscador = _raiz;              //Notar que _raiz!=NULL siempre por _size>0.
            Nodo *hastaAca = _raiz;             //Será el último elemento del camino recorrido por buscador que no queremos borrar; último
            //en el sentido de _head a buscador.
            Nodo *primeroABorrar = _raiz;       //Primer elemento del camino recorrido que se quiere borrar.
            int i = 0;
            int marcador = 0;                   //Para saber cuál sufijo de clave debo mirar para avanzar primeroABorrar.
            while (i < clave.size()) {
                if (hayCaracteresAdemasDe((*buscador), clave[i]) || buscador->definicion != nullptr) {  //Si buscador es un nodo que queremos mantener.
                    hastaAca = buscador;
                    buscador = buscador->siguientes[int(clave[i])];
                    primeroABorrar = buscador;
                    marcador = i;
                    i++;
                } else {
                    buscador = buscador->siguientes[int(clave[i++])];
                }
            }//Al finalizar el ciclo, primeroABorrar!=_raiz ya que _size>1. Además, marcador tiene la pos. del primer caracter de clave cuyo nodo
            // queremos borrar del trie.
            if (hayCaracteres(*buscador)) {           //Caso fácil, solo borramos el significado.
                delete buscador->definicion;
                _claves.remove(clave);
                buscador->definicion = nullptr;
            } else if (primeroABorrar == buscador) {     // Sii hastaAca->siguientes[int(clave[clave.size()-1])] == buscador.
                delete buscador->definicion;
                buscador->definicion = nullptr;
                delete buscador;
                hastaAca->siguientes[int(clave[clave.size() - 1])] = nullptr;
                _claves.remove(clave);


            } else {                                            // buscador!=primeroABorrar entonces iteramos hasta que sean nodos adyacentes.
                Nodo *borrador = primeroABorrar;
                hastaAca->siguientes[int(clave[marcador])] = nullptr;       //Desligamos nuestro trie de la rama que queremos podar.
                marcador++;
                while (borrador !=buscador) {              // Se borra primeroABorrar inclusive; al salir, queda por borrar buscador, que es borrador.
                    Nodo *siguiente = borrador->siguientes[int(clave[marcador])];
                    if (borrador->definicion != nullptr){
                        delete borrador->definicion;
                    }
                    delete borrador;
                    _claves.remove(clave);


                    borrador = siguiente;
                    marcador++;
                }
                delete buscador->definicion;
                delete buscador;
            }
        }
        _size--;
    }



template<typename T>
bool string_map<T>::hayCaracteres(const string_map<T>::Nodo &nodo) {
    int i = 0;
    while(i<256 && nodo.siguientes[i]== nullptr){
        i++;
    }
    return (i!=256);
}

template<typename T>
bool string_map<T>::hayCaracteresAdemasDe(const string_map<T>::Nodo &nodo, const char &l) {
    int i = 0;
    int cuantosHayDistintos = 0;
    const int letra = int(l);
    while(i<256){
        if( i != letra ){
            if (nodo.siguientes[i] != nullptr){
                cuantosHayDistintos++;
            }
        }
        i++;
    }
    return cuantosHayDistintos > 0;
}

template<typename T>
int string_map<T>::size() const {
    return _size;
}

template<typename T>
const list<string>& string_map<T>::claves() const{
    return _claves;
}

template<typename T>
bool string_map<T>::empty() const {
    if (_raiz->noHijo() && _raiz->definicion == nullptr) {
        return true;
    }
    return false;
}

template<typename T>
int string_map<T>::Nodo::hijos() {
    int hijos = 0;
    for (unsigned int i = 0; i < siguientes.size(); ++i) {
        if (siguientes[i] != nullptr) {
            hijos++;
        }
    }
    return hijos;


}

template<typename T>
bool string_map<T>::Nodo::unHijo() {
    return hijos() == 1;
}

template<typename T>
bool string_map<T>::Nodo::noHijo() {
    return hijos() == 0;
}


template<typename T>
void string_map<T>::eraseAux(string_map<T>::Nodo *n) {
    if (n != nullptr) {
        for (int i = 0; i < 255; ++i) {
            eraseAux(n->siguientes[i]);
        }

        if (n->definicion != nullptr) {
            delete n->definicion;
        }

        delete n;
    }
}
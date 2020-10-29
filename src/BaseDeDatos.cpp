#include "BaseDeDatos.h"
#include <list>

BaseDeDatos::BaseDeDatos() {
    _diccionario = string_map<Tabla>();
}

const list<NombreTabla> &BaseDeDatos::tablas() {
    return _diccionario.claves();
}

void BaseDeDatos::AgregarTabla(NombreTabla &nt, Tabla &t) {
    _diccionario.insert(make_pair(nt, t));

}

void BaseDeDatos::EliminarTabla(NombreTabla &nt) {
    _diccionario.erase(nt);

}

void BaseDeDatos::AgregarRegistro(NombreTabla nt, Registro r) {

    (_diccionario.at(nt)).insertar(&r);
}

void BaseDeDatos::eliminarRegistro(NombreTabla &nt, Registro &registro) {
    Tabla tabla = _diccionario.at(nt);
    NombreCampo campoClave = tabla.clave();
    Valor valorClave = registro[campoClave];
    (_diccionario.at(nt)).borrar(valorClave);

}

list<Registro> BaseDeDatos::consultar(const Consulta &q) {
    Tabla tabla1;
    Tabla tabla2;
    list<Registro> res;

    if (q.tipo_consulta() == FROM) {
        res = cFrom(q.nombre_tabla());
    }
    if (q.tipo_consulta() == SELECT) {
        if ((q.subconsulta1()).tipo_consulta() == FROM) {
            Tabla tabla = _diccionario.at(q.subconsulta1().nombre_tabla());
            if (tabla.clave() == q.campo1()) {
                res = selectConClave(tabla, q.valor());
            } else {
                res = selectSinClave(tabla, q.campo1(), q.valor());
            }
        } else {
            if ((q.subconsulta1()).tipo_consulta() == SELECT &&
                ((q.subconsulta1()).subconsulta1()).tipo_consulta() == FROM
                && q.campo1() == this->darTabla(q.subconsulta1().subconsulta1().nombre_tabla()).clave()
                && q.subconsulta1().campo1() != q.campo1()) {
                Tabla tabla = _diccionario.at(q.subconsulta1().subconsulta1().nombre_tabla());
                res = selectCSclave(tabla, q.valor());
            } else {
                if ((q.subconsulta1()).tipo_consulta() == FROM) {
                    Tabla tabla = _diccionario.at((q.subconsulta1()).nombre_tabla());
                    if (tabla.clave() != q.campo1()) {
                        res = selectSinClave(tabla, q.campo1(), q.valor());
                    }
                } else {
                    res = cSelect(consultar(q.subconsulta1()), q.campo1(), q.valor());
                }

            }
        }
    }

    if (q.tipo_consulta() == MATCH) {


        if (q.subconsulta1().tipo_consulta() == PRODUCT &&
            q.subconsulta1().subconsulta1().tipo_consulta() == FROM &&
            q.subconsulta1().subconsulta2().tipo_consulta() == FROM) {

            NombreTabla nomTabla1 = q.subconsulta1().subconsulta1().nombre_tabla();
            NombreTabla nomTabla2 = q.subconsulta1().subconsulta2().nombre_tabla();
            tabla1 = this->darTabla(nomTabla1);
            tabla2 = this->darTabla(nomTabla2);

            if (q.campo1() == tabla1.clave() &&
                q.campo2() == tabla2.clave() &&
                q.subconsulta1().subconsulta1().nombre_tabla() != q.subconsulta1().subconsulta2().nombre_tabla()) {

                res = joinConClaves(nomTabla1, nomTabla2);
            }
        } else {
            res = cMatch(consultar(q.subconsulta1()), q.campo1(), q.campo2());
        }

    }

    if (q.tipo_consulta() == PROJ) {
        res = cProj(consultar(q.subconsulta1()), q.conj_campos());
    }
    if (q.tipo_consulta() == RENAME) {
        res = cRename(consultar(q.subconsulta1()), q.campo1(), q.campo2());
    }
    if (q.tipo_consulta() == UNION) {
        res = cUnion(consultar(q.subconsulta1()), consultar(q.subconsulta2()));
    }
    if (q.tipo_consulta() == PRODUCT) {
        res = cProduct(consultar(q.subconsulta1()), consultar(q.subconsulta2()));
    }

    if (q.tipo_consulta() == INTER) {
        if (q.subconsulta1().tipo_consulta() == FROM && q.subconsulta2().tipo_consulta() == FROM){
         if ((q.subconsulta1()).nombre_tabla() != (q.subconsulta2()).nombre_tabla()) {
            NombreTabla nomTabla1 = q.subconsulta1().nombre_tabla();
            NombreTabla nomTabla2 = q.subconsulta2().nombre_tabla();
            tabla1 = this->darTabla(nomTabla1);
            tabla2 = this->darTabla(nomTabla2);
                if (tabla1.campos() == tabla2.campos() && tabla1.clave() == tabla2.clave()) {
                    res = interDosTablas(q.subconsulta1().nombre_tabla(), q.subconsulta2().nombre_tabla());
                }
        }  } else {
            res = cInter(consultar(q.subconsulta1()), consultar(q.subconsulta2()));
        }
    }


    return res;
}

list<Registro> BaseDeDatos::cSelect(list<Registro> q, NombreCampo c, Valor v) {
    list<Registro> res = list<Registro>();
    for (auto it = q.begin(); it != q.end(); it++) {
        if ((*it)[c] == v) {
            //Fast_insert en linear set
            res.push_back(*it);
        }
    }
    return res;
}

list<Registro> BaseDeDatos::cInter(list<Registro> q1, list<Registro> q2) {
    list<Registro> res = list<Registro>();
    if (q1.empty() || q2.empty()) {
        return res;
    } else {

        for (auto it1 = q1.begin(); it1 != q1.end(); it1++) {
            for (auto it2 = q2.begin(); it2 != q2.end(); it2++) {
                if (*it1 == *it2) {
                    res.push_back(*it1);
                }
            }
        }
    }
    return res;
}

list<Registro> BaseDeDatos::cUnion(list<Registro> q1, list<Registro> q2) {
    list<Registro> res = list<Registro>();
    if (q1.empty() || q2.empty()) {
        return res;
    } else if (q1.empty()) {
        res = q2;
    } else if (q2.empty()) {
        res = q1;
    } else {
        list<Registro>::iterator it1;
        for (it1 = q1.begin(); it1 != q1.end(); it1++) {
            res.push_back(*it1);
        }
        list<Registro>::iterator it2;
        for (it2 = q2.begin(); it2 != q2.end(); it2++) {
            res.push_back(*it2);
        }
    }
    return res;
}

Tabla &BaseDeDatos::darTabla(NombreTabla nt) {

     return _diccionario.at(nt);
}

list<Registro> BaseDeDatos::cFrom(NombreTabla nt) {
    Tabla tabla = darTabla(nt);
    return tabla.registros();
}

list<Registro> BaseDeDatos::cMatch(list<Registro> cr, NombreCampo c1, NombreCampo c2) {
    list<Registro> res = list<Registro>();
    for (auto it = cr.begin(); it != cr.end(); it++) {
        Registro reg = *it;
        if (reg[c1] == reg[c2]) {
            res.push_back(reg);
        }
    }
    return res;
}

list<Registro> BaseDeDatos::cProj(list<Registro> cr, set<NombreCampo> cc) {
    list<Registro> res = list<Registro>();
    for (auto it = cr.begin(); it != cr.end(); it++) {
        Registro temp = *it;
        temp.cProjReg(cc);
        res.push_back(temp);
    }
    return res;
}

list<Registro> BaseDeDatos::cRename(list<Registro> cr, NombreCampo c1, NombreCampo c2) {
    list<Registro> res = list<Registro>();
    for (auto it = cr.begin(); it != cr.end(); it++) {
        if (it->definido(c1)) {
            Valor valor = (*it)[c1];
            it->ReemplazarCampo(c1,c2, valor);
            res.push_back(*it);
        }
    }
    return res;
}


list<Registro> BaseDeDatos::cProduct(list<Registro> cr1, list<Registro> cr2) {
    list<Registro> res = list<Registro>();
    for (auto it1 = cr1.begin(); it1 != cr1.end(); it1++) {
        for (auto it2 = cr2.begin(); it2 != cr2.end(); it2++) {
            Registro nuevo = it1->concatenar(*it2);
            res.push_back(nuevo);
        }
    }
    return res;
}

list<Registro> BaseDeDatos::selectConClave(Tabla t, Valor v) {
    return t.selectConClaveAux(v);
}

list<Registro> BaseDeDatos::selectSinClave(Tabla t, const NombreCampo c, Valor v) {
    return t.selectSinClaveAux(c, v);
}

list<Registro> BaseDeDatos::selectCSclave(Tabla t, Valor v) {
    return t.selectCSClaveAux(v);
}


list<Registro> BaseDeDatos::interDosTablas(NombreTabla nt1, NombreTabla nt2) {
    Tabla t1 = this->darTabla(nt1);
    Tabla t2 = this->darTabla(nt2);
    return interDosTablasAux(t1, t2);
}

list<Registro> BaseDeDatos::joinConClaves(NombreTabla nt1, NombreTabla nt2) {
    Tabla t1 = this->darTabla(nt1);
    Tabla t2 = this->darTabla(nt2);
    return joinConClavesAux(t1, t2);
}



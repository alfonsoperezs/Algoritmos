/*
 * -- PRÁCTICA 3 --
 * Alfonso Pérez Sánchez (a.perezs@udc.es)
 * Brais Calvo Pensado (brais.calvo.pensado@udc.es)
 * Lucía Lagares Álvarez (lucia.lagares1@udc.es)
 */
#include "monticulo.h"

// funciones auxiliares
void hundir(pmonticulo m, int i){
    /*
     * Operación para mantener la propiedad de los montículos
     * tras una inserción o borrado de un elemento.
     */
    int j, hijoIzquierdo, hijoDerecho, aux;
    do {
        hijoIzquierdo = 2 * i + 1;
        hijoDerecho = 2 * i + 2;
        j = i;
        if(hijoDerecho <= m->ultimo && m->vector[hijoDerecho] < m->vector[i]){
            i = hijoDerecho;
        }
        if(hijoIzquierdo <= m->ultimo && m->vector[hijoIzquierdo] < m->vector[i]){
            i = hijoIzquierdo;
        }
        aux = m->vector[i];
        m->vector[i] = m->vector[j];
        m->vector[j] = aux;
    } while (j != i);
}
bool esMonticuloVacio(pmonticulo m){
    /*
     * Comprueba si el montículo está vacío.
     */
    return (m->ultimo == -1);
}

// funciones principales de los monticulos
void inicializarMonticulo(pmonticulo *m){
    /*
     * Inicializa un montículo vacío.
     */
    *m = malloc(sizeof(struct monticulo));
    (*m)->ultimo = -1;
}

void crearMonticulo(int v[], int n, pmonticulo m){
    /*
     * Inicializa un montículo de mínimos.
     */
    int i;

    m->ultimo = n - 1; // tamaño del monticulo
    for(i = 0; i < n; ++i){ // copiamos el vector en el monticulo
        m->vector[i] = v[i];
    }
    for (i = m->ultimo / 2; i >= 0; i--){
        hundir(m,i);
    }
}
int quitarMenor(pmonticulo m){
    /*
     * Elimina del montículo el elemento más pequeño.
     */
    int x;

    if(!esMonticuloVacio(m)){
        x = m->vector[0]; // el elemento mas pequeño está en la raiz
        m->vector[0] = m->vector[m->ultimo];
        m->ultimo = m->ultimo - 1;
        if(m->ultimo >= 0){
            hundir(m,0);
        }
        return x;
    }
}
void borrarMonticulo(pmonticulo *m) {
    /*
     * Libera la memoria asignada al montículo.
     */
    if (*m != NULL) {
        free(*m);
        *m = NULL;
    }
}
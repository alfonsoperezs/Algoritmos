/*
 * -- PRÁCTICA 3 --
 * Alfonso Pérez Sánchez (a.perezs@udc.es)
 * Brais Calvo Pensado (brais.calvo.pensado@udc.es)
 * Lucía Lagares Álvarez (lucia.lagares1@udc.es)
 */
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#define TAM 256000

typedef struct monticulo{
    int ultimo;
    int vector[TAM];
} monticulo;

typedef struct monticulo * pmonticulo;

void inicializarMonticulo(pmonticulo *m);
void crearMonticulo(int v [], int n, pmonticulo m);
int quitarMenor(pmonticulo m);
void mostrarMonticulo(pmonticulo m);
void borrarMonticulo(pmonticulo *m);
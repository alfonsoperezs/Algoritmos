#include <stdlib.h>
#include <stdio.h>

#define TAM_MAX 1000

typedef int ** matriz;

matriz crearMatriz(int n);
void iniMatriz(matriz m, int n);
void liberarMatriz(matriz m, int n);
void mostrarMatriz(matriz m, int n);
void asignarMatriz(int n,int v[n][n], matriz m);

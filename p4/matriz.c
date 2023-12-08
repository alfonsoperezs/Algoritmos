#include "matriz.h"

matriz crearMatriz(int n) {
    /*
     * Inicializa la matriz
     */
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

void iniMatriz(matriz m, int n) {
    /*
     * Asigna numeros aleatorios a la matriz
     */
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % TAM_MAX + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}
void liberarMatriz(matriz m, int n) {
    /*
     * Borra la matriz
     */
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

void mostrarMatriz(matriz m, int n){
    /*
     * Muestra la matriz m por pantalla
     */
    int i,j;
    for (i = 0; i < n; ++i) {
        printf("[ ");
        for (j = 0; j < n; ++j) {
            printf("%3d",m[i][j]);
        }
        printf("   ]\n");
    }
}

void asignarMatriz(int n,int v[n][n], matriz m){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m[i][j] = v[i][j];
        }
    }
}
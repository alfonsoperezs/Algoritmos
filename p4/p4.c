/*
 * -- PRÁCTICA 4 --
 * Alfonso Pérez Sánchez (a.perezs@udc.es)
 * Brais Calvo Pensado (brais.calvo.pensado@udc.es)
 * Lucía Lagares Álvarez (lucia.lagares1@udc.es)
 */

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "matriz.h"

#define K 100 //numero de repeticiones del algoritmo en caso de que no supere umbral de confianza

//ALGORITMOS DE ORDENACIÓN

void inicializar_semilla();
double microsegundos();

void showTime();
double calcularTiempo(int n, bool *bucle);
void mostrarTablas();

void test();
void dijkstra(matriz grafo, matriz distancias, int tam);

void dijkstra(matriz grafo, matriz distancias, int tam){
    /*
     * Algoritmo para calcular el camino mínimo de cada vértice a los
     * restantes en grafos ponderados siguiendo el algoritmo de Dijkstra.
     */
    int n, i, min, v = 0, j, k;
    int *noVisitados = malloc(tam * sizeof (int));

    for(n = 0; n < tam; ++n){
        for(i = 0; i < tam; ++i){
            noVisitados[i] = 1;
            distancias[n][i] = grafo[n][i];
        }
        noVisitados[n] = 0;
        for (i = 0; i < tam - 1; ++i){
            min = INT_MAX; // min inicialmente debe ser muy grande (hemos asignado el mayor int posible)
            for (j = 0; j < tam; ++j) {
                if (noVisitados[j] && distancias[n][j] < min) {
                    min = distancias[n][j];
                    v = j;
                }
            }
            noVisitados[v] = 0;
            for (k = 0; k < tam; k++) {
                if (noVisitados[k] && distancias[n][k] > distancias[n][v] + grafo[v][k]) {
                    distancias[n][k] = distancias[n][v] + grafo[v][k];
                }
            }
        }
    }
    free(noVisitados);
}


int main(){
    inicializar_semilla();
    test();

    mostrarTablas();
    mostrarTablas();
    return 0;
}

void test(){
    matriz m1 = crearMatriz(5);
    matriz m2 = crearMatriz(4);
    matriz m3 = crearMatriz(5);
    matriz m4 = crearMatriz(4);
    int inm1[5][5] = {{0,1,8,4,7},
                      {1,0,2,6,5},
                      {8,2,0,9,5},
                      {4,6,9,0,3},
                      {7,5,5,3,0}};
    int inm2[4][4] = {{0,1,4,7},
                      {1,0,2,8},
                      {4,2,0,3},
                      {7,8,3,0}};

    asignarMatriz(5,inm1,m1);
    asignarMatriz(4,inm2,m2);
    //figura 2
    puts("Matriz de adyacencia");
    mostrarMatriz(m1,5);
    puts("Distancias minimas");
    dijkstra(m1,m3,5);
    mostrarMatriz(m3,5);
    printf("\n");
    //figura 3
    puts("Matriz de adyacencia");
    mostrarMatriz(m2,4);
    puts("Distancias minimas");
    dijkstra(m2,m4,4);
    mostrarMatriz(m4,4);
    printf("\n");
    //borrado matrices
    liberarMatriz(m1,5);
    liberarMatriz(m2,4);
    liberarMatriz(m3,5);
    liberarMatriz(m4,4);
}

void inicializar_semilla() {
    // se establece la semilla de una nueva serie de enteros pseudo-aleatorios
    srand(time(NULL));
}

double microsegundos() {
    //obtiene la hora del sistema en microsegundos
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void showTime() {
    //muestra por pantalla la tabla con los tiempos obtenidos
    int n;
    double t, relsob, relinf, relaj;
    bool repeat;


    for(n = 15; n <= 1000; n = 2 * n){
        t = calcularTiempo(n,&repeat);
        //cálculo de relación tiempo/cotas
        relinf = t / pow((double) n, 2.5);
        relaj = t / pow((double) n, 2.9);
        relsob = t / pow((double) n, 3.5);
        if(repeat == true){ //mostramos (*) si los tiempos no superan el umbral de confianza
            printf("(*)%10d%15.3lf%15lf%15lf%15lf\n", n, t, relinf, relaj, relsob);
        } else{
            printf("%13d%15.3lf%15lf%15lf%15lf\n", n, t, relinf, relaj, relsob);
        }
    }
}
double calcularTiempo(int n, bool *bucle){
    //calcula el tiempo de ejecución del algoritmo
    double a,b,t,t1,t2;
    int i;
    matriz m = crearMatriz(n), md = crearMatriz(n);

    //caso general
    *bucle = false;
    iniMatriz(m,n);
    a = microsegundos();
    dijkstra(m,md,n);
    b = microsegundos();
    t = b - a;
    if(t < 500){ //caso tiempo obtenido no supera el umbral de confianza
        *bucle = true;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el algoritmo
            iniMatriz(m,n);
            dijkstra(m,md,n);
        }
        b = microsegundos();
        t1 = b - a;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el bucle y la ejecución
            iniMatriz(m,n);
        }
        b = microsegundos();
        t2 = b - a;
        t = (t1-t2)/ (double) K;
    }
    liberarMatriz(m,n);
    liberarMatriz(md,n);
    return t;
}

void mostrarTablas(){
    /*
     * Muestra los tiempos de ejecución de cada algoritmo en los siguientes casos:
     *
     */

    // tiempos de ordenacion por monticulos
    printf("Calculo de caminos minimos con el algoritmo de Dijkstra\n");
    printf("%12s%15s%17s%13s%18s\n","n","t(n)","t(n)/n^2.5","t(n)/2.9","t(n)/n^3.5");
    showTime();
}


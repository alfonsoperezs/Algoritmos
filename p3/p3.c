/*
 * -- PRÁCTICA 3 --
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
#include "monticulo.h"

#define K 100 //numero de repeticiones del algoritmo en caso de que no supere umbral de confianza
#define MAX_BUFFER 50

typedef void (*ordenacion) (int v[], int n); //puntero a las funciones de ordenacion
typedef void (*tDesorden) (int v [], int n); //puntero a las funciones de asignar valores a vectores

//ALGORITMOS DE ORDENACIÓN
void ordenarPorMonticulos(int v [], int n);
void crearMonticuloAUX(int v [], int n);

void testCrearMonticulos();
void testOrdenarMonticulos();
int quitarMenor(pmonticulo m);

void inicializar_semilla();
void descendente(int v [], int n);
void ascendente(int v [], int n);
void aleatorio(int v [], int n);
double microsegundos();
void listar_vector(int v[], int n);

void test(ordenacion algoritmo);
void showTime(ordenacion algoritmo, tDesorden desorden, int tCota);
double calcularTiempo(int n, bool *bucle, ordenacion algoritmo, tDesorden desorden);
void mostrarTablas();

int main(){
    inicializar_semilla();
    // probamos el correcto funcionamiento de cada uno de los algoritmos
    testOrdenarMonticulos();
    testCrearMonticulos();
    // mostramos los tiempos de ejecucion de cada algoritmo para tres situaciones distintas
    mostrarTablas();
    mostrarTablas();
    return 0;
}

void testOrdenarMonticulos(){
    int vector[] = {7,10,14,25, 21,40,38,5,30,18};
    pmonticulo monticulo;

    puts("*** Ordenacion por monticulo ***");
    inicializarMonticulo(&monticulo);
    puts("Vector de entrada:");
    listar_vector(vector,10); // mostramos vector de entrada (desordenado)
    puts("Vector ordenado:");
    ordenarPorMonticulos(vector,10);
    listar_vector(vector,10);
    borrarMonticulo(&monticulo);

}
void testCrearMonticulos(){
    int vector[] = {7,10,14,25, 21,40,38,5,30,18};
    pmonticulo monticulo;

    puts("*** Creacion de monticulos ***");
    inicializarMonticulo(&monticulo);
    puts("Vector de entrada:");
    listar_vector(vector,10); // mostramos vector de entrada (desordenado)
    crearMonticulo(vector,10,monticulo);
    puts("Monticulo creado: ");
    listar_vector(monticulo->vector,10); // mostramos el monticulo
    borrarMonticulo(&monticulo);
}
void listar_vector(int v[], int n){
    int i;
    for (i = 0; i < n; ++i) {
        printf(" %2d", v[i]);
        if(i<n-1) {
            printf(",");
        }
    }
    printf("\n");
}
void ordenarPorMonticulos(int v [], int n){
    pmonticulo monticulo;
    int i;

    inicializarMonticulo(&monticulo);
    crearMonticulo(v,n,monticulo);
    for(i = 0; i < n; ++i){
        v[i] = quitarMenor(monticulo);
    }
    borrarMonticulo(&monticulo);
}
void crearMonticuloAUX(int v [], int n){
    /*
     * Funcion auxiliar con la asignacion de memoria y liberacion
     * evitando fugas de memoria
     */
    pmonticulo monticulo;
    inicializarMonticulo(&monticulo);
    crearMonticulo(v,n,monticulo);
    borrarMonticulo(&monticulo);
}
void inicializar_semilla() {
    // se establece la semilla de una nueva serie de enteros pseudo-aleatorios
    srand(time(NULL));
}
void aleatorio(int v [], int n) {
    // se generan números pseudoaleatorio entre -n y +n y los almacena en v
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
void descendente(int v [], int n){
    // lista un vector ordenado de manera descendente {n, n-1, ... , 1}
    int i,num;
    num=n;
    for(i=0;i<n;i++){
        v[i]=num;
        --num;
    }
}
void ascendente(int v [], int n){
    // lista un vector ordenado de manera ascendente {1, 2, ... , n}
    int i;
    for(i=0;i<n;i++){
        v[i]=i+1;
    }
}
double microsegundos() {
    //obtiene la hora del sistema en microsegundos
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
void showTime(ordenacion algoritmo, tDesorden desorden, int tCota) {
    //muestra por pantalla la tabla con los tiempos obtenidos
    int n;
    double t, relsob, relinf, relaj;
    bool repeat;


    for(n = 500; n <= 256000; n = 2 * n){
        t = calcularTiempo(n,&repeat,algoritmo, desorden);
        //cálculo de relación tiempo/cotas
        switch (tCota) {
            case 1:
                relinf = t / pow((double) n, 0.8);
                relaj = t / pow((double) n, 1);
                relsob = t / pow((double) n, 1.2);
                break;
            case 2:
                relinf = t / n;
                relaj = t / (n * log2(n));
                relsob = t / pow((double) n, 1.2);
                break;
            default:
                break;
        }
        if(repeat == true){ //mostramos (*) si los tiempos no superan el umbral de confianza
            printf("(*)%10d%15.3lf%15lf%15lf%15lf\n", n, t, relinf, relaj, relsob);
        } else{
            printf("%13d%15.3lf%15lf%15lf%15lf\n", n, t, relinf, relaj, relsob);
        }
    }
}
double calcularTiempo(int n, bool *bucle, ordenacion algoritmo, tDesorden desorden){
    //calcula el tiempo de ejecución del algoritmo
    int v[256000];
    double a,b,t,t1,t2;
    int i;

    //caso general
    *bucle = false;
    desorden(v,n);
    a = microsegundos();
    algoritmo(v,n);
    b = microsegundos();
    t = b - a;
    if(t < 500){ //caso tiempo obtenido no supera el umbral de confianza
        *bucle = true;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el algoritmo
            desorden(v,n);
            algoritmo(v,n);
        }
        b = microsegundos();
        t1 = b - a;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el bucle y la ejecución
            desorden(v,n);
        }
        b = microsegundos();
        t2 = b - a;
        t = (t1-t2)/ (double) K;
    }
    return t;
}
void mostrarTablas(){
    /*
     * Muestra los tiempos de ejecución de cada algoritmo en los siguientes casos:
     * 1) Creación de monticulo
     * 1) Vector ordenado en orden ascendente
     * 2) Vector ordenado en orden descendente
     * 3) Vector desordenado
     */

    printf("Operacion de crear monticulos\n");
    printf("%12s%15s%17s%13s%18s\n","n","t(n)","t(n)/n^0.8","t(n)/n","t(n)/n^1.2");
    showTime(crearMonticuloAUX, aleatorio, 1);
    // tiempos de ordenacion por monticulos
    printf("Ordenacion por monticulos con inicializacion ascendente\n");
    printf("%12s%15s%15s%17s%15s\n","n","t(n)","t(n)/n","t(n)/n*logn","t(n)/n^1.2");
    showTime(ordenarPorMonticulos, ascendente, 2);
    printf("Ordenacion por monticulos con inicializacion descendente\n");
    printf("%12s%15s%15s%17s%15s\n","n","t(n)","t(n)/n","t(n)/n*logn","t(n)/n^1.2");
    showTime(ordenarPorMonticulos, descendente, 2);
    printf("Ordenacion por monticulos con inicializacion aleatoria\n");
    printf("%12s%15s%15s%17s%15s\n","n","t(n)","t(n)/n","t(n)/n*logn","t(n)/n^1.2");
    showTime(ordenarPorMonticulos, aleatorio, 2);
}




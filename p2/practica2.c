/*
 * -- PRÁCTICA 2 --
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

#define K 100 //numero de repeticiones del algoritmo en caso de que no supere umbral de confianza
#define MAX_BUFFER 50

typedef void (*ordenacion) (int v[], int n); //puntero a las funciones de ordenacion
typedef void (*tDesorden) (int v [], int n); //puntero a las funciones de asignar valores a vectores

//ALGORITMOS DE ORDENACIÓN
void ord_ins (int v [], int n);
void ord_shell (int v [], int n);

void inicializar_semilla();
void descendente(int v [], int n);
void ascendente(int v [], int n);
void aleatorio(int v [], int n);
double microsegundos();

void test(ordenacion algoritmo);
void showTime(ordenacion algoritmo, tDesorden desorden, int tCota);
double calcularTiempo(int n, bool *bucle, ordenacion algoritmo, tDesorden desorden);
void listar_vector(int v[], int n);
void mostrarTablas();

int main(){
    inicializar_semilla();
    // probamos el correcto funcionamiento de cada uno de los algoritmos
    test(ord_ins);
    test(ord_shell);
    // mostramos los tiempos de ejecucion de cada algoritmo para tres situaciones distintas
    mostrarTablas();
    mostrarTablas();
    return 0;
}

void ord_ins(int v[], int n){
    /*
     * Algoritmo de ordenación por inserción de un vector
     */
    int i, x, j;

    for (i = 1; i < n; i++){
        x = v[i];
        j = i - 1;
        while(j >= 0 && v[j] > x){
            v[j + 1] = v[j];
            j = j -1;
        }
        v[j +1] = x;
    }
}
void ord_shell(int v[], int n){
    /*
     * Algoritmo de ordenación shell de un vector
     */
    int incremento, tmp, i, j;
    bool seguir;

    incremento = n;
    do {
        incremento = incremento / 2;
        for (i = incremento; i < n; ++i){
            tmp = v[i];
            j = i;
            seguir = true;
            while (j - incremento >= 0 && seguir){
                if(tmp < v[j - incremento]){
                    v[j] = v[j - incremento];
                    j = j - incremento;
                } else{
                    seguir = false;
                }
            }
            v[j] = tmp;
        }
    } while (incremento != 1);
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
void test(ordenacion algoritmo){
    int v[17];

    //probamos el algoritmo con un vector aleatorio de 17 elementos
    aleatorio(v,17);
    puts("Inicializacion aleatoria");
    listar_vector(v,17);
    puts("Ordenacion por insercion");
    algoritmo(v,17);
    listar_vector(v,17);

    //probamos el algoritmo para un vector descendente
    puts("Inicializacion descendente");
    descendente(v,10);
    listar_vector(v,10);
    puts("Ordenacion por insercion");
    algoritmo(v,10);
    listar_vector(v,10);
    printf("\n");
}
void showTime(ordenacion algoritmo, tDesorden desorden, int tCota) {
    //muestra por pantalla la tabla con los tiempos obtenidos
    int n;
    double t, relsob, relinf, relaj;
    bool repeat;


    for(n = 500; n <= 64000; n = 2 * n){
        t = calcularTiempo(n,&repeat,algoritmo, desorden);
        //cálculo de relación tiempo/cotas
        switch (tCota) {
            case 1:
                relinf = t / pow((double) n, 0.8);
                relaj = t / pow((double) n, 1);
                relsob = t / pow((double) n, 1.2);
                break;
            case 2:
                relinf = t / pow((double) n, 1.8);
                relaj = t / pow((double) n, 2);
                relsob = t / pow((double) n, 2.2);
                break;
            case 3:
                relinf = t / (double) n;
                relaj = t / ((double) n * log((double) n));
                relsob = t / pow((double) n, 1.5);
                break;
            case 4:
                relinf = t / ((double) n * log((double) n));
                relaj = t / ((double) n * pow(log((double) n), 2));
                relsob = t / ((double) n * pow(log((double) n), 3));
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
    int v[64000];
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
void mostrarTablas(){
    /*
     * Muestra los tiempos de ejecución de cada algoritmo en los siguientes casos:
     * 1) Vector ordenado en orden ascendente
     * 2) Vector ordenado en orden descendente
     * 3) Vector desordenado
     */

    // tiempos de ordenacion por insercion
    printf("Ordenacion por insercion con inicializacion ascendente\n");
    printf("%12s%15s%17s%13s%18s\n","n","t(n)","t(n)/n^0.8","t(n)/n","t(n)/n^1.2");
    showTime(ord_ins, ascendente, 1);
    printf("Ordenacion por insercion con inicializacion descendente\n");
    printf("%12s%15s%17s%15s%16s\n","n","t(n)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
    showTime(ord_ins, descendente, 2);
    printf("Ordenacion por insercion con inicializacion aleatoria\n");
    printf("%12s%15s%17s%15s%16s\n","n","t(n)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
    showTime(ord_ins, aleatorio, 2);
    // tiempos de ordenacion shell
    printf("Ordenacion shell con inicializacion ascendente\n");
    printf("%12s%15s%16s%17s%16s\n","n","t(n)","t(n)/n","t(n)/nlog(n)","t(n)/n^1.5");
    showTime(ord_shell, ascendente, 3);
    printf("Ordenacion shell con inicializacion descendente\n");
    printf("%12s%15s%17s%17s%13s\n","n","t(n)","t(n)/nlog(n)","t(n)/nlog(n)^2","t(n)/n^3");
    showTime(ord_shell, descendente, 4);
    printf("Ordenacion shell con inicializacion aleatoria\n");
    printf("%12s%15s%17s%17s%13s\n","n","t(n)","t(n)/nlog(n)","t(n)/nlog(n)^2","t(n)/n^3");
    showTime(ord_shell, aleatorio, 4);
}

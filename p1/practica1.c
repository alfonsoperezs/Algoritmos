/*
 * -- PRÁCTICA 1 --
 * Alfonso Pérez Sánchez (a.perezs@udc.es)
 * Brais Calvo Pensado (brais.calvo.pensado@udc.es)
 * Lucía Lagares Álvarez (lucia.lagares1@udc.es)
 */
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define K 100 //numero de repeticiones del algoritmo en caso de que no supere umbral de confianza

typedef int (*sumaSubMax) (int v[], int n); //puntero a las funciones sumaSubMax

void inicializar_semilla();
void listar_vector(int v[], int n);
int sumaSubMax1(int v[], int n);
int sumaSubMax2(int v[], int n);
void test1();
void test2();
void mostrarResultado(int v[], int n);

double microsegundos();
double calcularTiempo(int n, bool *bucle, sumaSubMax funcion);
void showTime1();
void showTime2();

int main() {
    int n;

    inicializar_semilla();
    test1();
    test2();
    for(n = 0; n < 4; ++n){ //se repite la obtencion de los tiempos para una mayor fiabilidad
        showTime1();
    }
    for(n = 0; n < 4; ++n){ //se repite la obtencion de los tiempos para una mayor fiabilidad
        showTime2();
    }
    return 0;
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
int sumaSubMax1(int v[], int n){
    //algoritmo 1 que calcula la suma de la subsecuencia máxima
    int estaSuma, sumaMax = 0;
    int i, j;

    for( i = 0; i < n; i++){
        estaSuma = 0;
        for(j = i; j < n; j++){
            estaSuma += v[j];
            if(estaSuma > sumaMax)
                sumaMax = estaSuma;
        }
    }
    return sumaMax;
}
int sumaSubMax2(int v[], int n){
    //algoritmo 2 que calcula la suma de la subsecuencia máxima
    int estaSuma, sumaMax, j;

    estaSuma = 0;
    sumaMax =0;
    for(j = 0; j < n; ++j){
        estaSuma = estaSuma + v[j];
        if(estaSuma > sumaMax)
            sumaMax = estaSuma;
        else{
            if(estaSuma < 0)
                estaSuma = 0;
        }
    }
    return sumaMax;
}
void listar_vector(int v[], int n){
    //muestra el vector v por pantalla
    int i;

    printf("{ ");
    for (i = 0; i < n; ++i) {
        printf("  %2d  ", v[i]);
    }
    printf(" }");
}
void test1(){
    //comprueba el funcionamiento de los algoritmos con vectores fijos
    int v1[] = {-9, 2, -5, -4, 6};
    int v2[] = { 4, 0, 9, 2, 5};
    int v3[] = { -2, -1, -9, -7, -1};
    int v4[] = { 9, -2, 1, -7, -8};
    int v5[] = { 15, -2, -5, -4, 16};
    int v6[] = { 7, -5, 6, 7, -7};

    printf("**** TEST 1 ****\n");
    printf("%33s%20s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    mostrarResultado(v1,5);
    mostrarResultado(v2,5);
    mostrarResultado(v3,5);
    mostrarResultado(v4,5);
    mostrarResultado(v5,5);
    mostrarResultado(v6,5);
}
void test2(){
    //comprueba el funcionamiento de los algoritmos con vectores aleatorios
    int v[9], i;

    printf("**** TEST 2 ****\n");
    printf("%33s%45s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i = 0; i < 9; ++i){
        aleatorio(v,9);
        mostrarResultado(v,9);
    }
}
void mostrarResultado(int v[], int n){
    /*
     * muestra por pantalla el vector a calcular la subsecuencia máxima
     * y el resultado para ambos algoritmos
     */
    int v1,v2;

    listar_vector(v,n);
    v1 = sumaSubMax1(v,n);
    v2 = sumaSubMax2(v,n);
    printf("%15d %15d\n", v1, v2);
}

double microsegundos() {
    //obtiene la hora del sistema en microsegundos
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
double calcularTiempo(int n, bool *bucle, sumaSubMax funcion){
    //calcula el tiempo de ejecución del algoritmo
    int v[256000];
    double a,b,t,t1,t2;
    int i;

    //caso general
    *bucle = false;
    aleatorio(v,n);
    a = microsegundos();
    funcion(v,n);
    b = microsegundos();
    t = b - a;
    if(t < 500){ //caso tiempo obtenido no supera el umbral de confianza
        *bucle = true;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el algoritmo
            aleatorio(v,n);
            funcion(v,n);
        }
        b = microsegundos();
        t1 = b - a;
        a = microsegundos();
        for(i = K; i > 0; --i){ //se ejecuta K veces el bucle y la ejecución
            aleatorio(v,n);
        }
        b = microsegundos();
        t2 = b - a;
        t = (t1-t2)/ (double) K;
    }
    return t;
}
void showTime1(){
    //muestra por pantalla la tabla con los tiempos obtenidos para sumaSubMax1
    int n;
    double t, cotasob, cotainf, cota;
    bool repeat;

    printf("SumaSubMax 1\n");
    printf("%30s%30s%30s%30s%30s\n","n","t(n)","t(n)/n^1.8","t(n)/n^2","t(n)/n^2.2");
    for(n = 500; n <= 32000; n = 2 * n){
        t = calcularTiempo(n,&repeat,sumaSubMax1);
        //cálculo de relación tiempo/cotas
        cotainf = t / pow((double) n, 1.8);
        cota = t / pow((double) n, 2);
        cotasob = t / pow((double) n, 2.2);
        if(repeat == true){ //mostramos (*) si los tiempos no superan el umbral de confianza
            printf("(*)%27d%30.3lf%30lf%30lf%30lf\n", n, t, cotainf, cota, cotasob);
        } else{
            printf("%30d%30.3lf%30lf%30lf%30lf\n", n, t, cotainf, cota, cotasob);
        }
    }
}
void showTime2(){
    //muestra por pantalla la tabla con los tiempos obtenidos para sumaSubMax1
    int n;
    double t, cotasob, cotainf, cota;
    bool repeat;

    printf("SumaSubMax 2\n");
    printf("%30s%30s%30s%30s%30s\n","n","t(n)","t(n)/n^0.8","t(n)/n^1","t(n)/n^1.2");
    for(n = 500; n <= 256000; n = 2 * n){
        t = calcularTiempo(n,&repeat,sumaSubMax2);
        //cálculo de relación tiempo/cotas
        cotainf = t / pow((double) n, 0.8);
        cota = t / (double) n;
        cotasob = t / pow((double) n, 1.2);
        if(repeat == true){ //mostramos (*) si los tiempos no superan el umbral de confianza
            printf("(*)%27d%30.3lf%30lf%30lf%30lf\n", n, t, cotainf, cota, cotasob);
        } else{
            printf("%30d%30.3lf%30lf%30lf%30lf\n", n, t, cotainf, cota, cotasob);
        }
    }
}

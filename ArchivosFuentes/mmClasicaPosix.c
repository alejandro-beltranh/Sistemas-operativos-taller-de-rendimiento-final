/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: mmClasicaPosix.c
 * Descripción: Implementación del algoritmo de multiplicación clásica de matrices utilizando
 *              paralelismo mediante hilos POSIX. Cada hilo procesa un bloque de 
 *              filas de la matriz A para generar la porción correspondiente de la matriz C. 
 *              Incluye creación y sincronización de hilos, control básico con mutex,
 *              medición del tiempo total y manejo dinámico de memoria.
 ********************************************************************************************/

#include <stdio.h>      
#include <pthread.h>    
#include <unistd.h>     
#include <stdlib.h>     
#include <time.h>       
#include <sys/time.h>   
#include "funciones.h"  

// Mutex global para evitar condiciones de carrera 
pthread_mutex_t MM_mutex;

// Punteros globales a las matrices utilizadas por los hilos
double *matrixA, *matrixB, *matrixC;


// Estructura de parámetros contiene los datos necesarios que se envían a cada hilo al crearse.
struct parametros {
    int nH;   // Número total de hilos
    int idH;  // Identificador del hilo actual
    int N;    // Tamaño de la matriz (N x N)
};


// Multiplica matrices A y B en paralelo usando hilos POSIX.
// Cada hilo procesa un rango de filas especifico.
void *multiMatrixPosix(void *variables) {
    struct parametros *data = (struct parametros *)variables; // Conversión del puntero genérico a tipo 'parametros'
    int idH = data->idH; // Identificador del hilo actual
    int nH  = data->nH;    // Total de hilos activos
    int D   = data->N;   // Tamaño de la matriz

    // Cálculo de las filas que procesará el hilo
    int filaI = (D / nH) * idH; // Fila inicial del hilo
    int filaF = (D / nH) * (idH + 1);  // Fila final del hilo

    double Suma, *pA, *pB;     // Variables locales de cálculo

    // Recorre el rango de filas asignado al hilo
    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {    // Recorre columnas de la matriz B
            pA = matrixA + i * D;  // Apunta a la fila i de A
            pB = matrixB + j;  // Apunta a la columna j de B
            Suma = 0.0;  // Reinicia la suma parcial
            for (int k = 0; k < D; k++, pA++, pB += D) {  // Recorre los elementos de la fila y la columna
                Suma += *pA * *pB;       // Acumula el producto 
            }
            matrixC[i * D + j] = Suma;   // Guarda el resultado en la matriz C
        }
    }

    // Sección crítica protegida por el mutex 
    pthread_mutex_lock(&MM_mutex);
    pthread_mutex_unlock(&MM_mutex);

    pthread_exit(NULL);  // Finalizacion del hilo 
}

int main(int argc, char *argv[]) {
    if (argc < 3) { // Verifica que se pasen los parametros requeridos
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        exit(0); // Finaliza el programa si los parametros no son correctos
    }

    int N = atoi(argv[1]);   // Convierte el tamaño de la matriz a entero
    int n_threads = atoi(argv[2]);      // Convierte el número de hilos a entero
    pthread_t p[n_threads];      // Arreglo de identificadores de hilos
    pthread_attr_t atrMM;    // Atributos de los hilos (estado, tipo, etc.)

    
    matrixA = (double *)calloc(N * N, sizeof(double)); // se resrva memoria para las matrices
    matrixB = (double *)calloc(N * N, sizeof(double));
    matrixC = (double *)calloc(N * N, sizeof(double));

    srand(time(NULL));                                                   

    iniMatrix(matrixA, N);  // Inicializa la matriz A
    iniMatrix(matrixB, N);  // Inicializa la matriz B

    if (N < 9) {  // Si la matriz es menor a 9, imprime su contenido
        impMatrix(matrixA, N, 0);
        impMatrix(matrixB, N, 1);
    }

    InicioMuestra();  // Inicia la medición del tiempo de ejecución

    pthread_mutex_init(&MM_mutex, NULL);   // Inicializa el mutex
    pthread_attr_init(&atrMM);    // Inicializa los atributos de los hilos
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Configura los hilos como "joinable" 

    
    for (int j = 0; j < n_threads; j++) { // Crea los hilos y les pasa su estructura de parámetros
        struct parametros *datos = (struct parametros *)malloc(sizeof(struct parametros)); // Reserva espacio para la estructura
        datos->idH = j; // ID del hilo
        datos->nH  = n_threads;    // Número total de hilos
        datos->N   = N;    // Tamaño de la matriz
        pthread_create(&p[j], &atrMM, multiMatrixPosix, (void *)datos);  // Crea el hilo
    }

    // Espera a que todos los hilos terminen su ejecución
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL);
    }

    FinMuestra();  // Finaliza la medición del tiempo y lo muestra

    if (N < 9) {  // Si la matriz es menor a 9, imprime el resultado
        impMatrix(matrixC, N, 2);
    }

    // Libera la memoria de las matrices 
    free(matrixA);
    free(matrixB);
    free(matrixC);

    pthread_attr_destroy(&atrMM);   // Destruye los atributos de los hilos
    pthread_mutex_destroy(&MM_mutex);  // Libera el mutex
    pthread_exit(NULL); // Finaliza el hilo principal 

    return 0;                                                            
}

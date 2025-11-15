/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: mmClasicaOpenMP.c
 * Descripción: Implementación del algoritmo de multiplicación clásica de matrices utilizando
 *              paralelismo con OpenMP. La carga de trabajo se divide automáticamente entre 
 *              los hilos mediante la directiva 'omp for', permitiendo calcular la matriz 
 *              resultado de manera concurrente. Incluye inicialización de matrices, medición 
 *              del tiempo total de ejecución y liberación de memoria.
 ********************************************************************************************/


#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <time.h>      
#include <sys/time.h>   
#include <omp.h>        
#include "funciones.h"   

// Multiplica dos matrices cuadradas usando paralelismo con OpenMP.
void multiMatrixOpenMP(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB; // Variables locales para la suma  y punteros de recorrido

    #pragma omp parallel private(Suma, pA, pB)   // Cada hilo va a tener sus propias copias de Suma, pA y pB
    {
        #pragma omp for // Distribuye el bucle entre los hilos de forma paralela
        for (int i = 0; i < D; i++) {   // Recorre las filas de la matriz A
            for (int j = 0; j < D; j++) {  // Recorre las columnas de la matriz B
                pA = mA + i * D;               
                pB = mB + j;                   
                Suma = 0.0; // Inicializa el acumulador en 0
                for (int k = 0; k < D; k++, pA++, pB += D) {  // Multiplica fila x columna
                    Suma += *pA * *pB; // Suma parcial del producto
                }
                mC[i * D + j] = Suma;// Almacena el resultado de la suma en la matriz C
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) { // Verifica que se pasen los dos parametros necesarios
        printf("\nUso: $./clasicaOpenMP SIZE Hilos\n\n");  // si no imprime un mensaje 
        exit(0); // y se finaliza el programa
    }

    int N  = atoi(argv[1]);                                       
    int TH = atoi(argv[2]);                                       

    double *matrixA = (double *)calloc(N * N, sizeof(double));    // Reserva memoria para la matriz A
    double *matrixB = (double *)calloc(N * N, sizeof(double));    // Reserva memoria para la matriz B
    double *matrixC = (double *)calloc(N * N, sizeof(double));    // Reserva memoria para la matriz C 

    srand(time(NULL));  // Inicializa la semilla del generador de números aleatorios
    omp_set_num_threads(TH); // Establece la cantidad de hilos que se usaran

    iniMatrix(matrixA, N); // Inicializa la matriz A con valores aleatorios
    iniMatrix(matrixB, N);  // Inicializa la matriz B con valores aleatorios

    if (N < 9) {    // Si el tamaño es menor a 9, imprime las matrices
        impMatrix(matrixA, N, 0);  // Imprime la matriz A
        impMatrix(matrixB, N, 1);   // Imprime la matriz B
    }

    InicioMuestra();// Inicia la medición del tiempo
    multiMatrixOpenMP(matrixA, matrixB, matrixC, N); // Realiza la multiplicación con OpenMP
    FinMuestra(); // Finaliza y muestra el tiempo total

    if (N < 9) { // Si el tamaño es menor a 9, imprime la matriz resultado
        impMatrix(matrixC, N, 2); // Imprime la matriz C con el resultado
    }

    // Liberación de memoria 
    free(matrixA);  // Libera la memoria de la matriz A
    free(matrixB);  // Libera la memoria de la matriz B
    free(matrixC);    // Libera la memoria de la matriz C

    return 0;                                                    
}

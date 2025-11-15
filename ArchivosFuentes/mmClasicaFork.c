/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: mmClasicaFork.c
 * Descripción: Implementación del algoritmo de multiplicación clásica de matrices usando 
 *              paralelismo a nivel de procesos mediante fork(). Cada proceso hijo calcula 
 *              un rango específico de filas de la matriz resultado, mientras el proceso 
 *              padre coordina la ejecución y mide el tiempo total utilizando funciones 
 *              auxiliares definidas en 'funciones.c'.
 ********************************************************************************************/


#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     // Interfaz para llamadas al sistema POSIX, como fork() y getpid()
#include <sys/wait.h>   // interfaz que nos permite manejar la sincronización de procesos 
#include <sys/time.h>   // Interfaz para medición de tiempos
#include <time.h>       // interfaz para manejo de tiempo y generación de semillas aleatorias
#include "funciones.h"  // Interfaz con las funciones compartidas 

// Multiplica las matrices A y B en el rango de filas asignado (usando procesos con Fork)
void multiMatrixFork(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
    double Suma, *pA, *pB;                     // Variables locales para la suma y los punteros de recorrido
    for (int i = filaI; i < filaF; i++) { // Recorre las filas asignadas a este proceso
        for (int j = 0; j < D; j++) {          // Recorre las columnas de la matriz B
            Suma = 0.0; // Reinicia la suma para el nuevo elemento
            pA = mA + i * D;     // Apunta a la fila correspondiente de la matriz A
            pB = mB + j;  // Apunta a la columna correspondiente de la matriz B
            for (int k = 0; k < D; k++, pA++, pB += D) {   // Recorre los elementos para multiplicar y acumular
                Suma += *pA * *pB;             // Realiza la multiplicación y suma 
            }
            mC[i * D + j] = Suma; // Almacena el resultado en la posición (i, j)
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {   // Verifica que se hayan pasado los parametros necesarios 
        printf("\n\tUso: $./nom_ejecutable Size Hilos\n");         // Mensaje de ayuda si faltan argumentos
        exit(0);  // Termina el programa
    }

    int N = atoi(argv[1]);                                         
    int num_P = atoi(argv[2]);                                     

    double *matA = (double *)calloc(N * N, sizeof(double));      // Reserva memoria para la matriz A
    double *matB = (double *)calloc(N * N, sizeof(double));  // Reserva memoria para la matriz B
    double *matC = (double *)calloc(N * N, sizeof(double));         // Reserva memoria para la matriz C 

    srand(time(0));       // Inicializa lel generador de números aleatorios
    iniMatrix(matA, N);     // Inicializa la matriz A con valores aleatorios
    iniMatrix(matB, N);   // Inicializa la matriz B con valores aleatorios
    impMatrix(matA, N, 0);     // Imprime la matriz A (si N < 9)
    impMatrix(matB, N, 1);    // Imprime la matriz B (si N < 9)

    int rows_per_process = N / num_P; // Calcula cuántas filas procesará cada proceso hijo

    InicioMuestra();   // Inicia la medición del tiempo de ejecución

    for (int i = 0; i < num_P; i++) {     // Crea un proceso hijo por cada iteración
        pid_t pid = fork(); // Crea un nuevo proceso
        if (pid == 0) {                                            
            int start_row = i * rows_per_process;                  // Calcula la fila inicial del proceso hijo
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;   // Calcula la fila final 
            multiMatrixFork(matA, matB, matC, N, start_row, end_row);            // Llama a la función de multiplicación 

            if (N < 9) {   // Imprime las filas calculadas solo si la matriz es menor a 9
                printf("\nChild PID %d calculó filas %d a %d:\n", getpid(), start_row, end_row - 1);
                for (int r = start_row; r < end_row; r++) { 
                    for (int c = 0; c < N; c++) {  
                        printf(" %.2f ", matC[N * r + c]); // Imprime el valor calculado 
                    }
                    printf("\n");                                  
                }
            }
            exit(0);   // Termina el proceso hijo
        } else if (pid < 0) {  // Si fork() devuelve un valor negativo, arroja un error
            perror("Error al crear proceso (fork)");               
            exit(1);                                              
        }
    }

    for (int i = 0; i < num_P; i++) { // El proceso padre espera que todos los hijos terminen
        wait(NULL);    // Bloquea hasta que un proceso hijo finaliza
    }

    FinMuestra();  // Finaliza la medición del tiempo y muestra el resultado

    free(matA);  // Libera la memoria de la matriz A
    free(matB);  // Libera la memoria de la matriz B
    free(matC);      // Libera la memoria de la matriz C

    return 0;                                                    
}

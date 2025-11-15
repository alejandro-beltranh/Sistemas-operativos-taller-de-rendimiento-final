/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: mmFilasOpenMP.c
 * Descripción: Implementación optimizada de la multiplicación clásica de matrices utilizando
 *              OpenMP y la transpuesta de la matriz B para mejorar la localidad de memoria.
 *              Cada hilo procesa bloques de filas de la matriz A, permitiendo un acceso más 
 *              eficiente y reduciendo fallos de caché. Incluye inicialización de matrices, 
 *              medición del tiempo total y liberación de memoria dinámica.
 ********************************************************************************************/


#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <time.h>       
#include <sys/time.h>   
#include <omp.h>        
#include "funciones.h"  


// Multiplica matrices A y B, usando la transpuesta de B en paralelo con OpenMP.
void multiMatrixTrans(double *mA, double *mB, double *mC, int D) {
    double Suma, *pA, *pB;// Declaracion de variables locales para acumulación y punteros de recorrido

    #pragma omp parallel private(Suma, pA, pB) // Cada hilo tiene sus propias copias de estas variables
    {
        #pragma omp for    // Distribuye el bucle entre los hilos disponibles
        for (int i = 0; i < D; i++) {                    
            for (int j = 0; j < D; j++) {               
                pA = mA + i * D;                         
                pB = mB + j * D;                         
                Suma = 0.0;                              // Inicializa la suma parcial
                for (int k = 0; k < D; k++, pA++, pB++) { // Recorre cada elemento de la fila y la columna
                    Suma += *pA * *pB;    // Acumula el producto 
                }
                mC[i * D + j] = Suma;   // Guarda el resultado en la matriz C
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {  // Verifica que se pasen los parametros necesarios
        printf("\nUso: $./filasOpenMP SIZE Hilos\n\n"); //si no se pasaron imprime un mensaje de error
        exit(0); // Termina el programa si faltan argumentos
    }

    int N  = atoi(argv[1]); // Convierte el primer argumento 
    int TH = atoi(argv[2]);     // Convierte el segundo argumento 

     
    double *matrixA = (double *)calloc(N * N, sizeof(double)); // Reserva memoria dinámica para las tres matrices
    double *matrixB = (double *)calloc(N * N, sizeof(double));
    double *matrixC = (double *)calloc(N * N, sizeof(double));

    srand(time(NULL));                                              
    omp_set_num_threads(TH);   // Se Define el número de hilos que OpenMP usará

    iniMatrix(matrixA, N);  // Inicializa la matriz A con valores aleatorios
    iniMatrix(matrixB, N);   // Inicializa la matriz B con valores aleatorios

    if (N < 9) {    // Si las matrices son menores a 9, las imprime
        impMatrix(matrixA, N, 0);                                    
        impMatrix(matrixB, N, 1);                                    
    }

    InicioMuestra();  // Inicia la medición del tiempo
    multiMatrixTrans(matrixA, matrixB, matrixC, N);  // Realiza la multiplicación con OpenMP
    FinMuestra();  // Finaliza la medición del tiempo y lo muestra

    if (N < 9) {  // Si la matriz es de tamaño menor a 9, imprime el resultado
        impMatrix(matrixC, N, 2);                                    // Imprime el resultado en la matriz c 
    }

   
    free(matrixA);     // Libera la memoria de la matriz A
    free(matrixB); // Libera la memoria de la matriz B
    free(matrixC);  // Libera la memoria de la matriz C

    return 0; //Fin del programa                                                    
}

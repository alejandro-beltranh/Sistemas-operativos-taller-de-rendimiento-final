/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: funciones.c
 * Descripción: Implementación de funciones auxiliares para el proyecto de multiplicación de
 *              matrices. Incluye rutinas para medir el tiempo de ejecución, inicializar
 *              matrices con valores aleatorios e imprimir matrices pequeñas. Estas funciones
 *              son utilizadas por las versiones Fork, POSIX Threads y OpenMP.
 ********************************************************************************************/

#include "funciones.h"   // interfez donde están declaradas las funciones 

struct timeval inicio, fin;   // Estructuras globales para almacenar los tiempos de inicio y fin de la ejecución


// Inicia la medición de tiempo de ejecución usando gettimeofday().
void InicioMuestra() {   
    gettimeofday(&inicio, (void *)0);   // Captura el tiempo actual y lo guarda en la variable 'inicio'
}


// Calcula y muestra el tiempo total de ejecución en microsegundos.
void FinMuestra() {   
    gettimeofday(&fin, (void *)0);   // Captura el tiempo actual al final de la ejecución
    fin.tv_usec -= inicio.tv_usec;   // Resta los microsegundos iniciales a los finales
    fin.tv_sec  -= inicio.tv_sec;    // Resta los segundos iniciales a los finales
    double tiempo = (double)(fin.tv_sec * 1000000 + fin.tv_usec);   // Convierte todo a microsegundos
    printf("%9.0f µs\n", tiempo);   // Imprime el tiempo total en microsegundos
}


// Inicializa una matriz cuadrada.
void iniMatrix(double *matrix, int D) {   
    for (int i = 0; i < D * D; i++) {   // Recorre cada elemento de la matriz
        matrix[i] = ((double)(rand() % 1000) / 100.0);   // Asigna un valor aleatorio con dos decimales
    }
}


// Imprime una matriz cuadrada en formato de dos decimales.
// Solo sirve si D < 9.
void impMatrix(double *matrix, int D, int t) {   
    if (D < 9) {   // verifica si la matriz es de tamaño menor a 9 
        switch (t) {   // Selecciona el tipo de matriz a imprimir
            case 0:
                printf("\nMatriz A:\n");   // Identifica la matriz A
                break;
            case 1:
                printf("\nMatriz B:\n");   // Identifica la matriz B
                break;
            case 2:
                printf("\nMatriz C (Resultado):\n");   // Identifica la matriz resultado C
                break;
        }
        for (int i = 0; i < D * D; i++) {   // Recorre todos los elementos de la matriz
            if (i % D == 0) printf("\n");   // Salta de línea al finalizar cada fila
            printf("%.2f ", matrix[i]);   // Imprime el valor con dos decimales
        }
        printf("\n");   
    }
}

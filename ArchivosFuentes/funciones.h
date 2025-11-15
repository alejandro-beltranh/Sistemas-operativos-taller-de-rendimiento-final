/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Alejandro Beltrán Huertas
 * Fecha: 12/11/2025
 * Archivo: funciones.h
 * Descripción: Archivo deonde se declaran las funciones auxiliares utilizadas en el
 *              proyecto de multiplicación de matrices. Proporciona las firmas para la
 *              medición de tiempos, inicialización de matrices e impresión de matrices,
 *              además de declarar variables globales compartidas entre los programas.
 ********************************************************************************************/


#ifndef FUNCIONES_H    // Evita que el archivo se incluya más de una vez 
#define FUNCIONES_H    

#include <stdio.h>     
#include <stdlib.h>    // interfaz para el uso de memoria dinámica
#include <sys/time.h>  // interfaz para medir tiempos con la estructura timeval

extern struct timeval inicio, fin;   // Declaración de variables globales para guardar los tiempos de inicio y fin

void InicioMuestra();                // firma de la función que inicia la medición del tiempo
void FinMuestra();                   // firma de la función que finaliza la medición y muestra el tiempo total
void iniMatrix(double *matrix, int D);        // firma de la función que inicializa una matriz con valores aleatorios
void impMatrix(double *matrix, int D, int t); // firma de la función que imprime una matriz dependiendo de su tipo (A, B o C)

#endif    // Fin del bloque de protección del header

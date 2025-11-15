#********************************************************************************************
#* Pontificia Universidad Javeriana
#* Autor: Alejandro Beltrán Huertas
#* Fecha: 12/11/2025
#* Archivo: Makefile
#* Descripción: Archivo de automatización encargado de compilar las distintas versiones del
#*              algoritmo de multiplicación de matrices. 
#*              Define el compilador, flags de optimización y reglas para construir cada 
#*              ejecutable, así como una regla de limpieza para eliminar binarios generados.
#********************************************************************************************


# Compilador
CC = gcc-15
CFLAGS = -Wall -O3 -fopenmp -pthread

# Archivos fuente
COMMON = funciones.c
SRC = mmClasicaFork.c mmClasicaOpenMP.c mmClasicaPosix.c mmFilasOpenMP.c
EXEC = $(SRC:.c=)

# compila todos los ejecutables
all: $(EXEC)

# Compilación individual de cada programa 
mmClasicaFork: mmClasicaFork.c $(COMMON) funciones.h
	$(CC) $(CFLAGS) mmClasicaFork.c $(COMMON) -o mmClasicaFork

mmClasicaOpenMP: mmClasicaOpenMP.c $(COMMON) funciones.h
	$(CC) $(CFLAGS) mmClasicaOpenMP.c $(COMMON) -o mmClasicaOpenMP

mmClasicaPosix: mmClasicaPosix.c $(COMMON) funciones.h
	$(CC) $(CFLAGS) mmClasicaPosix.c $(COMMON) -o mmClasicaPosix

mmFilasOpenMP: mmFilasOpenMP.c $(COMMON) funciones.h
	$(CC) $(CFLAGS) mmFilasOpenMP.c $(COMMON) -o mmFilasOpenMP

# Limpieza de ejecutables
clean:
	rm -f $(EXEC)
 

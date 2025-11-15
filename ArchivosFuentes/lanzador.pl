#********************************************************************************************
#* Pontificia Universidad Javeriana
#* Autor: Alejandro Beltrán Huertas
#* Fecha: 12/11/2025
#* Archivo: lanzador.pl
#* Descripción: Script en Perl que automatiza la ejecución de las versiones Fork, POSIX y 
#*              OpenMP del algoritmo de multiplicación de matrices. Ejecuta cada programa 
#*              con distintos tamaños de matriz y diferentes cantidades de hilos, repitiendo 
#*              cada prueba 30 veces y almacenando los tiempos obtenidos en archivos dentro 
#*              de la carpeta 'resultados'.
#********************************************************************************************


use strict;          
use warnings;        

# Lista de tamaños de matrices que se van a usar
my @Size_Matriz = ("2", "10", "20");

# Lista de cantidades de hilos a usar para para cada ejecución
my @Num_Hilos   = (1, 2, 4, 8, 16, 20);

# Lista de programas que se van a ejecutar
my @Programas = ("mmClasicaFork", "mmClasicaPosix", "mmClasicaOpenMP", "mmFilasOpenMP");


# Carpeta donde se van a guardar los resultados 
my $dir_resultados = "resultados";

# Si la carpeta no existe, se crea 
mkdir $dir_resultados unless -d $dir_resultados;

print "\n=== Iniciando batería de pruebas ===\n";   # Mensaje que indica que el programa inicia


# Recorre cada programa de la lista
foreach my $prog (@Programas) {

    # Recorre cada tamaño de matriz definido
    foreach my $tam (@Size_Matriz) {

        # Recorre cada número de hilos configurado
        foreach my $hilos (@Num_Hilos) {
            
            # Define el nombre del archivo de salida según el programa, tamaño y número de hilos
            my $archivo_salida = "$dir_resultados/${prog}_${tam}x${tam}_${hilos}hilos.dat";
            
            # muestra en pantalla un mensaje informativo sobre la ejecución actual
            print "\nEjecutando: $prog con matriz ${tam}x${tam} y $hilos hilos...\n";
            
           #ejecuta el programa 30 veces
            for (my $i = 0; $i < 30; $i++) {
                # Construye el comando que ejecuta el programa con sus parámetros
                my $comando = "./$prog $tam $hilos >> $archivo_salida";
                
                # Llama al sistema operativo para ejecutar el comando
                system($comando);
            }
        }
    }
}

print "\n=== Ejecución completa ===\n";  # Mensaje de finalización
print "Archivos generados en la carpeta '$dir_resultados'\n";  # Indica dónde quedaron los resultados


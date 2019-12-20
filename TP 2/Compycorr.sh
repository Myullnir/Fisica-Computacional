#!/bin/bash
# Este programa compila y corre los archivos, me ahorra el gcc y la bola
# Favio 09/06/2019
if [ $# != 1 ]
then
	echo Tenés que mandarle un solo archivo para compilar.
	exit
else
	file=$1
	start=$seconds
	gcc -O3 -Wall $file.c -o $file.e -lm
	./$file.e
	duracion=$((seconds-start))
	echo $duracion
fi
echo Realice la compilación y corrida

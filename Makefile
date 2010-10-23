# Compilação do projeto no LINUX, a partir da raiz do trabalho

all: compila

compila: 
	g++ -o raytrace cpp/camara.cpp cpp/cenario.cpp cpp/io.cpp cpp/main.cpp cpp/material.cpp cpp/objeto.cpp cpp/varios.cpp cpp/allegro.cpp -Wall `allegro-config --libs`

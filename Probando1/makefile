all: prueba

prueba : main.o Objeto.o Pivote.o Bloque.o Nodo.o Saco.o
	g++ main.o Objeto.o Pivote.o Bloque.o Nodo.o Saco.o

main.o: main.cpp
	g++ -c main.cpp

Objeto.o: Objeto.cpp
	g++ -c Objeto.cpp

Pivote.o: Pivote.cpp
	g++ -c Pivote.cpp

Bloque.o: Bloque.cpp
	g++ -c Bloque.cpp

Nodo.o: Nodo.cpp
	g++ -c Nodo.cpp

Saco.o: Saco.cpp
	g++ -c Saco.cpp

clean:
	rm -rf *o prueba


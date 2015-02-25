CPPFLAGS=-std=c++0x

all: prueba

prueba : main.o Objeto.o Pivote.o Bloque.o Nodo.o Saco.o Consultor.o
	g++ $(CPPFLAGS) main.o Objeto.o Pivote.o Bloque.o Nodo.o Saco.o Consultor.o -o prueba

main.o: main.cpp
	g++ $(CPPFLAGS) -c main.cpp

Objeto.o: Objeto.cpp
	g++ $(CPPFLAGS) -c Objeto.cpp

Pivote.o: Pivote.cpp
	g++ $(CPPFLAGS) -c Pivote.cpp

Bloque.o: Bloque.cpp
	g++ $(CPPFLAGS) -c Bloque.cpp

Nodo.o: Nodo.cpp
	g++ $(CPPFLAGS) -c Nodo.cpp

Saco.o: Saco.cpp
	g++ $(CPPFLAGS) -c Saco.cpp

Consultor.o: Consultor.cpp
	g++ $(CPPFLAGS) -c Consultor.cpp

clean:
	rm -rf *o prueba


OBJ = Main.o Constants.o AssetManager.o Gui.o GameBuild.o GameLoop.o PathFinder.o
CXXFLAGS =-Wall -std=c++11

# la var OBJ dopo subito dopo etichetta serve per creare prima il file oggetto dal c++
JappDefence: $(OBJ)
	g++  -o JapanDefence $(OBJ) -lsfml-graphics -lsfml-window -lsfml-system

-include dependencies

.PHONY: JappDefence depend clean cleanall

#crea la lista delle dipendenze (installando SFML da apt-get e' pari a zero)
depend:
	g++ -MM *.cc > dependencies
clean:
	rm -f *.o
cleanAll:
	rm -f Main *.o *~

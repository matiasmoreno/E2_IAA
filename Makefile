flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = realInstance firstInstance lastInstance firstSeed lastSeed randLength nResets nIterations T0 alpha addP swapP
#Primera instancia de prueba
#PARAMS = 0 1 2 0 1 5 3000 5000 10000 0.995 0.2 0.6
#Todas las instancias reales
PARAMS = 1 1 2 0 5 5 80 400000 200 0.9999 0.25 0.6

debug=0

exe:$(prog)
	./$(prog) $(PARAMS)

$(prog): MCPSB_SA.cpp
	$(comp) $(prog).cpp -o $(prog)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*
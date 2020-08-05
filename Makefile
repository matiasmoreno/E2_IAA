flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

PARAMS = 0 1 2 0 5 5 1000 2000 10000 0.995 0.2

debug=0

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(PARAMS)

$(prog): MCPSB_SA.cpp
	$(comp) $(prog).cpp -o $(prog)



clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*
flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = SeednResets nIterations T0 alpha addP swapP path
PARAMS = 1 10 500000 80 0.99999 0.1 0.2 0.3 MCPSB/Real_Instances/instancia1.mcsb

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
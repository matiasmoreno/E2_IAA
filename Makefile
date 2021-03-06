flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = SeednResets nIterations T0 alpha addP swapP path
#3858 10 10000 226 1.0000 0 0.7100
PARAMS = 3 30 16000000 80 0.99999975 0.5 0.0 0.5 MCPSB/Real_Instances/instancia1.mcsb
#PARAMS = 1 10 500000 80 0.99999 0.33 0.33 MCPSB/Real_Instances/instancia1.mcsb

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
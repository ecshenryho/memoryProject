target: 	simulation


simulation:		simulation.o
		g++ -m64 -std=c++11 -Wall -o simulation simulation.o

simulation.o: 	simulation.cpp
		g++ -c -m64 -std=c++11 -Wall -o simulation.o simulation.cpp

clean: 
		rm -rf *.O simulation


a.out: *.o
	g++ -o a.out *.o -std=c++26
	rm -f *.o

%.o: %.cc %.h
	g++ -std=c++26 -c $<

clean:
	rm -f *.o a.out

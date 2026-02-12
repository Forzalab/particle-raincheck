CCFLAGS = -I/libs/include/ -Wall -Wextra -Wfatal-errors -fcompare-debug-second -pedantic -pedantic-errors -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -std=c++26 -g -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address -std=c++26
#
a.out: *.o
	g++ $(CCFLAGS) $^ -o a.out -std=c++26
#	rm -f *.o

%.o: %.cc %.h
	g++ $(CCFLAGS) $^ -c $<

clean:
	rm -f *.o a.out


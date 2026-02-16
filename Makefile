CCFLAGS = -Ilibs/include -Wall -Wfatal-errors -pedantic -pedantic-errors -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -g -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address -std=c++26
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)

a.out: $(OBJS)
	@g++ $(CCFLAGS) $^ -o $@
	@echo Done linking
	@echo -e '\t$(OBJS)'
	@echo to 'a.out'!

%.o: %.cc /public/colors.h 
	@g++ $(CCFLAGS) -MMD -MP -c $< -o $@
	@echo Done compiling '$<'!

-include $(DEPS)

clean:
	rm -f *.o *.d a.out

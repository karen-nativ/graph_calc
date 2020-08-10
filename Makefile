CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS=Exceptions.o gcalc.o gpy.o Graph.o
EXEC=gcalc
DEBUG_FLAG=-DNDEBUG
COMP_FLAG=-std=c++11 -Wall -Werror --pedantic-errors -fPIC


$(EXEC): $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@

Exceptions.o: Exceptions.cpp Exceptions.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

gpy.o: gpy.cpp gpy.h Graph.h gcalc.h Exceptions.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp
	
Graph.o: Graph.cpp Graph.h Exceptions.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

gcalc.o: gcalc.cpp Exceptions.h gcalc.h Graph.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

libgraph.a: $(OBJS)
	ar -rs $@ $^

tar: 
	zip gcalc design.pdf Makefile Exceptions.cpp Exceptions.h gcalc.cpp gcalc.h gpy.cpp gpy.h Graph.cpp Graph.h test_in.txt test_out.txt file with space.gc file.with.dot.gc badfile.gc goodfile.gc graph.i

clean:
	rm -f $(OBJS) $(EXEC)
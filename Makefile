CXX = gcc

all: spell

debug: CXXFLAGS += -g
debug: clean
debug: spell

spell: spell_checker.o spell.o *.h
	$(CXX) $(CXXFLAGS) spell_checker.o spell.o -o spell

spell_checker.o: spell_checker.c 
	$(CXX) $(CXXFLAGS) -c spell_checker.c  
	
spell.o: spell.c 
	$(CXX) $(CXXFLAGS) -c spell.c  

clean:
	rm -f *.o *~ spell a.out

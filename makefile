CC=g++
CPPFLAGS=-Wall -Wextra
LIB_SRC := $(wildcard alglib/*.cpp)
LIB_OBJ := $(patsubst alglib/%.cpp,obj/alglib/%.o,$(LIB_SRC))


most_isolated: objdir.temp obj/most_isolated.o $(LIB_OBJ)
	rm objdir.temp
	g++ $(CPPFLAGS) -o $@ obj/most_isolated.o $(LIB_OBJ)

objdir.temp:
	mkdir -p obj/alglib
	touch objdir.temp

obj/%.o: %.cpp 
	g++ $(CPPFLAGS) -c -o $@ $<




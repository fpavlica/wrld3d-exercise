CC=g++ -std=c++17
CPPFLAGS=-Wall -Wextra
# next two lines credit to https://stackoverflow.com/a/2908351
LIB_SRC := $(wildcard alglib/*.cpp)
LIB_OBJ := $(patsubst alglib/%.cpp,obj/alglib/%.o,$(LIB_SRC))

most_isolated: objdir.temp obj/most_isolated.o $(LIB_OBJ)
	rm objdir.temp
	$(CC) $(CPPFLAGS) -o $@ obj/most_isolated.o $(LIB_OBJ)

# this one is to make sure the directories exist idk how else to do it
objdir.temp:
	mkdir -p obj/alglib
	touch objdir.temp

obj/%.o: %.cpp 
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm -f objdir.temp
	rm -rf obj




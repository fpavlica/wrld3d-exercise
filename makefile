CC=g++ -std=c++17
CPPFLAGS=-Wall -Wextra
# next two lines credit to https://stackoverflow.com/a/2908351
LIB_SRC := $(wildcard alglib/*.cpp)
LIB_OBJ := $(patsubst alglib/%.cpp,obj/alglib/%.o,$(LIB_SRC))

most_isolated: obj/alglib obj/most_isolated.o $(LIB_OBJ)
	$(CC) $(CPPFLAGS) -o most_isolated obj/most_isolated.o $(LIB_OBJ)

# to make sure the directories exist first
obj/alglib:
	mkdir -p obj/alglib

obj/%.o: %.cpp 
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm -rf obj




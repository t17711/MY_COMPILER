SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
HEADERS := $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/*.hpp)

OBJDIR  := obj
OBJECTS := $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
TARGET  := main

COMMON  := -O2 -Wall -Wextra -DNDEBUG 


CFLAGS  := $(CFLAGS) $(COMMON) 
CC      := g++ -std=c++14 
LD      := $(CC)

# create build directory
$(shell mkdir -p $(OBJDIR))

all : $(TARGET) 
.PHONY : all

# {{{ for debugging
debug : CFLAGS += -g -DDEBUG=1
debug : $(TARGET)
.PHONY : debug
# }}}

# compile main to objects
main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# compile all srcs
$(OBJECTS) : $(OBJDIR)/%.o :$(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# call for objects and main to compile and use those to create main.exe
$(TARGET) : $(OBJECTS) main.o
	$(LD) -o $@ $^


.PHONY : clean
clean :
	rm -f $(OBJDIR)/* *.exe
	rm -f $(TARGET)
	rm -f *stackdump	
	rmdir $(OBJDIR)



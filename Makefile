CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := bin/main
TEST := bin/tester

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -g -Wall -O6
LIB := -pthread -lprofiler
INC :=

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

run: $(TARGET)
	@echo " Running $(TARGET):";
	$(TARGET)

profile: $(TARGET)
	@echo " Profiling $(TARGET):";
	LD_PRELOAD=/usr/lib/libprofiler.so CPUPROFILE=/tmp/prof.out $(TARGET) $(depth)
	google-pprof --gv $(TARGET) /tmp/prof.out

# Tests
test: test/tester.cpp build/Board.o
	$(CC) $(CFLAGS) build/Board.o test/tester.cpp $(INC) $(LIB) -o bin/tester

runtest: test
	@echo " Running tests:"
	bin/tester $(flags)

.PHONY: clean

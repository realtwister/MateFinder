CC := g++
SRCDIR := src
TESTDIR := test
BUILDDIR := build
BINDIR := bin
TARGET := MateFinder
TEST := tester

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTSOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT) -not -name main.cpp) test/tester.cpp
DOCSOURCES := $(shell find $(SRCDIR) -type f)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o)))
CFLAGS := -std=c++17 -g -Wall -O12
LIB := 
INC :=

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BINDIR)/$(TEST): $(TESTOBJECTS)
	@mkdir -p $(BINDIR)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(BINDIR)/$(TEST) $(LIB)"; $(CC) $^ -o $(BINDIR)/$(TEST) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INT) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR) $(TARGET) $(SRCDIR)/*.gch $(TESTDIR)/*.gch"; $(RM) -r $(BUILDDIR) $(BINDIR) $(TARGET) $(SRCDIR)/*.gch $(TESTDIR)/*.gch

run: $(TARGET)
	@echo " Running $(TARGET):";
	@./$(TARGET) $(flags)

test: $(BINDIR)/$(TEST)

runtest: $(BINDIR)/$(TEST)
	@echo " Running $(BINDIR)/$(TEST):"
	@$(BINDIR)/$(TEST) $(flags)

doc: $(DOCSOURCES)
	@doxygen
	@make -C doc/latex

.PHONY: clean

NAME = laplace
CXX = g++
CXXFLAGS = -fopenmp -fstrict-overflow -Werror -Wshadow -Wstrict-overflow=5 -pedantic
LDFLAGS = -lm -I $(CURDIR) 

#error if student directory is not set
ifndef SDIR
SDIR = student
endif
CXXFLAGS += -I $(SDIR)

# -Wall
#Compiler flags
#if mode variable is empty, setting release build mode
ifeq ($(mode),debug)
	CXXFLAGS += -O0 -g
else
	mode = release
	CXXFLAGS += -O0
endif

VPATH = $(SDIR)
SRC = $(wildcard *.cpp) $(wildcard */*.cpp)

OBJ_SEQ =  $(SDIR)/main_seq.o
OBJ_PAR =  $(SDIR)/main_par.o 
OBJ_UNIT = $(SDIR)/unit_test.o

# The dependency file names.
DEPS := $(OBJ_SEQ:.o=.d)

all: sequential parallel

sequential:  $(SDIR)/$(NAME)_seq

parallel:  $(SDIR)/$(NAME)_par

unit_test: $(SDIR)/unit_test

$(SDIR)/$(NAME)_seq : $(OBJ_SEQ)
	$(CXX) -o $@ $(OBJ_SEQ) $(CXXFLAGS) $(LDFLAGS)

$(SDIR)/$(NAME)_par : $(OBJ_PAR)
	$(CXX) -o $@ $(OBJ_PAR) $(CXXFLAGS) -DPARALLEL $(LDFLAGS)
	
$(SDIR)/unit_test: $(OBJ_UNIT)
	$(CXX) -o $@ $(OBJ_UNIT) $(CXXFLAGS) -DPARALLEL $(LDFLAGS)

$(SDIR)/main_seq.o: main.cpp
	$(CXX) -c -MMD -MP $< -o $@ $(CXXFLAGS) $(LDFLAGS)

$(SDIR)/main_par.o: main.cpp $(SDIR)/$(NAME)_par.h
	$(CXX) -c -MMD -MP -DPARALLEL $< -o $@ $(CXXFLAGS) $(LDFLAGS)

$(SDIR)/unit_test.o: unit_test.cpp $(SDIR)/$(NAME)_par.h
	$(CXX) -c -MMD -MP -DPARALLEL $< -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f  $(SDIR)/$(NAME)_seq  $(SDIR)/$(NAME)_par $(SDIR)/unit_test  $(SDIR)/*.o  $(SDIR)/*.d

-include $(DEPS)

.PHONY : all sequential parallel unit_test clean

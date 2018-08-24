NAME = reverse
CC = mpicc
CFLAGS = -std=gnu99 -fstrict-overflow -Werror -Wall -Wshadow -pedantic
LDFLAGS = -lm -lrt -I $(CURDIR)

#error if student directory is not set
ifndef SDIR
SDIR = student
endif

#Compiler flags
#if mode variable is empty, setting release build mode
ifeq ($(mode),debug)
	CFLAGS += -ggdb
else
	mode = release
	CFLAGS += -O2
endif

VPATH = $(SDIR)
SRC = $(wildcard *.c) $(wildcard */*.c)

OBJ_SEQ = $(SDIR)/main.o $(SDIR)/$(NAME)_seq.o $(SDIR)/helper.o
OBJ_PAR = $(SDIR)/main.o $(SDIR)/$(NAME)_par.o $(SDIR)/helper.o
OBJ_UNIT = $(SDIR)/unit_test.o $(SDIR)/$(NAME)_par.o $(SDIR)/helper.o

# The dependency file names.
DEPS := $(OBJ_SEQ:.o=.d)

all: sequential parallel unit_test

sequential: $(SDIR)/$(NAME)_seq

parallel: $(SDIR)/$(NAME)_par

unit_test: $(SDIR)/unit_test

$(SDIR)/performance_test: $(OBJ_PERF)
	$(CC) -o $@ $(OBJ_PERF) $(CFLAGS) $(LDFLAGS)

$(SDIR)/$(NAME)_seq : $(OBJ_SEQ)
	$(CC) -o $@ $(OBJ_SEQ) $(CFLAGS) $(LDFLAGS)

$(SDIR)/$(NAME)_par : $(OBJ_PAR)
	$(CC) -o $@ $(OBJ_PAR) $(CFLAGS) $(LDFLAGS)

$(SDIR)/unit_test: $(OBJ_UNIT)
	$(CC) -o $@ $(OBJ_UNIT) $(CFLAGS) $(LDFLAGS) 

$(SDIR)/%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(SDIR)/%.o: %.c
	$(CC) -c -MMD -MP $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(SDIR)/$(NAME)_par $(SDIR)/$(NAME)_seq $(SDIR)/unit_test $(SDIR)/*.o $(SDIR)/*.d
	
-include $(DEPS)

.PHONY : all sequential parallel unit_test clean performance_test




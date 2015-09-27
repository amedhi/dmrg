# Makefile for the DMRG project
#-------------------------------------------------------------
CC = /opt/local/bin/g++

#FPPFLAGS = -DHAVE_MPI
OPTFLAGS = -O3 -Wall
DBGFLAGS = -g -O0 -DDEBUG_MODE
#MKLPATH = $(PREFIX)/mkl/lib
#MKLINCLUDE = $(PREFIX)/mkl/include/intel64/lp64
#CCINCLUDE = $(PREFIX)/include
INCLUDE  = -I/usr/local/include #-I$(MKLINCLUDE) 
CCFLAGS = -std=c++11 $(OPTFLAGS) $(INCLUDE) # $(FPPFLAGS) 
CCGFLAGS = $(DBGFLAGS) $(INCLUDE) #$(FPPFLAGS)  
LDFLAGS = $(OPTFLAGS) -L/usr/local/lib  #-L$(MKLPATH) -L$(LIBGALAHAD)   
LDGFLAGS = $(DBGFLAGS) -L/usr/local/lib #-L$(MKLPATH) -L$(LIBGALAHAD)   
LIBS = -lboost_system -lboost_filesystem

#-------------------------------------------------------------
TAGT = a.out
GTAGT = dbg.out
SRCS = cmdargs.cc 
SRCS+= inputparams.cc 
SRCS+= taskparams.cc 
SRCS+= scheduler.cc
SRCS+= main.cc

HDRS = optionparser.h cmdargs.h inputparams.h scheduler.h

AUX = Makefile input.parm changelog
#-------------------------------------------------------------
# compilation and linking
VPATH = ./:./scheduler

OBJS = $(patsubst %.cc,%.o, $(SRCS))
GOBJS= $(patsubst %.cc,debug_objs/%.o, $(SRCS))

all: $(TAGT)
$(TAGT) : $(OBJS)
	$(CC) -o $(TAGT) $(OBJS) $(LDFLAGS) $(LIBS)

$(GTAGT) : $(GOBJS)
	$(CC) -o $(GTAGT) $(GOBJS) $(LDFLAGS) $(LIBS)

%.o: %.cc
	$(CC) -c $(CCFLAGS) -o $@ $<

debug_objs/%.o: %.cc mkdebugdir
	$(CC) -c $(CCGFLAGS) -o $@ $<

mkdebugdir:
	mkdir -p debug_objs
	
#-------------------------------------------------------
# Detailed dependencies
DEPHDRS = optionparser.h cmdargs.h
cmdargs.o: $(DEPHDRS)
debug_objs/cmdargs.o: $(DEPHDRS)
DEPHDRS += inputparams.h
inputparams.o: inputparams.h
debug_objs/inputparams.o: $(DEPHDRS)
taskparams.o: inputparams.h
debug_objs/taskparams.o: $(DEPHDRS)
DEPHDRS += scheduler.h 
scheduler.o: $(DEPHDRS) 
debug_objs/scheduler.o: $(DEPHDRS)

main.o: $(DEPHDRS)

#-------------------------------------------------------
.PHONY: clean
clean:
	rm -f *.o debug_objs/*.o 
	
.PHONY: xclean
xclean:
	rm -f jobinfo* *.data*


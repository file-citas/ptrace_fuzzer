CC            = gcc
CXX           = g++
CFLAGS        = -Wall -W
AM_CXXFLAGS   = -std=c++0x -Wall -W 
AM_LDFLAGS    = -Wl,-O1,--sort-common,--as-needed,-z,relro -Wl,-O1

bin_PROGRAMS = ptrfuzz
ptrfuzz_SOURCES = main.cpp main.h tag.cpp access.cpp val.cpp fuzzer.cpp fuzzer.h tracer.cpp target.cpp state.cpp elf.cpp break.cpp branch.cpp cond.cpp vrange.cpp inc.cpp
		    
LDADD =  -L/usr/local/lib -L/usr/lib -lelf -ldistorm3 -lm -lutil -lpthread
INCLUDES = -I$(top_builddir)/lib

OBJEXT=o

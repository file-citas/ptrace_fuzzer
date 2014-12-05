####### Compiler, tools and options

CC            = gcc
CXX           = g++
CFLAGS        = -pipe -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -Wall -W -D_REENTRANT
CXXFLAGS      = -std=c++0x -Wall -W 
#CXXFLAGS      = -pipe -std=c++0x -g -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -Wall -W -D_REENTRANT 
INCPATH       = -I. 
#INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1,--sort-common,--as-needed,-z,relro -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/local/lib -L/usr/lib -lelf -ldistorm3 -lm -lutil -lpthread 
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		val.cpp \
		tracer.cpp \
		target.cpp \
		tag.cpp \
		elf.cpp \
		cond.cpp \
		break.cpp \
		branch.cpp \
		access.cpp \
		fuzzer.cpp \
		state.cpp \
		vrange.cpp \
		inc.cpp \
		#moc_mainwindow.cpp \
		#mainwindow.cpp \
		#moc_tabmodel.cpp \
		#tabmodel.cpp \
		#moc_codemodel.cpp \
		#codemodel.cpp \
		#moc_vrangemodel.cpp \
		#vrangemodel.cpp \
		#moc_rdel.cpp \
		#rdel.cpp \
		#moc_outputthread.cpp \
		#outputthread.cpp 

OBJECTS       = main.o \
		val.o \
		tracer.o \
		target.o \
		tag.o \
		elf.o \
		cond.o \
		break.o \
		branch.o \
		access.o \
		fuzzer.o \
		state.o \
		vrange.o \
		inc.o \
		#moc_mainwindow.o \
		#mainwindow.o \
		#moc_tabmodel.o \
		#tabmodel.o \
		#moc_codemodel.o \
		#codemodel.o \
		#moc_vrangemodel.o \
		#vrangemodel.o \
		#moc_rdel.o \
		#rdel.o \
		#moc_outputthread.o \
		#outputthread.o 

DESTDIR       = 
TARGET        = bfuzz

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean: 
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 


check: first

####### Compile

main.o: main.cpp main.h \
		tag.h \
		types.h \
		access.h \
		val.h \
		trace.h \
		fuzzer.h \
		tracer.h \
		branch.h \
		cond.h \
		vrange.h \
		inc.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp


val.o: val.cpp val.h \
		types.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o val.o val.cpp

tracer.o: tracer.cpp tracer.h \
		types.h \
		tag.h \
		access.h \
		val.h \
		trace.h \
		branch.h \
		cond.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tracer.o tracer.cpp

target.o: target.cpp types.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h \
		state.h
	$(CXX) -c $(CXXFLAGS) -Wno-missing-field-initializers $(INCPATH) -o target.o target.cpp

tag.o: tag.cpp tag.h \
		types.h \
		access.h \
		val.h \
		trace.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tag.o tag.cpp

elf.o: elf.cpp myelf.h \
		types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o elf.o elf.cpp

cond.o: cond.cpp cond.h \
		val.h \
		types.h \
		tag.h \
		access.h \
		trace.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o cond.o cond.cpp

break.o: break.cpp break.h \
		types.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o break.o break.cpp

branch.o: branch.cpp branch.h \
		types.h \
		cond.h \
		val.h \
		tag.h \
		access.h \
		trace.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o branch.o branch.cpp

access.o: access.cpp access.h \
		types.h \
		val.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o access.o access.cpp

fuzzer.o: fuzzer.cpp fuzzer.h \
		types.h \
		tracer.h \
		tag.h \
		access.h \
		val.h \
		trace.h \
		branch.h \
		cond.h \
		vrange.h \
		inc.h \
		state.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fuzzer.o fuzzer.cpp

state.o: state.cpp state.h \
		types.h \
		target.h \
		myelf.h \
		break.h \
		distorm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o state.o state.cpp

inc.o: inc.cpp inc.h \
		val.h \
		types.h
	$(CXX) -c $(CXXFLAGS) -Wno-unused-parameter $(INCPATH) -o inc.o inc.cpp


####### Install

install:   FORCE

uninstall:   FORCE

FORCE:


## Pd library template version 1.0.14
# For instructions on how to use this template, see:
#  http://puredata.info/docs/developer/MakefileTemplate
LIBRARY_NAME = snn

# add your .cpp source files, one object per file, to the SOURCES
# variable, help files will be included automatically, and for GUI
# objects, the matching .tcl file too
SOURCES = easefloat.cpp  Neuron.cpp  Neurons_network.cpp  Synapse.cpp  utils.cpp snn.cpp 

# list all pd objects (i.e. myobject.pd) files here, and their helpfiles will
# be included automatically
PDOBJECTS = 

# example patches and related files, in the 'examples' subfolder
EXAMPLES =

# manuals and related files, in the 'manual' subfolder
MANUAL =

# if you want to include any other files in the source and binary tarballs,
# list them here.  This can be anything from header files, test patches,
# documentation, etc.  README.txt and LICENSE.txt are required and therefore
# automatically included
EXTRA_DIST = 

# unit tests and related files here, in the 'unittests' subfolder
UNITTESTS = 



#------------------------------------------------------------------------------#
#
# things you might need to edit if you are using other C libraries
#
#------------------------------------------------------------------------------#

ALL_CFLAGS = -I"$(PD_INCLUDE)"
ALL_LDFLAGS = -lm
SHARED_LDFLAGS =
ALL_LIBS = 


#------------------------------------------------------------------------------#
#
# you shouldn't need to edit anything below here, if we did it right :)
#
#------------------------------------------------------------------------------#

# these can be set from outside without (usually) breaking the build
CFLAGS = -Wall -W -g -std=c++11
LDFLAGS =
LIBS =

# get library version from meta file
LIBRARY_VERSION = $(shell sed -n 's|^\#X text [0-9][0-9]* [0-9][0-9]* VERSION \(.*\);|\1|p' $(LIBRARY_NAME)-meta.pd)

ALL_CFLAGS += -DPD -DVERSION='"$(LIBRARY_VERSION)"'

PD_INCLUDE = $(PD_PATH)/include/pd
# where to install the library, overridden below depending on platform
prefix = /usr/local
libdir = $(prefix)/lib
pkglibdir = $(libdir)/pd-externals
objectsdir = $(pkglibdir)

INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -p -m 644
INSTALL_DATA = $(INSTALL) -p -m 644
INSTALL_DIR     = $(INSTALL) -p -m 755 -d

ALLSOURCES := $(SOURCES) $(SOURCES_android) $(SOURCES_cygwin) $(SOURCES_macosx) \
           $(SOURCES_iphoneos) $(SOURCES_linux) $(SOURCES_windows)

DISTDIR=$(LIBRARY_NAME)-$(LIBRARY_VERSION)
ORIGDIR=pd-$(LIBRARY_NAME:~=)_$(LIBRARY_VERSION)


CPU := $(shell uname -m)
SOURCES += $(SOURCES_windows)
EXTENSION = dll
SHARED_EXTENSION = dll
OS = windows
PD_PATH = /mnt/c/Program Files (x86)/Pd
# $(shell cd "$$PROGRAMFILES/pd" && pwd)
# MinGW doesn't seem to include cc so force gcc
CC=x86_64-w64-mingw32-g++
OPT_CFLAGS = -O3 -funroll-loops -fomit-frame-pointer
ALL_CFLAGS += -mms-bitfields
ALL_LDFLAGS += -s -shared -Wl,--enable-auto-import
SHARED_LDFLAGS += -shared
ALL_LIBS += -L"$(PD_PATH)/src" -L"$(PD_PATH)/bin" -L"$(PD_PATH)/obj" \
-lpd -lwsock32 -lkernel32 -luser32 -lgdi32 -liberty $(LIBS_windows)
STRIP = strip --strip-unneeded -R .note -R .cppomment
DISTBINDIR=$(DISTDIR)-$(OS)


ALL_CFLAGS := $(ALL_CFLAGS) $(CFLAGS) $(OPT_CFLAGS)
ALL_LDFLAGS := $(LDFLAGS) $(ALL_LDFLAGS)
ALL_LIBS := $(LIBS) $(ALL_LIBS)


SHARED_SOURCE ?= $(wildcard lib$(LIBRARY_NAME).cpp)
SHARED_HEADER ?= $(shell test ! -e $(LIBRARY_NAME).h || echo $(LIBRARY_NAME).h)
SHARED_LIB ?= $(SHARED_SOURCE:.cpp=.$(SHARED_EXTENSION))
SHARED_TCL_LIB = $(wildcard lib$(LIBRARY_NAME).tcl)

.PHONY = $(LIBRARY_NAME)

# all: $(SOURCES:.cpp=.$(EXTENSION)) $(SHARED_LIB)
all: $(SOURCES:.cpp=.o)

# all: $(SHARED_LIB)

%.o: %.cpp
	$(CC) $(ALL_CFLAGS) -o "$*.o" -c "$*.cpp"

%.$(EXTENSION): %.o $(SHARED_LIB)
	$(CC) $(ALL_LDFLAGS) -o "$*.$(EXTENSION)" "$*.o"  $(ALL_LIBS) $(SHARED_LIB)
	chmod a-x "$*.$(EXTENSION)"

# this links everything into a single binary file
# $(LIBRARY_NAME): $(SOURCES:.cpp=.o) $(LIBRARY_NAME).o lib$(LIBRARY_NAME).o
# 	$(CC) $(ALL_LDFLAGS) -o $(LIBRARY_NAME).$(EXTENSION) $(SOURCES:.cpp=.o) \
# 	$(LIBRARY_NAME).o lib$(LIBRARY_NAME).o $(ALL_LIBS)
# 	chmod a-x $(LIBRARY_NAME).$(EXTENSION)

$(SHARED_LIB): $(SHARED_SOURCE:.cpp=.o)
	$(CC) $(SHARED_LDFLAGS) -o $(SHARED_LIB) $(SHARED_SOURCE:.cpp=.o) $(ALL_LIBS)
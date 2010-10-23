#  GenMake, a general Makefile for general use
#  Copyright (C) 2010 Andre de Abrantes D. P. e Souza
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

# THIS PROJECT'S NAME

PROJNAME    := raytrace

# THIS PROJECT'S DIRECTORY

PROJPWD     := $(shell pwd)

# PROJECT'S DIRECTORIES

PROJBINDIR  := $(PROJPWD)/bin
PROJINCDIR  := $(PROJPWD)/include
PROJLIBDIR  := $(PROJPWD)/lib
PROJOBJDIR  := $(PROJPWD)/obj
PROJSRCDIR  := $(PROJPWD)/src
PROJTOOLSDIR:= $(PROJPWD)/tools

PROJDIRS    := $(PROJBINDIR) $(PROJINCDIR) $(PROJLIBDIR) $(PROJOBJDIR) $(PROJSRCDIR)

# FLAGS TO BE GIVEN TO THE C++ COMPILER

PROJLDFLAGS := -Wall -O2 -ansi -g -ggdb
#-DNDEBUG -D_LINUX -D_REENTRANT
#-Wno-write-strings
PROJLDLIBS  := -lm `allegro-config --libs`

# -lvquality -lpq
PROJINC     :=
PROJTOOLS   := 

# PROJECT'S SOURCES, HEADERS AND OBJECTS

vpath %.h   $(PROJINCDIR)
vpath %.cpp $(PROJSRCDIR)
vpath %.o   $(PROJOBJDIR)

PROJSRCS    := $(wildcard $(PROJSRCDIR)/*.cpp)
PROJBINSRCS := $(wildcard $(PROJSRCDIR)/bin/*.cpp)
PROJINCS    := $(wildcard $(PROJINCDIR)/$(PROJNAME)/*.h)
PROJOBJS    := $(foreach obj,$(PROJSRCS),$(patsubst $(PROJSRCDIR)/%.cpp,$(PROJOBJDIR)/%.o,$(obj)))
PROJLIB     := $(PROJLIBDIR)/lib$(PROJNAME).a

PROJBINS    := $(foreach bin,$(PROJBINSRCS),$(patsubst $(PROJSRCDIR)/bin/%.cpp,$(PROJBINDIR)/%,$(bin)))

# COLORS

RED         := \033[1;31m
BLUE        := \033[1;34m
YELLOW      := \033[1;33m
COLOREND    := \033[m

##############################################################################

# We depend on the directories and on the final executables.

all: $(PROJDIRS) $(PROJTOOLS) $(PROJBINS)

# Here, we build the executables. They depend on the project's library (.a)
# and on the cpp files inside the src/bin folder, which will produce the so
# called executables.

$(PROJBINDIR)/% : $(PROJSRCDIR)/bin/%.cpp $(PROJLIB) $(PROJINCS)
	@echo -n "compilando $(BLUE)"
	@echo $< | awk -F/ '{print $$NF}' -
	@echo -n "$(COLOREND)"
	@$(CXX) $< $(PROJLIB) -o $@ -I$(PROJINCDIR) $(PROJINC) $(PROJLDFLAGS) $(PROJLDLIBS)

# The big library created by this project is created here. It depends on the
# existance of all the objects made of the source files, except the
# executables. The library is nothing but a package made of the objects.

$(PROJLIB): $(PROJOBJS)
	@echo "criando $(YELLOW)lib$(PROJNAME).a$(COLOREND)"
	@ar rcs $(PROJLIB) $(PROJOBJDIR)/*.o

# The objects (.o) of the minor source files are built here from the source
# files (.cpp) located inside the src folder.

$(PROJOBJDIR)/%.o : $(PROJSRCDIR)/%.cpp $(PROJINCS)
	@echo -n "compilando $(RED)"
	@echo $< | awk -F/ '{print $$NF}' -
	@echo -n "$(COLOREND)"
	@$(CXX) -c $< -o $@ -I$(PROJINCDIR) $(PROJLDFLAGS) $(PROJINC)

# If needed, any non-existing folder is created here.

$(PROJDIRS):
	mkdir -p $@

# Compile the tools (external libs, for example)

$(PROJTOOLS):
	$(MAKE) -C $(PROJTOOLSDIR)/$@

# To clean the project, just delete the library (.a), the objects (*.o) and
# the executables.

clean:
	rm -f $(PROJLIB)
	rm -f $(PROJOBJDIR)/*.o
	rm -f $(PROJBINDIR)/*


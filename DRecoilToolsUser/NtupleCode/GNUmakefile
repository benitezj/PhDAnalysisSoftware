 #############################################################################
 # Project: DReco                                                           #
 # Package: DRecoToolsUser                                                  #
 #    File: $Id: GNUmakefile.standalone					     #
 # Authors:                                                                  #
 #   Jose Benitez, Stanford, benitezj@slac.stanford.edu                      #
 #                                                                           #
 #   Standalone Makefile for                                                 #
 #   ----------------------------------                                      #
 #                                                                           #
 #   Instructions                                                            #
 #       - Review 'external configuration' section below                     #
 #         to match systems compilers setup                                  #
 #                                                                           #
 #       - Make sure the ROOTSYS environment variable is set and points      #
 #         to your ROOT release                                              #
 #                                                                           #
 #       - run 'gmake -f GNUMakefile.standalone <target>'                    #
 #         from the NtupleCode/ directory                                    #
 #                                                                           #
 #         >> For Linux/Unix choose 'shlib', for CygWin choose 'winlib' <<   #
 #                                                                           #
 #   Build targets                                                           #
 #     lib   - make libDRecoilToolsUser.a                                          #
 #     shlib - make libDRecoilToolsUser.so                                         #
 #    winlib - make libDRecoilToolsUser.dll (for CygWin)                           #
 #     clean - delete all intermediate and final build objects               #
 #                                                                           #
 ############################################################################# 


# --- External configuration ----------------------------------
CC       = g++
CCFLAGS  = -g -Wall -W -Woverloaded-virtual -fPIC
MFLAGS   = -MM
INCLUDES =
#WORKDIR  = ../../tmp/Linux24SL3_i386_gcc323
WORKDIR  = ../../tmp/Linux26SL5_i386_gcc412
LIBDIR   = $(WORKDIR)
# -------------------------------------------------------------


# Internal configuration
PACKAGE=DRecoilToolsUser
LD_LIBRARY_PATH:=$(ROOTSYS)/lib:$(LD_LIBRARY_PATH)
OBJDIR=$(WORKDIR)/objects/
INCDIR=$(WORKDIR)/$(PACKAGE)/NtupleCode
VPATH=$(INCDIR) $(OBJDIR)
#LDFLAGS  = -shared
INCLUDES += -I.. -I$(WORKDIR)/ -I$(ROOTSYS)/include
ROOTSYS  ?= ERROR_RootSysIsNotDefined
RDLLIST   = $(filter-out $(PACKAGE)_LinkDef.rdl,$(wildcard *.rdl))
SKIPLIST  = 
CINTFILE  = $(PACKAGE)Cint.cc
CINTOBJ   = $(PACKAGE)Cint.o
LIBFILE   = $(LIBDIR)/lib$(PACKAGE).a
SHLIBFILE = $(LIBDIR)/lib$(PACKAGE).so
DLLIBFILE = $(LIBDIR)/lib$(PACKAGE).dll
UNAME = $(shell uname)

default: shlib

# List of all includes files to copy from rdl
HHLIST=$(patsubst %.rdl,%.hh,$(RDLLIST))

# List of all source files to build
CCLIST=$(filter-out $(SKIPLIST),$(wildcard *.cc))

# List of all object files to build
OLIST=$(patsubst %.cc,%.o,$(CCLIST))

# List of all dependency file to make
DLIST=$(patsubst %.rdl,%.d,$(RDLLIST))

# Implicit rule making all dependency Makefiles included at the end of this makefile
$(INCDIR)/%.d: %.cc $(HHLIST)
	@echo "Making $@"
	@set -e; $(CC) $(MFLAGS) $(CPPFLAGS) $(INCLUDES) $< \
	          | sed 's/\($(notdir $*)\)\.o[ :]*/\1.o $(notdir $@) : /g' > $@; \
	        [ -s $@ ] || rm -f $@

# Implicit rule copying all RDL to INCDIR/HH
%.hh: %.rdl 
	@mkdir -p $(INCDIR)
	@cp $< $(INCDIR)/$@

# Implicit rule to compile all classes
%.o : %.cc 
	@echo "Compiling $<"
	@mkdir -p $(OBJDIR)
	@$(CC) $(CCFLAGS) -c $< -o $(OBJDIR)/$(notdir $@) $(INCLUDES) 


# Rule to make ROOTCINT output file
$(OBJDIR)/$(CINTOBJ): $(RDLLIST) $(PACKAGE)_LinkDef.hh $(INCDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(OBJDIR)
#	@cp -f *.hh $(INCDIR)
	@echo "Running rootcint"
	@ROOTSYS=$(ROOTSYS)
	@export ROOTSYS
	cd $(INCDIR); $(ROOTSYS)/bin/rootcint -f $(CINTFILE) -c $(INCLUDES)  $(HHLIST) $(ROOTSYS)/include/TRootIOCtor.h $(PACKAGE)_LinkDef.hh
	@echo "Compiling $(CINTFILE)"
	@$(CC) $(CCFLAGS) -c $(INCDIR)/$(CINTFILE) -o $(OBJDIR)/$(CINTOBJ) $(INCLUDES) 

# Rule to combine objects into a library
$(LIBFILE): $(OLIST) $(INCDIR)/$(CINTFILE)) $(patsubst %.cc,%.o,$(OBJDIR)/$(CINTFILE))
	@echo "Making $(LIBFILE)"
	@rm -f $(LIBFILE)
	@ar q $(LIBFILE) $(addprefix $(OBJDIR)/,$(OLIST) $(patsubst %.cc,%.o,$(CINTFILE))) 
	@ranlib $(LIBFILE)

# Rule to combine objects into a unix shared library
$(SHLIBFILE): $(OLIST) $(patsubst %.cc,%.o,$(OBJDIR)/$(CINTFILE))
	@echo "Making $(SHLIBFILE)"
	@rm -f $(SHLIBFILE)
	@$(CC) $(addprefix $(OBJDIR)/,$(OLIST) $(patsubst %.cc,%.o,$(CINTFILE))) -shared -o $(SHLIBFILE)

# Rule to combine objects into a windows shared library
$(DLLIBFILE): $(OLIST) $(patsubst %.cc,%.o,$(OBJDIR)/$(CINTFILE))
	@echo "Making $(DLLIBFILE)"
	@rm -f $(DLLIBFILE)
	$(CC) -Wl,--export-all-symbols -Wl,--export-dynamic -Wl,--enable-auto-import -Wl,-Bdynamic -shared --enable-auto-image-base -Wl,-soname -o $(DLLIBFILE) -Wl,--whole-archive $(addprefix $(OBJDIR)/,$(OLIST) $(patsubst %.cc,%.o,$(CINTFILE))) -Wl,--no-whole-archive -L$(ROOTSYS)/lib -lCore -lCint -lHist -lGraf -lGraf3d -lTree -lRint -lPostscript -lMatrix -lMinuit -lPhysics -lHtml -lm

# Useful build targets
lib: $(LIBFILE) 
shlib: $(SHLIBFILE)
winlib: $(DLLIBFILE)
clean:
	rm -f $(OBJDIR)/*
	rm -f $(INCDIR)/*
	rm -f $(LIBFILE)
	rm -f $(SHLIBFILE)
	rm -f $(DLLIBFILE)

.PHONY : winlib shlib lib default clean

-include $(addprefix $(INCDIR)/,$(DLIST))

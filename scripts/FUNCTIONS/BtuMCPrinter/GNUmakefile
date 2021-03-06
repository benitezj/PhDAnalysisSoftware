
# --- External configuration ----------------------------------
CC       = g++
CCFLAGS  = -g -Wall -W -Woverloaded-virtual
MFLAGS   = -MM
INCLUDES =
WORKDIR  = tmp/
LIBDIR   = $(WORKDIR)
# -------------------------------------------------------------


# Internal configuration
PACKAGE=BtuMCPrinter
LD_LIBRARY_PATH:=$(ROOTSYS)/lib:$(LD_LIBRARY_PATH)
OBJDIR=$(WORKDIR)/objects/
INCDIR=$(WORKDIR)/$(PACKAGE)/
VPATH=$(INCDIR) $(OBJDIR)

INCLUDES += -I.. -I$(WORKDIR)/ -I$(ROOTSYS)/include 
ROOTSYS  ?= ERROR_RootSysIsNotDefined
RDLLIST   = $(filter-out $(PACKAGE)_LinkDef.rdl,$(wildcard *.rdl))
SKIPLIST  = 
CINTFILE  = $(PACKAGE)Cint.cc
CINTOBJ   = $(PACKAGE)Cint.o
LIBFILE   = $(LIBDIR)/lib$(PACKAGE).a
SHLIBFILE = $(LIBDIR)/lib$(PACKAGE).so

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
	@echo $(HHLIST)
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
	@echo "Running rootcint"
	@ROOTSYS=$(ROOTSYS)
	@export ROOTSYS
	@cd $(INCDIR) ; $(ROOTSYS)/bin/rootcint -f $(CINTFILE) -c $(INCLUDES) $(HHLIST) $(PACKAGE)_LinkDef.hh
	@echo "Compiling $(CINTFILE)"
	@$(CC) $(CCFLAGS) -c $(INCDIR)/$(CINTFILE) -o $(OBJDIR)/$(CINTOBJ) $(INCLUDES)

# Rule to combine objects into a library
$(LIBFILE): $(OLIST) $(INCDIR)/$(CINTFILE)) $(patsubst %.cc,%.o,$(OBJDIR)/$(CINTFILE))
	@echo "Making $(LIBFILE)"
	@rm -f $(LIBFILE)
	@ar q $(LIBFILE) $(addprefix $(OBJDIR)/,$(OLIST) $(patsubst %.cc,%.o,$(CINTFILE))) 
	@ranlib $(LIBFILE)

# Rule to combine objects into a shared library
$(SHLIBFILE): $(OLIST) $(patsubst %.cc,%.o,$(OBJDIR)/$(CINTFILE))
	@echo "Making $(SHLIBFILE)"
	@rm -f $(SHLIBFILE)
	@$(CC) $(addprefix $(OBJDIR)/,$(OLIST) $(patsubst %.cc,%.o,$(CINTFILE))) -shared -o $(SHLIBFILE)

# Useful build targets
lib: $(LIBFILE) 
shlib: $(SHLIBFILE)
clean:
	rm -f $(OBJDIR)/*
	rm -f $(INCDIR)/*
	rm -f $(LIBFILE)
	rm -f $(SHLIBFILE)

.PHONY : shlib lib default clean

-include $(addprefix $(INCDIR)/,$(DLIST))

# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile,v 1.3 2004/09/12 00:54:13 chcheng Exp $
#################################################################
#++ library products				[build it with 'lib']

LIBREMOVEFILES := 
LIBTMPLFILES :=
LIBDDLORDERED := 

#################################################################
#++ extra binary products	[not in production, build it with extrabin]

EXTRABINS :=

$(addprefix $(bindir),$(EXTRABINS)): $(bindir)% : %.o

#################################################################
#++ binary products				[build it with 'bin']

BINS := BtaTupleApp
BINCCFILES := AppUserBuild.cc

#++ Binary rules		 [in production, build it with 'bin']

$(bindir)BtaTupleApp: AppUserBuild.o

#++ shell script products.. 			[build it with 'bin']
#BINSCRIPTS := testscript

#################################################################
#++ regression test scripts			[build it with 'test']

#$(testdir)mytest.T : mytest.tcl mytesttemp2

#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk

# Please see SoftRelTools/HOWTO-GNUmakefile for documentation
# $Id: GNUmakefile,v 1.2 2008/12/06 02:22:59 aidanrc Exp $
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

BINS := DRecoilApp
BINCCFILES := AppUserBuild.cc

#++ Binary rules		 [in production, build it with 'bin']

$(bindir)DRecoilApp: AppUserBuild.o

#++ shell script products.. 			[build it with 'bin']
#BINSCRIPTS := testscript

#################################################################
#++ regression test scripts			[build it with 'test']

#$(testdir)mytest.T : mytest.tcl mytesttemp2

#################################################################
#++ include standard makefile from SoftRelTools.
include SoftRelTools/standard.mk

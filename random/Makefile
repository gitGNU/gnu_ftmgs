#-------------------------------------------------------------------------------
# Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain
#
# This file is part of the <FTMGS> Library
#
# This library is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either
# version 2.1 of the License, or (at your option) any later
# version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with this library. If not, see
# <http://www.gnu.org/licenses/>.
#-------------------------------------------------------------------------------
#- Makefile --------------------------------------------------------
SRCS=test.c
SRCSLIB=random.c system_rnd.c
HDRS=random.h
PRIVHDRS=system_rnd.h system_mutex.h system_endian.h
AUXS=Makefile
#------
PRJLIBS=sha
SYSLIBS=pthread 
#-------------------------------------------------------------------------------
PROGNAME=
LNAME=
LIBNAME=
SHAREDLIBNAME=
VMAJOR=1
VMINOR=0
REVISION=0
#----------------------------------
INST_IDIR=../include 
INST_LDIR=../lib
IDIR=../include 
LDIR=../lib
INCDIR=-I$(IDIR) 
LIBDIR=-L$(LDIR)
#-------------------------------------------------------------------------------
LIBS:=$(addprefix -l,$(PRJLIBS) $(SYSLIBS)) 
PLIBS:=$(addprefix $(LDIR)/lib,$(PRJLIBS)) 
PLIBS:=$(addsuffix .a,$(PLIBS)) 
#-------------------------------------------------------------------------------
ifdef NTHREADS
  OPTFLAGS=-DNTHREADS__
  LIBS:=$(subst -lpthread,,$(LIBS))
else
  OPTFLAGS=
endif
#-------------------------------------------------------------------------------
ifeq ($(strip $(PROGNAME)),)
  PROGNAME=$(basename $(notdir $(firstword $(SRCS))))
endif
ifeq ($(strip $(LNAME)),)
  LNAME=$(basename $(notdir $(firstword $(HDRS))))
endif
ifeq ($(strip $(LIBNAME)),)
  LIBNAME=lib$(LNAME).a
endif
ifeq ($(strip $(SHAREDLIBNAME)),)
  SHAREDLIBNAME=lib$(LNAME).so
endif
#------------------------------------
ifdef NDEBUG
  CDBGFLAGS=-DNDEBUG -O3 -DOPTLEVEL=2 -s
else ifdef DEBUG
# -g -> para debugging info, -rdynamic -> para backtraces
  CDBGFLAGS=-g -rdynamic -O0 -DOPTLEVEL=0 -DDEBUGINFO
else
  CDBGFLAGS=-O2 -DOPTLEVEL=0 -s
endif
#----------------------------------
CERRFLAGS=-ansi -pedantic -Wall -Werror -Wextra -Wno-long-long -Wshadow -Wfloat-equal -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Wconversion -Wsign-conversion -Wchar-subscripts
#----------------------------------
CFLAGS=-fPIC $(CERRFLAGS) $(CDBGFLAGS) $(OPTFLAGS) $(INCDIR)
ALLDEFINES=$(CFLAGS)
DEPENDFLAGS=-Y
DEPEND=makedepend
MV=mv -f
CP=cp -f
RM=rm -f
LN=ln -sf
CD=cd
MD=mkdir -p
TAR=tar
LDCONFIG=ldconfig
CHMOD=chmod -f
#---------------------------------------
OBJS=$(SRCS:.c=.o)
OBJSLIB=$(SRCSLIB:.c=.o)
#----------------------------------
all: $(PROGNAME)
stlib: $(LIBNAME)
shlib: $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
install: installst installsh
installst: $(INST_LDIR)/$(LIBNAME)
installsh: $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
uninstall:
	$(RM) $(INST_LDIR)/$(LIBNAME) $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR) $(INST_LDIR)/$(SHAREDLIBNAME)
	$(CD) $(INST_IDIR) && $(RM) $(HDRS)
$(LIBNAME): $(PLIBS) $(OBJSLIB)
	$(RM) $@ ; $(AR) -rsc $@ $(OBJSLIB)
$(INST_LDIR)/$(LIBNAME): $(HDRS) $(LIBNAME)
	-$(MD) $(INST_IDIR) $(INST_LDIR)
	$(CP) $(HDRS) $(INST_IDIR)
	$(CP) $(LIBNAME) $(INST_LDIR)
	$(CD) $(INST_IDIR) && $(CHMOD) 0644 $(HDRS)
	$(CD) $(INST_LDIR) && $(CHMOD) 0644 $(LIBNAME)
$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR): $(PLIBS) $(OBJSLIB)
	$(LINK.c) -shared -fPIC -Wl,-soname,$(SHAREDLIBNAME).$(VMAJOR) -o $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(OBJSLIB) $(LIBDIR) $(LIBS)
$(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR): $(HDRS) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
	-$(MD) $(INST_IDIR) $(INST_LDIR)
	$(CP) $(HDRS) $(INST_IDIR)
	$(CP) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(INST_LDIR)
	$(CD) $(INST_LDIR) && $(LN) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(SHAREDLIBNAME).$(VMAJOR)
	$(CD) $(INST_LDIR) && $(LN) $(SHAREDLIBNAME).$(VMAJOR) $(SHAREDLIBNAME)
	$(CD) $(INST_LDIR) && $(CHMOD) 0644 $(SHAREDLIBNAME).*
	$(CD) $(INST_IDIR) && $(CHMOD) 0644 $(HDRS)
	$(LDCONFIG)
$(PROGNAME): $(OBJS) $(LIBNAME)
	$(LINK.c) -o $@ $(OBJS) -L. -l$(LNAME) $(LIBDIR) $(LIBS)
clean:
	$(RM) $(OBJS) $(OBJSLIB)
cleanup: cleanuplibs uninstalllibs
	$(RM) $(OBJS) $(OBJSLIB) $(LIBNAME) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(PROGNAME)
$(PLIBS):
	$(CD) ../$(patsubst $(LDIR)/lib%.a,%,$@) && $(MAKE) installst
cleanuplibs:
	$(foreach lib,$(PRJLIBS),$(CD) ../$(lib) && $(MAKE) cleanup ; )
uninstalllibs:
	$(foreach lib,$(PRJLIBS),$(CD) ../$(lib) && $(MAKE) uninstall ; )
zip: $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).zip
$(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).zip:
	@($(CD) .. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) .. && zip -r $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) -x .git \*~ \*.bak \*.o \*.a \*.so \*.so.\* )
	@($(CD) .. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
tgz: $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz
$(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz:
	@($(CD) .. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) .. && $(TAR) --exclude-backups --exclude='*.git' --exclude='*~' --exclude='*.bak' --exclude='*.o' --exclude='*.a' --exclude='*.so' --exclude='*.so.*' -zchf $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	@($(CD) .. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
depend:
	-@$(DEPEND) $(DEPENDFLAGS) -- $(ALLDEFINES) $(DEPEND_DEFINES) -- $(SRCS) $(SRCSLIB) 2> /dev/null

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

test.o: random.h
random.o: random.h ../include/sha.h system_rnd.h system_mutex.h
random.o: system_endian.h
system_rnd.o: system_rnd.h

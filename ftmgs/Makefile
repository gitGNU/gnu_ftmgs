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
SRCSLIB=claim.c		\
	group.c			\
	join.c			\
	link.c			\
	open.c			\
	revtrace.c		\
	sign.c			\
	syssph.c		\
	ftmgs_asn1.c	\
	ftmgs_asn1_tab.c
HDRS=ftmgs.h
PRIVHDRS=claim.h	\
	group.h			\
	join.h			\
	link.h			\
	open.h			\
	revtrace.h		\
	sign.h			\
	syssph.h		\
	system.h
AUXS=README Makefile ftmgs.asn1 makeit.sh
LIBS=-ltasn1 -lcrypto -lbigint -lbuffer -lrandom -lsha -lgmp -lm -lpthread
#----------------------------------
PROGNAME=
LNAME=
LIBNAME=
SHAREDLIBNAME=
VMAJOR=0
VMINOR=1
REVISION=0
#----------------------------------
INST_IDIR=/usr/include 
INST_LDIR=/usr/lib
IDIR=../include
LDIR=../lib
INCDIR=-I$(IDIR)
LIBDIR=-L$(LDIR)
#---------------------------------------
PRJLIBS=$(LDIR)/libsha.a $(LDIR)/librandom.a $(LDIR)/libbuffer.a $(LDIR)/libbigint.a $(LDIR)/libcrypto.a
#-------------------------------------------------------------------
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
ifdef DEBUG1
  CDBGFLAGS=-O2 -DOPTLEVEL=0 -s
else ifdef DEBUG2
# -g -> para debugging info, -rdynamic -> para backtraces
  CDBGFLAGS=-g -rdynamic -O0 -DOPTLEVEL=0 -DDEBUGINFO
else
  CDBGFLAGS=-DNDEBUG -O3 -DOPTLEVEL=2 -s
endif
#------------------------------------
ifdef TIMER
OPTFLAGS1=-DTIMER__
else
OPTFLAGS1=
endif
#------------------------------------
ifdef STS
OPTFLAGS2=-DPOWSTS__
else
OPTFLAGS2=
endif
#------------------------------------
ifdef NPRECOMP
OPTFLAGS3=
else
OPTFLAGS3=-DPRECOMPUTATIONS__
endif
#------------------------------------
ifdef NASN1
OPTFLAGS4=-DNCHECK_ASN1__
else
OPTFLAGS4=
endif
#---------------------------------------
ifdef NTHREADS
OPTFLAGS5=-DNTHREADS__
LIBS:=$(subst -lpthread,,$(LIBS))
else
OPTFLAGS5=
endif
#------------------------------------
OPTFLAGS0=-DFTMGS_BUILDING__
#------------------------------------
OPTFLAGS=$(OPTFLAGS0) $(OPTFLAGS1) $(OPTFLAGS2) $(OPTFLAGS3) $(OPTFLAGS4) $(OPTFLAGS5)
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
#---------------------------------------
all: lib $(PROGNAME)
lib: stlib shlib
stlib: $(LIBNAME)
shlib: $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
install: installst installsh
installst: $(INST_LDIR)/$(LIBNAME)
installsh: $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
uninstall:
	$(RM) $(INST_LDIR)/$(LIBNAME) $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR) $(INST_LDIR)/$(SHAREDLIBNAME)
	$(CD) $(INST_IDIR) && $(RM) $(HDRS)
$(LIBNAME): $(PRJLIBS) $(OBJSLIB)
	$(RM) $@ ; $(AR) -rsc $@ $(OBJSLIB)
	$(MD) tmp && $(CD) tmp && $(foreach lib,$(PRJLIBS), $(RM) *.o ; $(AR) x ../$(lib) ; $(AR) -qs ../$@ *.o ;  $(RM) *.o )
$(INST_LDIR)/$(LIBNAME): $(HDRS) $(LIBNAME)
	-$(MD) $(INST_IDIR) $(INST_LDIR)
	$(CP) $(HDRS) $(INST_IDIR)
	$(CP) $(LIBNAME) $(INST_LDIR)
	$(CD) $(INST_IDIR) && $(CHMOD) 0644 $(HDRS)
	$(CD) $(INST_LDIR) && $(CHMOD) 0644 $(LIBNAME)
$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR): $(PRJLIBS) $(OBJSLIB)
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
$(PROGNAME): $(OBJS) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
	$(LINK.c) -o $@ $(OBJS) -L. -l$(LNAME) $(LIBDIR) $(LIBS)
clean:
	$(RM) $(OBJS) $(OBJSLIB)
cleanup: cleanuplibs uninstalllibs
	$(RM) $(OBJS) $(OBJSLIB) $(LIBNAME) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(PROGNAME)
zip: $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).zip
$(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).zip:
	@($(CD) ../.. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) ../.. && zip -r $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) -x .git \*~ \*.bak \*.o \*.a \*.so \*.so.\* )
	@($(CD) ../.. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
tgz: $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz
$(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz:
	@($(CD) ../.. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) ../.. && $(TAR) --exclude-backups --exclude='.git' --exclude='*~' --exclude='*.bak' --exclude='*.o' --exclude='*.a' --exclude='*.so' --exclude='*.so.*' -zchf $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	@($(CD) ../.. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
depend:
	-@$(DEPEND) $(DEPENDFLAGS) -- $(ALLDEFINES) $(DEPEND_DEFINES) -- $(SRCS) $(SRCSLIB) 2> /dev/null
ftmgs_asn1_tab.c: ftmgs.asn1
	asn1Parser -o $@ $<
$(LDIR)/libsha.a:
	cd ../sha && $(MAKE) installst
$(LDIR)/librandom.a:
	cd ../random && $(MAKE) installst
$(LDIR)/libbuffer.a:
	cd ../buffer && $(MAKE) installst
$(LDIR)/libbigint.a:
	cd ../bigint && $(MAKE) installst
$(LDIR)/libcrypto.a:
	cd ../crypto && $(MAKE) installst
cleanuplibs:
	cd ../sha && $(MAKE) cleanup
	cd ../random && $(MAKE) cleanup
	cd ../buffer && $(MAKE) cleanup
	cd ../bigint && $(MAKE) cleanup
	cd ../crypto && $(MAKE) cleanup
uninstalllibs:
	cd ../sha && $(MAKE) uninstall
	cd ../random && $(MAKE) uninstall
	cd ../buffer && $(MAKE) uninstall
	cd ../bigint && $(MAKE) uninstall
	cd ../crypto && $(MAKE) uninstall
# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

test.o: ftmgs.h
claim.o: claim.h ftmgs.h group.h ../include/random.h syssph.h join.h sign.h
group.o: group.h ftmgs.h ../include/random.h syssph.h
join.o: join.h ftmgs.h ../include/random.h group.h syssph.h
link.o: link.h ftmgs.h group.h ../include/random.h syssph.h join.h sign.h
open.o: open.h ftmgs.h group.h ../include/random.h syssph.h join.h sign.h
revtrace.o: revtrace.h ftmgs.h group.h ../include/random.h syssph.h join.h
revtrace.o: sign.h
sign.o: sign.h ftmgs.h ../include/random.h group.h syssph.h join.h
syssph.o: syssph.h
ftmgs_asn1.o: system.h ftmgs.h group.h ../include/random.h syssph.h join.h
ftmgs_asn1.o: sign.h open.h revtrace.h claim.h link.h

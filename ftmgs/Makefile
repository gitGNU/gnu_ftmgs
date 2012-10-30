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
	system_endian.h
AUXS=README Makefile ftmgs.asn1 makeit.sh
#------
PRJLIBS=crypto bigint buffer vector random sha
SYSLIBS=tasn1 gmp m
#SYSLIBS=tasn1 gmp m pthread
#-------------------------------------------------------------------------------
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
#-------------------------------------------------------------------------------
LIBS:=$(addprefix -l,$(PRJLIBS) $(SYSLIBS)) 
PLIBS:=$(addprefix $(LDIR)/lib,$(PRJLIBS)) 
PLIBS:=$(addsuffix .a,$(PLIBS)) 
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
# ifdef NTHREADS
# OPTFLAGS5=-DNTHREADS__
# LIBS:=$(subst -lpthread,,$(LIBS))
# else
# OPTFLAGS5=
# endif
#------------------------------------
OPTFLAGS0=-DFTMGS_BUILDING__
#------------------------------------
OPTFLAGS=$(OPTFLAGS0) $(OPTFLAGS1) $(OPTFLAGS2) $(OPTFLAGS3) $(OPTFLAGS4) $(OPTFLAGS5)
#----------------------------------
CERRFLAGS=-ansi -pedantic -Wall -Werror -Wextra -Wno-long-long -Wshadow -Wfloat-equal -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Wchar-subscripts
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
	$(MD) tmp && $(CD) tmp && $(foreach lib,$(PLIBS), $(RM) *.o ; $(AR) x ../$(lib) ; $(AR) -qs ../$@ *.o ;  $(RM) *.o ; )
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

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

test.o: ftmgs.h
claim.o: claim.h ftmgs.h ../include/bigint.h group.h ../include/birnd.h
claim.o: ../include/random.h syssph.h ../include/syspar.h ../include/sha.h
claim.o: ../include/sphere.h ../include/nadrp.h ../include/elgamal_thr.h
claim.o: ../include/paillier_thr.h join.h sign.h ../include/iexport.h
claim.o: ../include/sok.h ../include/cdtor.h
group.o: group.h ftmgs.h ../include/bigint.h ../include/birnd.h
group.o: ../include/random.h syssph.h ../include/syspar.h ../include/sha.h
group.o: ../include/sphere.h ../include/nadrp.h ../include/elgamal_thr.h
group.o: ../include/paillier_thr.h ../include/generator.h ../include/cdtor.h
join.o: join.h ftmgs.h ../include/bigint.h ../include/birnd.h
join.o: ../include/random.h ../include/paillier_thr.h ../include/syspar.h
join.o: ../include/sha.h ../include/sphere.h ../include/nadrp.h group.h
join.o: syssph.h ../include/elgamal_thr.h ../include/iexport.h
join.o: ../include/sok.h ../include/cdtor.h
link.o: link.h ftmgs.h ../include/bigint.h group.h ../include/birnd.h
link.o: ../include/random.h syssph.h ../include/syspar.h ../include/sha.h
link.o: ../include/sphere.h ../include/nadrp.h ../include/elgamal_thr.h
link.o: ../include/paillier_thr.h join.h sign.h ../include/iexport.h
link.o: ../include/sok.h ../include/cdtor.h
open.o: open.h ftmgs.h ../include/bigint.h group.h ../include/birnd.h
open.o: ../include/random.h syssph.h ../include/syspar.h ../include/sha.h
open.o: ../include/sphere.h ../include/nadrp.h ../include/elgamal_thr.h
open.o: ../include/paillier_thr.h join.h sign.h ../include/iexport.h
open.o: ../include/cdtor.h
revtrace.o: revtrace.h ftmgs.h ../include/bigint.h group.h ../include/birnd.h
revtrace.o: ../include/random.h syssph.h ../include/syspar.h ../include/sha.h
revtrace.o: ../include/sphere.h ../include/nadrp.h ../include/elgamal_thr.h
revtrace.o: ../include/paillier_thr.h join.h sign.h ../include/iexport.h
revtrace.o: ../include/cdtor.h
sign.o: sign.h ftmgs.h ../include/bigint.h ../include/iexport.h
sign.o: ../include/sha.h ../include/birnd.h ../include/random.h
sign.o: ../include/elgamal_thr.h ../include/syspar.h ../include/sphere.h
sign.o: group.h syssph.h ../include/nadrp.h ../include/paillier_thr.h join.h
sign.o: ../include/sok.h ../include/cdtor.h
syssph.o: syssph.h ../include/bigint.h ../include/syspar.h ../include/sha.h
syssph.o: ../include/sphere.h ../include/birnd.h ../include/random.h
ftmgs_asn1.o: ../include/bigint.h ../include/iexport.h ../include/sha.h
ftmgs_asn1.o: ../include/buffer.h ../include/vector.h system_endian.h ftmgs.h
ftmgs_asn1.o: group.h ../include/birnd.h ../include/random.h syssph.h
ftmgs_asn1.o: ../include/syspar.h ../include/sphere.h ../include/nadrp.h
ftmgs_asn1.o: ../include/elgamal_thr.h ../include/paillier_thr.h join.h
ftmgs_asn1.o: sign.h open.h revtrace.h claim.h link.h

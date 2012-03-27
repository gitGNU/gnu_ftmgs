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
SRCSLIB=elgamal.c	\
	elgamal_thr.c	\
	generator.c		\
	nadrp.c			\
	paillier.c		\
	paillier_thr.c	\
	schnorr.c		\
	sok.c			\
	sphere.c		\
	syspar.c
HDRS=elgamal.h		\
	elgamal_thr.h	\
	generator.h		\
	nadrp.h			\
	paillier.h		\
	paillier_thr.h	\
	schnorr.h		\
	sok.h			\
	sphere.h		\
	syspar.h
PRIVHDRS=
AUXS=Makefile
LIBS=-lbigint -lrandom -lsha -lgmp -lm
#----------------------------------
PROGNAME=
LNAME=crypto
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
#---------------------------------------
PRJLIBS=$(LDIR)/libsha.a $(LDIR)/librandom.a $(LDIR)/libbigint.a
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
OPTFLAGS=$(OPTFLAGS1) $(OPTFLAGS2) $(OPTFLAGS3)
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
$(LIBNAME): $(PRJLIBS) $(OBJSLIB)
	$(RM) $@ ; $(AR) -rsc $@ $(OBJSLIB)
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
$(PROGNAME): $(OBJS) $(LIBNAME)
	$(LINK.c) -o $@ $(OBJS) -L. -l$(LNAME) $(LIBDIR) $(LIBS)
clean:
	$(RM) $(OBJS) $(OBJSLIB)
cleanup: cleanuplibs uninstalllibs
	$(RM) $(OBJS) $(OBJSLIB) $(LIBNAME) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(PROGNAME)
zip: $(LNAME)-$(VMAJOR).$(VMINOR).zip
$(LNAME)-$(VMAJOR).$(VMINOR).zip:
	@($(CD) .. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) .. && zip -r $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	@($(CD) .. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
tgz: $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz
$(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION).tgz:
	@($(CD) .. && $(LN) -n $(LNAME) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	($(CD) .. && $(TAR) --exclude-backups --exclude='*.bak' --exclude='*.o' --exclude='*.a' --exclude='*.so' --exclude='*.so.*' -zchf $@ $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
	@($(CD) .. && $(RM) $(LNAME)-$(VMAJOR).$(VMINOR).$(REVISION) )
depend:
	-@$(DEPEND) $(DEPENDFLAGS) -- $(ALLDEFINES) $(DEPEND_DEFINES) -- $(SRCS) $(SRCSLIB) 2> /dev/null
$(LDIR)/libsha.a:
	cd ../sha && $(MAKE) installst
$(LDIR)/librandom.a:
	cd ../random && $(MAKE) installst
$(LDIR)/libbigint.a:
	cd ../bigint && $(MAKE) installst
cleanuplibs:
	cd ../sha && $(MAKE) cleanup
	cd ../random && $(MAKE) cleanup
	cd ../bigint && $(MAKE) cleanup
uninstalllibs:
	cd ../sha && $(MAKE) uninstall
	cd ../random && $(MAKE) uninstall
	cd ../bigint && $(MAKE) uninstall
# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

test.o: elgamal.h ../include/bigint.h ../include/birnd.h ../include/random.h
test.o: sphere.h syspar.h ../include/sha.h elgamal_thr.h nadrp.h paillier.h
test.o: paillier_thr.h schnorr.h ../include/cdtor.h
elgamal.o: elgamal.h ../include/bigint.h ../include/birnd.h
elgamal.o: ../include/random.h sphere.h syspar.h ../include/sha.h
elgamal.o: ../include/iexport.h generator.h ../include/cdtor.h
elgamal_thr.o: elgamal_thr.h ../include/bigint.h ../include/birnd.h
elgamal_thr.o: ../include/random.h syspar.h ../include/sha.h sphere.h
elgamal_thr.o: ../include/iexport.h generator.h sok.h ../include/cdtor.h
generator.o: generator.h ../include/bigint.h ../include/random.h
generator.o: ../include/birnd.h
nadrp.o: nadrp.h ../include/bigint.h syspar.h ../include/sha.h sphere.h
nadrp.o: ../include/birnd.h ../include/random.h sok.h ../include/cdtor.h
paillier.o: paillier.h ../include/bigint.h ../include/birnd.h
paillier.o: ../include/random.h ../include/iexport.h ../include/sha.h
paillier.o: syspar.h generator.h ../include/cdtor.h
paillier_thr.o: paillier_thr.h ../include/bigint.h ../include/birnd.h
paillier_thr.o: ../include/random.h syspar.h ../include/sha.h sphere.h
paillier_thr.o: ../include/iexport.h generator.h sok.h ../include/cdtor.h
schnorr.o: schnorr.h ../include/sha.h ../include/bigint.h ../include/birnd.h
schnorr.o: ../include/random.h syspar.h sphere.h ../include/iexport.h
schnorr.o: generator.h
sok.o: sok.h ../include/bigint.h sphere.h ../include/birnd.h
sok.o: ../include/random.h syspar.h ../include/sha.h ../include/iexport.h
sok.o: ../include/cdtor.h
sphere.o: sphere.h ../include/bigint.h ../include/birnd.h ../include/random.h
sphere.o: syspar.h ../include/sha.h ../include/cdtor.h
syspar.o: syspar.h ../include/sha.h

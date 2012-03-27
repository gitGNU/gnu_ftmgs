#-------------------------------------------------------------------------------
# Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain
#
#    Redistribution and use in source and binary forms, with or
#    without modification, are permitted provided that the following
#    conditions are met:
# 
#    - Redistributions of source code must retain the above
#      copyright notice, this list of conditions and
#      the following disclaimer.
# 
#    - Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
# 
#    - Neither the name of Internet Society, IETF or IETF Trust, nor
#      the names of specific contributors, may be used to endorse or
#      promote products derived from this software without specific
#      prior written permission.
# 
#    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#    DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#-------------------------------------------------------------------------------
#- Makefile --------------------------------------------------------
SRCS=shatest.c
SRCSLIB=usha.c hkdf.c hmac.c sha1.c sha224-256.c sha384-512.c
HDRS=sha.h
PRIVHDRS=sha-private.h stdint-example.h
AUXS=Makefile COPYING_SHA
LIBS=
#----------------------------------
PROGNAME=
LNAME=sha
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
PRJLIBS=
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
#----------------------------------
CERRFLAGS=-Wall -Wextra -Wno-long-long -Wfloat-equal -Wsign-compare -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings 
#----------------------------------
CFLAGS=-fPIC $(CERRFLAGS) $(CDBGFLAGS) $(INCDIR)
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
	$(CP) COPYING_SHA $(INST_IDIR)
	$(CP) $(HDRS) $(INST_IDIR)
	$(CP) $(LIBNAME) $(INST_LDIR)
	$(CD) $(INST_IDIR) && $(CHMOD) 0644 $(HDRS)
	$(CD) $(INST_LDIR) && $(CHMOD) 0644 $(LIBNAME)
$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR): $(PRJLIBS) $(OBJSLIB)
	$(LINK.c) -shared -fPIC -Wl,-soname,$(SHAREDLIBNAME).$(VMAJOR) -o $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR) $(OBJSLIB) $(LIBDIR) $(LIBS)
$(INST_LDIR)/$(SHAREDLIBNAME).$(VMAJOR).$(VMINOR): $(HDRS) $(SHAREDLIBNAME).$(VMAJOR).$(VMINOR)
	-$(MD) $(INST_IDIR) $(INST_LDIR)
	$(CP) COPYING_SHA $(INST_IDIR)
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
cleanuplibs:
uninstalllibs:
# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

shatest.o: sha.h
usha.o: sha.h
hkdf.o: sha.h
hmac.o: sha.h
sha1.o: sha.h sha-private.h
sha224-256.o: sha.h sha-private.h
sha384-512.o: sha.h sha-private.h

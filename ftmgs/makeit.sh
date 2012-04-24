#!/bin/bash
#-------------------------------------
OPTS=''
PRECOMP=''
THREADS=''
DEBUG=''
TIMER=''
STS=''
ASN1=''
while true ; do
	case "$1" in
		-h)
			echo "usage: `basename $0` [-[no]precomp] [-[no]timer] [-[no]sts] [-[no]debug] [-[no]asn1] [-[no]threads] [target]"
			exit
			;;
		-noprecomp)
			PRECOMP='NPRECOMP=1'
			;;
		-precomp)
			PRECOMP=''
			;;
		-nothreads)
			THREADS='NTHREADS=1'
			;;
		-threads)
			THREADS=''
			;;
		-nodebug)
			DEBUG=''
			;;
		-debug)
			DEBUG='DEBUG1=1'
			;;
		-debug2)
			DEBUG='DEBUG2=1'
			;;
		-notimer)
			TIMER=''
			;;
		-timer)
			TIMER='TIMER=1'
			;;
		-nosts)
			STS=''
			;;
		-sts)
			STS='STS=1'
			;;
		-noasn1)
			ASN1='NASN1=1'
			;;
		-asn1)
			ASN1=''
			;;
		-*)
			OPTS="$OPTS $1"
			;;
		*)
			break
			;;
	esac
	shift
done
OPTS="$OPTS $DEBUG $STS $TIMER $PRECOMP $ASN1"
TARGET="$*"
echo make $OPTS $TARGET
make $OPTS $TARGET

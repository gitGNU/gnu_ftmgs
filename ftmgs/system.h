/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This file is part of the <FTMGS> Library                                   */
/*                                                                            */
/* This library is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation, either               */
/* version 2.1 of the License, or (at your option) any later                  */
/* version.                                                                   */
/*                                                                            */
/* This library is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU Lesser General Public License for more details.                        */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General                  */
/* Public License along with this library. If not, see                        */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
/* 
 * System Dependencies
 */
/*----------------------------------------------------------------------------*/
#ifndef umalccvbg_system_h__
#define umalccvbg_system_h__	1
#include <limits.h>
#if HAVE_CONFIG_H
#include "config.h"
#endif
/*----------------------------------------------------------------------------*/
#if HAVE_NETINET_IN_H || defined __linux__ || defined __unix__ || defined __APPLE__
#include <netinet/in.h>
#define HAVE_HTONL			1
#define HAVE_NTOHL			1
#elif HAVE_WINSOCK2_H || defined _MSC_VER || defined _WIN32 || defined __WIN32__
#include <Winsock2.h>
#define HAVE_HTONL			1
#define HAVE_NTOHL			1
#elif HAVE_ENDIAN_H
#include <endian.h>
#ifdef __BYTE_ORDER
#if __BYTE_ORDER == __BIG_ENDIAN
#define FTMGS_BIG_ENDIAN		1
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#define FTMGS_LITTLE_ENDIAN		1
#elif __BYTE_ORDER == __PDP_ENDIAN
#define FTMGS_PDP_ENDIAN		1
#else 
#define FTMGS_RUNTIME_ENDIAN	1
#endif
#else 
#define FTMGS_RUNTIME_ENDIAN	1
#endif
#elif WORDS_BIGENDIAN
#define FTMGS_BIG_ENDIAN	1
#elif defined(_MSC_VER) || defined(_WIN32)
#include <rpcndr.h>
#ifdef NDR_LOCAL_ENDIAN
#if NDR_LOCAL_ENDIAN == NDR_BIG_ENDIAN
#define FTMGS_BIG_ENDIAN		1
#elif NDR_LOCAL_ENDIAN == NDR_LITTLE_ENDIAN
#define FTMGS_LITTLE_ENDIAN		1
#else 
#define FTMGS_RUNTIME_ENDIAN	1
#endif
#else 
#define FTMGS_RUNTIME_ENDIAN	1
#endif
#else 
#define FTMGS_RUNTIME_ENDIAN	1
#endif
/*----------------------------------------------------------------------------*/
#if FTMGS_RUNTIME_ENDIAN
#define IS_LITTLE_ENDIAN()	is_little_endian()
/* 
 * GCC with -O2 calculates at compile-time the result of this function
 * and throws away the unnecessary branch of the conditional sentence
 * that relies on it, on GCC it is equivalent to a compile-time flag
 */
static int is_little_endian()
{
#if (INT_MAX != SCHAR_MAX)
	union { int x; char c; } z = { 0x0001 };
#elif (LONG_MAX != SCHAR_MAX)
	union { long x; char c; } z = { 0x0001 };
#else
#error "Size of char too large"
#endif
	return z.c;
}
#endif
/*----------------------------------------------------------------------------*/
/* 
 * Intel Endianness White Paper (Nov 15, 2004)
 *		[http://www.intel.com/design/intarch/papers/endian.pdf]
 * GCC FORUM:
 *		[http://gcc.gnu.org/ml/gcc-help/2007-07/msg00342.html]
 */
#if INT_MAX == 127
#define int_swap(num)	(num)
#elif INT_MAX == 32767
#ifdef __bswap_16
#define int_swap(num)	__bswap_16(num)
#else
#define int_swap(num)							\
	((int)(((unsigned)((num) >> 8) & 0x00FFU) |	\
		   ((unsigned)((num) << 8) & 0xFF00U)))
#endif
#elif INT_MAX == 2147483647
#ifdef __bswap_32
#define int_swap(num)	__bswap_32(num)
#else
#define int_swap(num)									\
	((int)(((unsigned)((num) >> 24) & 0x000000FFU) |	\
		   ((unsigned)((num) >> 8)  & 0x0000FF00U) |	\
		   ((unsigned)((num) << 8)  & 0x00FF0000U) |	\
		   ((unsigned)((num) << 24) & 0xFF000000U)))
#endif
#elif INT_MAX == 9223372036854775807
#ifdef __bswap_64
#define int_swap(num)	__bswap_64(num)
#else
#define int_swap(num)											\
	((int)(((unsigned)((num) >> 56) & 0x00000000000000FFU) |	\
		   ((unsigned)((num) >> 40) & 0x000000000000FF00U) |	\
		   ((unsigned)((num) >> 24) & 0x0000000000FF0000U) |	\
		   ((unsigned)((num) >> 8)  & 0x00000000FF000000U) |	\
		   ((unsigned)((num) << 8)  & 0x000000FF00000000U) |	\
		   ((unsigned)((num) << 24) & 0x0000FF0000000000U) |	\
		   ((unsigned)((num) << 40) & 0x00FF000000000000U) |	\
		   ((unsigned)((num) << 56) & 0xFF00000000000000U)))
#endif
#else
#error "INT_MAX undefined"
#endif
/*----------------------------------------------------------------------------*/
#if HAVE_HTONL && HAVE_NTOHL && (INT_MAX == 2147483647)
#define int2bigendian(num)	((int)htonl((unsigned)(num)))
#define bigendian2int(num)	((int)ntohl((unsigned)(num)))
#elif HAVE_HTONL && HAVE_NTOHL && (INT_MAX == 32767)
#define int2bigendian(num)	((int)htons((unsigned)(num)))
#define bigendian2int(num)	((int)ntohs((unsigned)(num)))
#elif FTMGS_RUNTIME_ENDIAN
#define int2bigendian(num)	((IS_LITTLE_ENDIAN()) ? int_swap(num) : (num))
#define bigendian2int(num)	((IS_LITTLE_ENDIAN()) ? int_swap(num) : (num))
#elif FTMGS_LITTLE_ENDIAN
#define int2bigendian(num)	int_swap(num)
#define bigendian2int(num)	int_swap(num)
#elif FTMGS_PDP_ENDIAN
#error "Code not prepared for PDP endianness"
#else
#define int2bigendian(num)	(num)
#define bigendian2int(num)	(num)
#endif
/*----------------------------------------------------------------------------*/
#endif

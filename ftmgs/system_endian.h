/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This file is part of the <TLA> Library                                     */
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
#ifndef umalccvbg_system_endian_h__
#define umalccvbg_system_endian_h__	1
#include <limits.h>
#include <stdint.h>
#if HAVE_CONFIG_H
#include "config.h"
#endif
/*----------------------------------------------------------------------------*/
#if HAVE_NETINET_IN_H || defined __linux__ || defined __unix__ || defined __APPLE__
#include <netinet/in.h>
#define HAVE_HTONL__			1
#define HAVE_NTOHL__			1
#elif HAVE_WINSOCK2_H || defined _WIN32 || defined _MSC_VER || defined __WIN32__
#include <Winsock2.h>
#define HAVE_HTONL__			1
#define HAVE_NTOHL__			1
#endif
/*----------------------------------------------------------------------------*/
#if HAVE_ENDIAN_H || defined __linux__ || defined __unix__ || defined __APPLE__
#include <endian.h>
#if defined __BYTE_ORDER && __BYTE_ORDER == __BIG_ENDIAN
#define BIG_ENDIAN__		1
#elif defined __BYTE_ORDER && __BYTE_ORDER == __LITTLE_ENDIAN
#define LITTLE_ENDIAN__		1
#elif defined __BYTE_ORDER && __BYTE_ORDER == __PDP_ENDIAN
#define PDP_ENDIAN__		1
#elif WORDS_BIGENDIAN
#define BIG_ENDIAN__		1
#else 
#define RUNTIME_ENDIAN__	1
#endif
/*----------------------------------------------------------------------------*/
#elif defined _WIN32 || defined _MSC_VER || defined __WIN32__
#include <rpcndr.h>
#if defined NDR_LOCAL_ENDIAN && NDR_LOCAL_ENDIAN == NDR_BIG_ENDIAN
#define BIG_ENDIAN__		1
#elif defined NDR_LOCAL_ENDIAN && NDR_LOCAL_ENDIAN == NDR_LITTLE_ENDIAN
#define LITTLE_ENDIAN__		1
#else 
#define RUNTIME_ENDIAN__	1
#endif
/*----------------------------------------------------------------------------*/
#else 
#define RUNTIME_ENDIAN__	1
#endif
/*----------------------------------------------------------------------------*/
#if RUNTIME_ENDIAN__
/* 
 * GCC with -O2 calculates at compile-time the result of this function
 * and throws away the unnecessary branch of the conditional sentence
 * that relies on it, on GCC it is equivalent to a compile-time flag
 */
#ifdef __GNUC__
__attribute__ ((unused))
#endif
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
/*----------------------------------------------------------------------------*/
#define int8_swap__(n__)	((n__) & 0xFF)
/*------------------------------------*/
#ifdef __bswap_16
#define int16_swap__(n__)		__bswap_16(n__)
#else
#define int16_swap__(n__)						\
	((((n__) >> 8) & 0x00FF) |					\
	 (((n__) << 8) & 0xFF00))
#endif
/*------------------------------------*/
#ifdef __bswap_32
#define int32_swap__(n__)		__bswap_32(n__)
#else
#define int32_swap__(n__)						\
	((((n__) >> 24) & 0x000000FF) |				\
	 (((n__) >> 8)  & 0x0000FF00) |				\
	 (((n__) << 8)  & 0x00FF0000) |				\
	 (((n__) << 24) & 0xFF000000))
#endif
/*------------------------------------*/
#ifdef __bswap_64
#define int64_swap__(n__)		__bswap_64(n__)
#else
#define int64_swap__(n__)						\
	((((n__) >> 56) & 0x00000000000000FF) |		\
	 (((n__) >> 40) & 0x000000000000FF00) |		\
	 (((n__) >> 24) & 0x0000000000FF0000) |		\
	 (((n__) >> 8)  & 0x00000000FF000000) |		\
	 (((n__) << 8)  & 0x000000FF00000000) |		\
	 (((n__) << 24) & 0x0000FF0000000000) |		\
	 (((n__) << 40) & 0x00FF000000000000) |		\
	 (((n__) << 56) & 0xFF00000000000000))
#endif
/*----------------------------------------------------------------------------*/
/* 
 * #if INT_MAX == 127
 * #define int_swap__(n__)		int8_swap__(n__)
 * #elif INT_MAX == 32767
 * #define int_swap__(n__)		int16_swap__(n__)
 * #elif INT_MAX == 2147483647
 * #define int_swap__(n__)		int32_swap__(n__)
 * #elif INT_MAX == 9223372036854775807
 * #define int_swap__(n__)		int64_swap__(n__)
 * #else
 * #error "INT_MAX undefined"
 * #endif
 */
/* 
 * Compile-time swap selection based on size of operand. Compile-time error
 * if unsuitable size of operand
 */
#define int_swap__(n__)							\
	((sizeof(n__) == 1)		? int8_swap__(n__)	\
	 : (sizeof(n__) == 2)	? int16_swap__(n__)	\
	 : (sizeof(n__) == 4)	? int32_swap__(n__)	\
	 : (sizeof(n__) == 8)	? int64_swap__(n__)	\
	 : (!!sizeof(struct{int unsuitable_size:((sizeof(n__) == 1)||(sizeof(n__) == 2)||(sizeof(n__) == 4)||(sizeof(n__) == 8))?1:-1;})) ? 0 : 0)
/*----------------------------------------------------------------------------*/
#if HAVE_HTONL__ && HAVE_NTOHL__ && (INT_MAX == 2147483647)
#define int2bigendian(n__)	(htonl(n__))
#define bigendian2int(n__)	(ntohl(n__))
#elif HAVE_HTONL__ && HAVE_NTOHL__ && (INT_MAX == 32767)
#define int2bigendian(n__)	(htons(n__))
#define bigendian2int(n__)	(ntohs(n__))
#elif RUNTIME_ENDIAN__
#define int2bigendian(n__)	((is_little_endian()) ? int_swap__(n__) : (n__))
#define bigendian2int(n__)	((is_little_endian()) ? int_swap__(n__) : (n__))
#elif LITTLE_ENDIAN__
#define int2bigendian(n__)	int_swap__(n__)
#define bigendian2int(n__)	int_swap__(n__)
#elif BIG_ENDIAN__
#define int2bigendian(n__)	(n__)
#define bigendian2int(n__)	(n__)
#else
#error "Code not prepared for unknown endianness"
#endif
/*----------------------------------------------------------------------------*/
#if RUNTIME_ENDIAN__
#define int2littleendian(n__)	((is_little_endian()) ? (n__) : int_swap__(n__))
#define littleendian2int(n__)	((is_little_endian()) ? (n__) : int_swap__(n__))
#elif BIG_ENDIAN__
#define int2littleendian(n__)	int_swap__(n__)
#define littleendian2int(n__)	int_swap__(n__)
#elif LITTLE_ENDIAN__
#define int2littleendian(n__)	(n__)
#define littleendian2int(n__)	(n__)
#else
#error "Code not prepared for unknown endianness"
#endif
/*----------------------------------------------------------------------------*/
#if RUNTIME_ENDIAN__
#define int2le(n__)	((is_little_endian()) ? (n__) : int_swap__(n__))
#define le2int(n__)	((is_little_endian()) ? (n__) : int_swap__(n__))
#define int2be(n__)	((is_little_endian()) ? int_swap__(n__) : (n__))
#define be2int(n__)	((is_little_endian()) ? int_swap__(n__) : (n__))
#elif BIG_ENDIAN__
#define int2le(n__)	int_swap__(n__)
#define le2int(n__)	int_swap__(n__)
#define int2be(n__)	(n__)
#define be2int(n__)	(n__)
#elif LITTLE_ENDIAN__
#define int2le(n__)	(n__)
#define le2int(n__)	(n__)
#define int2be(n__)	int_swap__(n__)
#define be2int(n__)	int_swap__(n__)
#else
#error "Code not prepared for unknown endianness"
#endif
/*----------------------------------------------------------------------------*/
#undef HAVE_HTONL__
#undef HAVE_NTOHL__
/*----------------------------------------------------------------------------*/
#endif

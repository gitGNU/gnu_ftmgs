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
#ifndef umalccvbg_stdint_h__
#define umalccvbg_stdint_h__	1
#include <limits.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
#if defined LONG_MAX && LONG_MAX>2147483647L && LONG_MAX==9223372036854775807L
#define UINT64_MAX		ULONG_MAX
#define INT64_MAX		LONG_MAX
#define INT64_MIN		LONG_MIN
typedef long			int64_t;		/* int 64-bit integer */
typedef unsigned long	uint64_t;		/* unsigned 64-bit integer */
#elif defined __GNUC__
	/* && defined LLONG_MAX && LLONG_MAX == 9223372036854775807LL */
#define UINT64_MAX			(18446744073709551615LL)
#define INT64_MAX			(9223372036854775807LL)
#define INT64_MIN			(-9223372036854775808LL)
__extension__
typedef long long			int64_t;	/* int 64-bit integer */
__extension__
typedef unsigned long long	uint64_t;	/* unsigned 64-bit integer */
#endif
/*----------------------------------------------------------------------------*/
#if INT_MAX == 2147483647
#define UINT32_MAX		UINT_MAX
#define INT32_MAX		INT_MAX
#define INT32_MIN		INT_MIN
typedef int				int32_t;		/* int 32-bit integer */
typedef unsigned int	uint32_t;		/* unsigned 32-bit integer */
#elif LONG_MAX == 2147483647
#define UINT32_MAX		ULONG_MAX
#define INT32_MAX		LONG_MAX
#define INT32_MIN		LONG_MIN
typedef long			int32_t;		/* int 32-bit integer */
typedef unsigned long	uint32_t;		/* unsigned 32-bit integer */
#endif
/*----------------------------------------------------------------------------*/
#if SHRT_MAX == 32767
#define UINT16_MAX		USHRT_MAX
#define INT16_MAX		SHRT_MAX
#define INT16_MIN		SHRT_MIN
typedef short			int16_t;		/* int 16-bit integer */
typedef unsigned short	uint16_t;		/* unsigned 16-bit integer */
#elif INT_MAX == 32767
#define UINT16_MAX		UINT_MAX
#define INT16_MAX		INT_MAX
#define INT16_MIN		INT_MIN
typedef int				int16_t;		/* int 16-bit integer */
typedef unsigned		uint16_t;		/* unsigned 16-bit integer */
#endif
/*----------------------------------------------------------------------------*/
#if SCHAR_MAX == 127
#define UINT8_MAX		UCHAR_MAX
#define INT8_MAX		SCHAR_MAX
#define INT8_MIN		SCHAR_MIN
typedef signed char		int8_t;			/* int 8-bit integer */
typedef unsigned char	uint8_t;		/* unsigned 8-bit integer */
#endif
/*----------------------------------------------------------------------------*/
#if defined INT8_MAX
typedef int8_t		int_least8_t;
typedef uint8_t		uint_least8_t;
typedef int8_t		int_fast8_t;
typedef uint8_t		uint_fast8_t;
#endif
#if defined INT16_MAX
typedef int16_t		int_least16_t;
typedef uint16_t	uint_least16_t;
#endif
#if defined INT32_MAX
typedef int32_t		int_least32_t;
typedef uint32_t	uint_least32_t;
typedef int32_t		int_fast16_t;
typedef uint32_t	uint_fast16_t;
typedef int32_t		int_fast32_t;
typedef uint32_t	uint_fast32_t;
#endif
#if defined INT64_MAX
typedef int64_t		int_least64_t;
typedef uint64_t	uint_least64_t;
typedef int64_t		int_fast64_t;
typedef uint64_t	uint_fast64_t;
#endif
/*----------------------------------------------------------------------------*/
#if defined INT64_MAX
typedef int64_t		intmax_t;
typedef uint64_t	uintmax_t;
#define INTMAX_MIN	INT64_MIN
#define INTMAX_MAX	INT64_MAX
#define UINTMAX_MAX	UINT64_MAX
#else
typedef int32_t		intmax_t;
typedef uint32_t	uintmax_t;
#define INTMAX_MIN	INT32_MIN
#define INTMAX_MAX	INT32_MAX
#define UINTMAX_MAX	UINT32_MAX
#endif
/*----------------------------------------------------------------------------*/
#if defined LONG_MAX && LONG_MAX>2147483647L && LONG_MAX==9223372036854775807L
typedef long			intptr_t;
typedef unsigned long	uintptr_t;
#define INTPTR_MIN		LONG_MIN
#define INTPTR_MAX		LONG_MAX
#define UINTPTR_MAX		ULONG_MAX
#else
typedef int				intptr_t;
typedef unsigned int	uintptr_t;
#define INTPTR_MIN		INT_MIN
#define INTPTR_MAX		INT_MAX
#define UINTPTR_MAX		UINT_MAX
#endif
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

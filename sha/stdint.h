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
/*-- Exact Width -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if defined INT_MAX && INT_MAX > 2147483647 && INT_MAX == 9223372036854775807
#define UINT64_MAX		UINT_MAX
#define INT64_MAX		INT_MAX
#define INT64_MIN		INT_MIN
typedef int				int64_t;		/* int 64-bit integer */
typedef unsigned int	uint64_t;		/* unsigned 64-bit integer */
#elif defined LONG_MAX && LONG_MAX>2147483647L && LONG_MAX==9223372036854775807L
#define UINT64_MAX		ULONG_MAX
#define INT64_MAX		LONG_MAX
#define INT64_MIN		LONG_MIN
typedef long			int64_t;		/* int 64-bit integer */
typedef unsigned long	uint64_t;		/* unsigned 64-bit integer */
#elif defined __GNUC__
#define UINT64_MAX			(18446744073709551615ULL)
#define INT64_MAX			(9223372036854775807LL)
#define INT64_MIN			(-9223372036854775808LL)
__extension__
typedef long long			int64_t;	/* int 64-bit integer */
__extension__
typedef unsigned long long	uint64_t;	/* unsigned 64-bit integer */
#elif defined _WIN32 || defined _MSC_VER || defined __WIN32__
#define UINT64_MAX			(18446744073709551615ULL)
#define INT64_MAX			(9223372036854775807LL)
#define INT64_MIN			(-9223372036854775808LL)
typedef __int64				int64_t;	/* int 64-bit integer */
typedef unsigned __int64	uint64_t;	/* unsigned 64-bit integer */
#endif
/*----------------------------------------------------------------------------*/
#if SHRT_MAX == 2147483647
#define UINT32_MAX		USHRT_MAX
#define INT32_MAX		SHRT_MAX
#define INT32_MIN		SHRT_MIN
typedef short			int32_t;		/* int 32-bit integer */
typedef unsigned short	uint32_t;		/* unsigned 32-bit integer */
#if INT_MAX == 2147483647
#define UINT32_MAX		UINT_MAX
#define INT32_MAX		INT_MAX
#define INT32_MIN		INT_MIN
typedef int				int32_t;		/* int 32-bit integer */
typedef unsigned int	uint32_t;		/* unsigned 32-bit integer */
#elif LONG_MAX == 2147483647L
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
#elif LONG_MAX == 32767L
#define UINT16_MAX		ULONG_MAX
#define INT16_MAX		LONG_MAX
#define INT16_MIN		LONG_MIN
typedef long			int16_t;		/* int 16-bit integer */
typedef unsigned long	uint16_t;		/* unsigned 16-bit integer */
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
/*-- Least Width -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if defined INT8_MAX
typedef int8_t			int_least8_t;
typedef uint8_t			uint_least8_t;
#elif defined INT16_MAX
typedef int16_t			int_least8_t;
typedef uint16_t		uint_least8_t;
#elif defined INT32_MAX
typedef int32_t			int_least8_t;
typedef uint32_t		uint_least8_t;
#elif defined INT64_MAX
typedef int64_t			int_least8_t;
typedef uint64_t		uint_least8_t;
#endif
#define UINT_LEAST8_MAX		(255U)
#define INT_LEAST8_MAX		(127)
#define INT_LEAST8_MIN		(-128)
/*--------------------------------*/
#if defined INT16_MAX
typedef int16_t			int_least16_t;
typedef uint16_t		uint_least16_t;
#elif defined INT32_MAX
typedef int32_t			int_least16_t;
typedef uint32_t		uint_least16_t;
#elif defined INT64_MAX
typedef int64_t			int_least16_t;
typedef uint64_t		uint_least16_t;
#endif
#define UINT_LEAST16_MAX	(65535U)
#define INT_LEAST16_MAX		(32767)
#define INT_LEAST16_MIN		(-32768)
	/*--------------------------------*/
#if defined INT32_MAX
typedef int32_t			int_least32_t;
typedef uint32_t		uint_least32_t;
#elif defined INT64_MAX
typedef int64_t			int_least32_t;
typedef uint64_t		uint_least32_t;
#endif
#define UINT_LEAST32_MAX	(4294967295U)
#define INT_LEAST32_MAX		(2147483647)
#define INT_LEAST32_MIN		(2147483648)
	/*--------------------------------*/
#if defined INT64_MAX
typedef int64_t			int_least64_t;
typedef uint64_t		uint_least64_t;
#define UINT_LEAST64_MAX	UINT64_MAX
#define INT_LEAST64_MAX		INT64_MAX
#define INT_LEAST64_MIN		INT32_MIN
#endif
/*----------------------------------------------------------------------------*/
/*-- Fast Width --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef int_least8_t	int_fast8_t;
typedef uint_least8_t	uint_fast8_t;
#define UINT_FAST8_MAX		UINT_LEAST8_MAX
#define INT_FAST8_MAX		INT_LEAST8_MAX
#define INT_FAST8_MIN		INT_LEAST8_MIN
#if defined INT_MAX && defined INT16_MAX && INT_MAX >= INT16_MAX
typedef int				int_fast16_t;
typedef unsigned int	uint_fast16_t;
#else
typedef int_least16_t	int_fast16_t;
typedef uint_least16_t	uint_fast16_t;
#endif
#define UINT_FAST16_MAX		UINT_LEAST16_MAX
#define INT_FAST16_MAX		INT_LEAST16_MAX
#define INT_FAST16_MIN		INT_LEAST16_MIN
#if defined INT_MAX && defined INT32_MAX && INT_MAX >= INT32_MAX
typedef int				int_fast32_t;
typedef unsigned int	uint_fast32_t;
#else
typedef int_least32_t	int_fast32_t;
typedef uint_least32_t	uint_fast32_t;
#endif
#define UINT_FAST32_MAX		UINT_LEAST32_MAX
#define INT_FAST32_MAX		INT_LEAST32_MAX
#define INT_FAST32_MIN		INT_LEAST32_MIN
#endif
#if defined INT_LEAST64_MAX
typedef int_least64_t	int_fast64_t;
typedef uint_least64_t	uint_fast64_t;
#define UINT_FAST64_MAX		UINT_LEAST64_MAX
#define INT_FAST64_MAX		INT_LEAST64_MAX
#define INT_FAST64_MIN		INT_LEAST32_MIN
#endif
/*----------------------------------------------------------------------------*/
/*-- IntMax Width ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if defined INT64_MAX
typedef int64_t		intmax_t;
typedef uint64_t	uintmax_t;
#define UINTMAX_MAX	UINT64_MAX
#define INTMAX_MAX	INT64_MAX
#define INTMAX_MIN	INT64_MIN
#define UINTMAX_C(c)	UINT64_C(c)
#define INTMAX_C(c)		INT64_C(c)
#if defined INT32_MAX
typedef int32_t		intmax_t;
typedef uint32_t	uintmax_t;
#define UINTMAX_MAX	UINT32_MAX
#define INTMAX_MAX	INT32_MAX
#define INTMAX_MIN	INT32_MIN
#define UINTMAX_C(c)	UINT32_C(c)
#define INTMAX_C(c)		INT32_C(c)
#if defined INT16_MAX
typedef int16_t		intmax_t;
typedef uint16_t	uintmax_t;
#define UINTMAX_MAX	UINT16_MAX
#define INTMAX_MAX	INT16_MAX
#define INTMAX_MIN	INT16_MIN
#define INTMAX_C(c)		INT16_C(c)
#define UINTMAX_C(c)	UINT16_C(c)
#if defined INT8_MAX
typedef int8_t		intmax_t;
typedef uint8_t		uintmax_t;
#define UINTMAX_MAX	UINT8_MAX
#define INTMAX_MAX	INT8_MAX
#define INTMAX_MIN	INT8_MIN
#define INTMAX_C(c)		INT8_C(c)
#define UINTMAX_C(c)	UINT8_C(c)
#endif
/*----------------------------------------------------------------------------*/
/*-- Integer Pointer ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if defined LONG_MAX && LONG_MAX>2147483647L && LONG_MAX==9223372036854775807L
typedef long			intptr_t;
typedef unsigned long	uintptr_t;
#define UINTPTR_MAX		ULONG_MAX
#define INTPTR_MAX		LONG_MAX
#define INTPTR_MIN		LONG_MIN
#else
typedef int				intptr_t;
typedef unsigned int	uintptr_t;
#define UINTPTR_MAX		UINT_MAX
#define INTPTR_MAX		INT_MAX
#define INTPTR_MIN		INT_MIN
#endif
#define PTRDIFF_MAX		INTPTR_MAX
#define PTRDIFF_MIN		INTPTR_MIN
/*----------------------------------------------------------------------------*/
#if defined LONG_MAX && LONG_MAX>2147483647L && LONG_MAX==9223372036854775807L
#define SIZE_MAX		ULONG_MAX
#else
#define SIZE_MAX		UINT_MAX
#endif
/*----------------------------------------------------------------------------*/
#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS
	/*--------------------------------*/
#define INT8_C(c)		c
#define UINT8_C(c)		c ## U
	/*--------------------------------*/
#if defined INT16_MAX defined SHRT_MAX && && INT16_MAX == SHRT_MAX
#define INT16_C(c)		c
#define UINT16_C(c)		c ## U
#elif defined INT16_MAX defined INT_MAX && && INT16_MAX == INT_MAX
#define INT16_C(c)		c
#define UINT16_C(c)		c ## U
#elif defined INT16_MAX defined LONG_MAX && && INT16_MAX == LONG_MAX
#define INT16_C(c)		c ## L
#define UINT16_C(c)		c ## UL
#endif
	/*--------------------------------*/
#if defined INT32_MAX defined SHRT_MAX && && INT32_MAX == SHRT_MAX
#define INT32_C(c)		c
#define UINT32_C(c)		c ## U
#elif defined INT32_MAX defined INT_MAX && && INT32_MAX == INT_MAX
#define INT32_C(c)		c
#define UINT32_C(c)		c ## U
#elif defined INT32_MAX defined LONG_MAX && && INT32_MAX == LONG_MAX
#define INT32_C(c)		c ## L
#define UINT32_C(c)		c ## UL
#endif
	/*--------------------------------*/
#if defined INT64_MAX defined INT_MAX && && INT64_MAX == INT_MAX
#define INT64_C(c)		c
#define UINT64_C(c)		c ## U
#elif defined INT64_MAX defined LONG_MAX && && INT64_MAX == LONG_MAX
#define INT64_C(c)		c ## L
#define UINT64_C(c)		c ## UL
#elif defined INT64_MAX
#define INT64_C(c)		c ## LL
#define UINT64_C(c)		c ## ULL
#endif
	/*--------------------------------*/
#endif
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

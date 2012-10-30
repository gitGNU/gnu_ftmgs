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
#ifndef umalccvbg_bigintop_h__
#define umalccvbg_bigintop_h__	1
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

/*------------------------------------------------------------------------*/
#ifndef NDEBUG
#define DBG(x) 		x
#define TIMER__		1
#define POWSTS__	1
#define CTORSTS__	1
#else
#define DBG(x)
#endif
/*----------------------------------------------------------------------------*/
#ifdef POWSTS__
#define POWSTS(x)	x
#else
#define POWSTS(x)
#endif
/*------------------------------------------------------------------------*/
/*----------------------------------*/
#include "bigint.h"
#include "bigint_cdtor.h"
/*----------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef UNUSED__
#ifdef __GNUC__
#define UNUSED__ __attribute__ ((unused))
#else
#define UNUSED__
#endif
#endif
/*----------------------------------------------------------------------------*/
#ifndef USE_RESULT__
#ifdef __GNUC__
#define USE_RESULT__ __attribute__ ((warn_unused_result))
#else
#define USE_RESULT__
#endif
#endif
/*----------------------------------------------------------------------------*/
#ifndef STATIC_ASSERT
#define STATIC_ASSERT(Expr__,Msg__) \
    extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#endif
/*----------------------------------------------------------------------------*/
/* 
 * STATIC_ASSERT RATIONALE:
 * 1.- If Static-Error then define a negative width for a bit-field
 *
 *		In C, a CONSTRAINT is a rule that the compiler must
 *		enforce. An example constraint is that C programs must not
 *		declare a bit-field with negative width [@3.5.2.1]. Tests can
 *		therefore reliably assume that programs with negative-width
 *		bit-fields are rejected by a compiler that conforms to the
 *		standard.
 *
 * 2.- The operand of sizeof is evaluated at compile-time [@3.3.3.4]
 *
 * 3.- !!sizeof(...) always yields 1 since sizeof(...) > 0
 *
 * 4.- array[...] forces the compile-time analysis of the size
 *
 * 5.- extern allows for multiple declarations of the same entity, even if 
 *		such an entity does not exist, but it does not reserve any space for
 *		it. It is only a compile-time hint.
 *
 * 6.- It is an array of ptr to functions to avoid warnings about unused
 * 		variable (in GCC)
 *
 * 7.- Msg must be a valid identifier (begins with a letter and follows
 *		letters, digits, and undescores, with no spaces)
 *
 * 8.- It is a declaration, so it must appear at file global scope
 *		or at declaration scope inside a block
 */
/*-----------------------------------------------------------------*/
#ifdef TIMER__
#define INIT_TIMER(name)								\
	do{{												\
	POWSTS(int powmod__##name = get_powmod_cnt();)		\
	clock_t time__##name = clock()
#define PRINT_TIMER(name)											\
	do{																\
		time__##name=clock()-(time__##name);						\
		POWSTS(powmod__##name = get_powmod_cnt()-(powmod__##name);)	\
		fprintf(stderr, "\t%s Time: %.8f seconds", #name,			\
			   ((double)(time__##name)/CLOCKS_PER_SEC));			\
		POWSTS(fprintf(stderr, "\tpowmod: %u", (powmod__##name));)	\
		fprintf(stderr,"\n");										\
	}while(0);														\
	}}while(0)
#else
#define INIT_TIMER(name)	do{{ void abort(void)
#define PRINT_TIMER(name)	}}while(0)
#endif
/*----------------------------------------------------------------------------*/
#ifndef EXTERN_C
#ifdef __cplusplus
#define BEGIN_EXTERN_C	extern "C" {
#define END_EXTERN_C	}
#define EXTERN_C		extern "C"
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#define EXTERN_C		extern
#endif
#endif
/*----------------------------------------------------------------------------*/
#ifdef CTORSTS__
BEGIN_EXTERN_C
extern int bi_ctor_cnt(int n);
extern int bi_dtor_cnt(int n);
extern int bi_cdtor_cntdiff();
END_EXTERN_C
#define get_bi_ctor_cnt()	bi_ctor_cnt(0)
#define get_bi_dtor_cnt()	bi_dtor_cnt(0)
#define inc_bi_ctor_cnt()	((void)bi_ctor_cnt(1))
#define inc_bi_dtor_cnt()	((void)bi_dtor_cnt(1))
#define BEG_BIGINT_CHK()									\
	int cdtorcnt__ = bi_cdtor_cntdiff()
#define END_BIGINT_CHK()												\
	do{																	\
		if (bi_cdtor_cntdiff()-cdtorcnt__ != 0) {						\
			fprintf(stderr, "CTOR_CNT: %d\tDTOR_CNT: %d\t"				\
					"CURR_DIFF: %d\tPREV_DIFF: %d\t"					\
					"DIFF: %d\n",										\
					get_bi_ctor_cnt(), get_bi_dtor_cnt(),				\
					bi_cdtor_cntdiff(),									\
					cdtorcnt__,											\
					bi_cdtor_cntdiff()-cdtorcnt__						\
					);													\
		}																\
		assert(bi_cdtor_cntdiff()-cdtorcnt__ == 0);						\
	}while(0)
/*----------------------------------*/
#undef bi_ctor
#define bi_ctor(bigint_var)						\
	(inc_bi_ctor_cnt(), mpz_init(bigint_var))
#undef bi_dtor
#define bi_dtor(bigint_var)						\
	(inc_bi_dtor_cnt(), mpz_clear(bigint_var))
/*----------------------------------*/
#else
#define bi_cdtor_cntdiff()	0
#define get_bi_ctor_cnt()	0
#define get_bi_dtor_cnt()	0
#define inc_bi_ctor_cnt()	((void)0)
#define inc_bi_dtor_cnt()	((void)0)
#define BEG_BIGINT_CHK()	EXTERN_C void abort(void)
#define END_BIGINT_CHK()		((void)0)
#endif
/*------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#ifdef POWSTS__
BEGIN_EXTERN_C
extern int powmod_cnt(int n);
END_EXTERN_C
#define get_powmod_cnt() powmod_cnt(0)
#define inc_powmod_cnt() ((void)powmod_cnt(1))
#define dec_powmod_cnt(x) ((void)powmod_cnt(-(x)))
#else
#define get_powmod_cnt() 0
#define inc_powmod_cnt() ((void)0)
#define dec_powmod_cnt(x) ((void)0)
#endif
/*-----------------------------------------------------------------*/
#ifndef BOOL_T_DEFINED__
#define BOOL_T_DEFINED__	1
typedef char bool_t;
#endif
#ifndef TRUE
#define TRUE	1
#elif TRUE == 0
#error "Bad definition of symbol TRUE"
#endif
#ifndef FALSE
#define FALSE	0
#elif FALSE != 0
#error "Bad definition of symbol FALSE"
#endif
	/*-------------------------------------------------------------*/
	/*- EXTERN FUNCTIONS ------------------------------------------*/
	/*-------------------------------------------------------------*/
BEGIN_EXTERN_C
extern bool_t bi_iscoprime(const bigint_t o1, const bigint_t o2);
extern bool_t bi_is2k(const bigint_t o1);
END_EXTERN_C
/*-----------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------*/

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
#ifndef umalccvbg_bigint_h__
#define umalccvbg_bigint_h__	1
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include <gmp.h>

/*-----------------------------------------------------------------*/
/* 
 * This file is a wrapper to the multi-precision Library (GMP)
 */
/*-----------------------------------------------------------------*/
#ifndef NDEBUG
#define DBG(x) 		x
#define TIMER__		1
#define POWSTS__	1
#else
#define DBG(x)
#endif
/*----------------------------------------------------------------------------*/
#ifdef POWSTS__
#define POWSTS(x)	x
#else
#define POWSTS(x)
#endif
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
#define STATIC_ASSERT(Expr__,Msg__) \
    extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
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
#ifdef __cplusplus
#define BEGIN_EXTERN_C	extern "C" {
#define END_EXTERN_C	}
#define EXTERN_C		extern "C"
#else
#define BEGIN_EXTERN_C
#define END_EXTERN_C
#define EXTERN_C		extern
#endif
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
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
	int cdtorcnt__ = get_bi_ctor_cnt()-get_bi_dtor_cnt()
#define END_BIGINT_CHK()													\
	do{																	\
		if (get_bi_ctor_cnt()-get_bi_dtor_cnt()-cdtorcnt__ != 0) {		\
			fprintf(stderr, "CTOR_CNT: %d\tDTOR_CNT: %d\t"				\
					"CURR_DIFF: %d\tPREV_DIFF: %d\t"					\
					"DIFF: %d\n",										\
					get_bi_ctor_cnt(), get_bi_dtor_cnt(),				\
					get_bi_ctor_cnt()-get_bi_dtor_cnt(),				\
					cdtorcnt__,											\
					get_bi_ctor_cnt()-get_bi_dtor_cnt()-cdtorcnt__ 		\
					);													\
		}																\
		assert(get_bi_ctor_cnt()-get_bi_dtor_cnt()-cdtorcnt__ == 0);	\
	}while(0)
#else
#define bi_cdtor_cntdiff()	0
#define get_bi_ctor_cnt()	0
#define get_bi_dtor_cnt()	0
#define inc_bi_ctor_cnt()	((void)0)
#define inc_bi_dtor_cnt()	((void)0)
#define BEG_BIGINT_CHK()	EXTERN_C void abort(void)
#define END_BIGINT_CHK()		((void)0)
#endif

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
/*-----------------------------------------------------------------*/
typedef mpz_t bigint_t;
/*-----------------------------------------------------------------*/
/* 
 * bigint_t constructors and destructor
 */
#define bi_ctor(bigint_var)						\
	(inc_bi_ctor_cnt(), mpz_init(bigint_var))
#define bi_dtor(bigint_var)						\
	(inc_bi_dtor_cnt(), mpz_clear(bigint_var))
/*------------------------------------*/
#define bigint_t_ctor(bigint_ptr)	(bi_ctor(*(bigint_ptr)))
#define bigint_t_dtor(bigint_ptr)	(bi_dtor(*(bigint_ptr)))
/*-----------------------------------------------------------------*/
/* 
 * bigint_t assignments
 */
#define bi_asg_si(bigint_var,  long_o1)			\
	mpz_set_si(bigint_var, long_o1)
#define bi_asg_ui(bigint_var,  ulong_o1)		\
	mpz_set_ui(bigint_var, ulong_o1)
#define bi_asg_str(bigint_var, str_o1)			\
	mpz_set_str(bigint_var, str_o1, 0)
#define bi_asg(bigint_var,  bigint_o1)		\
	mpz_set(bigint_var, bigint_o1)
/*-----------------------------------------------------------------*/
/* 
 * bigint_t conversions
 */
#define bi2long(bigint_o1)						\
	mpz_get_si(bigint_o1)
#define bi2ulong(bigint_o1)						\
	mpz_get_ui(bigint_o1)
#define bi2str(bigint_o1, base)					\
	mpz_get_str(NULL, base, bigint_o1)
#define bi2strx(bigint_o1, base, buff, len)							\
	(assert((buff!=NULL)&&(len>=mpz_sizeinbase(bigint_o1,base)+2)),	\
	 (void)len,	/* to avoid unused warning */						\
	 mpz_get_str(buff, base, bigint_o1))
/*-----------------------------------------------------------------*/
/* 
 * bigint_t swap
 */
#define bi_swap(bigint_var1, bigint_var2)		\
	mpz_swap(bigint_var1, bigint_var2)
/*-----------------------------------------------------------------*/
/* 
 * bigint_t properties
 */
#define bi_sign(bigint_o1)						\
	mpz_sgn(bigint_o1)
#define bi_iszero(bigint_o1)					\
	(mpz_cmp_si(bigint_o1, 0)==0)
#define bi_isodd(bigint_o1)						\
	mpz_odd_p(bigint_o1)
#define bi_iseven(bigint_o1)					\
	mpz_even_p(bigint_o1)
/*------------------------------------*/
#define bi_compare(bigint_o1, bigint_o2)		\
	mpz_cmp(bigint_o1, bigint_o2)
#define bi_compare_si(bigint_o1, long_o2)		\
	mpz_cmp_si(bigint_o1, long_o2)
#define bi_compare_ui(bigint_o1, ulong_o2)		\
	mpz_cmp_ui(bigint_o1, ulong_o2)
/*------------------------------------*/
#define bi_equals(bigint_o1, bigint_o2)			\
	(mpz_cmp(bigint_o1, bigint_o2) == 0)
#define bi_equals_si(bigint_o1, long_o2)		\
	(mpz_cmp_si(bigint_o1, long_o2) == 0)
#define bi_equals_ui(bigint_o1, ulong_o2)		\
	(mpz_cmp_ui(bigint_o1, ulong_o2) == 0)
/*------------------------------------*/
#define bi_less(bigint_o1, bigint_o2)			\
	(mpz_cmp(bigint_o1, bigint_o2) < 0)
#define bi_less_si(bigint_o1, long_o2)			\
	(mpz_cmp_si(bigint_o1, long_o2) < 0)
#define bi_less_ui(bigint_o1, ulong_o2)			\
	(mpz_cmp_ui(bigint_o1, ulong_o2) < 0)
/*------------------------------------*/
#define bi_lesseq(bigint_o1, bigint_o2)			\
	(mpz_cmp(bigint_o1, bigint_o2) <= 0)
#define bi_lesseq_si(bigint_o1, long_o2)		\
	(mpz_cmp_si(bigint_o1, long_o2) <= 0)
#define bi_lesseq_ui(bigint_o1, ulong_o2)		\
	(mpz_cmp_ui(bigint_o1, ulong_o2) <= 0)
/*------------------------------------*/
#define bi_greater(bigint_o1, bigint_o2)		\
	(mpz_cmp(bigint_o1, bigint_o2) > 0)
#define bi_greater_si(bigint_o1, long_o2)		\
	(mpz_cmp_si(bigint_o1, long_o2) > 0)
#define bi_greater_ui(bigint_o1, ulong_o2)		\
	(mpz_cmp_ui(bigint_o1, ulong_o2) > 0)
/*------------------------------------*/
#define bi_greatereq(bigint_o1, bigint_o2)		\
	(mpz_cmp(bigint_o1, bigint_o2) >= 0)
#define bi_greatereq_si(bigint_o1, long_o2)		\
	(mpz_cmp_si(bigint_o1, long_o2) >= 0)
#define bi_greatereq_ui(bigint_o1, ulong_o2)	\
	(mpz_cmp_ui(bigint_o1, ulong_o2) >= 0)
/*----------------------------	*/
#define bi_mkodd(bigint_var)					\
	mpz_setbit(bigint_var, 0)
/*----------------------------------*/
#define bi_isdiv(bigint_o1, bigint_o2)								\
	(assert(!bi_iszero(bigint_o2)), mpz_divisible_p(bigint_o1, bigint_o2))
#define bi_isdiv_ui(bigint_o1, ulong_o1)							\
	(assert(ulong_o1!=0), mpz_divisible_ui_p(bigint_o1, ulong_o1))
/*----------------------------------*/
#define bi_iscongruent(bigint_o1, bigint_o2, bigint_n)	\
	mpz_congruent_p(bigint_o1, bigint_o2, bigint_n)
#define bi_iscongruent_ui(bigint_o1, ulong_o2, ulong_n)	\
	mpz_congruent_ui_p(bigint_o1, ulong_o2, ulong_n)
/*----------------------------------*/
#define bi_isprime(bigint_o1, int_ntests)		\
	mpz_probab_prime_p(bigint_o1, int_ntests)
/*-----------------------------------------------------------------*/
#define bi_nextprime(bigint_var, bigint_o1)		\
	mpz_nextprime(bigint_var, bigint_o1)
/*-----------------------------------------------------------------*/
/* 
 * least common multiple
 */
/* var = lcm(o1, o2) // least common multiple	*/
#define bi_lcm(bigint_var, bigint_o1, bigint_o2)	\
	mpz_lcm(bigint_var, bigint_o1, bigint_o2)
/* var = lcm(o1, o2) // least common multiple	*/
#define bi_lcm_ui(bigint_var, bigint_o1, ulong_o2)	\
	mpz_lcm_ui(bigint_var, bigint_o1, ulong_o2
/*-----------------------------------------------------------------*/
/* 
 * greatest common divisor
 */
/* var = gcd(o1, o2) // greatest common divisor	*/
#define bi_gcd(bigint_var, bigint_o1, bigint_o2)	\
	mpz_gcd(bigint_var, bigint_o1, bigint_o2)
/* var = gcd(o1, o2) // greatest common divisor	*/
#define bi_gcd_ui(bigint_o1, ulong_o2)			\
	mpz_gcd_ui(NULL, bigint_o1, ulong_o2);
/*-----------------------------------------------------------------*/
/* 
 * import and export
 */
#define bi_import_abs(bigint_var, buff, len)		\
	mpz_import(bigint_var, len, 1, 1, 1, 0, buff)
#define bi_export_abs(bigint_n, ptr_len)				\
	(assert(bi_sign(bigint_n)>=0),						\
	 mpz_export(NULL, ptr_len, 1, 1, 1, 0, bigint_n))
#define bi_export_absx(bigint_n, buff, ptr_len)							\
	(assert((bi_sign(bigint_n)>=0)&&(buff!=NULL)&&(*ptr_len>=bi_nbytes_excl_sgn(bigint_n))),	\
	 mpz_export(buff, ptr_len, 1, 1, 1, 0, bigint_n))
#define bi_export_absxx(bigint_n, buff, ptr_len)							\
	(assert((buff!=NULL)&&(*ptr_len>=bi_nbytes_excl_sgn(bigint_n))),	\
	 mpz_export(buff, ptr_len, 1, 1, 1, 0, bigint_n))
/*----------------------------------*/
#define bi_scanf(file_stream, bigint_var, base)	\
	mpz_inp_str(bigint_var, file_stream, base)
#define bi_printf(file_stream, bigint_var, base)	\
	mpz_out_str(file_stream, base, bigint_var)
/*-----------------------------------------------------------------*/
/* 
 * Bit operations
 */
/*----------------------------	*/
#define bi_nbits(bigint_o1)						\
	((unsigned)mpz_sizeinbase(bigint_o1,2))
#define bi_nbytes(bigint_o1)											\
	((bi_nbits(bigint_o1)+CHAR_BIT)/CHAR_BIT) /* (NBits + Sign-Bit + RoundUp) */
#define bi_nbytes_excl_sgn(bigint_o1)									\
	((bi_nbits(bigint_o1)+(CHAR_BIT-1))/CHAR_BIT) /* (NBits + RoundUp) */
/*----------------------------------*/
#define bi_bit(bigint_o1, ulong_i)				\
	mpz_tstbit(bigint_o1, ulong_i)
#define bi_setbit(bigint_var, ulong_i)			\
	mpz_setbit(bigint_var, ulong_i)
#define bi_clrbit(bigint_var, ulong_i)			\
	mpz_clrbit(bigint_var, ulong_i)
#define bi_combit(bigint_var, ulong_i)			\
	mpz_combit(bigint_var, ulong_i)
/*----------------------------------*/
/* var = o1 AND o2	*/
#define bi_and(bigint_var, bigint_o1, bigint_o2)	\
	mpz_and(bigint_var, bigint_o1, bigint_o2)
/*----------------------------------*/
/* var = o1 OR o2	*/
#define bi_ior(bigint_var, bigint_o1, bigint_o2)	\
	mpz_ior(bigint_var, bigint_o1, bigint_o2)
/*----------------------------------*/
/* var = o1 XOR o2	*/
#define bi_xor(bigint_var, bigint_o1, bigint_o2)	\
	mpz_xor(bigint_var, bigint_o1, bigint_o2)
/*----------------------------------*/
/* var = complement_one(o1)	*/
#define bi_ones_compl(bigint_var, bigint_o1)	\
	mpz_com(bigint_var, bigint_o1)
/*----------------------------------*/
/* var = o1 * (2 ^ k)	*/
#define bi_lshift(bigint_var, bigint_o1, ulong_k)	\
	mpz_mul_2exp(bigint_var, bigint_o1, ulong_k)
/* var = o1 / (2 ^ k)	*/
#define bi_rshift(bigint_var, bigint_o1, ulong_k)	\
	mpz_fdiv_q_2exp(bigint_var, bigint_o1, ulong_k)
/*-----------------------------------------------------------------*/
/* 
 * Arithmetic operations
 */
/*-----------------------------------------------------------------*/
/* var = abs(o1)	*/
#define bi_abs(bigint_var, bigint_o1)			\
	mpz_abs(bigint_var, bigint_o1)
/* var = -o1	*/
#define bi_negate(bigint_var, bigint_o1)		\
	mpz_neg(bigint_var, bigint_o1)
/*-----------------------------------------------------------------*/
/* var = o1 % o2	*/
#define bi_mod(bigint_var, bigint_o1, bigint_o2)	\
	(assert(!bi_iszero(bigint_o2)),					\
	 mpz_mod(bigint_var, bigint_o1, bigint_o2))
/* var = o1 % o2	*/
#define bi_mod_ui(bigint_var, bigint_o1, ulong_o2)	\
	(assert(ulong_o2 != 0),							\
	 mpz_mod_ui(bigint_var, bigint_o1, ulong_o2))
/* return (o1 % 2)	*/
#define bi_mod2(bigint_o1)						\
	(!mpz_even_p(bigint_o1))
	/* mpz_fdiv_ui(bigint_o1, 2) */
/* var = o1 % (2 ^ k)	*/
#define bi_mod2k(bigint_var, bigint_o1, ulong_k)	\
	mpz_fdiv_r_2exp(bigint_var, bigint_o1, ulong_k)
/*-----------------------------------------------------------------*/
/* var = o1 + o2	*/
#define bi_add(bigint_var, bigint_o1, bigint_o2)	\
	mpz_add(bigint_var, bigint_o1, bigint_o2)
/* var = o1 + o2	*/
#define bi_add_ui(bigint_var, bigint_o1, ulong_o2)	\
	mpz_add_ui(bigint_var, bigint_o1, ulong_o2)
/* var = o1 + o2 (mod n)	*/
#define bi_addmod(bigint_var, bigint_o1, bigint_o2, bigint_n)			\
	do{																	\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_o2)>=0)			\
			   &&(bi_sign(bigint_n)>0));								\
		assert(bi_less(bigint_o1, bigint_n)&&bi_less(bigint_o2, bigint_n));	\
		bi_add(bigint_var, bigint_o1, bigint_o2);						\
		bi_mod(bigint_var, bigint_var, bigint_n);						\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 - o2	*/
#define bi_sub(bigint_var, bigint_o1, bigint_o2)	\
	mpz_sub(bigint_var, bigint_o1, bigint_o2)
/* var = o1 - o2	*/
#define bi_sub_ui(bigint_var, bigint_o1, ulong_o2)	\
	mpz_sub_ui(bigint_var, bigint_o1, ulong_o2)
/* var = o1 - o2	*/
#define bi_sub_uibi(bigint_var, ulong_o1, bigint_o2)	\
	mpz_ui_sub(bigint_var, ulong_o1, bigint_o2)
/* var = o1 - o2	*/
#define bi_subpos(bigint_var, bigint_o1, bigint_o2)		\
	do{														\
		assert((bi_sign(bigint_o2)>=0)						\
			   &&bi_greatereq(bigint_o1, bigint_o2));		\
		bi_sub(bigint_var, bigint_o1, bigint_o2);			\
	}while(0)
/* var = o1 - o2 (mod n)	*/
#define bi_submod(bigint_var, bigint_o1, bigint_o2, bigint_n)	\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_o2)>=0)	\
			   &&(bi_sign(bigint_n)>0));						\
		bi_sub(bigint_var, bigint_o1, bigint_o2);				\
		bi_mod(bigint_var, bigint_var, bigint_n);				\
	}while(0)
/*-----------------------------------------------------------------*/
/* var += o1 * o2	*/
#define bi_addmul(bigint_var, bigint_o1, bigint_o2)					\
	do{																	\
		mpz_addmul(bigint_var, bigint_o1, bigint_o2);					\
	}while(0)
/* var += o1 * o2	*/
#define bi_addmul_ui(bigint_var, bigint_o1, ulong_o2)					\
	do{																	\
		mpz_addmul_ui(bigint_var, bigint_o1, ulong_o2);					\
	}while(0)
/* var -= o1 * o2	*/
#define bi_submul(bigint_var, bigint_o1, bigint_o2)					\
	do{																	\
		mpz_submul(bigint_var, bigint_o1, bigint_o2);					\
	}while(0)
/* var -= o1 * o2	*/
#define bi_submul_ui(bigint_var, bigint_o1, ulong_o2)					\
	do{																	\
		mpz_submul_ui(bigint_var, bigint_o1, ulong_o2);					\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 * o2	*/
#define bi_mul(bigint_var, bigint_o1, bigint_o2)						\
	do{																	\
		mpz_mul(bigint_var, bigint_o1, bigint_o2);						\
	}while(0)
/* var = o1 * o2	*/
#define bi_mul_si(bigint_var, bigint_o1, long_o2)	\
	mpz_mul_si(bigint_var, bigint_o1, long_o2)
/* var = o1 * o2	*/
#define bi_mul_ui(bigint_var, bigint_o1, ulong_o2)	\
	mpz_mul_ui(bigint_var, bigint_o1, ulong_o2)
/* var = o1 * 2	*/
#define bi_mul2(bigint_var, bigint_o1)			\
	mpz_mul_2exp(bigint_var, bigint_o1, 1)
/* var = o1 * (2 ^ k)	*/
#define bi_mul2k(bigint_var, bigint_o1, ulong_k)	\
	mpz_mul_2exp(bigint_var, bigint_o1, ulong_k)
/* var = o1 * o2 (mod n)	*/
#define bi_mulmod(bigint_var, bigint_o1, bigint_o2, bigint_n)	\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_o2)>=0)	\
			   &&(bi_sign(bigint_n)>0));						\
		bi_mul(bigint_var, bigint_o1, bigint_o2);				\
		bi_mod(bigint_var, bigint_var, bigint_n);				\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 * o1 ; // var = o1 ^ 2	*/
#define bi_sq(bigint_var, bigint_o1)									\
	do{																	\
		mpz_mul(bigint_var, bigint_o1, bigint_o1);						\
	}while(0)
/* var = o1 * o1 (mod n) ; // var = o1 ^ 2 (mod n)	*/
#define bi_sqmod(bigint_var, bigint_o1, bigint_n)				\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_n)>0));	\
		bi_sq(bigint_var, bigint_o1);							\
		bi_mod(bigint_var, bigint_var, bigint_n);				\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 / o2 ; rem = o1 % o2	*/
#define bi_div(bigint_var, bigint_o1, bigint_o2, bigint_rem)		\
	do{																\
		assert(!bi_iszero(bigint_o2));								\
		mpz_fdiv_qr(bigint_var, bigint_rem, bigint_o1, bigint_o2);	\
	}while(0)
/* var = o1 / o2 	*/
#define bi_divq(bigint_var, bigint_o1, bigint_o2)		\
	do{													\
		assert(!bi_iszero(bigint_o2));					\
		mpz_fdiv_q(bigint_var, bigint_o1, bigint_o2);	\
	}while(0)
/* var = o1 / o2 ; rem = o1 % o2	*/
#define bi_div_ui(bigint_var, bigint_o1, ulong_o2, ulong_ref_rem)		\
	do{																	\
		assert(ulong_o2 != 0);											\
		ulong_ref_rem = mpz_fdiv_q_ui(bigint_var, bigint_o1, ulong_o2);	\
	}while(0)
/* var = o1 / 2	*/
#define bi_div2(bigint_var, bigint_o1)		\
	mpz_fdiv_q_2exp(bigint_var, bigint_o1, 1)
/* var = o1 / (2 ^ k)	*/
#define bi_div2k(bigint_var, bigint_o1, ulong_k)	\
	mpz_fdiv_q_2exp(bigint_var, bigint_o1, ulong_k)
/* var = o1 / o2 (mod n)	*/
#define bi_divmod(bigint_var, bigint_o1, bigint_o2, bigint_n)	\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_o2)>=0)	\
			   &&(bi_sign(bigint_n)>0));						\
		assert(bi_iscoprime(bigint_o2, bigint_n));				\
		bi_divq(bigint_var, bigint_o1, bigint_o2);				\
		bi_mod(bigint_var, bigint_var, bigint_n);				\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = inv(o1) (mod n)	*/
#define bi_invmod(bigint_var, bigint_o1, bigint_n)				\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_n)>0));	\
		assert(bi_iscoprime(bigint_o1, bigint_n));				\
		if (!mpz_invert(bigint_var, bigint_o1, bigint_n)) {		\
			mpz_set_ui(bigint_var, 0);							\
		}														\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 ^ o2	*/
#define bi_pow(bigint_var, bigint_o1, ulong_o2)						\
	do{																	\
		mpz_pow_ui(bigint_var, bigint_o1, ulong_o2);					\
	}while(0)
/* var = 2 ^ k	*/
#define bi_pow2k(bigint_var, ulong_k)			\
	do{											\
		mpz_set_ui(bigint_var, 0);				\
		mpz_setbit(bigint_var, ulong_k);		\
	}while(0)
/* var = o1 ^ o2 (mod n)	*/
#if __GNU_MP_VERSION_ >= 5
#define bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n)		\
	do{																	\
		assert((bi_sign(bigint_o2)>0)&&(bi_sign(bigint_n)>0)			\
			   &&(bi_isodd(bigint_n)));									\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		inc_powmod_cnt();												\
		mpz_powm_sec(bigint_var, bigint_o1, bigint_o2, bigint_n);		\
	}while(0)
#else
#define bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n)		\
	do{																	\
		assert((bi_sign(bigint_o2)>0)&&(bi_sign(bigint_n)>0)			\
			   &&(bi_isodd(bigint_n)));									\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		inc_powmod_cnt();												\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
#endif
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod(bigint_var, bigint_o1, bigint_o2, bigint_n)			\
	do{																	\
		assert((bi_sign(bigint_n)>0));								\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		inc_powmod_cnt();												\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod_ui(bigint_var, bigint_o1, ulong_o2, bigint_n)			\
	do{																	\
		assert((bi_sign(bigint_n)>0));								\
		/*assert((o2>=0)||(coprime(n, o1^(-o2)))));	*/					\
		mpz_powm_ui(bigint_var, bigint_o1, ulong_o2, bigint_n);			\
	}while(0)
/* var = 2 ^ o1 (mod n)	*/
#define bi_powmod2k(bigint_var, bigint_o1, bigint_n)					\
	do{																	\
		assert((bi_sign(bigint_n)>0));								\
		/*assert((bi_sign(bigint_o1)>=0)||(coprime(n, 2^(-o1)))));	*/	\
		mpz_set_ui(bigint_var, 2);										\
		bi_powmod_sec(bigint_var, bigint_var, bigint_o1, bigint_n);			\
	}while(0)
/* var = (o1^o2) * (o3^o4) (mod n)	*/
#define bi_mul_powmod(bigint_var, bigint_o1, bigint_o2,			\
					 bigint_o3, bigint_o4, bigint_n)			\
	do{															\
		bigint_t aux;											\
		assert((bi_sign(bigint_o2)>=0)&&(bi_sign(bigint_o4)>=0)	\
			   &&(bi_sign(bigint_n)>0));						\
		bi_ctor(aux);											\
		bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n);	\
		bi_powmod_sec(aux, bigint_o3, bigint_o4, bigint_n);			\
		bi_mulmod(bigint_var, bigint_var, aux, bigint_n);		\
		bi_dtor(aux);											\
	}while(0)
/*----------------------------	*/
#ifndef NDEBUG
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod_dbg(bigint_var, bigint_o1, bigint_o2, bigint_n)		\
	do{																	\
		assert((bi_sign(bigint_n)>0));								\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
#endif
	/*-------------------------------------------------------------*/
	/*- EXTERN FUNCTIONS ------------------------------------------*/
	/*-------------------------------------------------------------*/
BEGIN_EXTERN_C
extern bool_t bi_iscoprime(const bigint_t o1, const bigint_t o2);
extern bool_t bi_is2k(const bigint_t o1);
END_EXTERN_C
	/*-------------------------------------------------------------*/
	/*- BEG_VAR/END_VAR -------------------------------------------------*/
	/*-------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define BEG_VAR_A_bigint_t(v_a_r_1__, n_e_l_m_s__)							\
	do{{{{{																\
	bigint_t v_a_r_1__[n_e_l_m_s__];									\
	{int ctr_i__; for (ctr_i__ = 0; ctr_i__ < (n_e_l_m_s__); ++ctr_i__) { \
		bi_ctor(v_a_r_1__[ctr_i__]);									\
	}}																	\
	do{																	\
	typedef void Void_XTOR_A_bigint_t_##v_a_r_1__##n_e_l_m_s__ ;		\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_1_bigint_t(v_a_r_1__)					\
	do{{{{{											\
	bigint_t v_a_r_1__;								\
	bi_ctor(v_a_r_1__);								\
	do{												\
	typedef void Void_XTOR_1_bigint_t_##v_a_r_1__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_2_bigint_t(v_a_r_1__,								\
						v_a_r_2__)								\
	do{{{{{														\
	bigint_t v_a_r_1__;											\
	bigint_t v_a_r_2__;											\
	bi_ctor(v_a_r_1__);											\
	bi_ctor(v_a_r_2__);											\
	do{															\
	typedef void Void_XTOR_2_bigint_t_##v_a_r_1__##v_a_r_2__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_3_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	do{																	\
	typedef void Void_XTOR_3_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_4_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	do{																	\
	typedef void Void_XTOR_4_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_5_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	do{																	\
	typedef void Void_XTOR_5_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_6_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	do{																	\
	typedef void Void_XTOR_6_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_7_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	do{																	\
	typedef void Void_XTOR_7_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_8_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	do{																	\
	typedef void Void_XTOR_8_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_9_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__,										\
						v_a_r_9__)										\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	do{																	\
	typedef void Void_XTOR_9_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ ;	\
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_10_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	do{																	\
	typedef void Void_XTOR_10_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_11_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	do{																	\
	typedef void Void_XTOR_11_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_12_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	do{																	\
	typedef void Void_XTOR_12_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_13_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	do{																	\
	typedef void Void_XTOR_13_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_14_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	do{																	\
	typedef void Void_XTOR_14_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_15_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bigint_t v_a_r_15__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	bi_ctor(v_a_r_15__);												\
	do{																	\
	typedef void Void_XTOR_15_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
#define BEG_VAR_16_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__,									\
						 v_a_r_16__)									\
	do{{{{{																\
	bigint_t v_a_r_1__;													\
	bigint_t v_a_r_2__;													\
	bigint_t v_a_r_3__;													\
	bigint_t v_a_r_4__;													\
	bigint_t v_a_r_5__;													\
	bigint_t v_a_r_6__;													\
	bigint_t v_a_r_7__;													\
	bigint_t v_a_r_8__;													\
	bigint_t v_a_r_9__;													\
	bigint_t v_a_r_10__;												\
	bigint_t v_a_r_11__;												\
	bigint_t v_a_r_12__;												\
	bigint_t v_a_r_13__;												\
	bigint_t v_a_r_14__;												\
	bigint_t v_a_r_15__;												\
	bigint_t v_a_r_16__;												\
	bi_ctor(v_a_r_1__);													\
	bi_ctor(v_a_r_2__);													\
	bi_ctor(v_a_r_3__);													\
	bi_ctor(v_a_r_4__);													\
	bi_ctor(v_a_r_5__);													\
	bi_ctor(v_a_r_6__);													\
	bi_ctor(v_a_r_7__);													\
	bi_ctor(v_a_r_8__);													\
	bi_ctor(v_a_r_9__);													\
	bi_ctor(v_a_r_10__);												\
	bi_ctor(v_a_r_11__);												\
	bi_ctor(v_a_r_12__);												\
	bi_ctor(v_a_r_13__);												\
	bi_ctor(v_a_r_14__);												\
	bi_ctor(v_a_r_15__);												\
	bi_ctor(v_a_r_16__);												\
	do{																	\
	typedef void Void_XTOR_16_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ ; \
	EXTERN_C void abort(void)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define END_VAR_A_bigint_t(v_a_r_1__, n_e_l_m_s__)							\
	{EXTERN_C Void_XTOR_A_bigint_t_##v_a_r_1__##n_e_l_m_s__ abort(void);} \
	}while(0);															\
	{int ctr_i__; for (ctr_i__ = (n_e_l_m_s__)-1; ctr_i__ >= 0 ; --ctr_i__) { \
			bi_dtor(v_a_r_1__[ctr_i__]);								\
	}}																	\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_1_bigint_t(v_a_r_1__)										\
	{EXTERN_C Void_XTOR_1_bigint_t_##v_a_r_1__ abort(void);}			\
	}while(0);															\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_2_bigint_t(v_a_r_1__,										\
						v_a_r_2__)										\
	{EXTERN_C Void_XTOR_2_bigint_t_##v_a_r_1__##v_a_r_2__ abort(void);}	\
		}while(0);														\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_3_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__)										\
	{EXTERN_C Void_XTOR_3_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_4_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__)										\
	{EXTERN_C Void_XTOR_4_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_5_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__)										\
	{EXTERN_C Void_XTOR_5_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_6_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__)										\
	{EXTERN_C Void_XTOR_6_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_7_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__)										\
	{EXTERN_C Void_XTOR_7_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_8_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__)										\
	{EXTERN_C Void_XTOR_8_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_9_bigint_t(v_a_r_1__,										\
						v_a_r_2__,										\
						v_a_r_3__,										\
						v_a_r_4__,										\
						v_a_r_5__,										\
						v_a_r_6__,										\
						v_a_r_7__,										\
						v_a_r_8__,										\
						v_a_r_9__)										\
	{EXTERN_C Void_XTOR_9_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_10_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__)									\
	{EXTERN_C Void_XTOR_10_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_11_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__)									\
	{EXTERN_C Void_XTOR_11_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_12_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__)									\
	{EXTERN_C Void_XTOR_12_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_13_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__)									\
	{EXTERN_C Void_XTOR_13_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_14_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__)									\
	{EXTERN_C Void_XTOR_14_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_15_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__)									\
	{EXTERN_C Void_XTOR_15_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_15__);												\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/
#define END_VAR_16_bigint_t(v_a_r_1__,										\
						 v_a_r_2__,										\
						 v_a_r_3__,										\
						 v_a_r_4__,										\
						 v_a_r_5__,										\
						 v_a_r_6__,										\
						 v_a_r_7__,										\
						 v_a_r_8__,										\
						 v_a_r_9__,										\
						 v_a_r_10__,									\
						 v_a_r_11__,									\
						 v_a_r_12__,									\
						 v_a_r_13__,									\
						 v_a_r_14__,									\
						 v_a_r_15__,									\
						 v_a_r_16__)									\
	{EXTERN_C Void_XTOR_16_bigint_t_##v_a_r_1__##v_a_r_2__##v_a_r_3__##v_a_r_4__##v_a_r_5__##v_a_r_6__##v_a_r_7__##v_a_r_8__##v_a_r_9__##v_a_r_10__##v_a_r_11__##v_a_r_12__##v_a_r_13__##v_a_r_14__##v_a_r_15__##v_a_r_16__ abort(void);} \
		}while(0);														\
	bi_dtor(v_a_r_16__);												\
	bi_dtor(v_a_r_15__);												\
	bi_dtor(v_a_r_14__);												\
	bi_dtor(v_a_r_13__);												\
	bi_dtor(v_a_r_12__);												\
	bi_dtor(v_a_r_11__);												\
	bi_dtor(v_a_r_10__);												\
	bi_dtor(v_a_r_9__);													\
	bi_dtor(v_a_r_8__);													\
	bi_dtor(v_a_r_7__);													\
	bi_dtor(v_a_r_6__);													\
	bi_dtor(v_a_r_5__);													\
	bi_dtor(v_a_r_4__);													\
	bi_dtor(v_a_r_3__);													\
	bi_dtor(v_a_r_2__);													\
	bi_dtor(v_a_r_1__);													\
	}}}}}while(0)
/*----------------------------------------------------------------------------*/

	/*-------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------*/

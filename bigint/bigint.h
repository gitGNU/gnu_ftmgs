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
#include <assert.h>
#include <gmp.h>
/*-----------------------------------------------------------------*/
/* 
 * This file is a wrapper to the multi-precision Library (GMP)
 */
/*-----------------------------------------------------------------*/
#ifndef POWSTS
#define POWSTS(x)
#endif
/*-----------------------------------------------------------------*/
typedef mpz_t bigint_t;
/*-----------------------------------------------------------------*/
/* 
 * bigint_t constructor and destructor
 */
#define bi_ctor(bigint_var)		(mpz_init(bigint_var))
#define bi_dtor(bigint_var)		(mpz_clear(bigint_var))
/*------------------------------------*/
#define bigint_t_ctor(bigint_ptr)	(bi_ctor(*(bigint_ptr)))
#define bigint_t_dtor(bigint_ptr)	(bi_dtor(*(bigint_ptr)))
/*-----------------------------------------------------------------*/
/*
 * For security reasons, sometimes may be necessary to clear to zero
 * the memory hold by a secret number before being discarded
 *
 * This is a low-level implementation that depends on the GMP
 * implementation (not on the public interface)
 */
#define bi_clear_zero(bint_var)											\
	do{																	\
		if ((bint_var)->_mp_d != NULL) {								\
			memset((bint_var)->_mp_d, 0,								\
				   (sizeof(*(bint_var)->_mp_d)							\
					* (size_t)(bint_var)->_mp_alloc));					\
		}																\
	}while(0)
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
#define bi_asg(bigint_var,  bigint_o1)			\
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
#define bi_export_absx(bigint_n, buff, ptr_len)				\
	(assert((bi_sign(bigint_n)>=0)&&(buff!=NULL)			\
			&&(*ptr_len>=bi_nbytes_excl_sgn(bigint_n))),	\
	 mpz_export(buff, ptr_len, 1, 1, 1, 0, bigint_n))
#define bi_export_absxx(bigint_n, buff, ptr_len)						\
	(assert((buff!=NULL)&&(*ptr_len>=bi_nbytes_excl_sgn(bigint_n))),	\
	 mpz_export(buff, ptr_len, 1, 1, 1, 0, bigint_n))
/*----------------------------------*/
#define bi_printf(file_stream, bigint_var, base)	\
	mpz_out_str(file_stream, base, bigint_var)
#define bi_scanf(file_stream, bigint_var, base)	\
	mpz_inp_str(bigint_var, file_stream, base)
#define bi_sscanf(string, bigint_var)			\
	gmp_sscanf(string, " %Zi", bigint_var);
/*-----------------------------------------------------------------*/
/* 
 * Bit operations
 */
/*----------------------------	*/
#define bi_nbits(bigint_o1)						\
	((unsigned)mpz_sizeinbase(bigint_o1,2))
#define bi_nbytes(bigint_o1)											\
	((bi_nbits(bigint_o1)+CHAR_BIT)/CHAR_BIT) /* (NBits+Sign-Bit+RoundUp) */
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
		assert(bi_less(bigint_o1,bigint_n)&&bi_less(bigint_o2,bigint_n)); \
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
#define bi_subpos(bigint_var, bigint_o1, bigint_o2)			\
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
#define bi_addmul(bigint_var, bigint_o1, bigint_o2)		\
	do{													\
		mpz_addmul(bigint_var, bigint_o1, bigint_o2);	\
	}while(0)
/* var += o1 * o2	*/
#define bi_addmul_ui(bigint_var, bigint_o1, ulong_o2)	\
	do{													\
		mpz_addmul_ui(bigint_var, bigint_o1, ulong_o2);	\
	}while(0)
/* var -= o1 * o2	*/
#define bi_submul(bigint_var, bigint_o1, bigint_o2)		\
	do{													\
		mpz_submul(bigint_var, bigint_o1, bigint_o2);	\
	}while(0)
/* var -= o1 * o2	*/
#define bi_submul_ui(bigint_var, bigint_o1, ulong_o2)	\
	do{													\
		mpz_submul_ui(bigint_var, bigint_o1, ulong_o2);	\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 * o2	*/
#define bi_mul(bigint_var, bigint_o1, bigint_o2)	\
	do{												\
		mpz_mul(bigint_var, bigint_o1, bigint_o2);	\
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
#define bi_sq(bigint_var, bigint_o1)				\
	do{												\
		mpz_mul(bigint_var, bigint_o1, bigint_o1);	\
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
		/*assert(bi_iscoprime(bigint_o2, bigint_n));*/			\
		bi_divq(bigint_var, bigint_o1, bigint_o2);				\
		bi_mod(bigint_var, bigint_var, bigint_n);				\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = inv(o1) (mod n)	*/
#define bi_invmod(bigint_var, bigint_o1, bigint_n)				\
	do{															\
		assert((bi_sign(bigint_o1)>=0)&&(bi_sign(bigint_n)>0));	\
		/*assert(bi_iscoprime(bigint_o1, bigint_n));*/			\
		if (!mpz_invert(bigint_var, bigint_o1, bigint_n)) {		\
			mpz_set_ui(bigint_var, 0);							\
		}														\
	}while(0)
/*-----------------------------------------------------------------*/
/* var = o1 ^ o2	*/
#define bi_pow(bigint_var, bigint_o1, ulong_o2)			\
	do{													\
		mpz_pow_ui(bigint_var, bigint_o1, ulong_o2);	\
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
		POWSTS(inc_powmod_cnt());										\
		mpz_powm_sec(bigint_var, bigint_o1, bigint_o2, bigint_n);		\
	}while(0)
#else
#define bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n)		\
	do{																	\
		assert((bi_sign(bigint_o2)>0)&&(bi_sign(bigint_n)>0)			\
			   &&(bi_isodd(bigint_n)));									\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		POWSTS(inc_powmod_cnt());										\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
#endif
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod(bigint_var, bigint_o1, bigint_o2, bigint_n)			\
	do{																	\
		assert((bi_sign(bigint_n)>0));									\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		POWSTS(inc_powmod_cnt());										\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod_ui(bigint_var, bigint_o1, ulong_o2, bigint_n)			\
	do{																	\
		assert((bi_sign(bigint_n)>0));									\
		/*assert((o2>=0)||(coprime(n, o1^(-o2)))));	*/					\
		mpz_powm_ui(bigint_var, bigint_o1, ulong_o2, bigint_n);			\
	}while(0)
/* var = 2 ^ o1 (mod n)	*/
#define bi_powmod2k(bigint_var, bigint_o1, bigint_n)					\
	do{																	\
		assert((bi_sign(bigint_n)>0));									\
		/*assert((bi_sign(bigint_o1)>=0)||(coprime(n, 2^(-o1)))));	*/	\
		mpz_set_ui(bigint_var, 2);										\
		bi_powmod_sec(bigint_var, bigint_var, bigint_o1, bigint_n);		\
	}while(0)
/* var = (o1^o2) * (o3^o4) (mod n)	*/
#define bi_mul_powmod(bigint_var, bigint_o1, bigint_o2,				\
					  bigint_o3, bigint_o4, bigint_n)				\
	do{																\
		bigint_t aux;												\
		assert((bi_sign(bigint_o2)>=0)&&(bi_sign(bigint_o4)>=0)		\
			   &&(bi_sign(bigint_n)>0));							\
		bi_ctor(aux);												\
		bi_powmod_sec(bigint_var, bigint_o1, bigint_o2, bigint_n);	\
		bi_powmod_sec(aux, bigint_o3, bigint_o4, bigint_n);			\
		bi_mulmod(bigint_var, bigint_var, aux, bigint_n);			\
		bi_dtor(aux);												\
	}while(0)
/*----------------------------	*/
#ifndef NDEBUG
/* var = o1 ^ o2 (mod n)	*/
#define bi_powmod_dbg(bigint_var, bigint_o1, bigint_o2, bigint_n)		\
	do{																	\
		assert((bi_sign(bigint_n)>0));									\
		/*assert((bi_sign(bigint_o2)>=0)||(coprime(n, o1^(-o2)))));	*/	\
		mpz_powm(bigint_var, bigint_o1, bigint_o2, bigint_n);			\
	}while(0)
#endif
	/*-------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------*/

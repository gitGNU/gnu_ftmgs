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
#include "birnd.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

/*-----------------------------------------------------------------*/
/* x rnd such that 2^(n-1) <= x < 2^n */
void bi_random_nb(bigint_t x, unsigned nbits, rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	unsigned i, nb;
	unsigned nbytes = (nbits + (CHAR_BIT-1)) / CHAR_BIT; /* round-up */
	void* buff = malloc(nbytes);
	assert(rnd_ctx != NULL);
	assert(nbits > 5 && nbits < 524288); /*NISTSP80090 table 2: 2^19*/
	if (buff == NULL) {
		bi_asg_ui(x, 0);
	} else {
		random_bytes(buff, nbytes, rnd_ctx);
		bi_import_abs(x, buff, nbytes);
		free(buff);
	}
	/* the following is not specified in NISTSP80090 */
	bi_setbit(x, nbits-1); /* it assures match of nbits */
	nb = bi_nbits(x);
	for (i = nbits; i < nb; ++i) {
		bi_clrbit(x, i);
	}
	assert(bi_sign(x)>0);
	assert(bi_nbits(x)==nbits);
	END_BIGINT_CHK();
}
/*----------------------------------*/
/* r rnd such that 0 <= r < max */
unsigned bi_random_ui(unsigned max, rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	unsigned long r;
	BEG_VAR_1_bigint_t(x);
	bi_random_nb(x, 2*CHAR_BIT*sizeof(unsigned), rnd_ctx);
	r = bi2ulong(x);
	END_VAR_1_bigint_t(x);
	END_BIGINT_CHK();
	return (unsigned)((max*(double)r)/(1.0+UINT_MAX));
}
/*----------------------------------*/
/* r rnd such that 0 <= r < uppbnd */
/* 
 * NIST-SP-300-90 B.5.1.3
 */
void bi_random_ub(bigint_t r, const bigint_t uppbnd, rnd_ctx_t* rnd_ctx)
{
	assert((const void*)&r != (const void*)&uppbnd);
	bi_random_nb(r, 64+bi_nbits(uppbnd), rnd_ctx);
	bi_mod(r, r, uppbnd);
}
/*----------------------------------*/
/* r rnd such that lwbnd <= r < uppbnd */
/* 
 * NIST-SP-300-90 B.5.1
 */
void bi_random_lub(bigint_t r, const bigint_t lwbnd, const bigint_t uppbnd,
				   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(dif);
	assert(((const void*)&r != (const void*)&lwbnd));
	assert(((const void*)&r != (const void*)&uppbnd));
	assert(bi_greater(uppbnd, lwbnd));
	bi_sub(dif, uppbnd, lwbnd);
	bi_random_ub(r, dif, rnd_ctx);
	bi_add(r, r, lwbnd);
	assert(bi_greatereq(r, lwbnd) && bi_less(r, uppbnd));
	END_VAR_1_bigint_t(dif);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/* r rnd such that (0 <= r < uppbnd) and coprime(this, uppbnd) */
void bi_random_coprime(bigint_t r, const bigint_t uppbnd, rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(gcd);
	assert((const void*)&r != (const void*)&uppbnd);
	do {
		bi_random_ub(r, uppbnd, rnd_ctx);
		bi_gcd(gcd, r, uppbnd);
	} while (! bi_equals_si(gcd, 1));
	END_VAR_1_bigint_t(gcd);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/* 
 * ntests == 0 means request a safe secure ntests for a given nbits
 * FIPS-186-3. Appx C.3. Table C-1, C-2, C-3. pg 69. Appx F.2 & F.3
 */
int get_ntests(int ntests, unsigned nbits)
{
	if (ntests == DEF_NTESTS) {	/* Table C-1 */
		if (nbits <= 512) {
			ntests = 40;
		} else if (nbits <= 1024) {
			ntests = 40;
		} else if (nbits <= 2048) {
			ntests = 56;
		} else {
			ntests = 64;
		}
	} else 	if (ntests == DEF_NTESTS_LOWSEC) {	/* Table C-2 & C-3 */
		if (nbits <= 512) {
			ntests = 7;
		} else if (nbits <= 1024) {
			ntests = 5;
		} else if (nbits <= 2048) {
			ntests = 4;
		} else {
			ntests = 4;
		}
	}
	return ntests;
}
/*----------------------------------------------------------------------------*/
#define SEQ_PRIME__	1
/*----------------------------------*/
/* r rnd prime such that 2^(n-1) < r < 2^n */
void bi_random_prime_nb(bigint_t r, unsigned nbits, int ntests,
						rnd_ctx_t* rnd_ctx)
{
	INIT_TIMER(Prime_NB);
	/* assert(exists a prime in the range); */
	assert(nbits > 5);
	ntests = get_ntests(ntests, nbits);
#ifdef SEQ_PRIME__
	do {
		bi_random_nb(r, nbits, rnd_ctx);
		bi_mkodd(r);
		while (! bi_isprime(r, ntests)) {
			bi_add_ui(r, r, 2);
		}
	} while (bi_nbits(r) != nbits);
#else
	do {
		bi_random_nb(r, nbits, rnd_ctx);
		bi_mkodd(r);
	} while (!bi_isprime(r, ntests));
#endif
	PRINT_TIMER(Prime_NB);
	assert((bi_nbits(r) == nbits) && bi_isprime(r, ntests));
}
/*----------------------------------*/
/* r rnd prime such that (0 <= r < uppbnd) */
void bi_random_prime_ub(bigint_t r, const bigint_t uppbnd, int ntests,
						rnd_ctx_t* rnd_ctx)
{
	INIT_TIMER(Prime_UB);
	assert((const void*)&r != (const void*)&uppbnd);
	/* assert(exists a prime in the range); */
	ntests = get_ntests(ntests, bi_nbits(uppbnd));
#ifdef SEQ_PRIME__
	do {
		bi_random_ub(r, uppbnd, rnd_ctx);
		bi_mkodd(r);
		while (! bi_isprime(r, ntests)) {
			bi_add_ui(r, r, 2);
		}
	} while (bi_greatereq(r, uppbnd));
#else
	do {
		bi_random_ub(r, uppbnd, rnd_ctx);
		bi_mkodd(r);
	} while (!bi_isprime(r, ntests));
#endif
	PRINT_TIMER(Prime_UB);
	assert(bi_less(r, uppbnd) && bi_isprime(r, ntests));
}
/*----------------------------------*/
/* r rnd prime such that lwbnd <= r < uppbnd and */
void bi_random_prime_lub(bigint_t r, const bigint_t lwbnd,
						 const bigint_t uppbnd,
						 int ntests,
						 rnd_ctx_t* rnd_ctx)
{
	INIT_TIMER(Prime_LUB);
	assert(((const void*)&r != (const void*)&lwbnd));
	assert(((const void*)&r != (const void*)&uppbnd));
	assert(bi_greater(uppbnd, lwbnd));
	/* assert(exists a prime in the range); */
	ntests = get_ntests(ntests, bi_nbits(uppbnd));
#ifdef SEQ_PRIME__
	bi_random_lub(r, lwbnd, uppbnd, rnd_ctx);
	bi_mkodd(r);
	while (! bi_isprime(r, ntests)) {
		bi_add_ui(r, r, 2);
	}
	if (bi_greatereq(r, uppbnd)) {
		bi_asg(r, lwbnd);
		bi_mkodd(r);
		while (! bi_isprime(r, ntests)) {
			bi_add_ui(r, r, 2);
		}
	}
#else
	BEG_VAR_1_bigint_t(dif);
	bi_sub(dif, uppbnd, lwbnd);
	do {
		bi_random_ub(r, dif, rnd_ctx);
		bi_add(r, r, lwbnd);
		bi_mkodd(r);
	} while (!bi_isprime(r, ntests));
	END_VAR_1_bigint_t(dif);
#endif
	PRINT_TIMER(Prime_LUB);
	assert(bi_greatereq(r, lwbnd) && bi_less(r, uppbnd)
		   && bi_isprime(r, ntests));
}
/*-----------------------------------------------------------------*/
static bool_t are_sgprimes(const bigint_t p, const bigint_t q,
						   int ntests, rnd_ctx_t* rnd_ctx);
/*----------------------------------*/
/* p and q primes (2^(n-1) < p < 2^n) and (p == 2 * q + 1) */
void bi_random_sgprime_nb(bigint_t p, bigint_t q, unsigned p_nbits,
						  int ntests,
						  rnd_ctx_t* rnd_ctx)
{
	INIT_TIMER(SGPrime);
	assert(p_nbits > 5);
	assert(((const void*)&p != (const void*)&q));
	ntests = get_ntests(ntests, p_nbits);
#ifdef SEQ_PRIME__
	do {
		bi_random_nb(q, p_nbits-1, rnd_ctx);
		bi_mkodd(q);
		bi_mul2(p, q);
		bi_add_ui(p, p, 1);
		while (! are_sgprimes(p, q, ntests, rnd_ctx)) {
			bi_add_ui(q, q, 2);
			bi_add_ui(p, p, 4);
		}
	} while (bi_nbits(q) != p_nbits-1);
#else
	do {
		bi_random_nb(q, p_nbits-1, rnd_ctx);
		bi_mkodd(q);
		bi_mul2(p, q);
		bi_add_ui(p, p, 1);
	} while (! are_sgprimes(p, q, ntests, rnd_ctx));
#endif
	PRINT_TIMER(SGPrime);
	assert(bi_nbits(p) == p_nbits && bi_nbits(q) == p_nbits-1);
	assert(bi_isprime(q, ntests) && bi_isprime(p, ntests));
}
/*----------------------------------*/
static const unsigned SMALL_PRIMES[] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 
	31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 
	73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 
	127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
	179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
	233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
	283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 
	353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 
	419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 
	467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 
	547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 
	607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 
	661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 
	739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 
	811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 
	877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 
	947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013
};
static const int N_SMALL_PRIMES =
	sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);
static const int N_SMALL_PRIMES_CHECK =
	(sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0])) / 2;
/*----------------------------------*/
/* true if p and q are not divisible by small primes */
static bool_t small_primes_check(const bigint_t p, const bigint_t q)
{
	int i = 0;
	assert((bi_isodd(p))
		   && bi_greater_ui(p,SMALL_PRIMES[N_SMALL_PRIMES_CHECK-1]));
	assert((bi_isodd(q))
		   && bi_greater_ui(q,SMALL_PRIMES[N_SMALL_PRIMES_CHECK-1]));
	while ((i < N_SMALL_PRIMES_CHECK)
		   && (!bi_isdiv_ui(p, SMALL_PRIMES[i]))
		   && (!bi_isdiv_ui(q, SMALL_PRIMES[i]))) {
		++i;
	}
	return (i >= N_SMALL_PRIMES_CHECK);
}
/*----------------------------------*/
static bool_t miller_rabin_witness_check(const bigint_t n,
										 const bigint_t n_1,
										 const bigint_t r,
										 const bigint_t a,
										 unsigned s)
{
	BEG_BIGINT_CHK();
	bool_t ok = TRUE;
	unsigned j;
	BEG_VAR_1_bigint_t(y);
	/*- 2.2 -----------------*/
	bi_powmod_sec(y, a, r, n);
	/*- 2.3 -----------------*/
	if ( (! bi_equals_ui(y, 1)) && (! bi_equals(y, n_1))) {
		/*- 2.3.1 -----------*/
		for (j = 1; (j < s) && (! bi_equals(y, n_1)) && ok; ++j) {
			/*- 2.3.1.1 -----*/
			bi_sqmod(y, y, n);
			/*- 2.3.1.2 -----*/
			ok = ! bi_equals_ui(y, 1);
		}
		/*- 2.3.2 -----------*/
		ok = ok && bi_equals(y, n_1);
	}
	/*- 2.4 -----------------*/
	END_VAR_1_bigint_t(y);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------*/
/*
#define MILLER_RABIN_RANDOM_BASES__	1
*/
#ifndef MILLER_RABIN_RANDOM_BASES__
/*----------------------------------*/
/* 
 * Menezes: Ch 4:
 *
 * Remark 4.28: (fixed bases in Miller-Rabin) If a1 and a2 are strong
 *  liars for n, their product a1 a2 is very likely, but not certain,
 *  to also be a strong liar for n. A strategy that is sometimes
 *  employed is to fix the bases a in the Miller-Rabin algorithm to be
 *  the first few primes (composite bases are ignored because of the
 *  preceding statement), instead of choosing them at random.
 *
 * Definition 4.29: Let p1 , p2 , . . . , pt denote the first t
 *	primes. Then ψt is defined to be the smallest positive composite
 *	integer which is a strong pseudoprime to all the bases p1 , p2 ,
 *	. . . , pt .  The numbers ψt can be interpreted as follows: to
 *	determine the primality of any integer n < ψt , it is sufficient
 *	to apply the Miller-Rabin algorithm to n with the bases a being
 *	the first t prime numbers. With this choice of bases, the answer
 *	returned by Miller-Rabin is always correct.
 */
/*
 * As we don't have a truly ψt such that n < ψt, then the test is
 * still probabilistic (not deterministic). We iterate t times over
 * the fixed bases, so this test way may be suitable for attack over
 * some "prepared" numbers, so this is only valid to check for "truly"
 * random numbers, as those ones generated by this library, and not
 * for external ones, so these functions are only for internal use.
 */
static bool_t miller_rabin_fixed_bases(const bigint_t n, int t)
{
	BEG_BIGINT_CHK();
	bool_t ok = TRUE;
	unsigned s;
	int i;
	BEG_VAR_3_bigint_t(n_1, r, a);
	/*------------------------------*/
	assert((bi_isodd(n)) && bi_greatereq_ui(n, 3));
	/*- 1 -----------------------*/
	bi_sub_ui(n_1, n, 1);
	bi_asg(r, n_1);
	s = 0;
	while (bi_iseven(r) ) {
		bi_div2(r, r);
		++s;
	}
	/*- 2 -----------------------*/
	if (t > N_SMALL_PRIMES) {
		t = N_SMALL_PRIMES;
	}
	for (i = 0; (i < t) && ok; ++i) {
		/*- 2.1 -----------------*/
		bi_asg_ui(a, SMALL_PRIMES[i]);/* CHANGED from RANDOM to FIXED */
		/*- 2.2, 2.3 ------------*/
		ok = miller_rabin_witness_check(n, n_1, r, a, s);
		/*- 2.4 -----------------*/
	}
	/*- 3 -----------------------*/
	END_VAR_3_bigint_t(n_1, r, a);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------*/
/* true if p and q are sophie-germain primes */
/* 
 * Based on:
 *  Ronald Cramer and Victor Shoup,
 *  "Signature Schemes Based on the Strong RSA Assumption"
 *  ACM Transactions on Information and System Security
 *  May 9, 2000
 *  6th ACM Conf on Computer and Communication Security, 1999
 *  Section 7: "Remarks on Key Generation"
 */
static bool_t are_sgprimes(const bigint_t p, const bigint_t q, int ntests,
						   rnd_ctx_t* rnd_ctx UNUSED__)
{
	assert((bi_isodd(p)) && bi_greatereq_ui(p, 3));
	assert((bi_isodd(q)) && bi_greatereq_ui(q, 3));
	/*
	 * Cramer & Shoup:
	 * 2.- test if q or p are divisible by small primes
	 * 3.- test if TWO is a Miller-Rabin Witness to the compositeness of q
	 * 4(no).- if  2^q (mod p) != 1 then p is probably composite
	 * 4bis.- test if TWO is a Miller-Rabin Witness to the compositeness of p
	 * 5.- apply miller-rabin test to q some number of times
	 */
	return (small_primes_check(p, q)
			&& miller_rabin_fixed_bases(p, 1)
			&& miller_rabin_fixed_bases(q, ntests) );
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
#else
/*----------------------------------*/
/* 
 * Miller-Rabin modified to use 2 as base for the first test
 */
static bool_t miller_rabin_base2(const bigint_t n, int t,
								 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	bool_t ok = TRUE;
	unsigned s;
	int i;
	BEG_VAR_4_bigint_t(n_1, r, a, n_3);
	/*------------------------------*/
	/* First witness a equals 2 */
	assert((bi_isodd(n)) && bi_greatereq_ui(n, 3));
	/*- 1 -----------------------*/
	bi_sub_ui(n_1, n, 1);
	bi_asg_bi(r, n_1);
	s = 0;
	while (bi_iseven(r) ) {
		bi_div2(r, r);
		++s;
	}
	/*- 2 -------------------*/
	/*-----------------------*/
	/*-- First Witness : 2 --*/
	/*-----------------------*/
	/*- 2.1 -----------------*/
	bi_asg_ui(a, 2);
	/*- 2.2, 2.3 ------------*/
	ok = miller_rabin_witness_check(n, n_1, r, a, s);
	/*- 2.4 -----------------*/
	/*-----------------------*/
	/*-- Rest of Tests ------*/
	/*-----------------------*/
	bi_sub_ui(n_3, n, 3);
	for (i = 1; (i < t) && ok; ++i) {
		/*- 2.1 -----------------*/
		bi_random_ub(a, n_3, rnd_ctx);
		bi_add_ui(a, a, 2);
		/*- 2.2, 2.3 ------------*/
		ok =  miller_rabin_witness_check(n, n_1, r, a, s);
		/*- 2.4 -----------------*/
	}
	/*- 3 -----------------------*/
	END_VAR_4_bigint_t(n_1, r, a, n_3);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------*/
/* true if p and q are sophie-germain primes */
/* 
 * Based on:
 *  Ronald Cramer and Victor Shoup,
 *  "Signature Schemes Based on the Strong RSA Assumption"
 *  ACM Transactions on Information and System Security
 *  May 9, 2000
 *  6th ACM Conf on Computer and Communication Security, 1999
 *  Section 7: "Remarks on Key Generation"
 */
static bool_t are_sgprimes(const bigint_t p, const bigint_t q, int ntests,
						   rnd_ctx_t* rnd_ctx)
{
	assert((bi_isodd(p)) && bi_greatereq_ui(p, 3));
	assert((bi_isodd(q)) && bi_greatereq_ui(q, 3));
	/*
	 * Cramer & Shoup:
	 * 2.- test if q or p are divisible by small primes
	 * 3.- test if TWO is a Miller-Rabin Witness to the compositeness of q
	 * 4(no).- if  2^q (mod p) != 1 then p is probably composite
	 * 4bis.- test if TWO is a Miller-Rabin Witness to the compositeness of p
	 * 5.- apply miller-rabin test to q some number of times
	 */
	return (small_primes_check(p, q)
			&& miller_rabin_base2(p, 1, rnd_ctx)
			&& miller_rabin_base2(q, ntests, rnd_ctx) );
}
/*----------------------------------*/
#endif
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

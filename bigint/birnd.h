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
#ifndef umalccvbg_birnd_h__
#define umalccvbg_birnd_h__	1
#include "bigintop.h"
#include "random.h"
BEGIN_EXTERN_C
/*-------------------------------------------------------------*/
enum { DEF_NTESTS = 0, DEF_NTESTS_LOWSEC = 1 };
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* r rnd such that 0 <= r < max */
unsigned bi_random_ui(unsigned max, rnd_ctx_t* rnd_ctx);
/* r rnd such that 2^(n-1) <= r < 2^n */
void bi_random_nb(bigint_t r, unsigned nbits, rnd_ctx_t* rnd_ctx);
/* r rnd such that 0 <= x < uppbnd */
void bi_random_ub(bigint_t r, const bigint_t uppbnd, rnd_ctx_t* rnd_ctx);
/* r rnd such that lwbnd <= r < uppbnd */
void bi_random_lub(bigint_t r, const bigint_t lwbnd, const bigint_t uppbnd,
				   rnd_ctx_t* rnd_ctx);
/* r rnd (0 <= r < uppbnd) and coprime(this, uppbnd) */
void bi_random_coprime(bigint_t r, const bigint_t uppbnd, rnd_ctx_t* rnd_ctx);
/*-------------------------------------------------------------*/
/* 
 * ntests == 0 means request a safe secure ntests for a given nbits
 */
/*--------------------------------*/
/* r rnd prime such that 2^(n-1) < r < 2^n */
void bi_random_prime_nb(bigint_t r, unsigned nbits,
						int ntests, rnd_ctx_t* rnd_ctx);
/* r rnd prime such that 0 <= x < uppbnd */
void bi_random_prime_ub(bigint_t r, const bigint_t uppbnd,
						int ntests, rnd_ctx_t* rnd_ctx);
/* r rnd prime such that lwbnd <= r < uppbnd */
void bi_random_prime_lub(bigint_t r, const bigint_t lwbnd,
						 const bigint_t uppbnd,
						 int ntests, rnd_ctx_t* rnd_ctx);
/* p and q primes (2^(n-1) < p < 2^n) and (p == 2 * q + 1) */
void bi_random_sgprime_nb(bigint_t p, bigint_t q,
						  unsigned p_nbits,
						  int ntests, rnd_ctx_t* rnd_ctx);
/*-------------------------------------------------------------*/
BEGIN_EXTERN_C
#endif

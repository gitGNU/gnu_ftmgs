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
#include "generator.h"
#include <assert.h>
/*----------------------------------------------------------------------------*/
/* 
 * p=2q+1, p and q primes
 * phi = (p-1) = 2*q
 * order = phi ; order = 2*q
 */
void zpq_generator(bigint_t g, const bigint_t p, const bigint_t q,
				   rnd_ctx_t* rndctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_2_bigint_t(g_exp_2, g_exp_q);
	do {
		do {
			bi_random_ub(g, p, rndctx);
		} while (bi_lesseq_ui(g, 1U));
		/* divisors of phi (p-1) are 2 and q */
		bi_sqmod(g_exp_2, g, p);
		bi_powmod_sec(g_exp_q, g, q, p);
	} while (bi_equals_ui(g_exp_2, 1U) || bi_equals_ui(g_exp_q, 1U));
	END_VAR_2_bigint_t(g_exp_2, g_exp_q);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/* 
 * p=2q+1, p and q primes
 * phi = (p-1) = 2*q ; (p-1)/q = 2
 * order = q
 */
void gqzp_generator(bigint_t g, const bigint_t p, const bigint_t q UNUSED__,
					rnd_ctx_t* rndctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(h);
	do {
		do {
			bi_random_ub(h, p, rndctx);
		} while (bi_iszero(h));
		bi_sqmod(g, h, p);
		/* divisors of phi are 2 and q */
	} while (bi_equals_ui(g, 1U));
	DBG(bi_powmod_dbg(h, g, q, p));
	assert(bi_greatereq_ui(g, 2U) && bi_less(g, p) &&  bi_equals_ui(h, 1U));
	END_VAR_1_bigint_t(h);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/* 
 * void gqzp_generator_2(bigint_t g, const bigint_t p, const bigint_t q,
 *					   rnd_ctx_t* rndctx)
 * {
 *	BEG_BIGINT_CHK();
 * 	BEG_VAR_1_bigint_t(g_exp_q);
 * 	do {
 * 		do {
 * 			bi_random_ub(g, p, rndctx);
 * 		} while (bi_lesseq_ui(g, 1U)) ;
 * 		bi_powmod_sec(g_exp_q, g, q, p)
 * 	} while ( ! bi_equals_ui(g_exp_q, 1U) );
 * 	END_VAR_1_bigint_t(g_exp_q);
 *	END_BIGINT_CHK();
 * }
 */
/*----------------------------------------------------------------------------*/
/* 
 * n=p*q ; p=2p'+1 ; q=2q'+1 ; p, p', q, q' primes
 * phi = (p-1)*(q-1) = (2p')*(2q') = 4 (p' * q')
 * order = p' * q'
 */
void qrn_generator(bigint_t g, const bigint_t n, const bigint_t order,
				   const bigint_t p1, const bigint_t q1,
				   rnd_ctx_t* rndctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_4_bigint_t(bigint_4, x, g_exp_p1, g_exp_q1);
	bi_asg_ui(bigint_4, 4U);
	do {
		bi_random_ub(x, order, rndctx);
		bi_powmod_sec(g, bigint_4, x, n);
		/* divisors of phi are p1 and q1 */
		bi_powmod_sec(g_exp_p1, g, p1, n);
		bi_powmod_sec(g_exp_q1, g, q1, n);
	} while (bi_equals_ui(g_exp_p1, 1U) || bi_equals_ui(g_exp_q1, 1U));
	END_VAR_4_bigint_t(bigint_4, x, g_exp_p1, g_exp_q1);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void qrn_generator_2(bigint_t g, const bigint_t n,
					 const bigint_t p UNUSED__, const bigint_t p1,
					 const bigint_t q UNUSED__, const bigint_t q1,
					 rnd_ctx_t* rndctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(order);
	bi_mul(order, p1, q1);
	qrn_generator(g, n, order, p1, q1, rndctx);
	END_VAR_1_bigint_t(order);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/

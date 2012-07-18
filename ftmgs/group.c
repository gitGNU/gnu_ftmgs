/*--------------------------------------------------------------------------- */
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
/*--------------------------------------------------------------------------- */
#include "group.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "sphere.h"
#include "syspar.h"
#include "generator.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
STATIC_ASSERT(NU_1 == Nu1 && NU_2 == Nu2 && NU_3 == Nu3,
			  Bad_nu_definition);
STATIC_ASSERT(SHA1 == Sha1 && SHA224 == Sha224 && SHA256 == Sha256
			  && SHA384 == Sha384 && SHA512 == Sha512,
			  Bad_sha_mode_definition);
STATIC_ASSERT(SHA1HashSize == Sha1Size && SHA224HashSize == Sha224Size
			  && SHA256HashSize == Sha256Size && SHA384HashSize == Sha384Size
			  && SHA512HashSize == Sha512Size
			  && USHAMaxHashSize == ShaMaxSize,
			  Bad_sha_size_definition);
STATIC_ASSERT(TRUE_ENTROPY == TrueEntropy
			  && PSEUDO_ENTROPY == PseudoEntropy
			  && NO_ENTROPY == NoEntropy,
			  Bad_entropy_definition);
STATIC_ASSERT(FTMGS_ERROR == 0
			  && FTMGS_ERROR < FTMGS_OK
			  && FTMGS_OK < FTMGS_UNFINISHED,
			  Bad_retcode_definition);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static void ftmgs_square_faj_pk(ftmgs_faj_pbkey_share_t* faj_pk,
								const ftmgs_fa_pbkey_t* fa_pk)
{
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, fa_pk->pbkey.n);
	bi_sqmod(faj_pk->pbkey_j.yj, faj_pk->pbkey_j.yj, n2);
	END_VAR_1_bigint_t(n2);
}
/*----------------------------------------------------------------------------*/
static void ftmgs_square_faj_gpk(ftmgs_faj_gr_pbkey_share_t* faj_gpk,
								 const ftmgs_pbkey_t* gpk)
{
	bi_sqmod(faj_gpk->pbkey_j.yj, faj_gpk->pbkey_j.yj, gpk->gmpk.n);
	bi_sqmod(faj_gpk->hj, faj_gpk->hj, gpk->gmpk.n);
}
/*----------------------------------------------------------------------------*/
static void ftmgs_square_gpk(ftmgs_pbkey_t* gpk)
{
	bi_sqmod(gpk->gmpk.g, gpk->gmpk.g, gpk->gmpk.n);
	bi_sqmod(gpk->gmpk.y, gpk->gmpk.y, gpk->gmpk.n);
	bi_sqmod(gpk->a0, gpk->a0, gpk->gmpk.n);
	bi_sqmod(gpk->a,  gpk->a,  gpk->gmpk.n);
	bi_sqmod(gpk->b,  gpk->b,  gpk->gmpk.n);
	bi_sqmod(gpk->h,  gpk->h,  gpk->gmpk.n);
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, gpk->fapk.n);
	bi_sqmod(gpk->fapk.g, gpk->fapk.g, n2);
	bi_sqmod(gpk->fapk.y, gpk->fapk.y, n2);
	END_VAR_1_bigint_t(n2);
}
/*---------------------------------------------------------------------*/
static void ftmgs_gm_group_setup_base(ftmgs_pbkey_t* gpk)
{
	ftmgs_square_gpk(gpk);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	ftmgs_precomp_group(gpk);
#endif
}
/*----------------------------------------------------------------------------*/
/*-- FA Setup ----------------------------------------------------------------*/
/*--------------------------------------------------------------------------- */
void ftmgs_fa0_setup_mono(ftmgs_fa_pbkey_t* fa_pk_preimage,
						  unsigned nu,
						  rnd_ctx_t* rnd_ctx)
{
	paillier_thr_create_pbkey(&fa_pk_preimage->pbkey, nu, rnd_ctx);
}
/*----------------------------------------------------------------------------*/
/* faj_pk == square(faj_pk_preimage) */
void ftmgs_faj_setup(ftmgs_faj_pbkey_share_t* faj_pk,
					 ftmgs_faj_pbkey_share_t* faj_pk_preimage,
					 ftmgs_faj_prkey_t* faj_sk,
					 const ftmgs_fa_pbkey_t* fa_pk,
					 rnd_ctx_t* rnd_ctx)
{
	paillier_thr_create_prkey(&faj_pk->pbkey_j,
							  &faj_sk->prkey_j,
							  &fa_pk->pbkey,
							  rnd_ctx);
	/*--------------------------------*/
	if (faj_pk_preimage != NULL) {
		ftmgs_faj_pbkey_share_t_asg(faj_pk_preimage, faj_pk);
	}
	ftmgs_square_faj_pk(faj_pk, fa_pk);
}
/*-------------------------------------------------------------------------*/
/*-- Group Setup ----------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
void ftmgs_gm_init_setup(ftmgs_pbkey_t* gpk,
						 ftmgs_prkey_t* gsk,
						 unsigned nu,
						 rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_1_bigint_t(order);
	BEG_VAR_1(elgamal_thr_modord_t, p1q1);
	/*------------------------------- */
	elgamal_thr_create_pbkey(&gpk->gmpk, &gsk->npq, &p1q1, nu,
							 rnd_ctx);
	bi_mul(order, p1q1.p1, p1q1.q1);
	/*------------------------------- */
	do {
		qrn_generator(gpk->a0, gpk->gmpk.n,
					  order, p1q1.p1, p1q1.q1, rnd_ctx);
	} while (bi_equals(gpk->a0, gpk->gmpk.g));
	do {
		qrn_generator(gpk->a, gpk->gmpk.n,
					  order, p1q1.p1, p1q1.q1, rnd_ctx);
	} while (bi_equals(gpk->a, gpk->gmpk.g)
			 || bi_equals(gpk->a, gpk->a0));
	do {
		qrn_generator(gpk->b, gpk->gmpk.n,
					  order, p1q1.p1, p1q1.q1, rnd_ctx);
	} while (bi_equals(gpk->b, gpk->gmpk.g)
			 || bi_equals(gpk->b, gpk->a0)
			 || bi_equals(gpk->b, gpk->a));
	/*------------------------------- */
	bi_asg_ui(gpk->h, 1);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	create_syssph(&gpk->syssph, &gpk->gmpk.sp);
#endif
	/*------------------------------- */
	END_VAR_1(elgamal_thr_modord_t, p1q1);
	END_VAR_1_bigint_t(order);
}
/*---------------------------------------------------------------------*/
/* faj_gpk == square(faj_gpk_preimage) */
void ftmgs_faj_group_setup(ftmgs_faj_gr_pbkey_share_t* faj_gpk,
						   ftmgs_faj_gr_pbkey_share_t* faj_gpk_preimage,
						   ftmgs_faj_gr_prkey_t* faj_gsk,
						   const ftmgs_pbkey_t* gpk,
						   rnd_ctx_t* rnd_ctx)
{
	/*--------------------------------*/
	/* FA: Creating Group Public Key*/
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(aux);
	do {
		bi_random_prime_nb(aux, gpk->gmpk.sp.nu/2, DEF_NTESTS_LOWSEC, rnd_ctx);
		bi_powmod_sec(faj_gpk->hj, gpk->gmpk.g, aux, gpk->gmpk.n);
	} while (bi_equals(faj_gpk->hj, gpk->gmpk.g)
			 || bi_equals(faj_gpk->hj, gpk->a0)
			 || bi_equals(faj_gpk->hj, gpk->a)
			 || bi_equals(faj_gpk->hj, gpk->b));
	END_VAR_1_bigint_t(aux);
	/*--------------------------------*/
	/* FA: Creating Group Public Key & Group Open Key*/
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	do {
		elgamal_thr_create_prkey(&faj_gpk->pbkey_j,
								 &faj_gsk->prkey_j,
								 &gpk->gmpk,
								 &gpk->syssph.mu,
								 rnd_ctx);
	} while (bi_equals(faj_gpk->pbkey_j.yj, gpk->gmpk.g)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->a0)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->a)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->b)
			 || bi_equals(faj_gpk->pbkey_j.yj, faj_gpk->hj));
#else
	BEG_VAR_1(sphere_t, mu);
	create_sph_mu(&mu, &gpk->gmpk.sp);
	do {
		elgamal_thr_create_prkey(&faj_gpk->pbkey_j,
								 &faj_gsk->prkey_j,
								 &gpk->gmpk,
								 &mu,
								 rnd_ctx);
	} while (bi_equals(faj_gpk->pbkey_j.yj, gpk->gmpk.g)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->a0)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->a)
			 || bi_equals(faj_gpk->pbkey_j.yj, gpk->b)
			 || bi_equals(faj_gpk->pbkey_j.yj, faj_gpk->hj));
	END_VAR_1(sphere_t, mu);
#endif
	/*--------------------------------*/
	if (faj_gpk_preimage != NULL) {
		ftmgs_faj_gr_pbkey_share_t_asg(faj_gpk_preimage, faj_gpk);
	}
	ftmgs_square_faj_gpk(faj_gpk, gpk);
}
/*---------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_gm_group_setup(ftmgs_pbkey_t* gpk,
									 ftmgs_pbkey_t* gpk_preimage,
									 unsigned nfas,
									 const ftmgs_fa_pbkey_t* fa_pk,
									 const ftmgs_faj_pbkey_share_t* faj_pk,
									 const ftmgs_faj_gr_pbkey_share_t* faj_gpk)
{
	ftmgs_retcode_t rc = FTMGS_ERROR;
	if (gpk->gmpk.nkeys == 0) {
		bi_asg_ui(gpk->h, 1);
		bi_asg_ui(gpk->gmpk.y, 1);
	}
	if (gpk->fapk.nkeys == 0) {
		paillier_thr_pbkey_t_asg(&gpk->fapk, &fa_pk->pbkey);
		bi_asg_ui(gpk->fapk.y, 1);
	}
	/*--------------------------------*/
	if (bi_equals(gpk->fapk.n, fa_pk->pbkey.n)
		&& bi_equals(gpk->fapk.g, fa_pk->pbkey.g)) {
		/*----------------------------*/
		if ((gpk->fapk.nkeys < nfas)&&(gpk->gmpk.nkeys < nfas)) {
			paillier_thr_add_pbkey_share(&gpk->fapk, &faj_pk->pbkey_j);
			/*------------------------*/
			bi_mulmod(gpk->h, gpk->h, faj_gpk->hj, gpk->gmpk.n);
			elgamal_thr_add_pbkey_share(&gpk->gmpk, &faj_gpk->pbkey_j);
			rc = FTMGS_UNFINISHED;
		}
		/*----------------------------*/
		if ((gpk->fapk.nkeys == nfas)&&(gpk->gmpk.nkeys == nfas)) {
			if (gpk_preimage != NULL) {
				ftmgs_pbkey_t_asg(gpk_preimage, gpk);
			}
			ftmgs_gm_group_setup_base(gpk);
			rc = FTMGS_OK;
		}
		/*----------------------------*/
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/*-- Checks ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* checks if a has order equal to p'q' or 2p'q'*/
static bool_t has_right_order(const bigint_t a, const bigint_t n)
{
	/* if gcd(a+-1,n)=1 then order(a) is p'q' or 2p'q'*/
	bool_t ok;
	BEG_VAR_2_bigint_t(aux1, aux2);
	bi_add_ui(aux1, a, 1);
	bi_sub_ui(aux2, a, 1);
	ok = bi_iscoprime(aux1, n) && bi_iscoprime(aux2, n);
	END_VAR_2_bigint_t(aux1, aux2);
	return ok;
}
/*---------------------------------------------------------------------*/
/* checks if a2 equals to a^2 (mod n) */
USE_RESULT__
static bool_t check_sqmod(const bigint_t a2, const bigint_t a, const bigint_t n)
{
	bool_t ok;
	BEG_VAR_1_bigint_t(aa);
	bi_sqmod(aa, a, n);
	ok = bi_equals(aa, a2);
	END_VAR_1_bigint_t(aa);
	return ok;
}
/*---------------------------------------------------------------------*/
bool_t ftmgs_check_gpk(const ftmgs_pbkey_t* gpk,
					   const ftmgs_pbkey_t* gpk_preimage)
{
	/* check that n has no small prime factors n=pq*/
	/* todo !!!*/
	/* check that members of gpk are in QR(n)*/
	bool_t gpk_ok;
	BEG_VAR_2_bigint_t(aux, n2);
	bi_sq(n2, gpk->fapk.n);
	gpk_ok = (bi_equals(gpk->gmpk.n, gpk_preimage->gmpk.n)
			  && bi_equals(gpk->fapk.n, gpk_preimage->fapk.n)
			  /*----------------------*/
			  && has_right_order(gpk->gmpk.g, gpk->gmpk.n)
			  && has_right_order(gpk->gmpk.y, gpk->gmpk.n)
			  && has_right_order(gpk->a0, gpk->gmpk.n)
			  && has_right_order(gpk->a,  gpk->gmpk.n)
			  && has_right_order(gpk->b,  gpk->gmpk.n)
			  && has_right_order(gpk->h,  gpk->gmpk.n)
			  /*----------------------*/
			  && has_right_order(gpk->fapk.g, gpk->fapk.n)/* n2 ???*/
			  && has_right_order(gpk->fapk.y, gpk->fapk.n)/* n2 ??? */
			  /*----------------------*/
			  && check_sqmod(gpk->gmpk.g, gpk_preimage->gmpk.g, gpk->gmpk.n)
			  && check_sqmod(gpk->gmpk.y, gpk_preimage->gmpk.y, gpk->gmpk.n)
			  && check_sqmod(gpk->a0, gpk_preimage->a0, gpk->gmpk.n)
			  && check_sqmod(gpk->a, gpk_preimage->a, gpk->gmpk.n)
			  && check_sqmod(gpk->b, gpk_preimage->b, gpk->gmpk.n)
			  && check_sqmod(gpk->h, gpk_preimage->h, gpk->gmpk.n)
			  /*----------------------*/
			  && check_sqmod(gpk->fapk.g, gpk_preimage->fapk.g, n2)
			  && check_sqmod(gpk->fapk.y, gpk_preimage->fapk.y, n2)
			  );
	END_VAR_2_bigint_t(aux, n2);
	return gpk_ok;
}
/*----------------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_check_gpk_fa(ftmgs_pbkey_t* gpk_aux,
								   const ftmgs_pbkey_t* gpk,
								   const ftmgs_fa_pbkey_t* fa_pk,
								   const ftmgs_faj_pbkey_share_t* faj_pk,
								   const ftmgs_faj_gr_pbkey_share_t* faj_gpk)
{
	ftmgs_retcode_t rc = FTMGS_ERROR;
	BEG_VAR_1_bigint_t(n2);
	if ((gpk_aux->gmpk.nkeys == gpk_aux->fapk.nkeys)
		&& (gpk->gmpk.nkeys == gpk->fapk.nkeys)
		&& bi_equals(gpk->fapk.n, fa_pk->pbkey.n)) {
		/*----------------------------*/
		if (gpk_aux->gmpk.nkeys == 0) {
			elgamal_thr_pbkey_t_asg(&gpk_aux->gmpk, &gpk->gmpk);
			paillier_thr_pbkey_t_asg(&gpk_aux->fapk, &gpk->fapk);
			gpk_aux->gmpk.nkeys = 0;
			gpk_aux->fapk.nkeys = 0;
			bi_asg_ui(gpk_aux->h, 1);
			bi_asg_ui(gpk_aux->gmpk.y, 1);
			bi_asg_ui(gpk_aux->fapk.y, 1);
		}
		/*----------------------------*/
		if (gpk_aux->gmpk.nkeys < gpk->gmpk.nkeys) {
			paillier_thr_add_pbkey_share(&gpk_aux->fapk, &faj_pk->pbkey_j);
			bi_mulmod(gpk_aux->h, gpk_aux->h, faj_gpk->hj, gpk->gmpk.n);
			elgamal_thr_add_pbkey_share(&gpk_aux->gmpk, &faj_gpk->pbkey_j);
			rc = FTMGS_UNFINISHED;
		}
		/*----------------------------*/
		if (gpk_aux->gmpk.nkeys == gpk->gmpk.nkeys) {
			if (bi_equals(gpk_aux->h, gpk->h)
				&& bi_equals(gpk_aux->gmpk.y, gpk->gmpk.y)
				&& bi_equals(gpk->fapk.y, gpk->fapk.y) ) {
				rc = FTMGS_OK;
			} else {
				rc = FTMGS_ERROR;
			}
		}
	}
	END_VAR_1_bigint_t(n2);
	return rc;
}
/*----------------------------------------------------------------------------*/
/* 
 * bool_t ftmgs_check_gpk_fa(const ftmgs_pbkey_t* gpk,
 * 						  const ftmgs_pbkey_t* gpk_preimage,
 * 						  unsigned nfas,
 * 						  const ftmgs_fa_pbkey_t* fa_pk,
 * 						  const ftmgs_faj_pbkey_share_t* faj_pk[],
 * 						  const ftmgs_faj_gr_pbkey_share_t* faj_gpk[])
 * {
 * 	bool_t ok = ftmgs_check_gpk(gpk, gpk_preimage);
 * 	ok = ok
 * 		&& (nfas == gpk->gmpk.nkeys)
 * 		&& (nfas == gpk->fapk.nkeys)
 * 		&& bi_equals(gpk->fapk.n, fa_pk->pbkey.n)
 * 		&& bi_equals(gpk_preimage->fapk.g, fa_pk->pbkey.g);
 * 	if (ok) {
 * 		unsigned i;
 * 		BEG_VAR_4_bigint_t(n2, h, y, yy);
 * 		bi_sq(n2, gpk->fapk.n);
 * 		bi_asg_ui(h, 1);
 * 		bi_asg_ui(y, 1);
 * 		bi_asg_ui(yy, 1);
 * 		for (i = 0; i < nfas; ++i) {
 * 			bi_mulmod(h, h, faj_gpk[i]->hj, gpk->gmpk.n);
 * 			bi_mulmod(y, y, faj_gpk[i]->pbkey_j.yj, gpk->gmpk.n);
 * 			bi_mulmod(yy, yy, faj_pk[i]->pbkey_j.yj, n2);
 * 		}
 * 		ok = ok
 * 			&& bi_equals(h, gpk->h)
 * 			&& bi_equals(y, gpk->gmpk.y)
 * 			&& bi_equals(yy, gpk->fapk.y);
 * 		END_VAR_4_bigint_t(n2, h, y, yy);
 * 	}
 * 	return ok;
 * }
 */
/*---------------------------------------------------------------------*/
bool_t ftmgs_group_setup_prtcl(ftmgs_pbkey_t* gpk,
							   ftmgs_prkey_t* gsk,
							   unsigned nfas,
							   ftmgs_faj_pbkey_share_t* faj_pk[],
							   ftmgs_faj_prkey_t* faj_sk[],
							   ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
							   ftmgs_faj_gr_prkey_t* faj_gsk[],
							   unsigned nu,
							   rnd_ctx_t* rnd_ctx)
{
	unsigned i;
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	/*--------------------------------*/
	BEG_VAR_1(ftmgs_fa_pbkey_t, fa_pk_preimage);
	BEG_VAR_1(ftmgs_pbkey_t, gpk_preimage);
	BEG_VAR_1(ftmgs_pbkey_t, gpk_aux);
	BEG_VAR_V(ftmgs_faj_pbkey_share_t, faj_pk_preimage, nfas);
	BEG_VAR_V(ftmgs_faj_gr_pbkey_share_t, faj_gpk_preimage, nfas);
	INIT_TIMER(GroupSetup);
	/*- FA0 ------------------------------*/
	ftmgs_fa0_setup_mono(&fa_pk_preimage, nu, rnd_ctx);
	/*- FAj ------------------------------*/
	for (i = 0; i < nfas; ++i) {
		/* FA0 -> FAj: <fa_pk_preimage> */
		ftmgs_faj_setup(faj_pk[i], &faj_pk_preimage[i], faj_sk[i],
						&fa_pk_preimage, rnd_ctx);
	}
	/*- GM -------------------------------*/
	ftmgs_gm_init_setup(gpk, gsk, nu, rnd_ctx);
	/*- FAj ------------------------------*/
	for (i = 0; i < nfas; ++i) {
		/* GM -> FAj: <gpk> */
		ftmgs_faj_group_setup(faj_gpk[i], &faj_gpk_preimage[i], faj_gsk[i],
							  gpk, rnd_ctx);
	}
	/*- GM -------------------------------*/
	for (i = 0; (i < nfas)&&(rc == FTMGS_UNFINISHED); ++i) {
		/* FAj -> GM: <fa_pk_preimage, faj_pk_preimage, faj_gpk_preimage> */
		rc = ftmgs_gm_group_setup(gpk, &gpk_preimage, nfas, &fa_pk_preimage,
								  &faj_pk_preimage[i], &faj_gpk_preimage[i]);
	}
	assert(rc == FTMGS_OK);
	/* GM: db_store(gpk, gpk_preimage); */
	/* FAj: db_store(gpk-old, faj_pk, faj_gpk, faj_sk, faj_gsk); */
	/* FA0: db_store(fa_pk_preimage); */
	PRINT_TIMER(GroupSetup);
	/*----------------------------*/
	if (rc == FTMGS_OK) {
		rc = ftmgs_check_gpk(gpk,&gpk_preimage)? FTMGS_UNFINISHED : FTMGS_ERROR;
		/*----------------------------*/
		for (i = 0; (i < nfas)&&(rc == FTMGS_UNFINISHED); ++i) {
			rc = ftmgs_check_gpk_fa(&gpk_aux, gpk, &fa_pk_preimage,
									faj_pk[i], faj_gpk[i]);
		}
		assert(rc == FTMGS_OK);
	}
	/*----------------------------*/
	END_VAR_V(ftmgs_faj_gr_pbkey_share_t, faj_gpk_preimage, nfas);
	END_VAR_V(ftmgs_faj_pbkey_share_t, faj_pk_preimage, nfas);
	END_VAR_1(ftmgs_pbkey_t, gpk_aux);
	END_VAR_1(ftmgs_pbkey_t, gpk_preimage);
	END_VAR_1(ftmgs_fa_pbkey_t, fa_pk_preimage);
	/*--------------------------------*/
	return (rc == FTMGS_OK);
}
/*----------------------------------------------------------------------------*/
unsigned ftmgs_get_nfas_reveal(const ftmgs_pbkey_t* gpk)
{
	return gpk->fapk.nkeys;
}
/*----------------------------------------------------------------------------*/
unsigned ftmgs_get_nfas_open(const ftmgs_pbkey_t* gpk)
{
	return gpk->gmpk.nkeys;
}
/*----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/*-- Precomputations --------------------------------------------------*/
/*---------------------------------------------------------------------*/
#ifdef PRECOMPUTATIONS__
static void ftmgs_precomp_join(join_precomp_t* pcmp,
							   const paillier_thr_pbkey_t* fapk,
							   const sphere_t* xsph)
{
	bi_powmod_sec(pcmp->join_gg_r_psi, fapk->g, fapk->precomp.psi.center,
			  fapk->precomp.n2);
	bi_powmod_sec(pcmp->join_yy_r_psi, fapk->y, fapk->precomp.psi.center,
			  fapk->precomp.n2);
	BEG_VAR_1_bigint_t(hh);
	bi_add_ui(hh, fapk->n, 1);
	bi_powmod_sec(pcmp->join_hh_x_mu, hh, xsph->center,
			  fapk->precomp.n2);
	END_VAR_1_bigint_t(hh);
}
/*----------------------------------------------------------------------------*/
static void ftmgs_precomp_sign(sign_precomp_t* pcmp,
							   const elgamal_thr_pbkey_t* gmpk,
							   const bigint_t a0 UNUSED__,
							   const bigint_t a,
							   const bigint_t b UNUSED__,
							   const bigint_t h,
							   const syssph_t* syssph)
{
	bi_powmod_sec(pcmp->sign_g_r_mu, gmpk->g,
			  syssph->mu.center, gmpk->n);
	bi_powmod_sec(pcmp->sign_g_e_gamma, gmpk->g,
			  syssph->gamma.center, gmpk->n);
	bi_powmod_sec(pcmp->sign_g_h1_gamma_mu, gmpk->g,
			  syssph->gamma_mu.center, gmpk->n);
	bi_powmod_sec(pcmp->sign_h_r_mu, h,
			  syssph->mu.center, gmpk->n);
	bi_powmod_sec(pcmp->sign_y_h1_gamma_mu, gmpk->y,
			  syssph->gamma_mu.center, gmpk->n);
	bi_powmod_sec(pcmp->sign_a_x_mu, a,
			  syssph->mu.center, gmpk->n);
/* 
 * 	bi_powmod_sec(pcmp->sign_b_x1_lambda, b,
 * 			  syssph->lambda.center, gmpk->n);
 */
}
/*----------------------------------------------------------------------------*/
void ftmgs_precomp_group(ftmgs_pbkey_t* gpk)
{
	create_syssph(&gpk->syssph, &gpk->gmpk.sp);
	ftmgs_precomp_join(&gpk->join_precomp, &gpk->fapk, &gpk->syssph.mu);
	ftmgs_precomp_sign(&gpk->sign_precomp, &gpk->gmpk,
					   gpk->a0, gpk->a, gpk->b, gpk->h,
					   &gpk->syssph);
	nadrp_precomp(&gpk->nadrp_precomp,
				  gpk->gmpk.n,				/* modulus */
				  gpk->a,					/* calculates x and a^x (mod n) */
				  gpk->gmpk.g,
				  gpk->h,
				  gpk->gmpk.y,
				  &gpk->gmpk.sp,
				  &gpk->syssph.mu);			/* sphere for x */
}
#endif
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_precomp_t_chk_members(const struct join_precomp_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_join_precomp_t {
		bigint_t join_gg_r_psi;		/* zero */
		bigint_t join_yy_r_psi;		/* zero */
		bigint_t join_hh_x_mu;		/* zero */
	};
	CHK_FIELD__(dummy_join_precomp_t, join_precomp_t, join_gg_r_psi);
	CHK_FIELD__(dummy_join_precomp_t, join_precomp_t, join_yy_r_psi);
	CHK_FIELD__(dummy_join_precomp_t, join_precomp_t, join_hh_x_mu);
	CHK_SIZE__(dummy_join_precomp_t, join_precomp_t);
	return (code == 213635733); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_precomp_t_ctor(struct join_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_precomp_t_chk_members(p,213635733));
	bi_ctor(p->join_gg_r_psi);
	bi_ctor(p->join_yy_r_psi);
	bi_ctor(p->join_hh_x_mu);
}
/*----------------------------------------------------------------------------*/
void join_precomp_t_dtor(struct join_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_precomp_t_chk_members(p,213635733));
	bi_clear_zero(p->join_hh_x_mu);
	bi_dtor(p->join_hh_x_mu);
	bi_clear_zero(p->join_yy_r_psi);
	bi_dtor(p->join_yy_r_psi);
	bi_clear_zero(p->join_gg_r_psi);
	bi_dtor(p->join_gg_r_psi);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_precomp_t_asg(struct join_precomp_t* p, const struct join_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_precomp_t_chk_members(p,213635733));
	if (p != o) {
		bi_asg(p->join_gg_r_psi, o->join_gg_r_psi);
		bi_asg(p->join_yy_r_psi, o->join_yy_r_psi);
		bi_asg(p->join_hh_x_mu, o->join_hh_x_mu);
	}
}
/*----------------------------------------------------------------------------*/
void join_precomp_t_move(struct join_precomp_t* p, struct join_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_precomp_t_chk_members(p,213635733));
	if (p != o) {
		bi_asg_si(p->join_gg_r_psi, 0);
		bi_swap(p->join_gg_r_psi, o->join_gg_r_psi);
		bi_asg_si(p->join_yy_r_psi, 0);
		bi_swap(p->join_yy_r_psi, o->join_yy_r_psi);
		bi_asg_si(p->join_hh_x_mu, 0);
		bi_swap(p->join_hh_x_mu, o->join_hh_x_mu);
	}
}
/*----------------------------------------------------------------------------*/
struct join_precomp_t* join_precomp_t_new()/*auto*/
{
	struct join_precomp_t* p = (struct join_precomp_t*)malloc(sizeof(struct join_precomp_t));
	if (p != NULL) {
		join_precomp_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_precomp_t* join_precomp_t_clone(const struct join_precomp_t* o)/*auto*/
{
	struct join_precomp_t* p = NULL;
	if (o != NULL) {
		p = (struct join_precomp_t*)malloc(sizeof(struct join_precomp_t));
		if (p != NULL) {
			join_precomp_t_ctor(p);
			join_precomp_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_precomp_t_delete(struct join_precomp_t* p)/*auto*/
{
	if (p != NULL) {
		join_precomp_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int sign_precomp_t_chk_members(const struct sign_precomp_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_sign_precomp_t {
		bigint_t sign_g_r_mu;			/* zero */
		bigint_t sign_g_e_gamma;		/* zero */
		bigint_t sign_g_h1_gamma_mu;	/* zero */
		bigint_t sign_h_r_mu;			/* zero */
		bigint_t sign_y_h1_gamma_mu;	/* zero */
		bigint_t sign_a_x_mu;			/* zero */
	};
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_g_r_mu);
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_g_e_gamma);
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_g_h1_gamma_mu);
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_h_r_mu);
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_y_h1_gamma_mu);
	CHK_FIELD__(dummy_sign_precomp_t, sign_precomp_t, sign_a_x_mu);
	CHK_SIZE__(dummy_sign_precomp_t, sign_precomp_t);
	return (code == 315555660); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void sign_precomp_t_ctor(struct sign_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(sign_precomp_t_chk_members(p,315555660));
	bi_ctor(p->sign_g_r_mu);
	bi_ctor(p->sign_g_e_gamma);
	bi_ctor(p->sign_g_h1_gamma_mu);
	bi_ctor(p->sign_h_r_mu);
	bi_ctor(p->sign_y_h1_gamma_mu);
	bi_ctor(p->sign_a_x_mu);
}
/*----------------------------------------------------------------------------*/
void sign_precomp_t_dtor(struct sign_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(sign_precomp_t_chk_members(p,315555660));
	bi_clear_zero(p->sign_a_x_mu);
	bi_dtor(p->sign_a_x_mu);
	bi_clear_zero(p->sign_y_h1_gamma_mu);
	bi_dtor(p->sign_y_h1_gamma_mu);
	bi_clear_zero(p->sign_h_r_mu);
	bi_dtor(p->sign_h_r_mu);
	bi_clear_zero(p->sign_g_h1_gamma_mu);
	bi_dtor(p->sign_g_h1_gamma_mu);
	bi_clear_zero(p->sign_g_e_gamma);
	bi_dtor(p->sign_g_e_gamma);
	bi_clear_zero(p->sign_g_r_mu);
	bi_dtor(p->sign_g_r_mu);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void sign_precomp_t_asg(struct sign_precomp_t* p, const struct sign_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(sign_precomp_t_chk_members(p,315555660));
	if (p != o) {
		bi_asg(p->sign_g_r_mu, o->sign_g_r_mu);
		bi_asg(p->sign_g_e_gamma, o->sign_g_e_gamma);
		bi_asg(p->sign_g_h1_gamma_mu, o->sign_g_h1_gamma_mu);
		bi_asg(p->sign_h_r_mu, o->sign_h_r_mu);
		bi_asg(p->sign_y_h1_gamma_mu, o->sign_y_h1_gamma_mu);
		bi_asg(p->sign_a_x_mu, o->sign_a_x_mu);
	}
}
/*----------------------------------------------------------------------------*/
void sign_precomp_t_move(struct sign_precomp_t* p, struct sign_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(sign_precomp_t_chk_members(p,315555660));
	if (p != o) {
		bi_asg_si(p->sign_g_r_mu, 0);
		bi_swap(p->sign_g_r_mu, o->sign_g_r_mu);
		bi_asg_si(p->sign_g_e_gamma, 0);
		bi_swap(p->sign_g_e_gamma, o->sign_g_e_gamma);
		bi_asg_si(p->sign_g_h1_gamma_mu, 0);
		bi_swap(p->sign_g_h1_gamma_mu, o->sign_g_h1_gamma_mu);
		bi_asg_si(p->sign_h_r_mu, 0);
		bi_swap(p->sign_h_r_mu, o->sign_h_r_mu);
		bi_asg_si(p->sign_y_h1_gamma_mu, 0);
		bi_swap(p->sign_y_h1_gamma_mu, o->sign_y_h1_gamma_mu);
		bi_asg_si(p->sign_a_x_mu, 0);
		bi_swap(p->sign_a_x_mu, o->sign_a_x_mu);
	}
}
/*----------------------------------------------------------------------------*/
struct sign_precomp_t* sign_precomp_t_new()/*auto*/
{
	struct sign_precomp_t* p = (struct sign_precomp_t*)malloc(sizeof(struct sign_precomp_t));
	if (p != NULL) {
		sign_precomp_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct sign_precomp_t* sign_precomp_t_clone(const struct sign_precomp_t* o)/*auto*/
{
	struct sign_precomp_t* p = NULL;
	if (o != NULL) {
		p = (struct sign_precomp_t*)malloc(sizeof(struct sign_precomp_t));
		if (p != NULL) {
			sign_precomp_t_ctor(p);
			sign_precomp_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void sign_precomp_t_delete(struct sign_precomp_t* p)/*auto*/
{
	if (p != NULL) {
		sign_precomp_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_fa_pbkey_t_chk_members(const struct ftmgs_fa_pbkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_fa_pbkey_t {
		paillier_thr_pbkey_t pbkey; 				/* n, g, y, nkeys */
	};
	CHK_FIELD__(dummy_ftmgs_fa_pbkey_t, ftmgs_fa_pbkey_t, pbkey);
	CHK_SIZE__(dummy_ftmgs_fa_pbkey_t, ftmgs_fa_pbkey_t);
	return (code == 371478823); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_ctor(struct ftmgs_fa_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_fa_pbkey_t_chk_members(p,371478823));
	paillier_thr_pbkey_t_ctor(&p->pbkey);
}
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_dtor(struct ftmgs_fa_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_fa_pbkey_t_chk_members(p,371478823));
	paillier_thr_pbkey_t_dtor(&p->pbkey);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_asg(struct ftmgs_fa_pbkey_t* p, const struct ftmgs_fa_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_fa_pbkey_t_chk_members(p,371478823));
	if (p != o) {
		paillier_thr_pbkey_t_asg(&p->pbkey, &o->pbkey);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_move(struct ftmgs_fa_pbkey_t* p, struct ftmgs_fa_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_fa_pbkey_t_chk_members(p,371478823));
	if (p != o) {
		paillier_thr_pbkey_t_move(&p->pbkey, &o->pbkey);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_fa_pbkey_t* ftmgs_fa_pbkey_t_new()/*auto*/
{
	struct ftmgs_fa_pbkey_t* p = (struct ftmgs_fa_pbkey_t*)malloc(sizeof(struct ftmgs_fa_pbkey_t));
	if (p != NULL) {
		ftmgs_fa_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_fa_pbkey_t* ftmgs_fa_pbkey_t_clone(const struct ftmgs_fa_pbkey_t* o)/*auto*/
{
	struct ftmgs_fa_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_fa_pbkey_t*)malloc(sizeof(struct ftmgs_fa_pbkey_t));
		if (p != NULL) {
			ftmgs_fa_pbkey_t_ctor(p);
			ftmgs_fa_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_delete(struct ftmgs_fa_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_fa_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_faj_pbkey_share_t_chk_members(const struct ftmgs_faj_pbkey_share_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_faj_pbkey_share_t {
		paillier_thr_pbkey_share_t pbkey_j;			/* nadrp & reveal */
	};
	CHK_FIELD__(dummy_ftmgs_faj_pbkey_share_t, ftmgs_faj_pbkey_share_t, pbkey_j);
	CHK_SIZE__(dummy_ftmgs_faj_pbkey_share_t, ftmgs_faj_pbkey_share_t);
	return (code == 236581397); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_ctor(struct ftmgs_faj_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_pbkey_share_t_chk_members(p,236581397));
	paillier_thr_pbkey_share_t_ctor(&p->pbkey_j);
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_dtor(struct ftmgs_faj_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_pbkey_share_t_chk_members(p,236581397));
	paillier_thr_pbkey_share_t_dtor(&p->pbkey_j);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_asg(struct ftmgs_faj_pbkey_share_t* p, const struct ftmgs_faj_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_pbkey_share_t_chk_members(p,236581397));
	if (p != o) {
		paillier_thr_pbkey_share_t_asg(&p->pbkey_j, &o->pbkey_j);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_move(struct ftmgs_faj_pbkey_share_t* p, struct ftmgs_faj_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_pbkey_share_t_chk_members(p,236581397));
	if (p != o) {
		paillier_thr_pbkey_share_t_move(&p->pbkey_j, &o->pbkey_j);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_pbkey_share_t* ftmgs_faj_pbkey_share_t_new()/*auto*/
{
	struct ftmgs_faj_pbkey_share_t* p = (struct ftmgs_faj_pbkey_share_t*)malloc(sizeof(struct ftmgs_faj_pbkey_share_t));
	if (p != NULL) {
		ftmgs_faj_pbkey_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_pbkey_share_t* ftmgs_faj_pbkey_share_t_clone(const struct ftmgs_faj_pbkey_share_t* o)/*auto*/
{
	struct ftmgs_faj_pbkey_share_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_faj_pbkey_share_t*)malloc(sizeof(struct ftmgs_faj_pbkey_share_t));
		if (p != NULL) {
			ftmgs_faj_pbkey_share_t_ctor(p);
			ftmgs_faj_pbkey_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_delete(struct ftmgs_faj_pbkey_share_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_faj_pbkey_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_faj_prkey_t_chk_members(const struct ftmgs_faj_prkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_faj_prkey_t {
		paillier_thr_prkey_t prkey_j;				/* nadrp & reveal */
	};
	CHK_FIELD__(dummy_ftmgs_faj_prkey_t, ftmgs_faj_prkey_t, prkey_j);
	CHK_SIZE__(dummy_ftmgs_faj_prkey_t, ftmgs_faj_prkey_t);
	return (code == 9417070); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_ctor(struct ftmgs_faj_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_prkey_t_chk_members(p,9417070));
	paillier_thr_prkey_t_ctor(&p->prkey_j);
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_dtor(struct ftmgs_faj_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_prkey_t_chk_members(p,9417070));
	paillier_thr_prkey_t_dtor(&p->prkey_j);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_asg(struct ftmgs_faj_prkey_t* p, const struct ftmgs_faj_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_prkey_t_chk_members(p,9417070));
	if (p != o) {
		paillier_thr_prkey_t_asg(&p->prkey_j, &o->prkey_j);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_move(struct ftmgs_faj_prkey_t* p, struct ftmgs_faj_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_prkey_t_chk_members(p,9417070));
	if (p != o) {
		paillier_thr_prkey_t_move(&p->prkey_j, &o->prkey_j);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_prkey_t* ftmgs_faj_prkey_t_new()/*auto*/
{
	struct ftmgs_faj_prkey_t* p = (struct ftmgs_faj_prkey_t*)malloc(sizeof(struct ftmgs_faj_prkey_t));
	if (p != NULL) {
		ftmgs_faj_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_prkey_t* ftmgs_faj_prkey_t_clone(const struct ftmgs_faj_prkey_t* o)/*auto*/
{
	struct ftmgs_faj_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_faj_prkey_t*)malloc(sizeof(struct ftmgs_faj_prkey_t));
		if (p != NULL) {
			ftmgs_faj_prkey_t_ctor(p);
			ftmgs_faj_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_delete(struct ftmgs_faj_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_faj_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_pbkey_t_chk_members(const struct ftmgs_pbkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_pbkey_t {
		elgamal_thr_pbkey_t gmpk;	 				/* n, g, y, nkeys */
		bigint_t a0;				/* zero */
		bigint_t a;					/* zero */
		bigint_t b;					/* zero */
		bigint_t h;					/* zero */
		paillier_thr_pbkey_t fapk; 					/* n, g, y, nkeys */
#ifdef PRECOMPUTATIONS__
		syssph_t syssph;
		sign_precomp_t sign_precomp;
		join_precomp_t join_precomp;
		nadrp_precomp_t nadrp_precomp;
#endif
	};
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, gmpk);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, a0);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, a);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, b);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, h);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, fapk);
#ifdef PRECOMPUTATIONS__
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, syssph);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, sign_precomp);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, join_precomp);
	CHK_FIELD__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t, nadrp_precomp);
#endif
	CHK_SIZE__(dummy_ftmgs_pbkey_t, ftmgs_pbkey_t);
	return (code == 506930046); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_ctor(struct ftmgs_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_pbkey_t_chk_members(p,506930046));
	elgamal_thr_pbkey_t_ctor(&p->gmpk);
	bi_ctor(p->a0);
	bi_ctor(p->a);
	bi_ctor(p->b);
	bi_ctor(p->h);
	paillier_thr_pbkey_t_ctor(&p->fapk);
#ifdef PRECOMPUTATIONS__
	syssph_t_ctor(&p->syssph);
	sign_precomp_t_ctor(&p->sign_precomp);
	join_precomp_t_ctor(&p->join_precomp);
	nadrp_precomp_t_ctor(&p->nadrp_precomp);
#endif
}
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_dtor(struct ftmgs_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_pbkey_t_chk_members(p,506930046));
#ifdef PRECOMPUTATIONS__
	nadrp_precomp_t_dtor(&p->nadrp_precomp);
	join_precomp_t_dtor(&p->join_precomp);
	sign_precomp_t_dtor(&p->sign_precomp);
	syssph_t_dtor(&p->syssph);
#endif
	paillier_thr_pbkey_t_dtor(&p->fapk);
	bi_clear_zero(p->h);
	bi_dtor(p->h);
	bi_clear_zero(p->b);
	bi_dtor(p->b);
	bi_clear_zero(p->a);
	bi_dtor(p->a);
	bi_clear_zero(p->a0);
	bi_dtor(p->a0);
	elgamal_thr_pbkey_t_dtor(&p->gmpk);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_asg(struct ftmgs_pbkey_t* p, const struct ftmgs_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_pbkey_t_chk_members(p,506930046));
	if (p != o) {
		elgamal_thr_pbkey_t_asg(&p->gmpk, &o->gmpk);
		bi_asg(p->a0, o->a0);
		bi_asg(p->a, o->a);
		bi_asg(p->b, o->b);
		bi_asg(p->h, o->h);
		paillier_thr_pbkey_t_asg(&p->fapk, &o->fapk);
#ifdef PRECOMPUTATIONS__
		syssph_t_asg(&p->syssph, &o->syssph);
		sign_precomp_t_asg(&p->sign_precomp, &o->sign_precomp);
		join_precomp_t_asg(&p->join_precomp, &o->join_precomp);
		nadrp_precomp_t_asg(&p->nadrp_precomp, &o->nadrp_precomp);
#endif
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_move(struct ftmgs_pbkey_t* p, struct ftmgs_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_pbkey_t_chk_members(p,506930046));
	if (p != o) {
		elgamal_thr_pbkey_t_move(&p->gmpk, &o->gmpk);
		bi_asg_si(p->a0, 0);
		bi_swap(p->a0, o->a0);
		bi_asg_si(p->a, 0);
		bi_swap(p->a, o->a);
		bi_asg_si(p->b, 0);
		bi_swap(p->b, o->b);
		bi_asg_si(p->h, 0);
		bi_swap(p->h, o->h);
		paillier_thr_pbkey_t_move(&p->fapk, &o->fapk);
#ifdef PRECOMPUTATIONS__
		syssph_t_move(&p->syssph, &o->syssph);
		sign_precomp_t_move(&p->sign_precomp, &o->sign_precomp);
		join_precomp_t_move(&p->join_precomp, &o->join_precomp);
		nadrp_precomp_t_move(&p->nadrp_precomp, &o->nadrp_precomp);
#endif
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_pbkey_t* ftmgs_pbkey_t_new()/*auto*/
{
	struct ftmgs_pbkey_t* p = (struct ftmgs_pbkey_t*)malloc(sizeof(struct ftmgs_pbkey_t));
	if (p != NULL) {
		ftmgs_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_pbkey_t* ftmgs_pbkey_t_clone(const struct ftmgs_pbkey_t* o)/*auto*/
{
	struct ftmgs_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_pbkey_t*)malloc(sizeof(struct ftmgs_pbkey_t));
		if (p != NULL) {
			ftmgs_pbkey_t_ctor(p);
			ftmgs_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_delete(struct ftmgs_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_prkey_t_chk_members(const struct ftmgs_prkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_prkey_t {
		elgamal_thr_modfact_t npq;					/* join */
	};
	CHK_FIELD__(dummy_ftmgs_prkey_t, ftmgs_prkey_t, npq);
	CHK_SIZE__(dummy_ftmgs_prkey_t, ftmgs_prkey_t);
	return (code == 127033500); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_ctor(struct ftmgs_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_prkey_t_chk_members(p,127033500));
	elgamal_thr_modfact_t_ctor(&p->npq);
}
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_dtor(struct ftmgs_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_prkey_t_chk_members(p,127033500));
	elgamal_thr_modfact_t_dtor(&p->npq);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_asg(struct ftmgs_prkey_t* p, const struct ftmgs_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_prkey_t_chk_members(p,127033500));
	if (p != o) {
		elgamal_thr_modfact_t_asg(&p->npq, &o->npq);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_move(struct ftmgs_prkey_t* p, struct ftmgs_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_prkey_t_chk_members(p,127033500));
	if (p != o) {
		elgamal_thr_modfact_t_move(&p->npq, &o->npq);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_prkey_t* ftmgs_prkey_t_new()/*auto*/
{
	struct ftmgs_prkey_t* p = (struct ftmgs_prkey_t*)malloc(sizeof(struct ftmgs_prkey_t));
	if (p != NULL) {
		ftmgs_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_prkey_t* ftmgs_prkey_t_clone(const struct ftmgs_prkey_t* o)/*auto*/
{
	struct ftmgs_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_prkey_t*)malloc(sizeof(struct ftmgs_prkey_t));
		if (p != NULL) {
			ftmgs_prkey_t_ctor(p);
			ftmgs_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_delete(struct ftmgs_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_faj_gr_pbkey_share_t_chk_members(const struct ftmgs_faj_gr_pbkey_share_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_faj_gr_pbkey_share_t {
		elgamal_thr_pbkey_share_t pbkey_j;
		bigint_t hj;						/* zero */
	};
	CHK_FIELD__(dummy_ftmgs_faj_gr_pbkey_share_t, ftmgs_faj_gr_pbkey_share_t, pbkey_j);
	CHK_FIELD__(dummy_ftmgs_faj_gr_pbkey_share_t, ftmgs_faj_gr_pbkey_share_t, hj);
	CHK_SIZE__(dummy_ftmgs_faj_gr_pbkey_share_t, ftmgs_faj_gr_pbkey_share_t);
	return (code == 115542349); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_ctor(struct ftmgs_faj_gr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_gr_pbkey_share_t_chk_members(p,115542349));
	elgamal_thr_pbkey_share_t_ctor(&p->pbkey_j);
	bi_ctor(p->hj);
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_dtor(struct ftmgs_faj_gr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_gr_pbkey_share_t_chk_members(p,115542349));
	bi_clear_zero(p->hj);
	bi_dtor(p->hj);
	elgamal_thr_pbkey_share_t_dtor(&p->pbkey_j);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_asg(struct ftmgs_faj_gr_pbkey_share_t* p, const struct ftmgs_faj_gr_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_gr_pbkey_share_t_chk_members(p,115542349));
	if (p != o) {
		elgamal_thr_pbkey_share_t_asg(&p->pbkey_j, &o->pbkey_j);
		bi_asg(p->hj, o->hj);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_move(struct ftmgs_faj_gr_pbkey_share_t* p, struct ftmgs_faj_gr_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_gr_pbkey_share_t_chk_members(p,115542349));
	if (p != o) {
		elgamal_thr_pbkey_share_t_move(&p->pbkey_j, &o->pbkey_j);
		bi_asg_si(p->hj, 0);
		bi_swap(p->hj, o->hj);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_gr_pbkey_share_t* ftmgs_faj_gr_pbkey_share_t_new()/*auto*/
{
	struct ftmgs_faj_gr_pbkey_share_t* p = (struct ftmgs_faj_gr_pbkey_share_t*)malloc(sizeof(struct ftmgs_faj_gr_pbkey_share_t));
	if (p != NULL) {
		ftmgs_faj_gr_pbkey_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_gr_pbkey_share_t* ftmgs_faj_gr_pbkey_share_t_clone(const struct ftmgs_faj_gr_pbkey_share_t* o)/*auto*/
{
	struct ftmgs_faj_gr_pbkey_share_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_faj_gr_pbkey_share_t*)malloc(sizeof(struct ftmgs_faj_gr_pbkey_share_t));
		if (p != NULL) {
			ftmgs_faj_gr_pbkey_share_t_ctor(p);
			ftmgs_faj_gr_pbkey_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_delete(struct ftmgs_faj_gr_pbkey_share_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_faj_gr_pbkey_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_faj_gr_prkey_t_chk_members(const struct ftmgs_faj_gr_prkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_ftmgs_faj_gr_prkey_t {
		elgamal_thr_prkey_t prkey_j;				/* open */
	};
	CHK_FIELD__(dummy_ftmgs_faj_gr_prkey_t, ftmgs_faj_gr_prkey_t, prkey_j);
	CHK_SIZE__(dummy_ftmgs_faj_gr_prkey_t, ftmgs_faj_gr_prkey_t);
	return (code == 217729931); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_ctor(struct ftmgs_faj_gr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_gr_prkey_t_chk_members(p,217729931));
	elgamal_thr_prkey_t_ctor(&p->prkey_j);
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_dtor(struct ftmgs_faj_gr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_faj_gr_prkey_t_chk_members(p,217729931));
	elgamal_thr_prkey_t_dtor(&p->prkey_j);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_asg(struct ftmgs_faj_gr_prkey_t* p, const struct ftmgs_faj_gr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_gr_prkey_t_chk_members(p,217729931));
	if (p != o) {
		elgamal_thr_prkey_t_asg(&p->prkey_j, &o->prkey_j);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_move(struct ftmgs_faj_gr_prkey_t* p, struct ftmgs_faj_gr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_faj_gr_prkey_t_chk_members(p,217729931));
	if (p != o) {
		elgamal_thr_prkey_t_move(&p->prkey_j, &o->prkey_j);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_gr_prkey_t* ftmgs_faj_gr_prkey_t_new()/*auto*/
{
	struct ftmgs_faj_gr_prkey_t* p = (struct ftmgs_faj_gr_prkey_t*)malloc(sizeof(struct ftmgs_faj_gr_prkey_t));
	if (p != NULL) {
		ftmgs_faj_gr_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_faj_gr_prkey_t* ftmgs_faj_gr_prkey_t_clone(const struct ftmgs_faj_gr_prkey_t* o)/*auto*/
{
	struct ftmgs_faj_gr_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_faj_gr_prkey_t*)malloc(sizeof(struct ftmgs_faj_gr_prkey_t));
		if (p != NULL) {
			ftmgs_faj_gr_prkey_t_ctor(p);
			ftmgs_faj_gr_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_delete(struct ftmgs_faj_gr_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_faj_gr_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

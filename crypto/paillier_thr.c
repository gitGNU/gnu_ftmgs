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
#include "paillier_thr.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "iexport.h"
#include "generator.h"
#include "sok.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_precomp(paillier_thr_precomp_t* precomp,
								const paillier_thr_pbkey_t* pk)
{
	bi_sq(precomp->n2, pk->n);
	create_sph_upsilon(&precomp->upsilon, pk->n, &pk->sp);
	create_sph_psi(&precomp->psi, pk->n, &pk->sp);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_create_pbkey(paillier_thr_pbkey_t* pk, 
							   unsigned nu,
							   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_6_bigint_t(p, p1, q, q1, n2, g1);
	/*--------------------------------*/
	set_syspar(&pk->sp, nu);
	/*--------------------------------*/
	bi_random_sgprime_nb(p, p1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	do {
		bi_random_sgprime_nb(q, q1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	} while ( bi_equals(p, q) );
	bi_mul(pk->n, p, q);
	/*--------------------------------*/
	bi_sq(n2, pk->n);
	bi_random_ub(g1, n2, rnd_ctx);
	bi_sqmod(pk->g, g1, n2);
	bi_powmod_sec(pk->g, pk->g, pk->n, n2);
	/*--------------------------------*/
	bi_asg_ui(pk->y, 1);
	pk->nkeys = 0;
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	paillier_thr_pbkey_precomp(&pk->precomp, pk);
#endif
	/*--------------------------------*/
	END_VAR_6_bigint_t(p, p1, q, q1, n2, g1);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_thr_create_prkey(paillier_thr_pbkey_share_t* pkj,
							   paillier_thr_prkey_t* skj,
							   const paillier_thr_pbkey_t* pk,
							   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	/*--------------------------------*/
	do {
		bi_random_prime_sph_ek(skj->xj, &pk->precomp.upsilon, DEF_NTESTS_LOWSEC,
							   rnd_ctx);
	} while (bi_iszero(skj->xj));
	bi_powmod_sec(pkj->yj, pk->g, skj->xj, pk->precomp.n2);
	/*--------------------------------*/
#else
	BEG_VAR_1(sphere_t, upsilon);
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, pk->n);
	create_sph_upsilon(&upsilon, pk->n, &pk->sp);
	/*--------------------------------*/
	do {
		bi_random_prime_sph_ek(skj->xj, &upsilon, DEF_NTESTS_LOWSEC, rnd_ctx);
	} while (bi_iszero(skj->xj));
	bi_powmod_sec(pkj->yj, pk->g, skj->xj, n2);
	/*--------------------------------*/
	END_VAR_1_bigint_t(n2);
	END_VAR_1(sphere_t, upsilon);
#endif
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_thr_add_pbkey_share(paillier_thr_pbkey_t* pk,
								  const paillier_thr_pbkey_share_t* pkj)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	if (pk->nkeys == 0) {
		bi_asg_ui(pk->y, 1);
	}
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	/*--------------------------------*/
	bi_mulmod(pk->y, pk->y, pkj->yj, pk->precomp.n2);
	++pk->nkeys;
	/*--------------------------------*/
#else
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, pk->n);
	/*--------------------------------*/
	bi_mulmod(pk->y, pk->y, pkj->yj, n2);
	++pk->nkeys;
	/*--------------------------------*/
	END_VAR_1_bigint_t(n2);
#endif
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_thr_encrypt_k(paillier_thr_ciphertext_t* ct,
							bigint_t k,
							const bigint_t msg,
							const paillier_thr_pbkey_t* pk,
							rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1_bigint_t(h);
	/*--------------------------------*/
	assert( ! bi_iszero(msg) && bi_less(msg, pk->n));
	do {
		bi_random_sph_ek(k, &pk->precomp.psi, rnd_ctx);
	} while (bi_iszero(k));
	/*--------------------------------*/
	bi_add_ui(h, pk->n, 1);
	bi_powmod_sec(h, h, msg, pk->precomp.n2);
	/*--------------------------------*/
	bi_powmod_sec(ct->alpha, pk->g, k, pk->precomp.n2);
	bi_powmod_sec(ct->beta, pk->y, k, pk->precomp.n2);
	bi_mulmod(ct->beta, ct->beta, h, pk->precomp.n2);
	/*--------------------------------*/
	END_VAR_1_bigint_t(h);
#else
	BEG_VAR_1(sphere_t, psi);
	BEG_VAR_2_bigint_t(n2, h);
	bi_sq(n2, pk->n);
	create_sph_psi(&psi, pk->n, &pk->sp);
	/*--------------------------------*/
	assert( ! bi_iszero(msg) && bi_less(msg, pk->n));
	do {
		bi_random_sph_ek(k, &psi, rnd_ctx);
	} while (bi_iszero(k));
	/*--------------------------------*/
	bi_add_ui(h, pk->n, 1);
	bi_powmod_sec(h, h, msg, n2);
	/*--------------------------------*/
	bi_powmod_sec(ct->alpha, pk->g, k, n2);
	bi_powmod_sec(ct->beta, pk->y, k, n2);
	bi_mulmod(ct->beta, ct->beta, h, n2);
	/*--------------------------------*/
	END_VAR_2_bigint_t(n2, h);
	END_VAR_1(sphere_t, psi);
#endif
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_thr_encrypt(paillier_thr_ciphertext_t* ct,
						  const bigint_t msg,
						  const paillier_thr_pbkey_t* pk,
						  rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(k);
	paillier_thr_encrypt_k(ct, k, msg, pk, rnd_ctx);
	END_VAR_1_bigint_t(k);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
bool_t paillier_thr_decrypt_share(paillier_thr_decrypt_share_t* dsj,
								const paillier_thr_ciphertext_t* ct,
								const paillier_thr_pbkey_t* pk,
								const paillier_thr_pbkey_share_t* pkj,
								const paillier_thr_prkey_t* skj,
								const void* dat_digest,
								unsigned dat_digestlen,
								rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = &pk->precomp.n2;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &((const paillier_thr_decrypt_share_t*)dsj)->alpha_xj;
	dlog_prf[1].n = &pk->precomp.n2;
	/*--------------------------------*/
	bi_powmod_sec(dsj->alpha_xj, ct->alpha, skj->xj, pk->precomp.n2);
	/*--------------------------------*/
	ok = dlog_proof(&dsj->c, &dsj->sx,
			   dat_digest, dat_digestlen,
			   array_sz(dlog_prf),
			   dlog_prf,
			   &pk->precomp.upsilon,
			   &skj->xj,
			   &pk->sp,
			   rnd_ctx);
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	/*--------------------------------*/
#else
	BEG_VAR_1(sphere_t, upsilon);
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, pk->n);
	create_sph_upsilon(&upsilon, pk->n, &pk->sp);
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = (const bigint_t*)&n2;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &((const paillier_thr_decrypt_share_t*)dsj)->alpha_xj;
	dlog_prf[1].n = (const bigint_t*)&n2;
	/*--------------------------------*/
	bi_powmod_sec(dsj->alpha_xj, ct->alpha, skj->xj, n2);
	/*--------------------------------*/
	ok = dlog_proof(&dsj->c, &dsj->sx,
			   dat_digest, dat_digestlen,
			   array_sz(dlog_prf),
			   dlog_prf,
			   &upsilon,
			   &skj->xj,
			   &pk->sp,
			   rnd_ctx);
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_VAR_1_bigint_t(n2);
	END_VAR_1(sphere_t, upsilon);
#endif
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t paillier_thr_add_decrypt_share(paillier_thr_dshare_acc_t* alpha_pi,
									  const paillier_thr_decrypt_share_t* dsj,
									  const paillier_thr_ciphertext_t* ct,
									  const paillier_thr_pbkey_t* pk,
									  const paillier_thr_pbkey_share_t* pkj,
									  const void* dat_digest,
									  unsigned dat_digestlen)
{
	BEG_BIGINT_CHK();
	bool_t ok = FALSE;
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = &pk->precomp.n2;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &dsj->alpha_xj;
	dlog_prf[1].n = &pk->precomp.n2;
	/*--------------------------------*/
	if ( dlog_vrfy(&dsj->c, &dsj->sx,
				   dat_digest, dat_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   &pk->precomp.upsilon,
				   &pk->sp)) {
		if (alpha_pi->nshares == 0) {
			bi_asg_ui(alpha_pi->a, 1);
		}
		ok = TRUE;
		bi_mulmod(alpha_pi->a, alpha_pi->a, dsj->alpha_xj, pk->precomp.n2);
		++alpha_pi->nshares;
	}
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	/*--------------------------------*/
#else
	BEG_VAR_1(sphere_t, upsilon);
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, pk->n);
	create_sph_upsilon(&upsilon, pk->n, &pk->sp);
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = (const bigint_t*)&n2;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &dsj->alpha_xj;
	dlog_prf[1].n = (const bigint_t*)&n2;
	/*--------------------------------*/
	if ( dlog_vrfy(&dsj->c, &dsj->sx,
				   dat_digest, dat_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   &upsilon,
				   &pk->sp)) {
		if (alpha_pi->nshares == 0) {
			bi_asg_ui(alpha_pi->a, 1);
		}
		ok = TRUE;
		bi_mulmod(alpha_pi->a, alpha_pi->a, dsj->alpha_xj, n2);
		++alpha_pi->nshares;
	}
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_VAR_1_bigint_t(n2);
	END_VAR_1(sphere_t, upsilon);
#endif
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t paillier_thr_decrypt(bigint_t msg,
							const paillier_thr_ciphertext_t* ct,
							const paillier_thr_pbkey_t* pk,
							const paillier_thr_dshare_acc_t* alpha_pi)
{
	BEG_BIGINT_CHK();
	bool_t ok;
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	/*--------------------------------*/
	if (alpha_pi->nshares != pk->nkeys) {
		ok = FALSE;
		bi_asg_ui(msg, 0);
	} else {
		BEG_VAR_2_bigint_t(t, rem);
		ok = TRUE;
		bi_invmod(msg, alpha_pi->a, pk->precomp.n2);
		bi_mulmod(msg, msg, ct->beta, pk->precomp.n2);
		/*----------------------------*/
		bi_asg_ui(t, 2);
		bi_invmod(t, t, pk->n);
		bi_sqmod(msg, msg, pk->precomp.n2);
		bi_powmod_sec(msg, msg, t, pk->precomp.n2);
		bi_sub_ui(msg, msg, 1);
		bi_div(msg, msg, pk->n, rem);
		if (! bi_iszero(rem) ) {
			ok = FALSE;
			bi_asg_ui(msg, 0);
		}
		END_VAR_2_bigint_t(t, rem);
	}
	/*--------------------------------*/
#else
	/*--------------------------------*/
	if (alpha_pi->nshares != pk->nkeys) {
		ok = FALSE;
		bi_asg_ui(msg, 0);
	} else {
		BEG_VAR_3_bigint_t(n2, t, rem);
		ok = TRUE;
		bi_sq(n2, pk->n);
		bi_invmod(msg, alpha_pi->a, n2);
		bi_mulmod(msg, msg, ct->beta, n2);
		/*----------------------------*/
		bi_asg_ui(t, 2);
		bi_invmod(t, t, pk->n);
		bi_sqmod(msg, msg, n2);
		bi_powmod_sec(msg, msg, t, n2);
		bi_sub_ui(msg, msg, 1);
		bi_div(msg, msg, pk->n, rem);
		if (! bi_iszero(rem) ) {
			ok = FALSE;
			bi_asg_ui(msg, 0);
		}
		END_VAR_3_bigint_t(n2, t, rem);
	}
	/*--------------------------------*/
#endif
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_precomp_t_chk_members(struct paillier_thr_precomp_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_precomp_t {
		bigint_t n2;
		sphere_t upsilon;			/* { 0 .. Z_{n^2/4} } */
		sphere_t psi;				/* { 0 .. Z_{n/4} } */
	};
	CHK_FIELD__(dummy_paillier_thr_precomp_t, paillier_thr_precomp_t, n2);
	CHK_FIELD__(dummy_paillier_thr_precomp_t, paillier_thr_precomp_t, upsilon);
	CHK_FIELD__(dummy_paillier_thr_precomp_t, paillier_thr_precomp_t, psi);
	CHK_SIZE__(dummy_paillier_thr_precomp_t, paillier_thr_precomp_t);
	return (p!=NULL)&&(code == 224976149);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_precomp_t_ctor(struct paillier_thr_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_precomp_t_chk_members(p,224976149));
	bi_ctor(p->n2);
	sphere_t_ctor(&p->upsilon);
	sphere_t_ctor(&p->psi);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_precomp_t_dtor(struct paillier_thr_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_precomp_t_chk_members(p,224976149));
	sphere_t_dtor(&p->psi);
	sphere_t_dtor(&p->upsilon);
	bi_dtor(p->n2);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_precomp_t_asg(struct paillier_thr_precomp_t* p, const struct paillier_thr_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_precomp_t_chk_members(p,224976149));
	if (p != o) {
		bi_asg(p->n2, o->n2);
		sphere_t_asg(&p->upsilon, &o->upsilon);
		sphere_t_asg(&p->psi, &o->psi);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_precomp_t* paillier_thr_precomp_t_new()/*auto*/
{
	struct paillier_thr_precomp_t* p = (struct paillier_thr_precomp_t*)malloc(sizeof(struct paillier_thr_precomp_t));
	if (p != NULL) {
		paillier_thr_precomp_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_precomp_t* paillier_thr_precomp_t_clone(const struct paillier_thr_precomp_t* o)/*auto*/
{
	struct paillier_thr_precomp_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_precomp_t*)malloc(sizeof(struct paillier_thr_precomp_t));
		if (p != NULL) {
			paillier_thr_precomp_t_ctor(p);
			paillier_thr_precomp_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_precomp_t_delete(struct paillier_thr_precomp_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_precomp_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_pbkey_t_chk_members(struct paillier_thr_pbkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_pbkey_t {
		syspar_t sp;
		bigint_t n;
		bigint_t g;
		bigint_t y;					/* y = PROD_j(yj) */
		unsigned nkeys;
#ifdef PRECOMPUTATIONS__
		paillier_thr_precomp_t precomp;
#endif
	};
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, sp);
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, n);
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, g);
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, y);
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, nkeys);
#ifdef PRECOMPUTATIONS__
	CHK_FIELD__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t, precomp);
#endif
	CHK_SIZE__(dummy_paillier_thr_pbkey_t, paillier_thr_pbkey_t);
	return (p!=NULL)&&(code == 118408740);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_t_ctor(struct paillier_thr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_pbkey_t_chk_members(p,118408740));
	syspar_t_ctor(&p->sp);
	bi_ctor(p->n);
	bi_ctor(p->g);
	bi_ctor(p->y);
	p->nkeys = 0;
#ifdef PRECOMPUTATIONS__
	paillier_thr_precomp_t_ctor(&p->precomp);
#endif
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_t_dtor(struct paillier_thr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_pbkey_t_chk_members(p,118408740));
#ifdef PRECOMPUTATIONS__
	paillier_thr_precomp_t_dtor(&p->precomp);
#endif
	bi_dtor(p->y);
	bi_dtor(p->g);
	bi_dtor(p->n);
	syspar_t_dtor(&p->sp);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_t_asg(struct paillier_thr_pbkey_t* p, const struct paillier_thr_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_pbkey_t_chk_members(p,118408740));
	if (p != o) {
		syspar_t_asg(&p->sp, &o->sp);
		bi_asg(p->n, o->n);
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
		p->nkeys = o->nkeys;
#ifdef PRECOMPUTATIONS__
		paillier_thr_precomp_t_asg(&p->precomp, &o->precomp);
#endif
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_pbkey_t* paillier_thr_pbkey_t_new()/*auto*/
{
	struct paillier_thr_pbkey_t* p = (struct paillier_thr_pbkey_t*)malloc(sizeof(struct paillier_thr_pbkey_t));
	if (p != NULL) {
		paillier_thr_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_pbkey_t* paillier_thr_pbkey_t_clone(const struct paillier_thr_pbkey_t* o)/*auto*/
{
	struct paillier_thr_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_pbkey_t*)malloc(sizeof(struct paillier_thr_pbkey_t));
		if (p != NULL) {
			paillier_thr_pbkey_t_ctor(p);
			paillier_thr_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_t_delete(struct paillier_thr_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_pbkey_share_t_chk_members(struct paillier_thr_pbkey_share_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_pbkey_share_t {
		bigint_t yj;				/* yj = g^xj (mod n) */
	};
	CHK_FIELD__(dummy_paillier_thr_pbkey_share_t, paillier_thr_pbkey_share_t, yj);
	CHK_SIZE__(dummy_paillier_thr_pbkey_share_t, paillier_thr_pbkey_share_t);
	return (p!=NULL)&&(code == 159599017);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_share_t_ctor(struct paillier_thr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_pbkey_share_t_chk_members(p,159599017));
	bi_ctor(p->yj);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_share_t_dtor(struct paillier_thr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_pbkey_share_t_chk_members(p,159599017));
	bi_dtor(p->yj);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_share_t_asg(struct paillier_thr_pbkey_share_t* p, const struct paillier_thr_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_pbkey_share_t_chk_members(p,159599017));
	if (p != o) {
		bi_asg(p->yj, o->yj);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_pbkey_share_t* paillier_thr_pbkey_share_t_new()/*auto*/
{
	struct paillier_thr_pbkey_share_t* p = (struct paillier_thr_pbkey_share_t*)malloc(sizeof(struct paillier_thr_pbkey_share_t));
	if (p != NULL) {
		paillier_thr_pbkey_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_pbkey_share_t* paillier_thr_pbkey_share_t_clone(const struct paillier_thr_pbkey_share_t* o)/*auto*/
{
	struct paillier_thr_pbkey_share_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_pbkey_share_t*)malloc(sizeof(struct paillier_thr_pbkey_share_t));
		if (p != NULL) {
			paillier_thr_pbkey_share_t_ctor(p);
			paillier_thr_pbkey_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_share_t_delete(struct paillier_thr_pbkey_share_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_pbkey_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_prkey_t_chk_members(struct paillier_thr_prkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_prkey_t {
		bigint_t xj;
	};
	CHK_FIELD__(dummy_paillier_thr_prkey_t, paillier_thr_prkey_t, xj);
	CHK_SIZE__(dummy_paillier_thr_prkey_t, paillier_thr_prkey_t);
	return (p!=NULL)&&(code == 198508566);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_prkey_t_ctor(struct paillier_thr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_prkey_t_chk_members(p,198508566));
	bi_ctor(p->xj);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_prkey_t_dtor(struct paillier_thr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_prkey_t_chk_members(p,198508566));
	bi_dtor(p->xj);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_prkey_t_asg(struct paillier_thr_prkey_t* p, const struct paillier_thr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_prkey_t_chk_members(p,198508566));
	if (p != o) {
		bi_asg(p->xj, o->xj);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_prkey_t* paillier_thr_prkey_t_new()/*auto*/
{
	struct paillier_thr_prkey_t* p = (struct paillier_thr_prkey_t*)malloc(sizeof(struct paillier_thr_prkey_t));
	if (p != NULL) {
		paillier_thr_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_prkey_t* paillier_thr_prkey_t_clone(const struct paillier_thr_prkey_t* o)/*auto*/
{
	struct paillier_thr_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_prkey_t*)malloc(sizeof(struct paillier_thr_prkey_t));
		if (p != NULL) {
			paillier_thr_prkey_t_ctor(p);
			paillier_thr_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_prkey_t_delete(struct paillier_thr_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_ciphertext_t_chk_members(struct paillier_thr_ciphertext_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_ciphertext_t {
		bigint_t alpha;
		bigint_t beta;
	};
	CHK_FIELD__(dummy_paillier_thr_ciphertext_t, paillier_thr_ciphertext_t, alpha);
	CHK_FIELD__(dummy_paillier_thr_ciphertext_t, paillier_thr_ciphertext_t, beta);
	CHK_SIZE__(dummy_paillier_thr_ciphertext_t, paillier_thr_ciphertext_t);
	return (p!=NULL)&&(code == 211691810);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_ciphertext_t_ctor(struct paillier_thr_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_ciphertext_t_chk_members(p,211691810));
	bi_ctor(p->alpha);
	bi_ctor(p->beta);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_ciphertext_t_dtor(struct paillier_thr_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_ciphertext_t_chk_members(p,211691810));
	bi_dtor(p->beta);
	bi_dtor(p->alpha);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_ciphertext_t_asg(struct paillier_thr_ciphertext_t* p, const struct paillier_thr_ciphertext_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_ciphertext_t_chk_members(p,211691810));
	if (p != o) {
		bi_asg(p->alpha, o->alpha);
		bi_asg(p->beta, o->beta);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_ciphertext_t* paillier_thr_ciphertext_t_new()/*auto*/
{
	struct paillier_thr_ciphertext_t* p = (struct paillier_thr_ciphertext_t*)malloc(sizeof(struct paillier_thr_ciphertext_t));
	if (p != NULL) {
		paillier_thr_ciphertext_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_ciphertext_t* paillier_thr_ciphertext_t_clone(const struct paillier_thr_ciphertext_t* o)/*auto*/
{
	struct paillier_thr_ciphertext_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_ciphertext_t*)malloc(sizeof(struct paillier_thr_ciphertext_t));
		if (p != NULL) {
			paillier_thr_ciphertext_t_ctor(p);
			paillier_thr_ciphertext_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_ciphertext_t_delete(struct paillier_thr_ciphertext_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_ciphertext_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_decrypt_share_t_chk_members(struct paillier_thr_decrypt_share_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_decrypt_share_t {
		bigint_t alpha_xj;
		bigint_t c;
		bigint_t sx;
	};
	CHK_FIELD__(dummy_paillier_thr_decrypt_share_t, paillier_thr_decrypt_share_t, alpha_xj);
	CHK_FIELD__(dummy_paillier_thr_decrypt_share_t, paillier_thr_decrypt_share_t, c);
	CHK_FIELD__(dummy_paillier_thr_decrypt_share_t, paillier_thr_decrypt_share_t, sx);
	CHK_SIZE__(dummy_paillier_thr_decrypt_share_t, paillier_thr_decrypt_share_t);
	return (p!=NULL)&&(code == 524542999);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_decrypt_share_t_ctor(struct paillier_thr_decrypt_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_decrypt_share_t_chk_members(p,524542999));
	bi_ctor(p->alpha_xj);
	bi_ctor(p->c);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_decrypt_share_t_dtor(struct paillier_thr_decrypt_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_decrypt_share_t_chk_members(p,524542999));
	bi_dtor(p->sx);
	bi_dtor(p->c);
	bi_dtor(p->alpha_xj);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_decrypt_share_t_asg(struct paillier_thr_decrypt_share_t* p, const struct paillier_thr_decrypt_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_decrypt_share_t_chk_members(p,524542999));
	if (p != o) {
		bi_asg(p->alpha_xj, o->alpha_xj);
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_decrypt_share_t* paillier_thr_decrypt_share_t_new()/*auto*/
{
	struct paillier_thr_decrypt_share_t* p = (struct paillier_thr_decrypt_share_t*)malloc(sizeof(struct paillier_thr_decrypt_share_t));
	if (p != NULL) {
		paillier_thr_decrypt_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_decrypt_share_t* paillier_thr_decrypt_share_t_clone(const struct paillier_thr_decrypt_share_t* o)/*auto*/
{
	struct paillier_thr_decrypt_share_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_decrypt_share_t*)malloc(sizeof(struct paillier_thr_decrypt_share_t));
		if (p != NULL) {
			paillier_thr_decrypt_share_t_ctor(p);
			paillier_thr_decrypt_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_decrypt_share_t_delete(struct paillier_thr_decrypt_share_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_decrypt_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_thr_dshare_acc_t_chk_members(struct paillier_thr_dshare_acc_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_paillier_thr_dshare_acc_t {
		unsigned nshares;
		bigint_t a;
	};
	CHK_FIELD__(dummy_paillier_thr_dshare_acc_t, paillier_thr_dshare_acc_t, nshares);
	CHK_FIELD__(dummy_paillier_thr_dshare_acc_t, paillier_thr_dshare_acc_t, a);
	CHK_SIZE__(dummy_paillier_thr_dshare_acc_t, paillier_thr_dshare_acc_t);
	return (p!=NULL)&&(code == 266953664);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_thr_dshare_acc_t_ctor(struct paillier_thr_dshare_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_dshare_acc_t_chk_members(p,266953664));
	p->nshares = 0;
	bi_ctor(p->a);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_dshare_acc_t_dtor(struct paillier_thr_dshare_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_thr_dshare_acc_t_chk_members(p,266953664));
	bi_dtor(p->a);
}
/*----------------------------------------------------------------------------*/
void paillier_thr_dshare_acc_t_asg(struct paillier_thr_dshare_acc_t* p, const struct paillier_thr_dshare_acc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_thr_dshare_acc_t_chk_members(p,266953664));
	if (p != o) {
		p->nshares = o->nshares;
		bi_asg(p->a, o->a);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_dshare_acc_t* paillier_thr_dshare_acc_t_new()/*auto*/
{
	struct paillier_thr_dshare_acc_t* p = (struct paillier_thr_dshare_acc_t*)malloc(sizeof(struct paillier_thr_dshare_acc_t));
	if (p != NULL) {
		paillier_thr_dshare_acc_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_thr_dshare_acc_t* paillier_thr_dshare_acc_t_clone(const struct paillier_thr_dshare_acc_t* o)/*auto*/
{
	struct paillier_thr_dshare_acc_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_thr_dshare_acc_t*)malloc(sizeof(struct paillier_thr_dshare_acc_t));
		if (p != NULL) {
			paillier_thr_dshare_acc_t_ctor(p);
			paillier_thr_dshare_acc_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_thr_dshare_acc_t_delete(struct paillier_thr_dshare_acc_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_thr_dshare_acc_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

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
#include "elgamal_thr.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "iexport.h"
#include "generator.h"
#include "sok.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void elgamal_thr_create_pbkey(elgamal_thr_pbkey_t* pk, 
							  elgamal_thr_modfact_t* npq,
							  elgamal_thr_modord_t* p1q1,
							  unsigned nu,
							  rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_5_bigint_t(p, q, p1, q1, order);
	/*--------------------------------*/
	set_syspar(&pk->sp, nu);
	/*--------------------------------*/
	bi_random_sgprime_nb(p, p1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	do {
		bi_random_sgprime_nb(q, q1, pk->sp.nu/2, DEF_NTESTS, rnd_ctx);
	} while ( bi_equals(p, q) );
	bi_mul(pk->n, p, q);
	bi_mul(order, p1, q1);
	/*--------------------------------*/
	qrn_generator(pk->g, pk->n, order, p1, q1, rnd_ctx);
	/*--------------------------------*/
	bi_asg_ui(pk->y, 1);
	pk->nkeys = 0;
	/*--------------------------------*/
	if (npq != NULL) {
		bi_swap(npq->p , p);
		bi_swap(npq->q , q);
	}
	if (p1q1 != NULL) {
		bi_swap(p1q1->p1 , p1);
		bi_swap(p1q1->q1 , q1);
	}
	/*--------------------------------*/
	END_VAR_5_bigint_t(p, q, p1, q1, order);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_create_prkey(elgamal_thr_pbkey_share_t* pkj,
							  elgamal_thr_prkey_t* skj,
							  const elgamal_thr_pbkey_t* pk,
							  const sphere_t* sph,
							  rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	do {
		bi_random_prime_sph_ek(skj->xj, sph, DEF_NTESTS_LOWSEC, rnd_ctx);
		/*bi_random_prime_nb(skj->xj, pk->sp.nu/2,DEF_NTESTS_LOWSEC, rnd_ctx);*/
	} while (bi_iszero(skj->xj));
	bi_powmod_sec(pkj->yj, pk->g, skj->xj, pk->n);
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_add_pbkey_share(elgamal_thr_pbkey_t* pk,
								 const elgamal_thr_pbkey_share_t* pkj)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	if (pk->nkeys == 0) {
		bi_asg_ui(pk->y, 1);
	}
	/*--------------------------------*/
	bi_mulmod(pk->y, pk->y, pkj->yj, pk->n);
	++pk->nkeys;
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_encrypt_k(elgamal_thr_ciphertext_t* ct,
						   bigint_t k,
						   const bigint_t msg,
						   const elgamal_thr_pbkey_t* pk,
						   const sphere_t* sph,
						   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	assert( ! bi_iszero(msg) && bi_less(msg, pk->n));
	/*--------------------------------*/
	do {
		bi_random_sph_ek(k, sph, rnd_ctx);
		/*bi_random_nb(k, sp->nu/2, rnd_ctx);*/
	} while (bi_iszero(k));
	/*--------------------------------*/
	bi_powmod_sec(ct->alpha, pk->g, k, pk->n);
	bi_powmod_sec(ct->beta, pk->y, k, pk->n);
	bi_mulmod(ct->beta, ct->beta, msg, pk->n);
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_encrypt(elgamal_thr_ciphertext_t* ct,
						 const bigint_t msg,
						 const elgamal_thr_pbkey_t* pk,
						 const sphere_t* sph,
						 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(k);
	elgamal_thr_encrypt_k(ct, k, msg, pk, sph, rnd_ctx);
	END_VAR_1_bigint_t(k);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
bool_t elgamal_thr_decrypt_share(elgamal_thr_decrypt_share_t* dsj,
							   const elgamal_thr_ciphertext_t* ct,
							   const elgamal_thr_pbkey_t* pk,
							   const elgamal_thr_pbkey_share_t* pkj,
							   const elgamal_thr_prkey_t* skj,
							   const sphere_t* sph,
							   const void* dat_digest,
							   unsigned dat_digestlen,
							   rnd_ctx_t* rnd_ctx)
{
	bool_t ok = FALSE;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = &pk->n;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &((const elgamal_thr_decrypt_share_t*)dsj)->alpha_xj;
	dlog_prf[1].n = &pk->n;
	/*--------------------------------*/
	bi_powmod_sec(dsj->alpha_xj, ct->alpha, skj->xj, pk->n);
	/*--------------------------------*/
	ok = dlog_proof(&dsj->c, &dsj->sx,
			   dat_digest, dat_digestlen,
			   array_sz(dlog_prf),
			   dlog_prf,
			   sph,
			   &skj->xj,
			   &pk->sp,
			   rnd_ctx);
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t elgamal_thr_add_decrypt_share(elgamal_thr_dshare_acc_t* alpha_pi,
									 const elgamal_thr_decrypt_share_t* dsj,
									 const elgamal_thr_ciphertext_t* ct,
									 const elgamal_thr_pbkey_t* pk,
									 const elgamal_thr_pbkey_share_t* pkj,
									 const sphere_t* sph,
									 const void* dat_digest,
									 unsigned dat_digestlen)
{
	bool_t ok = FALSE;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	dlog_prf[0].g = &pk->g;
	dlog_prf[0].y = &pkj->yj;
	dlog_prf[0].n = &pk->n;
	dlog_prf[1].g = &ct->alpha;
	dlog_prf[1].y = &dsj->alpha_xj;
	dlog_prf[1].n = &pk->n;
	/*--------------------------------*/
	if ( dlog_vrfy(&dsj->c, &dsj->sx,
				   dat_digest, dat_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   sph,
				   &pk->sp)) {
		if (alpha_pi->nshares == 0) {
			bi_asg_ui(alpha_pi->a, 1);
		}
		ok = TRUE;
		bi_mulmod(alpha_pi->a, alpha_pi->a, dsj->alpha_xj, pk->n);
		++alpha_pi->nshares;
	}
	/*--------------------------------*/
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t elgamal_thr_decrypt(bigint_t msg,
						   const elgamal_thr_ciphertext_t* ct,
						   const elgamal_thr_pbkey_t* pk,
						   const elgamal_thr_dshare_acc_t* alpha_pi)
{
	BEG_BIGINT_CHK();
	bool_t ok;
	/*--------------------------------*/
	if (alpha_pi->nshares != pk->nkeys) {
		ok = FALSE;
		bi_asg_ui(msg, 0);
	} else {
		ok = TRUE;
		bi_invmod(msg, alpha_pi->a, pk->n);
		bi_mulmod(msg, msg, ct->beta, pk->n);
	}
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_modfact_t_chk_members(struct elgamal_thr_modfact_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_modfact_t {
		bigint_t p;
		bigint_t q;
	};
	CHK_FIELD__(dummy_elgamal_thr_modfact_t, elgamal_thr_modfact_t, p);
	CHK_FIELD__(dummy_elgamal_thr_modfact_t, elgamal_thr_modfact_t, q);
	CHK_SIZE__(dummy_elgamal_thr_modfact_t, elgamal_thr_modfact_t);
	return (p!=NULL)&&(code == 178844723);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_modfact_t_ctor(struct elgamal_thr_modfact_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_modfact_t_chk_members(p,178844723));
	bi_ctor(p->p);
	bi_ctor(p->q);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modfact_t_dtor(struct elgamal_thr_modfact_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_modfact_t_chk_members(p,178844723));
	bi_dtor(p->q);
	bi_dtor(p->p);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modfact_t_asg(struct elgamal_thr_modfact_t* p, const struct elgamal_thr_modfact_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_modfact_t_chk_members(p,178844723));
	if (p != o) {
		bi_asg(p->p, o->p);
		bi_asg(p->q, o->q);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_modfact_t* elgamal_thr_modfact_t_new()/*auto*/
{
	struct elgamal_thr_modfact_t* p = (struct elgamal_thr_modfact_t*)malloc(sizeof(struct elgamal_thr_modfact_t));
	if (p != NULL) {
		elgamal_thr_modfact_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_modfact_t* elgamal_thr_modfact_t_clone(const struct elgamal_thr_modfact_t* o)/*auto*/
{
	struct elgamal_thr_modfact_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_modfact_t*)malloc(sizeof(struct elgamal_thr_modfact_t));
		if (p != NULL) {
			elgamal_thr_modfact_t_ctor(p);
			elgamal_thr_modfact_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modfact_t_delete(struct elgamal_thr_modfact_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_modfact_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_modord_t_chk_members(struct elgamal_thr_modord_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_modord_t {
		bigint_t p1;
		bigint_t q1;
	};
	CHK_FIELD__(dummy_elgamal_thr_modord_t, elgamal_thr_modord_t, p1);
	CHK_FIELD__(dummy_elgamal_thr_modord_t, elgamal_thr_modord_t, q1);
	CHK_SIZE__(dummy_elgamal_thr_modord_t, elgamal_thr_modord_t);
	return (p!=NULL)&&(code == 407299759);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_modord_t_ctor(struct elgamal_thr_modord_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_modord_t_chk_members(p,407299759));
	bi_ctor(p->p1);
	bi_ctor(p->q1);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modord_t_dtor(struct elgamal_thr_modord_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_modord_t_chk_members(p,407299759));
	bi_dtor(p->q1);
	bi_dtor(p->p1);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modord_t_asg(struct elgamal_thr_modord_t* p, const struct elgamal_thr_modord_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_modord_t_chk_members(p,407299759));
	if (p != o) {
		bi_asg(p->p1, o->p1);
		bi_asg(p->q1, o->q1);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_modord_t* elgamal_thr_modord_t_new()/*auto*/
{
	struct elgamal_thr_modord_t* p = (struct elgamal_thr_modord_t*)malloc(sizeof(struct elgamal_thr_modord_t));
	if (p != NULL) {
		elgamal_thr_modord_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_modord_t* elgamal_thr_modord_t_clone(const struct elgamal_thr_modord_t* o)/*auto*/
{
	struct elgamal_thr_modord_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_modord_t*)malloc(sizeof(struct elgamal_thr_modord_t));
		if (p != NULL) {
			elgamal_thr_modord_t_ctor(p);
			elgamal_thr_modord_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_modord_t_delete(struct elgamal_thr_modord_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_modord_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_pbkey_t_chk_members(struct elgamal_thr_pbkey_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_pbkey_t {
		syspar_t sp;
		bigint_t n;
		bigint_t g;
		bigint_t y;			/* y = PROD_j(yj) */
		unsigned nkeys;
	};
	CHK_FIELD__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t, sp);
	CHK_FIELD__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t, n);
	CHK_FIELD__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t, g);
	CHK_FIELD__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t, y);
	CHK_FIELD__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t, nkeys);
	CHK_SIZE__(dummy_elgamal_thr_pbkey_t, elgamal_thr_pbkey_t);
	return (p!=NULL)&&(code == 320364432);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_t_ctor(struct elgamal_thr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_pbkey_t_chk_members(p,320364432));
	syspar_t_ctor(&p->sp);
	bi_ctor(p->n);
	bi_ctor(p->g);
	bi_ctor(p->y);
	p->nkeys = 0;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_t_dtor(struct elgamal_thr_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_pbkey_t_chk_members(p,320364432));
	bi_dtor(p->y);
	bi_dtor(p->g);
	bi_dtor(p->n);
	syspar_t_dtor(&p->sp);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_t_asg(struct elgamal_thr_pbkey_t* p, const struct elgamal_thr_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_pbkey_t_chk_members(p,320364432));
	if (p != o) {
		syspar_t_asg(&p->sp, &o->sp);
		bi_asg(p->n, o->n);
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
		p->nkeys = o->nkeys;
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_pbkey_t* elgamal_thr_pbkey_t_new()/*auto*/
{
	struct elgamal_thr_pbkey_t* p = (struct elgamal_thr_pbkey_t*)malloc(sizeof(struct elgamal_thr_pbkey_t));
	if (p != NULL) {
		elgamal_thr_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_pbkey_t* elgamal_thr_pbkey_t_clone(const struct elgamal_thr_pbkey_t* o)/*auto*/
{
	struct elgamal_thr_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_pbkey_t*)malloc(sizeof(struct elgamal_thr_pbkey_t));
		if (p != NULL) {
			elgamal_thr_pbkey_t_ctor(p);
			elgamal_thr_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_t_delete(struct elgamal_thr_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_pbkey_share_t_chk_members(struct elgamal_thr_pbkey_share_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_pbkey_share_t {
		bigint_t yj;		/* yj = g^xj (mod n) */
	};
	CHK_FIELD__(dummy_elgamal_thr_pbkey_share_t, elgamal_thr_pbkey_share_t, yj);
	CHK_SIZE__(dummy_elgamal_thr_pbkey_share_t, elgamal_thr_pbkey_share_t);
	return (p!=NULL)&&(code == 29563889);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_share_t_ctor(struct elgamal_thr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_pbkey_share_t_chk_members(p,29563889));
	bi_ctor(p->yj);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_share_t_dtor(struct elgamal_thr_pbkey_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_pbkey_share_t_chk_members(p,29563889));
	bi_dtor(p->yj);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_share_t_asg(struct elgamal_thr_pbkey_share_t* p, const struct elgamal_thr_pbkey_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_pbkey_share_t_chk_members(p,29563889));
	if (p != o) {
		bi_asg(p->yj, o->yj);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_pbkey_share_t* elgamal_thr_pbkey_share_t_new()/*auto*/
{
	struct elgamal_thr_pbkey_share_t* p = (struct elgamal_thr_pbkey_share_t*)malloc(sizeof(struct elgamal_thr_pbkey_share_t));
	if (p != NULL) {
		elgamal_thr_pbkey_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_pbkey_share_t* elgamal_thr_pbkey_share_t_clone(const struct elgamal_thr_pbkey_share_t* o)/*auto*/
{
	struct elgamal_thr_pbkey_share_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_pbkey_share_t*)malloc(sizeof(struct elgamal_thr_pbkey_share_t));
		if (p != NULL) {
			elgamal_thr_pbkey_share_t_ctor(p);
			elgamal_thr_pbkey_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_share_t_delete(struct elgamal_thr_pbkey_share_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_pbkey_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_prkey_t_chk_members(struct elgamal_thr_prkey_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_prkey_t {
		bigint_t xj;
	};
	CHK_FIELD__(dummy_elgamal_thr_prkey_t, elgamal_thr_prkey_t, xj);
	CHK_SIZE__(dummy_elgamal_thr_prkey_t, elgamal_thr_prkey_t);
	return (p!=NULL)&&(code == 198508566);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_prkey_t_ctor(struct elgamal_thr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_prkey_t_chk_members(p,198508566));
	bi_ctor(p->xj);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_prkey_t_dtor(struct elgamal_thr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_prkey_t_chk_members(p,198508566));
	bi_dtor(p->xj);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_prkey_t_asg(struct elgamal_thr_prkey_t* p, const struct elgamal_thr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_prkey_t_chk_members(p,198508566));
	if (p != o) {
		bi_asg(p->xj, o->xj);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_prkey_t* elgamal_thr_prkey_t_new()/*auto*/
{
	struct elgamal_thr_prkey_t* p = (struct elgamal_thr_prkey_t*)malloc(sizeof(struct elgamal_thr_prkey_t));
	if (p != NULL) {
		elgamal_thr_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_prkey_t* elgamal_thr_prkey_t_clone(const struct elgamal_thr_prkey_t* o)/*auto*/
{
	struct elgamal_thr_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_prkey_t*)malloc(sizeof(struct elgamal_thr_prkey_t));
		if (p != NULL) {
			elgamal_thr_prkey_t_ctor(p);
			elgamal_thr_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_prkey_t_delete(struct elgamal_thr_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_ciphertext_t_chk_members(struct elgamal_thr_ciphertext_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_ciphertext_t {
		bigint_t alpha;
		bigint_t beta;
	};
	CHK_FIELD__(dummy_elgamal_thr_ciphertext_t, elgamal_thr_ciphertext_t, alpha);
	CHK_FIELD__(dummy_elgamal_thr_ciphertext_t, elgamal_thr_ciphertext_t, beta);
	CHK_SIZE__(dummy_elgamal_thr_ciphertext_t, elgamal_thr_ciphertext_t);
	return (p!=NULL)&&(code == 211691810);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_ciphertext_t_ctor(struct elgamal_thr_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_ciphertext_t_chk_members(p,211691810));
	bi_ctor(p->alpha);
	bi_ctor(p->beta);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_ciphertext_t_dtor(struct elgamal_thr_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_ciphertext_t_chk_members(p,211691810));
	bi_dtor(p->beta);
	bi_dtor(p->alpha);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_ciphertext_t_asg(struct elgamal_thr_ciphertext_t* p, const struct elgamal_thr_ciphertext_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_ciphertext_t_chk_members(p,211691810));
	if (p != o) {
		bi_asg(p->alpha, o->alpha);
		bi_asg(p->beta, o->beta);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_ciphertext_t* elgamal_thr_ciphertext_t_new()/*auto*/
{
	struct elgamal_thr_ciphertext_t* p = (struct elgamal_thr_ciphertext_t*)malloc(sizeof(struct elgamal_thr_ciphertext_t));
	if (p != NULL) {
		elgamal_thr_ciphertext_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_ciphertext_t* elgamal_thr_ciphertext_t_clone(const struct elgamal_thr_ciphertext_t* o)/*auto*/
{
	struct elgamal_thr_ciphertext_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_ciphertext_t*)malloc(sizeof(struct elgamal_thr_ciphertext_t));
		if (p != NULL) {
			elgamal_thr_ciphertext_t_ctor(p);
			elgamal_thr_ciphertext_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_ciphertext_t_delete(struct elgamal_thr_ciphertext_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_ciphertext_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_decrypt_share_t_chk_members(struct elgamal_thr_decrypt_share_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_decrypt_share_t {
		bigint_t alpha_xj;
		bigint_t c;
		bigint_t sx;
	};
	CHK_FIELD__(dummy_elgamal_thr_decrypt_share_t, elgamal_thr_decrypt_share_t, alpha_xj);
	CHK_FIELD__(dummy_elgamal_thr_decrypt_share_t, elgamal_thr_decrypt_share_t, c);
	CHK_FIELD__(dummy_elgamal_thr_decrypt_share_t, elgamal_thr_decrypt_share_t, sx);
	CHK_SIZE__(dummy_elgamal_thr_decrypt_share_t, elgamal_thr_decrypt_share_t);
	return (p!=NULL)&&(code == 524542999);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_decrypt_share_t_ctor(struct elgamal_thr_decrypt_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_decrypt_share_t_chk_members(p,524542999));
	bi_ctor(p->alpha_xj);
	bi_ctor(p->c);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_decrypt_share_t_dtor(struct elgamal_thr_decrypt_share_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_decrypt_share_t_chk_members(p,524542999));
	bi_dtor(p->sx);
	bi_dtor(p->c);
	bi_dtor(p->alpha_xj);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_decrypt_share_t_asg(struct elgamal_thr_decrypt_share_t* p, const struct elgamal_thr_decrypt_share_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_decrypt_share_t_chk_members(p,524542999));
	if (p != o) {
		bi_asg(p->alpha_xj, o->alpha_xj);
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_decrypt_share_t* elgamal_thr_decrypt_share_t_new()/*auto*/
{
	struct elgamal_thr_decrypt_share_t* p = (struct elgamal_thr_decrypt_share_t*)malloc(sizeof(struct elgamal_thr_decrypt_share_t));
	if (p != NULL) {
		elgamal_thr_decrypt_share_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_decrypt_share_t* elgamal_thr_decrypt_share_t_clone(const struct elgamal_thr_decrypt_share_t* o)/*auto*/
{
	struct elgamal_thr_decrypt_share_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_decrypt_share_t*)malloc(sizeof(struct elgamal_thr_decrypt_share_t));
		if (p != NULL) {
			elgamal_thr_decrypt_share_t_ctor(p);
			elgamal_thr_decrypt_share_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_decrypt_share_t_delete(struct elgamal_thr_decrypt_share_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_decrypt_share_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_thr_dshare_acc_t_chk_members(struct elgamal_thr_dshare_acc_t* p, int code)/*auto*/
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
	struct dummy_elgamal_thr_dshare_acc_t {
		unsigned nshares;
		bigint_t a;
	};
	CHK_FIELD__(dummy_elgamal_thr_dshare_acc_t, elgamal_thr_dshare_acc_t, nshares);
	CHK_FIELD__(dummy_elgamal_thr_dshare_acc_t, elgamal_thr_dshare_acc_t, a);
	CHK_SIZE__(dummy_elgamal_thr_dshare_acc_t, elgamal_thr_dshare_acc_t);
	return (p!=NULL)&&(code == 266953664);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_thr_dshare_acc_t_ctor(struct elgamal_thr_dshare_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_dshare_acc_t_chk_members(p,266953664));
	p->nshares = 0;
	bi_ctor(p->a);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_dshare_acc_t_dtor(struct elgamal_thr_dshare_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_thr_dshare_acc_t_chk_members(p,266953664));
	bi_dtor(p->a);
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_dshare_acc_t_asg(struct elgamal_thr_dshare_acc_t* p, const struct elgamal_thr_dshare_acc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_thr_dshare_acc_t_chk_members(p,266953664));
	if (p != o) {
		p->nshares = o->nshares;
		bi_asg(p->a, o->a);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_dshare_acc_t* elgamal_thr_dshare_acc_t_new()/*auto*/
{
	struct elgamal_thr_dshare_acc_t* p = (struct elgamal_thr_dshare_acc_t*)malloc(sizeof(struct elgamal_thr_dshare_acc_t));
	if (p != NULL) {
		elgamal_thr_dshare_acc_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_thr_dshare_acc_t* elgamal_thr_dshare_acc_t_clone(const struct elgamal_thr_dshare_acc_t* o)/*auto*/
{
	struct elgamal_thr_dshare_acc_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_thr_dshare_acc_t*)malloc(sizeof(struct elgamal_thr_dshare_acc_t));
		if (p != NULL) {
			elgamal_thr_dshare_acc_t_ctor(p);
			elgamal_thr_dshare_acc_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_thr_dshare_acc_t_delete(struct elgamal_thr_dshare_acc_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_thr_dshare_acc_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

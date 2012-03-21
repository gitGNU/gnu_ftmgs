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
#include "sign.h"
#include <assert.h>
#include <stdlib.h>
#include "iexport.h"
#include "sphere.h"
#include "syssph.h"
#include "sok.h"
#include "elgamal_thr.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
#define array_sz(array__) (sizeof(array__)/sizeof(*(array__)))
/*----------------------------------------------------------------------------*/
/*
 *--------------------------------
 * PK{(x, x', e, r, h'): T2=g^r ; T3=g^e h^r ; T2^e=g^h' ; T5^x=T4 ;
 *						 T7^x'=T6 ; a0 a^x b^x' y^h'=T1^e }(msg)
 *--------------------------------
 * Vbles	{ x,  x',  e,  r,  h' }
 * Spheres	 Mek Lek  Gek Mek MGek
 *----------------------------------------------------
 * Objs		{ g,  h, T2^-1, T5, T7,  y, T1^-1, a, b, a0, T3, T4, T6 } | MOD
 *----------------------------------------------------
 * Rels		{ r,  0,  1,     0,  0,  0,  0,    0, 0,  0,  0,  0,  0 } | n
 *			{ e,  r,  0,     0,  0,  0,  0,    0, 0,  0, -1,  0,  0 } | n
 *			{ h', 0,  e,     0,  0,  0,  0,    0, 0,  0,  0,  0,  0 } | n
 *			{ 0,  0,  0,     x,  0,  0,  0,    0, 0,  0,  0, -1,  0 } | n
 *			{ 0,  0,  0,     0,  x', 0,  0,    0, 0,  0,  0,  0, -1 } | n
 *			{ 0,  0,  0,     0,  0,  h', e,    x, x', 1,  0,  0,  0 } | n
 *----------------------------------------------------
 */
#define X	set_vble_idx(0)
#define X1	set_vble_idx(1)
#define E	set_vble_idx(2)
#define R	set_vble_idx(3)
#define H1	set_vble_idx(4)
#define SIGN_SK_NVBLES		5
#define SIGN_SK_NOBJS		13
#define SIGN_SK_NRELS		6
static const sok_relset_t SIGN_SK = {
	SIGN_SK_NVBLES,		/* NVbles */
	SIGN_SK_NOBJS,		/* NObjs */
	SIGN_SK_NRELS,		/* NRels */
	{
		{  R,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  E,  R,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0 },
		{ H1,  0,  E,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{  0,  0,  0,  X,  0,  0,  0,  0,  0,  0,  0, -1,  0 },
		{  0,  0,  0,  0, X1,  0,  0,  0,  0,  0,  0,  0, -1 },
		{  0,  0,  0,  0,  0, H1,  E,  X, X1,  1,  0,  0,  0 } 
	}
};
#undef X
#undef X1
#undef E
#undef R
#undef H1
/*----------------------------------------------------------------------------*/
bool_t ftmgs_sign_dgst(ftmgs_sign_t* sg,
					 const void* dat_digest,
					 unsigned dat_digestlen,
					 const ftmgs_pbkey_t* gpk,
					 const ftmgs_mbr_prkey_t* msk,
					 rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_VAR_1(sok_params_t, sokp);
	BEG_VAR_6_bigint_t(r, k, k1, h1, t1inv, t2inv);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x_sph = &gpk->syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* e_sph = &gpk->syssph.gamma;
	const sphere_t* r_sph = &gpk->syssph.mu;
	const sphere_t* h1_sph = &gpk->syssph.gamma_mu;
	x1_sph = get_sphere_lambda_x(msk->x1i, &gpk->syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x_sph = &syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* e_sph = &syssph.gamma;
	const sphere_t* r_sph = &syssph.mu;
	const sphere_t* h1_sph = &syssph.gamma_mu;
	create_syssph(&syssph, &gpk->gmpk.sp);
	x1_sph = get_sphere_lambda_x(msk->x1i, &syssph);
#endif
	/*--------------------------------*/
	bi_random_sph(k, x_sph, rnd_ctx);
	bi_random_sph(k1, x_sph, rnd_ctx);
	/*bi_random_sph_ek(r, x_sph, rnd_ctx);*/
	/*bi_powmod(sg->T1, gpk->gmpk.y, r, gpk->gmpk.n);*/
	/*bi_mulmod(sg->T1, sg->T1, msk->Ai, gpk->gmpk.n);*/
	/*bi_powmod(sg->T2, gpk->gmpk.g, r, gpk->gmpk.n);*/
	elgamal_thr_encrypt_k(&sg->T1_T2, r, msk->Ai, &gpk->gmpk, x_sph, rnd_ctx);
	bi_mul_powmod(sg->T3, gpk->gmpk.g, msk->ei, gpk->h, r, gpk->gmpk.n);
	bi_powmod(sg->T5, gpk->gmpk.g, k, gpk->gmpk.n);
	bi_powmod(sg->T4, sg->T5, msk->xi, gpk->gmpk.n);
	bi_powmod(sg->T7, gpk->gmpk.g, k1, gpk->gmpk.n);
	bi_powmod(sg->T6, sg->T7, msk->x1i, gpk->gmpk.n);
	/*--------------------------------*/
	bi_mul(h1, r, msk->ei);
	bi_invmod(t1inv, get_T1(sg), gpk->gmpk.n);
	bi_invmod(t2inv, get_T2(sg), gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, &gpk->gmpk.g);
	sok_params_add_obj(&sokp, &gpk->h);
	sok_params_add_obj(&sokp, (const bigint_t*)&t2inv);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T5);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T7);
	sok_params_add_obj(&sokp, &gpk->gmpk.y);
	sok_params_add_obj(&sokp, (const bigint_t*)&t1inv);
	sok_params_add_obj(&sokp, &gpk->a);
	sok_params_add_obj(&sokp, &gpk->b);
	sok_params_add_obj(&sokp, &gpk->a0);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T3);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T4);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T6);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &sg->c);
	/*--------------------------------*/
	sok_params_add_vble(&sokp, &sg->sx, x_sph, &msk->xi);
	sok_params_add_vble(&sokp, &sg->sx1, x1_sph, &msk->x1i);
	sok_params_add_vble(&sokp, &sg->se, e_sph, &msk->ei);
	sok_params_add_vble(&sokp, &sg->sr, r_sph, (const bigint_t*)&r);
	sok_params_add_vble(&sokp, &sg->sh1, h1_sph, (const bigint_t*)&h1);
	/*--------------------------------*/
	ok = sok(&SIGN_SK, &sokp, dat_digest, dat_digestlen, &gpk->gmpk.sp,
			 rnd_ctx);
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_6_bigint_t(r, k, k1, h1, t1inv, t2inv);
	END_VAR_1(sok_params_t, sokp);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_sign_dat(unsigned which_sha,
					ftmgs_sign_t* sg,
					const void* dat, unsigned datlen,
					const ftmgs_pbkey_t* gpk,
					const ftmgs_mbr_prkey_t* msk,
					rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_sign_dgst(sg, dat_digest, dat_digestlen, gpk, msk, rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
UNUSED__
static void sign_precomp_sk(sok_precomp_t* precomp,
							const sign_precomp_t* sign_precomp)
{
	sok_precomp_init(precomp, &SIGN_SK);
	sok_set_precomp(precomp, &SIGN_SK, 0, 0, &sign_precomp->sign_g_r_mu);
	sok_set_precomp(precomp, &SIGN_SK, 1, 0, &sign_precomp->sign_g_e_gamma);
	sok_set_precomp(precomp, &SIGN_SK, 2, 0, &sign_precomp->sign_g_h1_gamma_mu);
	sok_set_precomp(precomp, &SIGN_SK, 1, 1, &sign_precomp->sign_h_r_mu);
	sok_set_precomp(precomp, &SIGN_SK, 5, 5, &sign_precomp->sign_y_h1_gamma_mu);
	sok_set_precomp(precomp, &SIGN_SK, 5, 7, &sign_precomp->sign_a_x_mu);
/*sok_set_precomp(precomp, &SIGN_SK, 5, 8, &sign_precomp->sign_b_x1_lambda);*/
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_dgst(const ftmgs_sign_t* sg,
					   const void* dat_digest,
					   unsigned dat_digestlen,
					   const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	BEG_VAR_1(sokv_params_t, sokp);
	BEG_VAR_2_bigint_t(t1inv, t2inv);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(sok_precomp_t, precomp);
	const sphere_t* x_sph = &gpk->syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* e_sph = &gpk->syssph.gamma;
	const sphere_t* r_sph = &gpk->syssph.mu;
	const sphere_t* h1_sph = &gpk->syssph.gamma_mu;
	sign_precomp_sk(&precomp, &gpk->sign_precomp);
	x1_sph = get_sphere_lambda_sx(sg->sx1, &gpk->syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x_sph = &syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* e_sph = &syssph.gamma;
	const sphere_t* r_sph = &syssph.mu;
	const sphere_t* h1_sph = &syssph.gamma_mu;
	create_syssph(&syssph, &gpk->gmpk.sp);
	x1_sph = get_sphere_lambda_sx(sg->sx1, &syssph);
#endif
	/*--------------------------------*/
	bi_invmod(t1inv, get_T1(sg), gpk->gmpk.n);
	bi_invmod(t2inv, get_T2(sg), gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, &gpk->gmpk.g);
	sok_params_add_obj(&sokp, &gpk->h);
	sok_params_add_obj(&sokp, (const bigint_t*)&t2inv);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T5);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T7);
	sok_params_add_obj(&sokp, &gpk->gmpk.y);
	sok_params_add_obj(&sokp, (const bigint_t*)&t1inv);
	sok_params_add_obj(&sokp, &gpk->a);
	sok_params_add_obj(&sokp, &gpk->b);
	sok_params_add_obj(&sokp, &gpk->a0);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T3);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T4);
	sok_params_add_obj(&sokp, &((const ftmgs_sign_t*)sg)->T6);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &sg->c);
	/*--------------------------------*/
	sok_params_add_sph(&sokp, &sg->sx, x_sph);
	sok_params_add_sph(&sokp, &sg->sx1, x1_sph);
	sok_params_add_sph(&sokp, &sg->se, e_sph);
	sok_params_add_sph(&sokp, &sg->sr, r_sph);
	sok_params_add_sph(&sokp, &sg->sh1, h1_sph);
	/*--------------------------------*/
	ok = sokv(&SIGN_SK, &sokp, dat_digest, dat_digestlen, &gpk->gmpk.sp
#ifdef PRECOMPUTATIONS__
			  , &precomp
#endif
			  );
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	END_VAR_1(sok_precomp_t, precomp);
#else
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_2_bigint_t(t1inv, t2inv);
	END_VAR_1(sokv_params_t, sokp);
	/*--------------------------------*/
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_dat(unsigned which_sha,
					  const ftmgs_sign_t* sg,
					  const void* dat, unsigned datlen,
					  const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_vrfy_dgst(sg, dat_digest, dat_digestlen, gpk);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
void extract_uauth_from_sg(dlog_t* uauth,
						   const ftmgs_sign_t* sg,
						   const ftmgs_pbkey_t* gpk)
{
	bi_asg(uauth->g, sg->T7);
	bi_asg(uauth->y, sg->T6);
	bi_asg(uauth->n, gpk->gmpk.n);
}
/*----------------------------------------------------------------------------*/
void ftmgs_hash_add_sign(USHAContext* sha_ctx, const ftmgs_sign_t* sg)
{
	bi_sha_input(sha_ctx, get_T1(sg));
	bi_sha_input(sha_ctx, get_T2(sg));
	bi_sha_input(sha_ctx, sg->T3);
	bi_sha_input(sha_ctx, sg->T4);
	bi_sha_input(sha_ctx, sg->T5);
	bi_sha_input(sha_ctx, sg->T6);
	bi_sha_input(sha_ctx, sg->T7);
	bi_sha_input(sha_ctx, sg->c);
	bi_sha_input(sha_ctx, sg->sx);
	bi_sha_input(sha_ctx, sg->sx1);
	bi_sha_input(sha_ctx, sg->se);
	bi_sha_input(sha_ctx, sg->sr);
	bi_sha_input(sha_ctx, sg->sh1);
}
/*----------------------------------------------------------------------------*/
void ftmgs_hash_sign(unsigned which_sha,
					 void* sg_digest,
					 unsigned* sg_digestlen,
					 const ftmgs_sign_t* sg)
{
	USHAContext sha_ctx;
	bi_sha_reset(&sha_ctx, which_sha);
	ftmgs_hash_add_sign(&sha_ctx, sg);
	sha_result(&sha_ctx, sg_digest, sg_digestlen);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_sign_t_chk_members(struct ftmgs_sign_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_sign_t {
		elgamal_thr_ciphertext_t T1_T2; /* T1: beta ; T2: alpha */
		bigint_t T3;
		bigint_t T4;
		bigint_t T5;
		bigint_t T6;
		bigint_t T7;
		bigint_t c;
		bigint_t sx;
		bigint_t sx1;
		bigint_t se;
		bigint_t sr;
		bigint_t sh1;
	};
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T1_T2);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T3);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T4);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T5);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T6);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, T7);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, c);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, sx);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, sx1);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, se);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, sr);
	CHK_FIELD__(dummy_ftmgs_sign_t, ftmgs_sign_t, sh1);
	CHK_SIZE__(dummy_ftmgs_sign_t, ftmgs_sign_t);
	return (p!=NULL)&&(code == 373412698);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_sign_t_ctor(struct ftmgs_sign_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_sign_t_chk_members(p,373412698));
	elgamal_thr_ciphertext_t_ctor(&p->T1_T2);
	bi_ctor(p->T3);
	bi_ctor(p->T4);
	bi_ctor(p->T5);
	bi_ctor(p->T6);
	bi_ctor(p->T7);
	bi_ctor(p->c);
	bi_ctor(p->sx);
	bi_ctor(p->sx1);
	bi_ctor(p->se);
	bi_ctor(p->sr);
	bi_ctor(p->sh1);
}
/*----------------------------------------------------------------------------*/
void ftmgs_sign_t_dtor(struct ftmgs_sign_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_sign_t_chk_members(p,373412698));
	bi_dtor(p->sh1);
	bi_dtor(p->sr);
	bi_dtor(p->se);
	bi_dtor(p->sx1);
	bi_dtor(p->sx);
	bi_dtor(p->c);
	bi_dtor(p->T7);
	bi_dtor(p->T6);
	bi_dtor(p->T5);
	bi_dtor(p->T4);
	bi_dtor(p->T3);
	elgamal_thr_ciphertext_t_dtor(&p->T1_T2);
}
/*----------------------------------------------------------------------------*/
void ftmgs_sign_t_asg(struct ftmgs_sign_t* p, const struct ftmgs_sign_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_sign_t_chk_members(p,373412698));
	if (p != o) {
		elgamal_thr_ciphertext_t_asg(&p->T1_T2, &o->T1_T2);
		bi_asg(p->T3, o->T3);
		bi_asg(p->T4, o->T4);
		bi_asg(p->T5, o->T5);
		bi_asg(p->T6, o->T6);
		bi_asg(p->T7, o->T7);
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
		bi_asg(p->sx1, o->sx1);
		bi_asg(p->se, o->se);
		bi_asg(p->sr, o->sr);
		bi_asg(p->sh1, o->sh1);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_sign_t* ftmgs_sign_t_new()/*auto*/
{
	struct ftmgs_sign_t* p = (struct ftmgs_sign_t*)malloc(sizeof(struct ftmgs_sign_t));
	if (p != NULL) {
		ftmgs_sign_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_sign_t* ftmgs_sign_t_clone(const struct ftmgs_sign_t* o)/*auto*/
{
	struct ftmgs_sign_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_sign_t*)malloc(sizeof(struct ftmgs_sign_t));
		if (p != NULL) {
			ftmgs_sign_t_ctor(p);
			ftmgs_sign_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_sign_t_delete(struct ftmgs_sign_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_sign_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

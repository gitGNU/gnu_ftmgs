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
#include "link.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sok.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
static void hash_link(unsigned which_sha,
					  void* lnk_digest,
					  unsigned* lnk_digestlen,
					  const void* dat_digest,
					  unsigned dat_digestlen,
					  const ftmgs_sign_t* sg0,
					  const ftmgs_sign_t* sg1)
{
	USHAContext sha_ctx;
	bi_sha_reset(&sha_ctx, which_sha);
	sha_input(&sha_ctx, dat_digest, dat_digestlen);
	ftmgs_hash_add_sign(&sha_ctx, sg0);
	ftmgs_hash_add_sign(&sha_ctx, sg1);
	sha_result(&sha_ctx, lnk_digest, lnk_digestlen);
}
/*----------------------------------------------------------------------------*/
static void hash_link_m(unsigned which_sha,
						void* lnk_digest,
						unsigned* lnk_digestlen,
						const void* dat_digest,
						unsigned dat_digestlen,
						unsigned nsg,
						const ftmgs_sign_t* sg[])
{
	unsigned i;
	USHAContext sha_ctx;
	bi_sha_reset(&sha_ctx, which_sha);
	sha_input(&sha_ctx, dat_digest, dat_digestlen);
	for (i = 0; i < nsg; ++i) {
		ftmgs_hash_add_sign(&sha_ctx, sg[i]);
	}
	sha_result(&sha_ctx, lnk_digest, lnk_digestlen);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
bool_t ftmgs_link_dgst(ftmgs_link_t* lnk,
					   const void* dat_digest,
					   unsigned dat_digestlen,
					   const ftmgs_sign_t* sg0,
					   const ftmgs_pbkey_t* gpk0,
					   const ftmgs_sign_t* sg1,
					   const ftmgs_pbkey_t* gpk1,
					   const ftmgs_mbr_prkey_t* msk,
					   rnd_ctx_t* rnd_ctx)
{
	bool_t ok = TRUE;
	unsigned i;
	const syspar_t* gmpk_sp;
	unsigned lnk_digestlen = USHAMaxHashSize;
	char lnk_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(gx);
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x1_sph;
	if (gpk0->gmpk.sp.k >= gpk1->gmpk.sp.k) {
		gmpk_sp = &gpk0->gmpk.sp;
		x1_sph = get_sphere_lambda_x(msk->x1i, &gpk0->syssph);
	} else {
		gmpk_sp = &gpk1->gmpk.sp;
		x1_sph = get_sphere_lambda_x(msk->x1i, &gpk1->syssph);
	}
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	if (gpk0->gmpk.sp.k >= gpk1->gmpk.sp.k) {
		gmpk_sp = &gpk0->gmpk.sp;
	} else {
		gmpk_sp = &gpk1->gmpk.sp;
	}
	create_syssph(&syssph, gmpk_sp);
	x1_sph = get_sphere_lambda_x(msk->x1i, &syssph);
#endif
	/*--------------------------------*/
	dlog_prf[0].g = &sg0->T7;
	dlog_prf[0].y = &sg0->T6;
	dlog_prf[0].n = &gpk0->gmpk.n;
	dlog_prf[1].g = &sg1->T7;
	dlog_prf[1].y = &sg1->T6;
	dlog_prf[1].n = &gpk1->gmpk.n;
	/*--------------------------------*/
	for (i = 0; ok && (i < array_sz(dlog_prf)); ++i) {
		bi_powmod_sec(gx, *dlog_prf[i].g, msk->x1i, *dlog_prf[i].n);
		ok = bi_equals(gx, *dlog_prf[i].y);
	}
	/*--------------------------------*/
	if (ok) {
		hash_link(gmpk_sp->k,
				  lnk_digest, &lnk_digestlen,
				  dat_digest, dat_digestlen,
				  sg0, sg1);
		ok = dlog_proof(&lnk->c, &lnk->sx,
				   lnk_digest, lnk_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   x1_sph,
				   &msk->x1i,
				   gmpk_sp,
				   rnd_ctx);
	}
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_VAR_1_bigint_t(gx);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_link_dat(unsigned which_sha,
					  ftmgs_link_t* lnk,
					  const void* dat, unsigned datlen,
					  const ftmgs_sign_t* sg0,
					  const ftmgs_pbkey_t* gpk0,
					  const ftmgs_sign_t* sg1,
					  const ftmgs_pbkey_t* gpk1,
					  const ftmgs_mbr_prkey_t* msk,
					  rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_link_dgst(lnk, dat_digest, dat_digestlen,
						 sg0, gpk0, sg1, gpk1, msk, rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_link_dgst(const ftmgs_link_t* lnk,
							const void* dat_digest,
							unsigned dat_digestlen,
							const ftmgs_sign_t* sg0,
							const ftmgs_pbkey_t* gpk0,
							const ftmgs_sign_t* sg1,
							const ftmgs_pbkey_t* gpk1)
{
	bool_t ok;
	const syspar_t* gmpk_sp;
	unsigned lnk_digestlen = USHAMaxHashSize;
	char lnk_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 2);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x1_sph;
	if (gpk0->gmpk.sp.k >= gpk1->gmpk.sp.k) {
		gmpk_sp = &gpk0->gmpk.sp;
		x1_sph = get_sphere_lambda_sx(lnk->sx, &gpk0->syssph);
	} else {
		gmpk_sp = &gpk1->gmpk.sp;
		x1_sph = get_sphere_lambda_sx(lnk->sx, &gpk1->syssph);
	}
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	if (gpk0->gmpk.sp.k >= gpk1->gmpk.sp.k) {
		gmpk_sp = &gpk0->gmpk.sp;
	} else {
		gmpk_sp = &gpk1->gmpk.sp;
	}
	create_syssph(&syssph, gmpk_sp);
	x1_sph = get_sphere_lambda_sx(lnk->sx, &syssph);
#endif
	/*--------------------------------*/
	dlog_prf[0].g = &sg0->T7;
	dlog_prf[0].y = &sg0->T6;
	dlog_prf[0].n = &gpk0->gmpk.n;
	dlog_prf[1].g = &sg1->T7;
	dlog_prf[1].y = &sg1->T6;
	dlog_prf[1].n = &gpk1->gmpk.n;
	/*--------------------------------*/
	hash_link(gmpk_sp->k,
			  lnk_digest, &lnk_digestlen,
			  dat_digest, dat_digestlen,
			  sg0, sg1);
	ok = dlog_vrfy(&lnk->c, &lnk->sx,
				   lnk_digest, lnk_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   x1_sph,
				   gmpk_sp);
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_A(dlog_p_t, dlog_prf, 2);
	END_BIGINT_CHK();
	/*--------------------------------*/
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_link_dat(unsigned which_sha,
						   const ftmgs_link_t* lnk,
						   const void* dat, unsigned datlen,
						   const ftmgs_sign_t* sg0,
						   const ftmgs_pbkey_t* gpk0,
						   const ftmgs_sign_t* sg1,
						   const ftmgs_pbkey_t* gpk1)
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_vrfy_link_dgst(lnk, dat_digest, dat_digestlen,
							  sg0, gpk0, sg1, gpk1);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_link_m_dgst(ftmgs_link_t* lnk,
						 const void* dat_digest,
						 unsigned dat_digestlen,
						 unsigned nsg,
						 const ftmgs_sign_t* sg[],
						 const ftmgs_pbkey_t* gpk[],
						 const ftmgs_mbr_prkey_t* msk,
						 rnd_ctx_t* rnd_ctx)
{
	bool_t ok = TRUE;
	unsigned i;
	const syspar_t* gmpk_sp;
	unsigned lnk_digestlen = USHAMaxHashSize;
	char lnk_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(gx);
	BEG_VAR_V(dlog_p_t, dlog_prf, nsg);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const syssph_t* syssph;
	const sphere_t* x1_sph;
	assert(nsg > 0);
	syssph = &gpk[0]->syssph;
	gmpk_sp = &gpk[0]->gmpk.sp;
	for (i = 1; (i < nsg); ++i) {
		if (gpk[i]->gmpk.sp.k > gmpk_sp->k) {
			syssph = &gpk[i]->syssph;
			gmpk_sp = &gpk[i]->gmpk.sp;
		}
	}
	x1_sph = get_sphere_lambda_x(msk->x1i, syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	assert(nsg > 0);
	gmpk_sp = &gpk[0]->gmpk.sp;
	for (i = 1; (i < nsg); ++i) {
		if (gpk[i]->gmpk.sp.k > gmpk_sp->k) {
			gmpk_sp = &gpk[i]->gmpk.sp;
		}
	}
	create_syssph(&syssph, gmpk_sp);
	x1_sph = get_sphere_lambda_x(msk->x1i, &syssph);
#endif
	/*--------------------------------*/
	ok = (dlog_prf != NULL);
	for (i = 0; ok && (i < nsg); ++i) {
		dlog_prf[i].g = &sg[i]->T7;
		dlog_prf[i].y = &sg[i]->T6;
		dlog_prf[i].n = &gpk[i]->gmpk.n;
		/*----------------------------*/
		bi_powmod_sec(gx, *dlog_prf[i].g, msk->x1i, *dlog_prf[i].n);
		ok = bi_equals(gx, *dlog_prf[i].y);
	}
	/*--------------------------------*/
	if (ok) {
		hash_link_m(gmpk_sp->k,
					lnk_digest, &lnk_digestlen,
					dat_digest, dat_digestlen,
					nsg, sg);
		ok = dlog_proof(&lnk->c, &lnk->sx,
				   lnk_digest, lnk_digestlen,
				   nsg,
				   dlog_prf,
				   x1_sph,
				   &msk->x1i,
				   gmpk_sp,
				   rnd_ctx);
	}
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_V(dlog_p_t, dlog_prf, nsg);
	END_VAR_1_bigint_t(gx);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_link_m_dat(unsigned which_sha,
						ftmgs_link_t* lnk,
						const void* dat, unsigned datlen,
						unsigned nsg,
						const ftmgs_sign_t* sg[],
						const ftmgs_pbkey_t* gpk[],
						const ftmgs_mbr_prkey_t* msk,
						rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_link_m_dgst(lnk, dat_digest, dat_digestlen,
						   nsg, sg, gpk, msk, rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_link_m_dgst(const ftmgs_link_t* lnk,
							  const void* dat_digest,
							  unsigned dat_digestlen,
							  unsigned nsg,
							  const ftmgs_sign_t* sg[],
							  const ftmgs_pbkey_t* gpk[])
{
	bool_t ok;
	unsigned i;
	const syspar_t* gmpk_sp;
	unsigned lnk_digestlen = USHAMaxHashSize;
	char lnk_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_V(dlog_p_t, dlog_prf, nsg);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const syssph_t* syssph;
	const sphere_t* x1_sph;
	assert(nsg > 0);
	syssph = &gpk[0]->syssph;
	gmpk_sp = &gpk[0]->gmpk.sp;
	for (i = 1; (i < nsg); ++i) {
		if (gpk[i]->gmpk.sp.k > gmpk_sp->k) {
			syssph = &gpk[i]->syssph;
			gmpk_sp = &gpk[i]->gmpk.sp;
		}
	}
	x1_sph = get_sphere_lambda_sx(lnk->sx, syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	assert(nsg > 0);
	gmpk_sp = &gpk[0]->gmpk.sp;
	for (i = 1; (i < nsg); ++i) {
		if (gpk[i]->gmpk.sp.k > gmpk_sp->k) {
			gmpk_sp = &gpk[i]->gmpk.sp;
		}
	}
	create_syssph(&syssph, gmpk_sp);
	x1_sph = get_sphere_lambda_sx(lnk->sx, &syssph);
#endif
	/*--------------------------------*/
	ok = (dlog_prf != NULL);
	if (ok) {
		for (i = 0; i < nsg; ++i) {
			dlog_prf[i].g = &sg[i]->T7;
			dlog_prf[i].y = &sg[i]->T6;
			dlog_prf[i].n = &gpk[i]->gmpk.n;
		}
		/*----------------------------*/
		hash_link_m(gmpk_sp->k,
						 lnk_digest, &lnk_digestlen,
						 dat_digest, dat_digestlen,
						 nsg, sg);
		ok = dlog_vrfy(&lnk->c, &lnk->sx,
					   lnk_digest, lnk_digestlen,
					   nsg,
					   dlog_prf,
					   x1_sph,
					   gmpk_sp);
	}
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_V(dlog_p_t, dlog_prf, nsg);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_link_m_dat(unsigned which_sha,
							 const ftmgs_link_t* lnk,
							 const void* dat, unsigned datlen,
							 unsigned nsg,
							 const ftmgs_sign_t* sg[],
							 const ftmgs_pbkey_t* gpk[])
{
	bool_t ok;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = ftmgs_vrfy_link_m_dgst(lnk, dat_digest, dat_digestlen, nsg, sg, gpk);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_link_t_chk_members(const struct ftmgs_link_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_link_t {
		bigint_t c;		/* zero */
		bigint_t sx;	/* zero */
	};
	CHK_FIELD__(dummy_ftmgs_link_t, ftmgs_link_t, c);
	CHK_FIELD__(dummy_ftmgs_link_t, ftmgs_link_t, sx);
	CHK_SIZE__(dummy_ftmgs_link_t, ftmgs_link_t);
	return (code == 476205863); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_ctor(struct ftmgs_link_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_link_t_chk_members(p,476205863));
	bi_ctor(p->c);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_dtor(struct ftmgs_link_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_link_t_chk_members(p,476205863));
	bi_clear_zero(p->sx);
	bi_dtor(p->sx);
	bi_clear_zero(p->c);
	bi_dtor(p->c);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_asg(struct ftmgs_link_t* p, const struct ftmgs_link_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_link_t_chk_members(p,476205863));
	if (p != o) {
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_move(struct ftmgs_link_t* p, struct ftmgs_link_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_link_t_chk_members(p,476205863));
	if (p != o) {
		bi_asg_si(p->c, 0);
		bi_swap(p->c, o->c);
		bi_asg_si(p->sx, 0);
		bi_swap(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_link_t* ftmgs_link_t_new()/*auto*/
{
	struct ftmgs_link_t* p = (struct ftmgs_link_t*)malloc(sizeof(struct ftmgs_link_t));
	if (p != NULL) {
		ftmgs_link_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_link_t* ftmgs_link_t_clone(const struct ftmgs_link_t* o)/*auto*/
{
	struct ftmgs_link_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_link_t*)malloc(sizeof(struct ftmgs_link_t));
		if (p != NULL) {
			ftmgs_link_t_ctor(p);
			ftmgs_link_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_delete(struct ftmgs_link_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_link_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

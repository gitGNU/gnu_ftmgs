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
#include "claim.h"
#include <assert.h>
#include <stdlib.h>
#include "sok.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
static void hash_claim(unsigned which_sha,
					   void* clm_digest,
					   unsigned* clm_digestlen,
					   const void* dat_digest,
					   unsigned dat_digestlen,
					   const ftmgs_sign_t* sg)
{
	USHAContext sha_ctx;
	bi_sha_reset(&sha_ctx, which_sha);
	sha_input(&sha_ctx, dat_digest, dat_digestlen);
	ftmgs_hash_add_sign(&sha_ctx, sg);
	sha_result(&sha_ctx, clm_digest, clm_digestlen);
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_claim_dgst(ftmgs_claim_t* clm,
						const ftmgs_sign_t* sg,
						const void* dat_digest,
						unsigned dat_digestlen,
						const ftmgs_pbkey_t* gpk,
						const ftmgs_mbr_prkey_t* msk,
						rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned clm_digestlen = USHAMaxHashSize;
	char clm_digest[USHAMaxHashSize];
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(gx);
	BEG_VAR_A(dlog_p_t, dlog_prf, 1);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x1_sph = get_sphere_lambda_x(msk->x1i, &gpk->syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	create_syssph(&syssph, &gpk->gmpk.sp);
	x1_sph = get_sphere_lambda_x(msk->x1i, &syssph);
#endif
	/*--------------------------------*/
	dlog_prf[0].g = &sg->T7;
	dlog_prf[0].y = &sg->T6;
	dlog_prf[0].n = &gpk->gmpk.n;
	/*--------------------------------*/
	bi_powmod_sec(gx, *dlog_prf[0].g, msk->x1i, *dlog_prf[0].n);
	ok = bi_equals(gx, *dlog_prf[0].y);
	/*--------------------------------*/
	if (ok) {
		hash_claim(gpk->gmpk.sp.k,
				   clm_digest, &clm_digestlen,
				   dat_digest, dat_digestlen,
				   sg);
		ok = dlog_proof(&clm->c, &clm->sx,
				   clm_digest, clm_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   x1_sph,
				   &msk->x1i,
				   &gpk->gmpk.sp,
				   rnd_ctx);
	}
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_A(dlog_p_t, dlog_prf, 1);
	END_VAR_1_bigint_t(gx);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_claim_dat(unsigned which_sha,
					   ftmgs_claim_t* clm,
					   const ftmgs_sign_t* sg,
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
	ok = ftmgs_claim_dgst(clm, sg, dat_digest, dat_digestlen, gpk, msk,rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_claim_dgst(const ftmgs_claim_t* clm,
							 const ftmgs_sign_t* sg,
							 const void* dat_digest,
							 unsigned dat_digestlen,
							 const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned clm_digestlen = USHAMaxHashSize;
	char clm_digest[USHAMaxHashSize];
	/*--------------------------------*/
	BEG_VAR_A(dlog_p_t, dlog_prf, 1);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x1_sph = get_sphere_lambda_sx(clm->sx, &gpk->syssph);
#else
	BEG_VAR_1(syssph_t, syssph);
	const sphere_t* x1_sph;
	create_syssph(&syssph, &gpk->gmpk.sp);
	x1_sph = get_sphere_lambda_sx(clm->sx, &syssph);
#endif
	/*--------------------------------*/
	dlog_prf[0].g = &sg->T7;
	dlog_prf[0].y = &sg->T6;
	dlog_prf[0].n = &gpk->gmpk.n;
	/*--------------------------------*/
	hash_claim(gpk->gmpk.sp.k,
			   clm_digest, &clm_digestlen,
			   dat_digest, dat_digestlen,
			   sg);
	ok = dlog_vrfy(&clm->c, &clm->sx,
				   clm_digest, clm_digestlen,
				   array_sz(dlog_prf),
				   dlog_prf,
				   x1_sph,
				   &gpk->gmpk.sp);
	/*--------------------------------*/
#ifndef PRECOMPUTATIONS__
	END_VAR_1(syssph_t, syssph);
#endif
	END_VAR_A(dlog_p_t, dlog_prf, 1);
	END_BIGINT_CHK();
	/*--------------------------------*/
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_claim_dat(unsigned which_sha,
							const ftmgs_claim_t* clm,
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
	ok = ftmgs_vrfy_claim_dgst(clm, sg, dat_digest, dat_digestlen, gpk);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_claim_t_chk_members(struct ftmgs_claim_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_claim_t {
		bigint_t c;
		bigint_t sx;
	};
	CHK_FIELD__(dummy_ftmgs_claim_t, ftmgs_claim_t, c);
	CHK_FIELD__(dummy_ftmgs_claim_t, ftmgs_claim_t, sx);
	CHK_SIZE__(dummy_ftmgs_claim_t, ftmgs_claim_t);
	return (p!=NULL)&&(code == 156720532);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_claim_t_ctor(struct ftmgs_claim_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_claim_t_chk_members(p,156720532));
	bi_ctor(p->c);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void ftmgs_claim_t_dtor(struct ftmgs_claim_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_claim_t_chk_members(p,156720532));
	bi_dtor(p->sx);
	bi_dtor(p->c);
}
/*----------------------------------------------------------------------------*/
void ftmgs_claim_t_asg(struct ftmgs_claim_t* p, const struct ftmgs_claim_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_claim_t_chk_members(p,156720532));
	if (p != o) {
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_claim_t* ftmgs_claim_t_new()/*auto*/
{
	struct ftmgs_claim_t* p = (struct ftmgs_claim_t*)malloc(sizeof(struct ftmgs_claim_t));
	if (p != NULL) {
		ftmgs_claim_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_claim_t* ftmgs_claim_t_clone(const struct ftmgs_claim_t* o)/*auto*/
{
	struct ftmgs_claim_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_claim_t*)malloc(sizeof(struct ftmgs_claim_t));
		if (p != NULL) {
			ftmgs_claim_t_ctor(p);
			ftmgs_claim_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_claim_t_delete(struct ftmgs_claim_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_claim_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

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
#include "open.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "sign.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
bool_t ftmgs_open_dshare_j(ftmgs_opensharej_t* osj,
						 const ftmgs_sign_t* sg,
						 const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
						 const ftmgs_faj_gr_prkey_t* faj_gsk,
						 const ftmgs_pbkey_t* gpk,
						 rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	unsigned sg_digestlen = USHAMaxHashSize;
	char sg_digest[USHAMaxHashSize];
#ifdef PRECOMPUTATIONS__
	const sphere_t* sph = &gpk->syssph.mu;
#else
	BEG_VAR_1(sphere_t, mu);
	const sphere_t* sph = &mu;
	create_sph_mu(&mu, &gpk->gmpk.sp);
#endif
	ftmgs_hash_sign(gpk->gmpk.sp.k, sg_digest, &sg_digestlen, sg);
	ok = elgamal_thr_decrypt_share(&osj->dsharej,
							  &sg->T1_T2,
							  &gpk->gmpk,
							  &faj_gpk->pbkey_j,
							  &faj_gsk->prkey_j,
							  sph,
							  sg_digest, sg_digestlen,
							  rnd_ctx);
#ifndef PRECOMPUTATIONS__
	END_VAR_1(sphere_t, mu);
#endif
	return ok;
}
/*----------------------------------------------------------------------------*/
static bool_t ftmgs_open_add_dshare_j(ftmgs_openacc_t* osa,
									  const ftmgs_sign_t* sg,
									  const ftmgs_opensharej_t* osj,
									  const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
									  const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	unsigned sg_digestlen = USHAMaxHashSize;
	char sg_digest[USHAMaxHashSize];
#ifdef PRECOMPUTATIONS__
	const sphere_t* sph = &gpk->syssph.mu;
#else
	BEG_VAR_1(sphere_t, mu);
	const sphere_t* sph = &mu;
	create_sph_mu(&mu, &gpk->gmpk.sp);
#endif
	ftmgs_hash_sign(gpk->gmpk.sp.k, sg_digest, &sg_digestlen, sg);
	ok = elgamal_thr_add_decrypt_share(&osa->dshacc,
									   &osj->dsharej,
									   &sg->T1_T2,
									   &gpk->gmpk,
									   &faj_gpk->pbkey_j,
									   sph,
									   sg_digest, sg_digestlen);
#ifndef PRECOMPUTATIONS__
	END_VAR_1(sphere_t, mu);
#endif
	return ok;
}
/*----------------------------------------------------------------------------*/
static bool_t ftmgs_open_sign_base(ftmgs_open_t* op,
							  const ftmgs_openacc_t* osa,
							  const ftmgs_sign_t* sg,
							  const ftmgs_pbkey_t* gpk)
{					   
	return elgamal_thr_decrypt(op->A, &sg->T1_T2, &gpk->gmpk, &osa->dshacc);
}
/*----------------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_open_sign(ftmgs_open_t* op,
								ftmgs_openacc_t* osa,
								const ftmgs_sign_t* sg,
								const ftmgs_opensharej_t* osj,
								const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
								const ftmgs_pbkey_t* gpk)
{
	bool_t ok = TRUE;
	ftmgs_retcode_t rc = FTMGS_ERROR;
	if (osa->dshacc.nshares < gpk->gmpk.nkeys) {
		if ((ok = ftmgs_open_add_dshare_j(osa, sg, osj, faj_gpk, gpk))) {
			rc = FTMGS_UNFINISHED;
		}
	}
	if (ok && osa->dshacc.nshares == gpk->gmpk.nkeys) {
		if ((ok = ftmgs_open_sign_base(op, osa, sg, gpk))) {
			rc = FTMGS_OK;
		}
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_open_check(const ftmgs_open_t* op,
						const ftmgs_mbr_ref_t* mr,
						const ftmgs_pbkey_t* gpk)
{
	return bi_equals(mr->Ai, op->A) && ftmgs_vrfy_join_log(mr, gpk);
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_open_prtcl(ftmgs_open_t* op,
						const ftmgs_sign_t* sg,
						const ftmgs_pbkey_t* gpk,
						unsigned nfas,
						const ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
						const ftmgs_faj_gr_prkey_t* faj_gsk[],
						rnd_ctx_t* rnd_ctx)
{					   
	bool_t ok = (nfas == ftmgs_get_nfas_open(gpk));
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	unsigned i;
	BEG_VAR_2(ftmgs_openacc_t, osa,
			  ftmgs_opensharej_t, osj);
	/*Any -> Judge: <gpk, sg> */ 
	for (i = 0; ok && (rc == FTMGS_UNFINISHED) && (i < nfas); ++i) {
		/*Judge -> FAj: <gpk, sg> */
		/* FAj */
		ok = ftmgs_open_dshare_j(&osj, sg, faj_gpk[i], faj_gsk[i], gpk, rnd_ctx);
		/*FAj -> Judge: <osj> */ 
		/* Judge */
		if (ok) {
			rc = ftmgs_open_sign(op, &osa, sg, &osj, faj_gpk[i], gpk);
		} else {
			rc = FTMGS_ERROR;
		}
	}
	/*Judge -> GM: <gpk, op> */
	/*GM: db_foreach(UsrId, <gpk, mr>) DO ftmgs_open_ckeck(op, mr, gpk);*/
	/*GM -> Judge: <gpk, op, mr, UsrId> */
	END_VAR_2(ftmgs_openacc_t, osa,
			  ftmgs_opensharej_t, osj);
	return (rc == FTMGS_OK);
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_open_t_chk_members(const struct ftmgs_open_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_open_t {
		bigint_t A;		/* zero */
	};
	CHK_FIELD__(dummy_ftmgs_open_t, ftmgs_open_t, A);
	CHK_SIZE__(dummy_ftmgs_open_t, ftmgs_open_t);
	return (code == 510635963); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_ctor(struct ftmgs_open_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_open_t_chk_members(p,510635963));
	bi_ctor(p->A);
}
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_dtor(struct ftmgs_open_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_open_t_chk_members(p,510635963));
	bi_clear_zero(p->A);
	bi_dtor(p->A);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_asg(struct ftmgs_open_t* p, const struct ftmgs_open_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_open_t_chk_members(p,510635963));
	if (p != o) {
		bi_asg(p->A, o->A);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_move(struct ftmgs_open_t* p, struct ftmgs_open_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_open_t_chk_members(p,510635963));
	if (p != o) {
		bi_asg_si(p->A, 0);
		bi_swap(p->A, o->A);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_open_t* ftmgs_open_t_new()/*auto*/
{
	struct ftmgs_open_t* p = (struct ftmgs_open_t*)malloc(sizeof(struct ftmgs_open_t));
	if (p != NULL) {
		ftmgs_open_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_open_t* ftmgs_open_t_clone(const struct ftmgs_open_t* o)/*auto*/
{
	struct ftmgs_open_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_open_t*)malloc(sizeof(struct ftmgs_open_t));
		if (p != NULL) {
			ftmgs_open_t_ctor(p);
			ftmgs_open_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_delete(struct ftmgs_open_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_open_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_opensharej_t_chk_members(const struct ftmgs_opensharej_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_opensharej_t {
		elgamal_thr_decrypt_share_t dsharej;
	};
	CHK_FIELD__(dummy_ftmgs_opensharej_t, ftmgs_opensharej_t, dsharej);
	CHK_SIZE__(dummy_ftmgs_opensharej_t, ftmgs_opensharej_t);
	return (code == 377619568); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_ctor(struct ftmgs_opensharej_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_opensharej_t_chk_members(p,377619568));
	elgamal_thr_decrypt_share_t_ctor(&p->dsharej);
}
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_dtor(struct ftmgs_opensharej_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_opensharej_t_chk_members(p,377619568));
	elgamal_thr_decrypt_share_t_dtor(&p->dsharej);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_asg(struct ftmgs_opensharej_t* p, const struct ftmgs_opensharej_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_opensharej_t_chk_members(p,377619568));
	if (p != o) {
		elgamal_thr_decrypt_share_t_asg(&p->dsharej, &o->dsharej);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_move(struct ftmgs_opensharej_t* p, struct ftmgs_opensharej_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_opensharej_t_chk_members(p,377619568));
	if (p != o) {
		elgamal_thr_decrypt_share_t_move(&p->dsharej, &o->dsharej);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_opensharej_t* ftmgs_opensharej_t_new()/*auto*/
{
	struct ftmgs_opensharej_t* p = (struct ftmgs_opensharej_t*)malloc(sizeof(struct ftmgs_opensharej_t));
	if (p != NULL) {
		ftmgs_opensharej_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_opensharej_t* ftmgs_opensharej_t_clone(const struct ftmgs_opensharej_t* o)/*auto*/
{
	struct ftmgs_opensharej_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_opensharej_t*)malloc(sizeof(struct ftmgs_opensharej_t));
		if (p != NULL) {
			ftmgs_opensharej_t_ctor(p);
			ftmgs_opensharej_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_delete(struct ftmgs_opensharej_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_opensharej_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_openacc_t_chk_members(const struct ftmgs_openacc_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_openacc_t {
		elgamal_thr_dshare_acc_t dshacc;
	};
	CHK_FIELD__(dummy_ftmgs_openacc_t, ftmgs_openacc_t, dshacc);
	CHK_SIZE__(dummy_ftmgs_openacc_t, ftmgs_openacc_t);
	return (code == 528357397); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_ctor(struct ftmgs_openacc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_openacc_t_chk_members(p,528357397));
	elgamal_thr_dshare_acc_t_ctor(&p->dshacc);
}
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_dtor(struct ftmgs_openacc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_openacc_t_chk_members(p,528357397));
	elgamal_thr_dshare_acc_t_dtor(&p->dshacc);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_asg(struct ftmgs_openacc_t* p, const struct ftmgs_openacc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_openacc_t_chk_members(p,528357397));
	if (p != o) {
		elgamal_thr_dshare_acc_t_asg(&p->dshacc, &o->dshacc);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_move(struct ftmgs_openacc_t* p, struct ftmgs_openacc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_openacc_t_chk_members(p,528357397));
	if (p != o) {
		elgamal_thr_dshare_acc_t_move(&p->dshacc, &o->dshacc);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_openacc_t* ftmgs_openacc_t_new()/*auto*/
{
	struct ftmgs_openacc_t* p = (struct ftmgs_openacc_t*)malloc(sizeof(struct ftmgs_openacc_t));
	if (p != NULL) {
		ftmgs_openacc_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_openacc_t* ftmgs_openacc_t_clone(const struct ftmgs_openacc_t* o)/*auto*/
{
	struct ftmgs_openacc_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_openacc_t*)malloc(sizeof(struct ftmgs_openacc_t));
		if (p != NULL) {
			ftmgs_openacc_t_ctor(p);
			ftmgs_openacc_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_delete(struct ftmgs_openacc_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_openacc_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

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
#include "revtrace.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
bool_t ftmgs_reveal_dshare_j(ftmgs_mtkey_sharej_t* mtk_shj,
							 const ftmgs_mbr_ref_t* mr,
							 const ftmgs_faj_pbkey_share_t* faj_pk,
							 const ftmgs_faj_prkey_t* faj_sk,
							 const ftmgs_pbkey_t* gpk,
							 rnd_ctx_t* rnd_ctx)
{
	unsigned mr_digestlen = USHAMaxHashSize;
	char mr_digest[USHAMaxHashSize];
	bool_t ok = ftmgs_vrfy_join_log(mr, gpk);
	if (ok) {
		ftmgs_hash_join_log(gpk->gmpk.sp.k, mr_digest, &mr_digestlen, mr);
		ok = paillier_thr_decrypt_share(&mtk_shj->dsharej, &mr->UiVi,
										&gpk->fapk, &faj_pk->pbkey_j,
										&faj_sk->prkey_j,
										mr_digest, mr_digestlen, rnd_ctx);
	}
	return ok;
}
/*----------------------------------------------------------------------------*/
static bool_t ftmgs_reveal_add_dshare_j(ftmgs_mtkey_acc_t* mtka,
										const ftmgs_mtkey_sharej_t* mtk_shj,
										const ftmgs_mbr_ref_t* mr,
										const ftmgs_faj_pbkey_share_t* faj_pk,
										const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	unsigned mr_digestlen = USHAMaxHashSize;
	char mr_digest[USHAMaxHashSize];
	ftmgs_hash_join_log(gpk->gmpk.sp.k, mr_digest, &mr_digestlen, mr);
	ok = paillier_thr_add_decrypt_share(&mtka->dshacc,
										&mtk_shj->dsharej,
										&mr->UiVi,
										&gpk->fapk,
										&faj_pk->pbkey_j,
										mr_digest, mr_digestlen);
	return ok;
}
/*----------------------------------------------------------------------------*/
static bool_t ftmgs_reveal_mtkey_base(ftmgs_mtkey_t* mtk,
									  const ftmgs_mtkey_acc_t* mtka,
									  const ftmgs_mbr_ref_t* mr,
									  const ftmgs_pbkey_t* gpk)
{
	bool_t ok = ftmgs_vrfy_join_log(mr, gpk);
	if (ok) {
		bi_asg(mtk->n, gpk->gmpk.n);
		ok =  paillier_thr_decrypt(mtk->x,
								   &mr->UiVi,
								   &gpk->fapk,
								   &mtka->dshacc);
	}
	return ok;
}
/*----------------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_reveal_mtkey(ftmgs_mtkey_t* mtk,
								   ftmgs_mtkey_acc_t* mtka,
								   const ftmgs_mtkey_sharej_t* mtk_shj,
								   const ftmgs_mbr_ref_t* mr,
								   const ftmgs_faj_pbkey_share_t* faj_pk,
								   const ftmgs_pbkey_t* gpk)
{
	bool_t ok = TRUE;
	ftmgs_retcode_t rc = FTMGS_ERROR;
	if (mtka->dshacc.nshares < gpk->fapk.nkeys) {
		if ((ok = ftmgs_reveal_add_dshare_j(mtka, mtk_shj, mr, faj_pk, gpk))) {
			rc = FTMGS_UNFINISHED;
		}
	}
	if (ok && mtka->dshacc.nshares == gpk->fapk.nkeys) {
		if ((ok = ftmgs_reveal_mtkey_base(mtk, mtka, mr, gpk))) {
			rc = FTMGS_OK;
		}
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_trace(const ftmgs_sign_t* sg, const ftmgs_mtkey_t* mtk)
{
	bool_t ok;
	BEG_VAR_1_bigint_t(t5x);
	bi_powmod(t5x, sg->T5, mtk->x, mtk->n);
	ok = bi_equals(t5x, sg->T4);
	END_VAR_1_bigint_t(t5x);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_reveal_prtcl(ftmgs_mtkey_t* mtk,
						  const ftmgs_mbr_ref_t* mr,
						  const ftmgs_pbkey_t* gpk,
						  unsigned nfas,
						  const ftmgs_faj_pbkey_share_t* faj_pk[],
						  const ftmgs_faj_prkey_t* faj_sk[],
						  rnd_ctx_t* rnd_ctx)
{
	bool_t ok = (nfas == ftmgs_get_nfas_reveal(gpk));
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	unsigned i;
	BEG_VAR_2(ftmgs_mtkey_sharej_t, mtk_shj,
			  ftmgs_mtkey_acc_t, mtka);
	/*Judge -> GM: <UsrId> */
	/*GM: db_search(UsrId, <gpk, mr>);*/
	/*GM -> Judge: <gpk, mr> */ 
	for (i = 0; ok && (rc == FTMGS_UNFINISHED) && (i < nfas); ++i) {
		/*Judge -> FAj: <gpk, mr> */
		/* FAj */
		ok =  ftmgs_reveal_dshare_j(&mtk_shj, mr, faj_pk[i], faj_sk[i],
									gpk, rnd_ctx);
		/*FAj -> Judge: <mtk_shj> */ 
		/* Judge */
		if (ok) {
			rc = ftmgs_reveal_mtkey(mtk, &mtka, &mtk_shj, mr, faj_pk[i], gpk);
		} else {
			rc = FTMGS_ERROR;
		}
	}
	/*Judge -> TAj: <mtk> */
	END_VAR_2(ftmgs_mtkey_sharej_t, mtk_shj,
			  ftmgs_mtkey_acc_t, mtka);
	return (rc == FTMGS_OK);
}
/*----------------------------------------------------------------------------*/
	
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_mtkey_t_chk_members(const struct ftmgs_mtkey_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_mtkey_t {
		bigint_t n;		/* zero */
		bigint_t x;		/* zero */
	};
	CHK_FIELD__(dummy_ftmgs_mtkey_t, ftmgs_mtkey_t, n);
	CHK_FIELD__(dummy_ftmgs_mtkey_t, ftmgs_mtkey_t, x);
	CHK_SIZE__(dummy_ftmgs_mtkey_t, ftmgs_mtkey_t);
	return (code == 405954268); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_ctor(struct ftmgs_mtkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_t_chk_members(p,405954268));
	bi_ctor(p->n);
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_dtor(struct ftmgs_mtkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_t_chk_members(p,405954268));
	bi_clear_zero(p->x);
	bi_dtor(p->x);
	bi_clear_zero(p->n);
	bi_dtor(p->n);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_asg(struct ftmgs_mtkey_t* p, const struct ftmgs_mtkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_t_chk_members(p,405954268));
	if (p != o) {
		bi_asg(p->n, o->n);
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_move(struct ftmgs_mtkey_t* p, struct ftmgs_mtkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_t_chk_members(p,405954268));
	if (p != o) {
		bi_asg_si(p->n, 0);
		bi_swap(p->n, o->n);
		bi_asg_si(p->x, 0);
		bi_swap(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_t* ftmgs_mtkey_t_new()/*auto*/
{
	struct ftmgs_mtkey_t* p = (struct ftmgs_mtkey_t*)malloc(sizeof(struct ftmgs_mtkey_t));
	if (p != NULL) {
		ftmgs_mtkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_t* ftmgs_mtkey_t_clone(const struct ftmgs_mtkey_t* o)/*auto*/
{
	struct ftmgs_mtkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_mtkey_t*)malloc(sizeof(struct ftmgs_mtkey_t));
		if (p != NULL) {
			ftmgs_mtkey_t_ctor(p);
			ftmgs_mtkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_delete(struct ftmgs_mtkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_mtkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_mtkey_sharej_t_chk_members(const struct ftmgs_mtkey_sharej_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_mtkey_sharej_t {
		paillier_thr_decrypt_share_t dsharej;
	};
	CHK_FIELD__(dummy_ftmgs_mtkey_sharej_t, ftmgs_mtkey_sharej_t, dsharej);
	CHK_SIZE__(dummy_ftmgs_mtkey_sharej_t, ftmgs_mtkey_sharej_t);
	return (code == 137491709); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_ctor(struct ftmgs_mtkey_sharej_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_sharej_t_chk_members(p,137491709));
	paillier_thr_decrypt_share_t_ctor(&p->dsharej);
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_dtor(struct ftmgs_mtkey_sharej_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_sharej_t_chk_members(p,137491709));
	paillier_thr_decrypt_share_t_dtor(&p->dsharej);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_asg(struct ftmgs_mtkey_sharej_t* p, const struct ftmgs_mtkey_sharej_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_sharej_t_chk_members(p,137491709));
	if (p != o) {
		paillier_thr_decrypt_share_t_asg(&p->dsharej, &o->dsharej);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_move(struct ftmgs_mtkey_sharej_t* p, struct ftmgs_mtkey_sharej_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_sharej_t_chk_members(p,137491709));
	if (p != o) {
		paillier_thr_decrypt_share_t_move(&p->dsharej, &o->dsharej);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_sharej_t* ftmgs_mtkey_sharej_t_new()/*auto*/
{
	struct ftmgs_mtkey_sharej_t* p = (struct ftmgs_mtkey_sharej_t*)malloc(sizeof(struct ftmgs_mtkey_sharej_t));
	if (p != NULL) {
		ftmgs_mtkey_sharej_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_sharej_t* ftmgs_mtkey_sharej_t_clone(const struct ftmgs_mtkey_sharej_t* o)/*auto*/
{
	struct ftmgs_mtkey_sharej_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_mtkey_sharej_t*)malloc(sizeof(struct ftmgs_mtkey_sharej_t));
		if (p != NULL) {
			ftmgs_mtkey_sharej_t_ctor(p);
			ftmgs_mtkey_sharej_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_delete(struct ftmgs_mtkey_sharej_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_mtkey_sharej_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_mtkey_acc_t_chk_members(const struct ftmgs_mtkey_acc_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_mtkey_acc_t {
		paillier_thr_dshare_acc_t dshacc;
	};
	CHK_FIELD__(dummy_ftmgs_mtkey_acc_t, ftmgs_mtkey_acc_t, dshacc);
	CHK_SIZE__(dummy_ftmgs_mtkey_acc_t, ftmgs_mtkey_acc_t);
	return (code == 102264701); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_ctor(struct ftmgs_mtkey_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_acc_t_chk_members(p,102264701));
	paillier_thr_dshare_acc_t_ctor(&p->dshacc);
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_dtor(struct ftmgs_mtkey_acc_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mtkey_acc_t_chk_members(p,102264701));
	paillier_thr_dshare_acc_t_dtor(&p->dshacc);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_asg(struct ftmgs_mtkey_acc_t* p, const struct ftmgs_mtkey_acc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_acc_t_chk_members(p,102264701));
	if (p != o) {
		paillier_thr_dshare_acc_t_asg(&p->dshacc, &o->dshacc);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_move(struct ftmgs_mtkey_acc_t* p, struct ftmgs_mtkey_acc_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mtkey_acc_t_chk_members(p,102264701));
	if (p != o) {
		paillier_thr_dshare_acc_t_move(&p->dshacc, &o->dshacc);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_acc_t* ftmgs_mtkey_acc_t_new()/*auto*/
{
	struct ftmgs_mtkey_acc_t* p = (struct ftmgs_mtkey_acc_t*)malloc(sizeof(struct ftmgs_mtkey_acc_t));
	if (p != NULL) {
		ftmgs_mtkey_acc_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_acc_t* ftmgs_mtkey_acc_t_clone(const struct ftmgs_mtkey_acc_t* o)/*auto*/
{
	struct ftmgs_mtkey_acc_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_mtkey_acc_t*)malloc(sizeof(struct ftmgs_mtkey_acc_t));
		if (p != NULL) {
			ftmgs_mtkey_acc_t_ctor(p);
			ftmgs_mtkey_acc_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_delete(struct ftmgs_mtkey_acc_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_mtkey_acc_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

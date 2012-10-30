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
#ifndef umalccvbg_paillier_thr_h__
#define umalccvbg_paillier_thr_h__	1
#include "bigintop.h"
#include "birnd.h"
#include "random.h"
#include "syspar.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*------------------------------------------------------------------------*/
typedef struct paillier_thr_precomp_t {
	bigint_t n2;		/* zero */
	sphere_t upsilon;			/* { 0 .. Z_{n^2/4} } */
	sphere_t psi;				/* { 0 .. Z_{n/4} } */
} paillier_thr_precomp_t;
/*----------------------------------------------------------------------------*/
/* y = PROD_j(yj) */
typedef struct paillier_thr_pbkey_t {
	syspar_t sp;		/* zero */
	bigint_t n;			/* zero */
	bigint_t g;			/* zero */
	bigint_t y;			/* zero */
	unsigned nkeys;		/* zero */
#ifdef PRECOMPUTATIONS__
	paillier_thr_precomp_t precomp;
#endif
} paillier_thr_pbkey_t;
/*----------------------------------------------------------------------------*/
/* yj = g^xj (mod n) */
typedef struct paillier_thr_pbkey_share_t {
	bigint_t yj;		/* zero */
} paillier_thr_pbkey_share_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_thr_prkey_t {
	bigint_t xj;		/* zero */
} paillier_thr_prkey_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_thr_ciphertext_t {
	bigint_t alpha;		/* zero */
	bigint_t beta;		/* zero */
} paillier_thr_ciphertext_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_thr_decrypt_share_t {
	bigint_t alpha_xj;	/* zero */
	bigint_t c;			/* zero */
	bigint_t sx;		/* zero */
} paillier_thr_decrypt_share_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_thr_dshare_acc_t {
	unsigned nshares;	/* zero */
	bigint_t a;			/* zero */
} paillier_thr_dshare_acc_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_precomp(paillier_thr_precomp_t* precomp,
								const paillier_thr_pbkey_t* pk);
/*------------------------------------*/
void paillier_thr_create_pbkey(paillier_thr_pbkey_t* pk, 
							   unsigned nu,
							   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void paillier_thr_create_prkey(paillier_thr_pbkey_share_t* pkj,
							   paillier_thr_prkey_t* skj,
							   const paillier_thr_pbkey_t* pk,
							   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void paillier_thr_add_pbkey_share(paillier_thr_pbkey_t* pk,
								  const paillier_thr_pbkey_share_t* pkj);
/*--------------------------------*/
void paillier_thr_encrypt_k(paillier_thr_ciphertext_t* ct,
							bigint_t k,
							const bigint_t msg,
							const paillier_thr_pbkey_t* pk,
							rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void paillier_thr_encrypt(paillier_thr_ciphertext_t* ct,
						  const bigint_t msg,
						  const paillier_thr_pbkey_t* pk,
						  rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
USE_RESULT__
bool_t paillier_thr_decrypt_share(paillier_thr_decrypt_share_t* dsj,
								const paillier_thr_ciphertext_t* ct,
								const paillier_thr_pbkey_t* pk,
								const paillier_thr_pbkey_share_t* pkj,
								const paillier_thr_prkey_t* skj,
								const void* dat_digest,
								unsigned dat_digestlen,
								rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
USE_RESULT__
bool_t paillier_thr_add_decrypt_share(paillier_thr_dshare_acc_t* alpha_pi,
									  const paillier_thr_decrypt_share_t* dsj,
									  const paillier_thr_ciphertext_t* ct,
									  const paillier_thr_pbkey_t* pk,
									  const paillier_thr_pbkey_share_t* pkj,
									  const void* dat_digest,
									  unsigned dat_digestlen);
/*--------------------------------*/
USE_RESULT__
bool_t paillier_thr_decrypt(bigint_t msg,
							const paillier_thr_ciphertext_t* ct,
							const paillier_thr_pbkey_t* pk,
							const paillier_thr_dshare_acc_t* alpha_pi);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void paillier_thr_precomp_t_ctor(struct paillier_thr_precomp_t* p);
void paillier_thr_precomp_t_dtor(struct paillier_thr_precomp_t* p);
void paillier_thr_precomp_t_asg(struct paillier_thr_precomp_t* p, const struct paillier_thr_precomp_t* o);
void paillier_thr_precomp_t_move(struct paillier_thr_precomp_t* p, struct paillier_thr_precomp_t* o);
struct paillier_thr_precomp_t* paillier_thr_precomp_t_new();
struct paillier_thr_precomp_t* paillier_thr_precomp_t_clone(const struct paillier_thr_precomp_t* o);
void paillier_thr_precomp_t_delete(struct paillier_thr_precomp_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_t_ctor(struct paillier_thr_pbkey_t* p);
void paillier_thr_pbkey_t_dtor(struct paillier_thr_pbkey_t* p);
void paillier_thr_pbkey_t_asg(struct paillier_thr_pbkey_t* p, const struct paillier_thr_pbkey_t* o);
void paillier_thr_pbkey_t_move(struct paillier_thr_pbkey_t* p, struct paillier_thr_pbkey_t* o);
struct paillier_thr_pbkey_t* paillier_thr_pbkey_t_new();
struct paillier_thr_pbkey_t* paillier_thr_pbkey_t_clone(const struct paillier_thr_pbkey_t* o);
void paillier_thr_pbkey_t_delete(struct paillier_thr_pbkey_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_pbkey_share_t_ctor(struct paillier_thr_pbkey_share_t* p);
void paillier_thr_pbkey_share_t_dtor(struct paillier_thr_pbkey_share_t* p);
void paillier_thr_pbkey_share_t_asg(struct paillier_thr_pbkey_share_t* p, const struct paillier_thr_pbkey_share_t* o);
void paillier_thr_pbkey_share_t_move(struct paillier_thr_pbkey_share_t* p, struct paillier_thr_pbkey_share_t* o);
struct paillier_thr_pbkey_share_t* paillier_thr_pbkey_share_t_new();
struct paillier_thr_pbkey_share_t* paillier_thr_pbkey_share_t_clone(const struct paillier_thr_pbkey_share_t* o);
void paillier_thr_pbkey_share_t_delete(struct paillier_thr_pbkey_share_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_prkey_t_ctor(struct paillier_thr_prkey_t* p);
void paillier_thr_prkey_t_dtor(struct paillier_thr_prkey_t* p);
void paillier_thr_prkey_t_asg(struct paillier_thr_prkey_t* p, const struct paillier_thr_prkey_t* o);
void paillier_thr_prkey_t_move(struct paillier_thr_prkey_t* p, struct paillier_thr_prkey_t* o);
struct paillier_thr_prkey_t* paillier_thr_prkey_t_new();
struct paillier_thr_prkey_t* paillier_thr_prkey_t_clone(const struct paillier_thr_prkey_t* o);
void paillier_thr_prkey_t_delete(struct paillier_thr_prkey_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_ciphertext_t_ctor(struct paillier_thr_ciphertext_t* p);
void paillier_thr_ciphertext_t_dtor(struct paillier_thr_ciphertext_t* p);
void paillier_thr_ciphertext_t_asg(struct paillier_thr_ciphertext_t* p, const struct paillier_thr_ciphertext_t* o);
void paillier_thr_ciphertext_t_move(struct paillier_thr_ciphertext_t* p, struct paillier_thr_ciphertext_t* o);
struct paillier_thr_ciphertext_t* paillier_thr_ciphertext_t_new();
struct paillier_thr_ciphertext_t* paillier_thr_ciphertext_t_clone(const struct paillier_thr_ciphertext_t* o);
void paillier_thr_ciphertext_t_delete(struct paillier_thr_ciphertext_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_decrypt_share_t_ctor(struct paillier_thr_decrypt_share_t* p);
void paillier_thr_decrypt_share_t_dtor(struct paillier_thr_decrypt_share_t* p);
void paillier_thr_decrypt_share_t_asg(struct paillier_thr_decrypt_share_t* p, const struct paillier_thr_decrypt_share_t* o);
void paillier_thr_decrypt_share_t_move(struct paillier_thr_decrypt_share_t* p, struct paillier_thr_decrypt_share_t* o);
struct paillier_thr_decrypt_share_t* paillier_thr_decrypt_share_t_new();
struct paillier_thr_decrypt_share_t* paillier_thr_decrypt_share_t_clone(const struct paillier_thr_decrypt_share_t* o);
void paillier_thr_decrypt_share_t_delete(struct paillier_thr_decrypt_share_t* p);
/*----------------------------------------------------------------------------*/
void paillier_thr_dshare_acc_t_ctor(struct paillier_thr_dshare_acc_t* p);
void paillier_thr_dshare_acc_t_dtor(struct paillier_thr_dshare_acc_t* p);
void paillier_thr_dshare_acc_t_asg(struct paillier_thr_dshare_acc_t* p, const struct paillier_thr_dshare_acc_t* o);
void paillier_thr_dshare_acc_t_move(struct paillier_thr_dshare_acc_t* p, struct paillier_thr_dshare_acc_t* o);
struct paillier_thr_dshare_acc_t* paillier_thr_dshare_acc_t_new();
struct paillier_thr_dshare_acc_t* paillier_thr_dshare_acc_t_clone(const struct paillier_thr_dshare_acc_t* o);
void paillier_thr_dshare_acc_t_delete(struct paillier_thr_dshare_acc_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

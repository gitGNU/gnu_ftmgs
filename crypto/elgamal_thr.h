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
#ifndef umalccvbg_elgamal_thr_h__
#define umalccvbg_elgamal_thr_h__	1
#include "bigint.h"
#include "birnd.h"
#include "random.h"
#include "syspar.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_modfact_t {
	bigint_t p;		/* zero */
	bigint_t q;		/* zero */
} elgamal_thr_modfact_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_modord_t {
	bigint_t p1;		/* zero */
	bigint_t q1;		/* zero */
} elgamal_thr_modord_t;
/*----------------------------------------------------------------------------*/
/* y = PROD_j(yj) */
typedef struct elgamal_thr_pbkey_t {
	syspar_t sp;
	bigint_t n;		/* zero */
	bigint_t g;		/* zero */
	bigint_t y;		/* zero */
	unsigned nkeys;	/* zero */
} elgamal_thr_pbkey_t;
/*----------------------------------------------------------------------------*/
/* yj = g^xj (mod n) */
typedef struct elgamal_thr_pbkey_share_t {
	bigint_t yj;		/* zero */
} elgamal_thr_pbkey_share_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_prkey_t {
	bigint_t xj;		/* zero */
} elgamal_thr_prkey_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_ciphertext_t {
	bigint_t alpha;		/* zero */
	bigint_t beta;		/* zero */
} elgamal_thr_ciphertext_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_decrypt_share_t {
	bigint_t alpha_xj;	/* zero */
	bigint_t c;			/* zero */
	bigint_t sx;		/* zero */
} elgamal_thr_decrypt_share_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_thr_dshare_acc_t {
	unsigned nshares;	/* zero */
	bigint_t a;			/* zero */
} elgamal_thr_dshare_acc_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void elgamal_thr_create_pbkey(elgamal_thr_pbkey_t* pk, 
							  elgamal_thr_modfact_t* npq,
							  elgamal_thr_modord_t* p1q1,
							  unsigned nu,
							  rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_thr_create_prkey(elgamal_thr_pbkey_share_t* pkj,
							  elgamal_thr_prkey_t* skj,
							  const elgamal_thr_pbkey_t* pk,
							  const sphere_t* sph,
							  rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_thr_add_pbkey_share(elgamal_thr_pbkey_t* pk,
								 const elgamal_thr_pbkey_share_t* pkj);
/*--------------------------------*/
void elgamal_thr_encrypt_k(elgamal_thr_ciphertext_t* ct,
						   bigint_t k,
						   const bigint_t msg,
						   const elgamal_thr_pbkey_t* pk,
						   const sphere_t* sph,
						   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_thr_encrypt(elgamal_thr_ciphertext_t* ct,
						 const bigint_t msg,
						 const elgamal_thr_pbkey_t* pk,
						 const sphere_t* sph,
						 rnd_ctx_t* rnd_ctx
						 );
/*--------------------------------*/
USE_RESULT__
bool_t elgamal_thr_decrypt_share(elgamal_thr_decrypt_share_t* dsj,
							   const elgamal_thr_ciphertext_t* ct,
							   const elgamal_thr_pbkey_t* pk,
							   const elgamal_thr_pbkey_share_t* pkj,
							   const elgamal_thr_prkey_t* skj,
							   const sphere_t* sph,
							   const void* dat_digest,
							   unsigned dat_digestlen,
							   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
USE_RESULT__
bool_t elgamal_thr_add_decrypt_share(elgamal_thr_dshare_acc_t* alpha_pi,
									 const elgamal_thr_decrypt_share_t* dsj,
									 const elgamal_thr_ciphertext_t* ct,
									 const elgamal_thr_pbkey_t* pk,
									 const elgamal_thr_pbkey_share_t* pkj,
									 const sphere_t* sph,
									 const void* dat_digest,
									 unsigned dat_digestlen);
/*--------------------------------*/
USE_RESULT__
bool_t elgamal_thr_decrypt(bigint_t msg,
						   const elgamal_thr_ciphertext_t* ct,
						   const elgamal_thr_pbkey_t* pk,
						   const elgamal_thr_dshare_acc_t* alpha_pi);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void elgamal_thr_modfact_t_ctor(struct elgamal_thr_modfact_t* p);
void elgamal_thr_modfact_t_dtor(struct elgamal_thr_modfact_t* p);
void elgamal_thr_modfact_t_asg(struct elgamal_thr_modfact_t* p, const struct elgamal_thr_modfact_t* o);
void elgamal_thr_modfact_t_move(struct elgamal_thr_modfact_t* p, struct elgamal_thr_modfact_t* o);
struct elgamal_thr_modfact_t* elgamal_thr_modfact_t_new();
struct elgamal_thr_modfact_t* elgamal_thr_modfact_t_clone(const struct elgamal_thr_modfact_t* o);
void elgamal_thr_modfact_t_delete(struct elgamal_thr_modfact_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_modord_t_ctor(struct elgamal_thr_modord_t* p);
void elgamal_thr_modord_t_dtor(struct elgamal_thr_modord_t* p);
void elgamal_thr_modord_t_asg(struct elgamal_thr_modord_t* p, const struct elgamal_thr_modord_t* o);
void elgamal_thr_modord_t_move(struct elgamal_thr_modord_t* p, struct elgamal_thr_modord_t* o);
struct elgamal_thr_modord_t* elgamal_thr_modord_t_new();
struct elgamal_thr_modord_t* elgamal_thr_modord_t_clone(const struct elgamal_thr_modord_t* o);
void elgamal_thr_modord_t_delete(struct elgamal_thr_modord_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_t_ctor(struct elgamal_thr_pbkey_t* p);
void elgamal_thr_pbkey_t_dtor(struct elgamal_thr_pbkey_t* p);
void elgamal_thr_pbkey_t_asg(struct elgamal_thr_pbkey_t* p, const struct elgamal_thr_pbkey_t* o);
void elgamal_thr_pbkey_t_move(struct elgamal_thr_pbkey_t* p, struct elgamal_thr_pbkey_t* o);
struct elgamal_thr_pbkey_t* elgamal_thr_pbkey_t_new();
struct elgamal_thr_pbkey_t* elgamal_thr_pbkey_t_clone(const struct elgamal_thr_pbkey_t* o);
void elgamal_thr_pbkey_t_delete(struct elgamal_thr_pbkey_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_pbkey_share_t_ctor(struct elgamal_thr_pbkey_share_t* p);
void elgamal_thr_pbkey_share_t_dtor(struct elgamal_thr_pbkey_share_t* p);
void elgamal_thr_pbkey_share_t_asg(struct elgamal_thr_pbkey_share_t* p, const struct elgamal_thr_pbkey_share_t* o);
void elgamal_thr_pbkey_share_t_move(struct elgamal_thr_pbkey_share_t* p, struct elgamal_thr_pbkey_share_t* o);
struct elgamal_thr_pbkey_share_t* elgamal_thr_pbkey_share_t_new();
struct elgamal_thr_pbkey_share_t* elgamal_thr_pbkey_share_t_clone(const struct elgamal_thr_pbkey_share_t* o);
void elgamal_thr_pbkey_share_t_delete(struct elgamal_thr_pbkey_share_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_prkey_t_ctor(struct elgamal_thr_prkey_t* p);
void elgamal_thr_prkey_t_dtor(struct elgamal_thr_prkey_t* p);
void elgamal_thr_prkey_t_asg(struct elgamal_thr_prkey_t* p, const struct elgamal_thr_prkey_t* o);
void elgamal_thr_prkey_t_move(struct elgamal_thr_prkey_t* p, struct elgamal_thr_prkey_t* o);
struct elgamal_thr_prkey_t* elgamal_thr_prkey_t_new();
struct elgamal_thr_prkey_t* elgamal_thr_prkey_t_clone(const struct elgamal_thr_prkey_t* o);
void elgamal_thr_prkey_t_delete(struct elgamal_thr_prkey_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_ciphertext_t_ctor(struct elgamal_thr_ciphertext_t* p);
void elgamal_thr_ciphertext_t_dtor(struct elgamal_thr_ciphertext_t* p);
void elgamal_thr_ciphertext_t_asg(struct elgamal_thr_ciphertext_t* p, const struct elgamal_thr_ciphertext_t* o);
void elgamal_thr_ciphertext_t_move(struct elgamal_thr_ciphertext_t* p, struct elgamal_thr_ciphertext_t* o);
struct elgamal_thr_ciphertext_t* elgamal_thr_ciphertext_t_new();
struct elgamal_thr_ciphertext_t* elgamal_thr_ciphertext_t_clone(const struct elgamal_thr_ciphertext_t* o);
void elgamal_thr_ciphertext_t_delete(struct elgamal_thr_ciphertext_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_decrypt_share_t_ctor(struct elgamal_thr_decrypt_share_t* p);
void elgamal_thr_decrypt_share_t_dtor(struct elgamal_thr_decrypt_share_t* p);
void elgamal_thr_decrypt_share_t_asg(struct elgamal_thr_decrypt_share_t* p, const struct elgamal_thr_decrypt_share_t* o);
void elgamal_thr_decrypt_share_t_move(struct elgamal_thr_decrypt_share_t* p, struct elgamal_thr_decrypt_share_t* o);
struct elgamal_thr_decrypt_share_t* elgamal_thr_decrypt_share_t_new();
struct elgamal_thr_decrypt_share_t* elgamal_thr_decrypt_share_t_clone(const struct elgamal_thr_decrypt_share_t* o);
void elgamal_thr_decrypt_share_t_delete(struct elgamal_thr_decrypt_share_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_thr_dshare_acc_t_ctor(struct elgamal_thr_dshare_acc_t* p);
void elgamal_thr_dshare_acc_t_dtor(struct elgamal_thr_dshare_acc_t* p);
void elgamal_thr_dshare_acc_t_asg(struct elgamal_thr_dshare_acc_t* p, const struct elgamal_thr_dshare_acc_t* o);
void elgamal_thr_dshare_acc_t_move(struct elgamal_thr_dshare_acc_t* p, struct elgamal_thr_dshare_acc_t* o);
struct elgamal_thr_dshare_acc_t* elgamal_thr_dshare_acc_t_new();
struct elgamal_thr_dshare_acc_t* elgamal_thr_dshare_acc_t_clone(const struct elgamal_thr_dshare_acc_t* o);
void elgamal_thr_dshare_acc_t_delete(struct elgamal_thr_dshare_acc_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

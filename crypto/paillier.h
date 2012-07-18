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
#ifndef umalccvbg_paillier_h__
#define umalccvbg_paillier_h__	1
#include "bigint.h"
#include "birnd.h"
#include "random.h"
BEGIN_EXTERN_C
/*------------------------------------------------------------------------*/
/* y = g^x (mod n^2) */
typedef struct paillier_pbkey_t {
	bigint_t n;		/* zero */
	bigint_t g;		/* zero */
	bigint_t y;		/* zero */
} paillier_pbkey_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_prkey_t {
	bigint_t x;		/* zero */
} paillier_prkey_t;
/*----------------------------------------------------------------------------*/
typedef struct paillier_ciphertext_t {
	bigint_t alpha;		/* zero */
	bigint_t beta;		/* zero */
} paillier_ciphertext_t;
/*----------------------------------------------------------------------------*/
void paillier_create_keys(paillier_pbkey_t* pk,
						  paillier_prkey_t* sk,
						  unsigned nu,
						  rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void paillier_encrypt_k(paillier_ciphertext_t* ct,
					   bigint_t k,
					   const bigint_t msg,
					   const paillier_pbkey_t* pk,
					   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void paillier_encrypt(paillier_ciphertext_t* ct,
					 const bigint_t msg,
					 const paillier_pbkey_t* pk,
					 rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
USE_RESULT__
bool_t paillier_decrypt(bigint_t msg,
						const paillier_ciphertext_t* ct,
						const paillier_pbkey_t* pk,
						const paillier_prkey_t* sk);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void paillier_pbkey_t_ctor(struct paillier_pbkey_t* p);
void paillier_pbkey_t_dtor(struct paillier_pbkey_t* p);
void paillier_pbkey_t_asg(struct paillier_pbkey_t* p, const struct paillier_pbkey_t* o);
void paillier_pbkey_t_move(struct paillier_pbkey_t* p, struct paillier_pbkey_t* o);
struct paillier_pbkey_t* paillier_pbkey_t_new();
struct paillier_pbkey_t* paillier_pbkey_t_clone(const struct paillier_pbkey_t* o);
void paillier_pbkey_t_delete(struct paillier_pbkey_t* p);
/*----------------------------------------------------------------------------*/
void paillier_prkey_t_ctor(struct paillier_prkey_t* p);
void paillier_prkey_t_dtor(struct paillier_prkey_t* p);
void paillier_prkey_t_asg(struct paillier_prkey_t* p, const struct paillier_prkey_t* o);
void paillier_prkey_t_move(struct paillier_prkey_t* p, struct paillier_prkey_t* o);
struct paillier_prkey_t* paillier_prkey_t_new();
struct paillier_prkey_t* paillier_prkey_t_clone(const struct paillier_prkey_t* o);
void paillier_prkey_t_delete(struct paillier_prkey_t* p);
/*----------------------------------------------------------------------------*/
void paillier_ciphertext_t_ctor(struct paillier_ciphertext_t* p);
void paillier_ciphertext_t_dtor(struct paillier_ciphertext_t* p);
void paillier_ciphertext_t_asg(struct paillier_ciphertext_t* p, const struct paillier_ciphertext_t* o);
void paillier_ciphertext_t_move(struct paillier_ciphertext_t* p, struct paillier_ciphertext_t* o);
struct paillier_ciphertext_t* paillier_ciphertext_t_new();
struct paillier_ciphertext_t* paillier_ciphertext_t_clone(const struct paillier_ciphertext_t* o);
void paillier_ciphertext_t_delete(struct paillier_ciphertext_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

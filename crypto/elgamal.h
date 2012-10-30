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
#ifndef umalccvbg_elgamal_h__
#define umalccvbg_elgamal_h__	1
#include "bigintop.h"
#include "birnd.h"
#include "random.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
typedef struct elgamal_modfact_t {
	bigint_t p;		/* zero */
	bigint_t q;		/* zero */
} elgamal_modfact_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_modord_t {
	bigint_t p1;		/* zero */
	bigint_t q1;		/* zero */
} elgamal_modord_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_pbkey_t {
	bigint_t n;		/* zero */
	bigint_t g;		/* zero */
	bigint_t y;		/* zero */
} elgamal_pbkey_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_prkey_t {
	bigint_t x;		/* zero */
} elgamal_prkey_t;
/*----------------------------------------------------------------------------*/
typedef struct elgamal_ciphertext_t {
	bigint_t alpha;		/* zero */
	bigint_t beta;		/* zero */
} elgamal_ciphertext_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void elgamal_create_keys(elgamal_pbkey_t* pk,
						 elgamal_prkey_t* sk,
						 elgamal_modfact_t* npq,
						 elgamal_modord_t* p1q1,
						 const sphere_t* sph,
						 unsigned nu,
						 rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_encrypt_k(elgamal_ciphertext_t* ct,
					   bigint_t k,
					   const bigint_t msg,
					   const elgamal_pbkey_t* pk,
					   const sphere_t* sph,
					   rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_encrypt(elgamal_ciphertext_t* ct,
					 const bigint_t msg,
					 const elgamal_pbkey_t* pk,
					 const sphere_t* sph,
					 rnd_ctx_t* rnd_ctx);
/*--------------------------------*/
void elgamal_decrypt(bigint_t msg,
					 const elgamal_ciphertext_t* ct,
					 const elgamal_pbkey_t* pk,
					 const elgamal_prkey_t* sk);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void elgamal_modfact_t_ctor(struct elgamal_modfact_t* p);
void elgamal_modfact_t_dtor(struct elgamal_modfact_t* p);
void elgamal_modfact_t_asg(struct elgamal_modfact_t* d, const struct elgamal_modfact_t* o);
void elgamal_modfact_t_move(struct elgamal_modfact_t* d, struct elgamal_modfact_t* o);
struct elgamal_modfact_t* elgamal_modfact_t_new();
struct elgamal_modfact_t* elgamal_modfact_t_clone(const struct elgamal_modfact_t* o);
void elgamal_modfact_t_delete(struct elgamal_modfact_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_modord_t_ctor(struct elgamal_modord_t* p);
void elgamal_modord_t_dtor(struct elgamal_modord_t* p);
void elgamal_modord_t_asg(struct elgamal_modord_t* d, const struct elgamal_modord_t* o);
void elgamal_modord_t_move(struct elgamal_modord_t* d, struct elgamal_modord_t* o);
struct elgamal_modord_t* elgamal_modord_t_new();
struct elgamal_modord_t* elgamal_modord_t_clone(const struct elgamal_modord_t* o);
void elgamal_modord_t_delete(struct elgamal_modord_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_pbkey_t_ctor(struct elgamal_pbkey_t* p);
void elgamal_pbkey_t_dtor(struct elgamal_pbkey_t* p);
void elgamal_pbkey_t_asg(struct elgamal_pbkey_t* d, const struct elgamal_pbkey_t* o);
void elgamal_pbkey_t_move(struct elgamal_pbkey_t* d, struct elgamal_pbkey_t* o);
struct elgamal_pbkey_t* elgamal_pbkey_t_new();
struct elgamal_pbkey_t* elgamal_pbkey_t_clone(const struct elgamal_pbkey_t* o);
void elgamal_pbkey_t_delete(struct elgamal_pbkey_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_prkey_t_ctor(struct elgamal_prkey_t* p);
void elgamal_prkey_t_dtor(struct elgamal_prkey_t* p);
void elgamal_prkey_t_asg(struct elgamal_prkey_t* d, const struct elgamal_prkey_t* o);
void elgamal_prkey_t_move(struct elgamal_prkey_t* d, struct elgamal_prkey_t* o);
struct elgamal_prkey_t* elgamal_prkey_t_new();
struct elgamal_prkey_t* elgamal_prkey_t_clone(const struct elgamal_prkey_t* o);
void elgamal_prkey_t_delete(struct elgamal_prkey_t* p);
/*----------------------------------------------------------------------------*/
void elgamal_ciphertext_t_ctor(struct elgamal_ciphertext_t* p);
void elgamal_ciphertext_t_dtor(struct elgamal_ciphertext_t* p);
void elgamal_ciphertext_t_asg(struct elgamal_ciphertext_t* d, const struct elgamal_ciphertext_t* o);
void elgamal_ciphertext_t_move(struct elgamal_ciphertext_t* d, struct elgamal_ciphertext_t* o);
struct elgamal_ciphertext_t* elgamal_ciphertext_t_new();
struct elgamal_ciphertext_t* elgamal_ciphertext_t_clone(const struct elgamal_ciphertext_t* o);
void elgamal_ciphertext_t_delete(struct elgamal_ciphertext_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

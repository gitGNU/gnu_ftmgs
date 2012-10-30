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
#ifndef umalccvbg_schnorr_h__
#define umalccvbg_schnorr_h__	1
#include "sha.h"
#include "bigintop.h"
#include "birnd.h"
#include "random.h"
#include "syspar.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*------------------------------------------------------------------------*/
/* y = g^x (mod n) */
typedef struct schnorr_pbkey_t {
	syspar_t sp;
	bigint_t n;		/* zero */
	bigint_t g;		/* zero */
	bigint_t y;		/* zero */
} schnorr_pbkey_t;
/*------------------------------------*/
typedef struct schnorr_prkey_t {
	bigint_t x;		/* zero */
} schnorr_prkey_t;
/*------------------------------------*/
typedef struct schnorr_sign_t {
	bigint_t c;		/* zero */
	bigint_t sx;	/* zero */
} schnorr_sign_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void schnorr_create_keys(schnorr_pbkey_t* pk,
						 schnorr_prkey_t* sk,
						 const sphere_t* sph,
						 unsigned nu,
						 rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t schnorr_sign(schnorr_sign_t* sg,
				  const void* dat_digest,
				  unsigned dat_digestlen,
				  const schnorr_pbkey_t* pk,
				  const schnorr_prkey_t* sk,
				  const sphere_t* sph,
				  rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t schnorr_sign_dat(unsigned which_sha,
					  schnorr_sign_t* sg,
					  const void* dat,
					  unsigned datlen,
					  const schnorr_pbkey_t* pk,
					  const schnorr_prkey_t* sk,
					  const sphere_t* sph,
					  rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t schnorr_vrfy(const schnorr_sign_t* sg,
					const void* dat_digest,
					unsigned dat_digestlen,
					const schnorr_pbkey_t* pk,
					const sphere_t* sph);
/*------------------------------------*/
USE_RESULT__
bool_t schnorr_vrfy_dat(unsigned which_sha,
						const schnorr_sign_t* sg,
						const void* dat,
						unsigned datlen,
						const schnorr_pbkey_t* pk,
						const sphere_t* sph);
/*----------------------------------------------------------------------------*/
void schnorr_pbkey_t_ctor(struct schnorr_pbkey_t* p);
void schnorr_pbkey_t_dtor(struct schnorr_pbkey_t* p);
void schnorr_pbkey_t_asg(struct schnorr_pbkey_t* p, const struct schnorr_pbkey_t* o);
void schnorr_pbkey_t_move(struct schnorr_pbkey_t* p, struct schnorr_pbkey_t* o);
struct schnorr_pbkey_t* schnorr_pbkey_t_new();
struct schnorr_pbkey_t* schnorr_pbkey_t_clone(const struct schnorr_pbkey_t* o);
void schnorr_pbkey_t_delete(struct schnorr_pbkey_t* p);
/*------------------------------------*/
void schnorr_prkey_t_ctor(struct schnorr_prkey_t* p);
void schnorr_prkey_t_dtor(struct schnorr_prkey_t* p);
void schnorr_prkey_t_asg(struct schnorr_prkey_t* p, const struct schnorr_prkey_t* o);
void schnorr_prkey_t_move(struct schnorr_prkey_t* p, struct schnorr_prkey_t* o);
struct schnorr_prkey_t* schnorr_prkey_t_new();
struct schnorr_prkey_t* schnorr_prkey_t_clone(const struct schnorr_prkey_t* o);
void schnorr_prkey_t_delete(struct schnorr_prkey_t* p);
/*------------------------------------*/
void schnorr_sign_t_ctor(struct schnorr_sign_t* p);
void schnorr_sign_t_dtor(struct schnorr_sign_t* p);
void schnorr_sign_t_asg(struct schnorr_sign_t* p, const struct schnorr_sign_t* o);
void schnorr_sign_t_move(struct schnorr_sign_t* p, struct schnorr_sign_t* o);
struct schnorr_sign_t* schnorr_sign_t_new();
struct schnorr_sign_t* schnorr_sign_t_clone(const struct schnorr_sign_t* o);
void schnorr_sign_t_delete(struct schnorr_sign_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

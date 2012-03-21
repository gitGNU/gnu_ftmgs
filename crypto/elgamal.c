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
#include "elgamal.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "syspar.h"
#include "iexport.h"
#include "generator.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
void elgamal_create_keys(elgamal_pbkey_t* pk,
						 elgamal_prkey_t* sk,
						 elgamal_modfact_t* npq,
						 elgamal_modord_t* p1q1,
						 const sphere_t* sph,
						 unsigned nu,
						 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1(syspar_t, sp);
	BEG_VAR_5_bigint_t(p, p1, q, q1, order);
	/*--------------------------------*/
	set_syspar(&sp, nu);
	/*--------------------------------*/
	bi_random_sgprime_nb(p, p1, sp.nu/2, DEF_NTESTS, rnd_ctx);
	do {
		bi_random_sgprime_nb(q, q1, sp.nu/2, DEF_NTESTS, rnd_ctx);
	} while ( bi_equals(p, q) );
	bi_mul(pk->n, p, q);
	bi_mul(order, p1, q1);
	/*--------------------------------*/
	qrn_generator(pk->g, pk->n, order, p1, q1, rnd_ctx);
	/*--------------------------------*/
	do {
		bi_random_sph_ek(sk->x, sph, rnd_ctx);
	} while (bi_iszero(sk->x));
	bi_powmod_sec(pk->y, pk->g, sk->x, pk->n);
	/*--------------------------------*/
	if (npq != NULL) {
		bi_swap(npq->p , p);
		bi_swap(npq->q , q);
	}
	if (p1q1 != NULL) {
		bi_swap(p1q1->p1 , p1);
		bi_swap(p1q1->q1 , q1);
	}
	/*--------------------------------*/
	END_VAR_5_bigint_t(p, p1, q, q1, order);
	END_VAR_1(syspar_t, sp);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_encrypt_k(elgamal_ciphertext_t* ct,
					   bigint_t k,
					   const bigint_t msg,
					   const elgamal_pbkey_t* pk,
					   const sphere_t* sph,
					   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	assert( ! bi_iszero(msg) && bi_less(msg, pk->n));
	/*--------------------------------*/
	do {
		bi_random_sph_ek(k, sph, rnd_ctx);
	} while (bi_iszero(k));
	/*--------------------------------*/
	bi_powmod_sec(ct->alpha, pk->g, k, pk->n);
	bi_powmod_sec(ct->beta, pk->y, k, pk->n);
	bi_mulmod(ct->beta, ct->beta, msg, pk->n);
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_encrypt(elgamal_ciphertext_t* ct,
					 const bigint_t msg,
					 const elgamal_pbkey_t* pk,
					 const sphere_t* sph,
					 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(k);
	elgamal_encrypt_k(ct, k, msg, pk, sph, rnd_ctx);
	END_VAR_1_bigint_t(k);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void elgamal_decrypt(bigint_t msg,
					 const elgamal_ciphertext_t* ct,
					 const elgamal_pbkey_t* pk,
					 const elgamal_prkey_t* sk)
{
	BEG_BIGINT_CHK();
	bi_powmod_sec(msg, ct->alpha, sk->x, pk->n);
	bi_invmod(msg, msg, pk->n);
	bi_mulmod(msg, msg, ct->beta, pk->n);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_modfact_t_chk_members(struct elgamal_modfact_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_elgamal_modfact_t {
		bigint_t p;
		bigint_t q;
	};
	CHK_FIELD__(dummy_elgamal_modfact_t, elgamal_modfact_t, p);
	CHK_FIELD__(dummy_elgamal_modfact_t, elgamal_modfact_t, q);
	CHK_SIZE__(dummy_elgamal_modfact_t, elgamal_modfact_t);
	return (p!=NULL)&&(code == 178844723);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_modfact_t_ctor(struct elgamal_modfact_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_modfact_t_chk_members(p,178844723));
	bi_ctor(p->p);
	bi_ctor(p->q);
}
/*----------------------------------------------------------------------------*/
void elgamal_modfact_t_dtor(struct elgamal_modfact_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_modfact_t_chk_members(p,178844723));
	bi_dtor(p->q);
	bi_dtor(p->p);
}
/*----------------------------------------------------------------------------*/
void elgamal_modfact_t_asg(struct elgamal_modfact_t* p, const struct elgamal_modfact_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_modfact_t_chk_members(p,178844723));
	if (p != o) {
		bi_asg(p->p, o->p);
		bi_asg(p->q, o->q);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_modfact_t* elgamal_modfact_t_new()/*auto*/
{
	struct elgamal_modfact_t* p = (struct elgamal_modfact_t*)malloc(sizeof(struct elgamal_modfact_t));
	if (p != NULL) {
		elgamal_modfact_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_modfact_t* elgamal_modfact_t_clone(const struct elgamal_modfact_t* o)/*auto*/
{
	struct elgamal_modfact_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_modfact_t*)malloc(sizeof(struct elgamal_modfact_t));
		if (p != NULL) {
			elgamal_modfact_t_ctor(p);
			elgamal_modfact_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_modfact_t_delete(struct elgamal_modfact_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_modfact_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_modord_t_chk_members(struct elgamal_modord_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_elgamal_modord_t {
		bigint_t p1;
		bigint_t q1;
	};
	CHK_FIELD__(dummy_elgamal_modord_t, elgamal_modord_t, p1);
	CHK_FIELD__(dummy_elgamal_modord_t, elgamal_modord_t, q1);
	CHK_SIZE__(dummy_elgamal_modord_t, elgamal_modord_t);
	return (p!=NULL)&&(code == 407299759);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_modord_t_ctor(struct elgamal_modord_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_modord_t_chk_members(p,407299759));
	bi_ctor(p->p1);
	bi_ctor(p->q1);
}
/*----------------------------------------------------------------------------*/
void elgamal_modord_t_dtor(struct elgamal_modord_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_modord_t_chk_members(p,407299759));
	bi_dtor(p->q1);
	bi_dtor(p->p1);
}
/*----------------------------------------------------------------------------*/
void elgamal_modord_t_asg(struct elgamal_modord_t* p, const struct elgamal_modord_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_modord_t_chk_members(p,407299759));
	if (p != o) {
		bi_asg(p->p1, o->p1);
		bi_asg(p->q1, o->q1);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_modord_t* elgamal_modord_t_new()/*auto*/
{
	struct elgamal_modord_t* p = (struct elgamal_modord_t*)malloc(sizeof(struct elgamal_modord_t));
	if (p != NULL) {
		elgamal_modord_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_modord_t* elgamal_modord_t_clone(const struct elgamal_modord_t* o)/*auto*/
{
	struct elgamal_modord_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_modord_t*)malloc(sizeof(struct elgamal_modord_t));
		if (p != NULL) {
			elgamal_modord_t_ctor(p);
			elgamal_modord_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_modord_t_delete(struct elgamal_modord_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_modord_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_pbkey_t_chk_members(struct elgamal_pbkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_elgamal_pbkey_t {
		bigint_t n;
		bigint_t g;
		bigint_t y;			/* y = g^x (mod n) */
	};
	CHK_FIELD__(dummy_elgamal_pbkey_t, elgamal_pbkey_t, n);
	CHK_FIELD__(dummy_elgamal_pbkey_t, elgamal_pbkey_t, g);
	CHK_FIELD__(dummy_elgamal_pbkey_t, elgamal_pbkey_t, y);
	CHK_SIZE__(dummy_elgamal_pbkey_t, elgamal_pbkey_t);
	return (p!=NULL)&&(code == 33279281);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_pbkey_t_ctor(struct elgamal_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_pbkey_t_chk_members(p,33279281));
	bi_ctor(p->n);
	bi_ctor(p->g);
	bi_ctor(p->y);
}
/*----------------------------------------------------------------------------*/
void elgamal_pbkey_t_dtor(struct elgamal_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_pbkey_t_chk_members(p,33279281));
	bi_dtor(p->y);
	bi_dtor(p->g);
	bi_dtor(p->n);
}
/*----------------------------------------------------------------------------*/
void elgamal_pbkey_t_asg(struct elgamal_pbkey_t* p, const struct elgamal_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_pbkey_t_chk_members(p,33279281));
	if (p != o) {
		bi_asg(p->n, o->n);
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_pbkey_t* elgamal_pbkey_t_new()/*auto*/
{
	struct elgamal_pbkey_t* p = (struct elgamal_pbkey_t*)malloc(sizeof(struct elgamal_pbkey_t));
	if (p != NULL) {
		elgamal_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_pbkey_t* elgamal_pbkey_t_clone(const struct elgamal_pbkey_t* o)/*auto*/
{
	struct elgamal_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_pbkey_t*)malloc(sizeof(struct elgamal_pbkey_t));
		if (p != NULL) {
			elgamal_pbkey_t_ctor(p);
			elgamal_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_pbkey_t_delete(struct elgamal_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_prkey_t_chk_members(struct elgamal_prkey_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_elgamal_prkey_t {
		bigint_t x;
	};
	CHK_FIELD__(dummy_elgamal_prkey_t, elgamal_prkey_t, x);
	CHK_SIZE__(dummy_elgamal_prkey_t, elgamal_prkey_t);
	return (p!=NULL)&&(code == 411837210);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_prkey_t_ctor(struct elgamal_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_prkey_t_chk_members(p,411837210));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void elgamal_prkey_t_dtor(struct elgamal_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_prkey_t_chk_members(p,411837210));
	bi_dtor(p->x);
}
/*----------------------------------------------------------------------------*/
void elgamal_prkey_t_asg(struct elgamal_prkey_t* p, const struct elgamal_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_prkey_t_chk_members(p,411837210));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_prkey_t* elgamal_prkey_t_new()/*auto*/
{
	struct elgamal_prkey_t* p = (struct elgamal_prkey_t*)malloc(sizeof(struct elgamal_prkey_t));
	if (p != NULL) {
		elgamal_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_prkey_t* elgamal_prkey_t_clone(const struct elgamal_prkey_t* o)/*auto*/
{
	struct elgamal_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_prkey_t*)malloc(sizeof(struct elgamal_prkey_t));
		if (p != NULL) {
			elgamal_prkey_t_ctor(p);
			elgamal_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_prkey_t_delete(struct elgamal_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int elgamal_ciphertext_t_chk_members(struct elgamal_ciphertext_t* p, int code)/*auto*/
{
#define STATIC_ASSERT__(Expr__,Msg__) \
	extern int (*StAssert__())[!!sizeof(struct{ unsigned Msg__:(Expr__)?1:-1;})]
#define CHK_FIELD__(Type1,Type2,Field) \
	STATIC_ASSERT__((&((struct Type1*)0)->Field==&((struct Type2*)0)->Field), \
					Field_does_not_match__)
#define CHK_SIZE__(Type1,Type2) \
	STATIC_ASSERT__((sizeof(struct Type1)==sizeof(struct Type2)), \
					Size_does_not_match__)
	/* It doesn't catch new members defined inside padding holes */
	/* Compile-time member's address comparison checks offset and type */
	/* Code number checks that functions refer to the same definition */
	struct dummy_elgamal_ciphertext_t {
		bigint_t alpha;
		bigint_t beta;
	};
	CHK_FIELD__(dummy_elgamal_ciphertext_t, elgamal_ciphertext_t, alpha);
	CHK_FIELD__(dummy_elgamal_ciphertext_t, elgamal_ciphertext_t, beta);
	CHK_SIZE__(dummy_elgamal_ciphertext_t, elgamal_ciphertext_t);
	return (p!=NULL)&&(code == 211691810);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void elgamal_ciphertext_t_ctor(struct elgamal_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_ciphertext_t_chk_members(p,211691810));
	bi_ctor(p->alpha);
	bi_ctor(p->beta);
}
/*----------------------------------------------------------------------------*/
void elgamal_ciphertext_t_dtor(struct elgamal_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(elgamal_ciphertext_t_chk_members(p,211691810));
	bi_dtor(p->beta);
	bi_dtor(p->alpha);
}
/*----------------------------------------------------------------------------*/
void elgamal_ciphertext_t_asg(struct elgamal_ciphertext_t* p, const struct elgamal_ciphertext_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(elgamal_ciphertext_t_chk_members(p,211691810));
	if (p != o) {
		bi_asg(p->alpha, o->alpha);
		bi_asg(p->beta, o->beta);
	}
}
/*----------------------------------------------------------------------------*/
struct elgamal_ciphertext_t* elgamal_ciphertext_t_new()/*auto*/
{
	struct elgamal_ciphertext_t* p = (struct elgamal_ciphertext_t*)malloc(sizeof(struct elgamal_ciphertext_t));
	if (p != NULL) {
		elgamal_ciphertext_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct elgamal_ciphertext_t* elgamal_ciphertext_t_clone(const struct elgamal_ciphertext_t* o)/*auto*/
{
	struct elgamal_ciphertext_t* p = NULL;
	if (o != NULL) {
		p = (struct elgamal_ciphertext_t*)malloc(sizeof(struct elgamal_ciphertext_t));
		if (p != NULL) {
			elgamal_ciphertext_t_ctor(p);
			elgamal_ciphertext_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void elgamal_ciphertext_t_delete(struct elgamal_ciphertext_t* p)/*auto*/
{
	if (p != NULL) {
		elgamal_ciphertext_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

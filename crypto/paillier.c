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
#include "paillier.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "iexport.h"
#include "syspar.h"
#include "generator.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
void paillier_create_keys(paillier_pbkey_t* pk,
						  paillier_prkey_t* sk,
						  unsigned nu,
						  rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1(syspar_t, sp);
	BEG_VAR_7_bigint_t(p, p1, q, q1, n2, g1, n24);
	/*--------------------------------*/
	set_syspar(&sp, nu);
	/*--------------------------------*/
	bi_random_sgprime_nb(p, p1, sp.nu/2, DEF_NTESTS, rnd_ctx);
	do {
		bi_random_sgprime_nb(q, q1, sp.nu/2, DEF_NTESTS, rnd_ctx);
	} while ( bi_equals(p, q) );
	bi_mul(pk->n, p, q);
	/*--------------------------------*/
	bi_sq(n2, pk->n);
	bi_random_ub(g1, n2, rnd_ctx);
	bi_sqmod(pk->g, g1, n2);
	bi_powmod_sec(pk->g, pk->g, pk->n, n2);
	/*--------------------------------*/
	bi_div2k(n24, n2, 2);	/* (n^2)/4 */
	do {
		bi_random_ub(sk->x, n24, rnd_ctx);	/* 1 <= x < (n^2)/4 */
	} while (bi_iszero(sk->x));
	bi_powmod_sec(pk->y, pk->g, sk->x, n2);
	/*--------------------------------*/
	END_VAR_7_bigint_t(p, p1, q, q1, n2, g1, n24);
	END_VAR_1(syspar_t, sp);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_encrypt_k(paillier_ciphertext_t* ct,
					   bigint_t k,
					   const bigint_t msg,
					   const paillier_pbkey_t* pk,
					   rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_3_bigint_t(n2, n4, h);
	assert( ! bi_iszero(msg) && bi_less(msg, pk->n));
	/*--------------------------------*/
	bi_sq(n2, pk->n);			/* n2 = n^2 */
	bi_div2k(n4, pk->n, 2);		/* n4 = n/4 */
	bi_add_ui(h, pk->n, 1);
	bi_powmod_sec(h, h, msg, n2);	/* h = (n+1)^msg (mod n^2) */
	do {
		bi_random_ub(k, n4, rnd_ctx);
	} while (bi_iszero(k));
	/*--------------------------------*/
	bi_powmod_sec(ct->alpha, pk->g, k, n2);
	bi_powmod_sec(ct->beta, pk->y, k, n2);
	bi_mulmod(ct->beta, ct->beta, h, n2);
	/*--------------------------------*/
	END_VAR_3_bigint_t(n2, n4, h);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void paillier_encrypt(paillier_ciphertext_t* ct,
					 const bigint_t msg,
					 const paillier_pbkey_t* pk,
					 rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(k);
	paillier_encrypt_k(ct, k, msg, pk, rnd_ctx);
	END_VAR_1_bigint_t(k);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
bool_t paillier_decrypt(bigint_t msg, const paillier_ciphertext_t* ct,
						const paillier_pbkey_t* pk, const paillier_prkey_t* sk)
{
	bool_t ok = TRUE;
	BEG_BIGINT_CHK();
	BEG_VAR_3_bigint_t(n2, t, rem);
	bi_sq(n2, pk->n);			/* n2 = n^2 */
	/*--------------------------------*/
	bi_powmod_sec(msg, ct->alpha, sk->x, n2);
	bi_invmod(msg, msg, n2);
	bi_mulmod(msg, msg, ct->beta, n2);
	/*--------------------------------*/
	bi_asg_ui(t, 2);
	bi_invmod(t, t, pk->n);
	bi_sqmod(msg, msg, n2);
	bi_powmod_sec(msg, msg, t, n2);
	bi_sub_ui(msg, msg, 1);
	bi_div(msg, msg, pk->n, rem);
	if (! bi_iszero(rem) ) {
		bi_asg_ui(msg, 0);
		ok = FALSE;
	}
	/*--------------------------------*/
	END_VAR_3_bigint_t(n2, t, rem);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_pbkey_t_chk_members(struct paillier_pbkey_t* p, int code)/*auto*/
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
	struct dummy_paillier_pbkey_t {
		bigint_t n;
		bigint_t g;
		bigint_t y;			/* y = g^x (mod n^2) */
	};
	CHK_FIELD__(dummy_paillier_pbkey_t, paillier_pbkey_t, n);
	CHK_FIELD__(dummy_paillier_pbkey_t, paillier_pbkey_t, g);
	CHK_FIELD__(dummy_paillier_pbkey_t, paillier_pbkey_t, y);
	CHK_SIZE__(dummy_paillier_pbkey_t, paillier_pbkey_t);
	return (p!=NULL)&&(code == 26604317);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_pbkey_t_ctor(struct paillier_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_pbkey_t_chk_members(p,26604317));
	bi_ctor(p->n);
	bi_ctor(p->g);
	bi_ctor(p->y);
}
/*----------------------------------------------------------------------------*/
void paillier_pbkey_t_dtor(struct paillier_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_pbkey_t_chk_members(p,26604317));
	bi_dtor(p->y);
	bi_dtor(p->g);
	bi_dtor(p->n);
}
/*----------------------------------------------------------------------------*/
void paillier_pbkey_t_asg(struct paillier_pbkey_t* p, const struct paillier_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_pbkey_t_chk_members(p,26604317));
	if (p != o) {
		bi_asg(p->n, o->n);
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_pbkey_t* paillier_pbkey_t_new()/*auto*/
{
	struct paillier_pbkey_t* p = (struct paillier_pbkey_t*)malloc(sizeof(struct paillier_pbkey_t));
	if (p != NULL) {
		paillier_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_pbkey_t* paillier_pbkey_t_clone(const struct paillier_pbkey_t* o)/*auto*/
{
	struct paillier_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_pbkey_t*)malloc(sizeof(struct paillier_pbkey_t));
		if (p != NULL) {
			paillier_pbkey_t_ctor(p);
			paillier_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_pbkey_t_delete(struct paillier_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_prkey_t_chk_members(struct paillier_prkey_t* p, int code)/*auto*/
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
	struct dummy_paillier_prkey_t {
		bigint_t x;
	};
	CHK_FIELD__(dummy_paillier_prkey_t, paillier_prkey_t, x);
	CHK_SIZE__(dummy_paillier_prkey_t, paillier_prkey_t);
	return (p!=NULL)&&(code == 411837210);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_prkey_t_ctor(struct paillier_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_prkey_t_chk_members(p,411837210));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void paillier_prkey_t_dtor(struct paillier_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_prkey_t_chk_members(p,411837210));
	bi_dtor(p->x);
}
/*----------------------------------------------------------------------------*/
void paillier_prkey_t_asg(struct paillier_prkey_t* p, const struct paillier_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_prkey_t_chk_members(p,411837210));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_prkey_t* paillier_prkey_t_new()/*auto*/
{
	struct paillier_prkey_t* p = (struct paillier_prkey_t*)malloc(sizeof(struct paillier_prkey_t));
	if (p != NULL) {
		paillier_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_prkey_t* paillier_prkey_t_clone(const struct paillier_prkey_t* o)/*auto*/
{
	struct paillier_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_prkey_t*)malloc(sizeof(struct paillier_prkey_t));
		if (p != NULL) {
			paillier_prkey_t_ctor(p);
			paillier_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_prkey_t_delete(struct paillier_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int paillier_ciphertext_t_chk_members(struct paillier_ciphertext_t* p, int code)/*auto*/
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
	struct dummy_paillier_ciphertext_t {
		bigint_t alpha;
		bigint_t beta;
	};
	CHK_FIELD__(dummy_paillier_ciphertext_t, paillier_ciphertext_t, alpha);
	CHK_FIELD__(dummy_paillier_ciphertext_t, paillier_ciphertext_t, beta);
	CHK_SIZE__(dummy_paillier_ciphertext_t, paillier_ciphertext_t);
	return (p!=NULL)&&(code == 211691810);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void paillier_ciphertext_t_ctor(struct paillier_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_ciphertext_t_chk_members(p,211691810));
	bi_ctor(p->alpha);
	bi_ctor(p->beta);
}
/*----------------------------------------------------------------------------*/
void paillier_ciphertext_t_dtor(struct paillier_ciphertext_t* p)/*auto*/
{
	assert(p != NULL);
	assert(paillier_ciphertext_t_chk_members(p,211691810));
	bi_dtor(p->beta);
	bi_dtor(p->alpha);
}
/*----------------------------------------------------------------------------*/
void paillier_ciphertext_t_asg(struct paillier_ciphertext_t* p, const struct paillier_ciphertext_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(paillier_ciphertext_t_chk_members(p,211691810));
	if (p != o) {
		bi_asg(p->alpha, o->alpha);
		bi_asg(p->beta, o->beta);
	}
}
/*----------------------------------------------------------------------------*/
struct paillier_ciphertext_t* paillier_ciphertext_t_new()/*auto*/
{
	struct paillier_ciphertext_t* p = (struct paillier_ciphertext_t*)malloc(sizeof(struct paillier_ciphertext_t));
	if (p != NULL) {
		paillier_ciphertext_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct paillier_ciphertext_t* paillier_ciphertext_t_clone(const struct paillier_ciphertext_t* o)/*auto*/
{
	struct paillier_ciphertext_t* p = NULL;
	if (o != NULL) {
		p = (struct paillier_ciphertext_t*)malloc(sizeof(struct paillier_ciphertext_t));
		if (p != NULL) {
			paillier_ciphertext_t_ctor(p);
			paillier_ciphertext_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void paillier_ciphertext_t_delete(struct paillier_ciphertext_t* p)/*auto*/
{
	if (p != NULL) {
		paillier_ciphertext_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

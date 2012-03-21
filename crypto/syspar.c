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
#include "syspar.h"
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
/*-----------------------------------------------------------------*/
/*
 * 2.Preliminaries: nu is the nbits of the order of the group QR(n)
 * nu = ceil(log2(p1*q1))
 * 5.DLRAP: for simplicity, it is assumed that nu is divisible by 4
 * EPSILON > 1
 * k is a security parameter for DLRS ZK proofs: sec > 1/(2^k)
 * lemma_9: (EPSILON-1)*(mu+k) > log2_k | mu = nu/4-1
 */
/*-----------------------------------------------------------------*/
/* 
 * In this code, nu represents the number of bits of the modulus n
 * for QR(n), such that the nbits(p1q1) equals nbits(n)-2, but
 * as it is supposed it is divisible by 4, it is rounded-up and
 * then both are considered the same !!!!!!!!
 */
/*-----------------------------------------------------------------*/
#define	EPSILON			1.05
/*----------------------------------*/
enum security_par_k_t {
	K_1 = 128, K_2 = 256, K_3 = 384
};
/* 
 * K = NU / 4
 */
/*-----------------------------------------------------------------*/
static bool_t is_valid(unsigned nu, unsigned k)
{
	unsigned kk = k;
	unsigned nbits_k = 0;
	while (kk != 0) {
		++nbits_k;
		kk >>= 1;
	}
	/* mu = (nu / 4) - 1; */
	return ((k%CHAR_BIT==0) && (nu%CHAR_BIT==0)
			&& (nu%4==0) && (((EPSILON-1)*(nu/4-1+k)) > nbits_k));
}
/*-----------------------------------------------------------------*/
static bool_t set_syspar_val(syspar_t* sp, unsigned nu, unsigned k)
{
	bool_t ok;
	if (is_valid(nu, k)) {
		sp->nu = nu;
		sp->k = k;
		ok = TRUE;
	} else {
		set_default_syspar(sp);
		ok = FALSE;
	}
	return ok;
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
void set_default_syspar(syspar_t* sp)
{
	sp->nu = NU_1;
	sp->k = K_1;
}
/*-----------------------------------------------------------------*/
bool_t set_syspar(syspar_t* sp, unsigned nu)
{
	bool_t ok;
	switch (nu) {
	case NU_1:
		ok = set_syspar_val(sp, NU_1, K_1);
		break;
	case NU_2:
		ok = set_syspar_val(sp, NU_2, K_2);
		break;
	case NU_3:
		ok = set_syspar_val(sp, NU_3, K_3);
		break;
	default:
		if (nu < NU_1) {
			set_syspar_val(sp, NU_1, K_1);
		} else if (nu < NU_2) {
			set_syspar_val(sp, NU_2, K_2);
		} else {
			set_syspar_val(sp, NU_3, K_3);
		}
		ok = FALSE;
	}
	return ok;
}
/*-----------------------------------------------------------------*/
bool_t get_syspar(syspar_t* sp, unsigned idx)
{
	bool_t ok;
	switch (idx) {
	case 0:
		ok = set_syspar_val(sp, NU_1, K_1);
		break;
	case 1:
		ok = set_syspar_val(sp, NU_2, K_2);
		break;
	case 2:
		ok = set_syspar_val(sp, NU_3, K_3);
		break;
	default:
		set_syspar_val(sp, NU_1, K_1);
		ok = FALSE;
	}
	return ok;
}
/*-----------------------------------------------------------------*/
unsigned radius_ek(const syspar_t* sp, unsigned nbits)
{
	unsigned nbits_ek = (unsigned)/*trunc*/(((nbits - 1)/EPSILON) - sp->k);
#ifndef NDEBUG
	if (nbits != (unsigned)ceil(EPSILON * (nbits_ek + sp->k) + 1) ) {
		fprintf(stderr, "Nbits: %d, NBitsEk: %d\n",
				nbits, nbits_ek);
	}
#endif
	assert(nbits == (unsigned)ceil(EPSILON * (nbits_ek + sp->k) + 1) );
	return nbits_ek;
}
/*-----------------------------------------------------------------*/
unsigned radius(const syspar_t* sp, unsigned nbits_ek)
{
	unsigned nbits = (unsigned)ceil(EPSILON * (nbits_ek + sp->k) + 1);
#ifndef NDEBUG
	if (nbits_ek != (unsigned)/*trunc*/(((nbits - 1)/EPSILON) - sp->k) ) {
		fprintf(stderr, "NbitsEk: %d, NBits: %d\n",
				nbits_ek, nbits);
	}
#endif
	assert(nbits_ek == (unsigned)/*trunc*/(((nbits - 1)/EPSILON) - sp->k) );
	return nbits;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int syspar_t_chk_members(struct syspar_t* p, int code)/*auto*/
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
	struct dummy_syspar_t {
		unsigned nu;	/* security parameter: nbits of modulus */
		unsigned k;		/* security parameter ZK proofs 		*/
	};
	CHK_FIELD__(dummy_syspar_t, syspar_t, nu);
	CHK_FIELD__(dummy_syspar_t, syspar_t, k);
	CHK_SIZE__(dummy_syspar_t, syspar_t);
	return (p!=NULL)&&(code == 221546803);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*-----------------------------------------------------------------*/
void syspar_t_ctor(struct syspar_t* p)/*mod*/
{
	assert(p != NULL);
	assert(syspar_t_chk_members(p,221546803));
	p->nu = NU_1;
	p->k = K_1;
}
/*void syspar_t_dtor(struct syspar_t* p)/ *noauto*/
/*void syspar_t_asg(struct syspar_t* p, const struct syspar_t* o)/ *noauto*/
/*-----------------------------------------------------------------*/
void syspar_t_swap(struct syspar_t* p1, struct syspar_t* p2)
{
	unsigned aux;
	assert(syspar_t_chk_members(p1,221546803));
	aux = p1->nu;
	p1->nu = p2->nu;
	p2->nu = aux;
	aux = p1->k;
	p1->k = p2->k;
	p2->k = aux;
}
/*----------------------------------------------------------------------------*/
struct syspar_t* syspar_t_new()/*auto*/
{
	struct syspar_t* p = (struct syspar_t*)malloc(sizeof(struct syspar_t));
	if (p != NULL) {
		syspar_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct syspar_t* syspar_t_clone(const struct syspar_t* o)/*auto*/
{
	struct syspar_t* p = NULL;
	if (o != NULL) {
		p = (struct syspar_t*)malloc(sizeof(struct syspar_t));
		if (p != NULL) {
			syspar_t_ctor(p);
			syspar_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void syspar_t_delete(struct syspar_t* p)/*auto*/
{
	if (p != NULL) {
		syspar_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

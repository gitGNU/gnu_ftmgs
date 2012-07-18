/*--------------------------------------------------------------------------- */
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
/*--------------------------------------------------------------------------- */
#include "syssph.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
static bool_t nbits_in_range(unsigned nbx, unsigned nb)
{
	return (nbx >= nb)&&(nbx <= nb+1);
}
/*----------------------------------------------------------------------------*/
static unsigned sp2idx(const syspar_t* sp)
{
	unsigned i;
	switch (sp->nu) {
	case NU_1:	i = 0;	break;
	case NU_2:	i = 1;	break;
	case NU_3:	i = 2;	break;
	default:	i = 0;	break;
	}
	return i;
}
/*----------------------------------------------------------------------------*/
static void create_sphere_lambda_sp(sphere_t lambda_sp[MAX_SYSPAR],
									const syspar_t* curr_sp)
{
	syspar_t spxi;
	unsigned i;
	syspar_t_ctor(&spxi);
	for (i = 0; get_syspar(&spxi, i); ++i) {
		create_sph_lambda_sp(&lambda_sp[i], &spxi, curr_sp);
	}
	assert(i == MAX_SYSPAR);
	syspar_t_dtor(&spxi);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------- */
void create_syssph(syssph_t* p, const syspar_t* sp)
{
	/* create_sph_lambda(&p->lambda, sp); */
	create_sphere_lambda_sp(p->lambda_sp, sp);
	create_sph_lambda_dsa(&p->lambda_dsa, sp);
	create_sph_mu(&p->mu, sp);
	create_sph_gamma(&p->gamma, sp);
	create_sph_gamma_mu(&p->gamma_mu, &p->gamma, &p->mu, sp);
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_lambda_x(const bigint_t x, const syssph_t* ss)
{
	unsigned i = 0;
	const sphere_t* res = NULL;
	while ((i < MAX_SYSPAR)
		   && ! nbits_in_range(bi_nbits(x)+1,
							   bi_nbits(ss->lambda_sp[i].center))) {
		assert( ! bi_belongs_sph_ek(x, &ss->lambda_sp[i]));
		++i;
	}
	if (i < MAX_SYSPAR) {
		res = &ss->lambda_sp[i];
	} else {
		res = &ss->lambda_dsa;
	}
	assert(bi_belongs_sph_ek(x, res));
	return res;
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_lambda_sx(const bigint_t sx, const syssph_t* ss)
{
	unsigned i = 0;
	const sphere_t* res = NULL;
	while ((i < MAX_SYSPAR)
		   && ! nbits_in_range(bi_nbits(sx),
							   ss->lambda_sp[i].log2radius) ) {
		++i;
	}
	if (i < MAX_SYSPAR) {
		res = &ss->lambda_sp[i];
	} else {
		res = &ss->lambda_dsa;
	}
	assert(nbits_in_range(bi_nbits(sx), res->log2radius));
	return res;
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_lambda(const syspar_t* sp, const syssph_t* ss)
{
	return &ss->lambda_sp[sp2idx(sp)];
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_mu(const syssph_t* ss)
{
	return &ss->mu;
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_gamma(const syssph_t* ss)
{
	return &ss->gamma;
}
/*----------------------------------------------------------------------------*/
const sphere_t* get_sphere_gamma_mu(const syssph_t* ss)
{
	return &ss->gamma_mu;
}
/*--------------------------------------------------------------------------- */
/*--------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int syssph_t_chk_members(const struct syssph_t* p, int code)/*auto*/
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
	struct dummy_syssph_t {
		sphere_t lambda_sp[MAX_SYSPAR];
		sphere_t lambda_dsa;
		sphere_t mu;
		sphere_t gamma;
		sphere_t gamma_mu;
	};
	CHK_FIELD__(dummy_syssph_t, syssph_t, lambda_sp);
	CHK_FIELD__(dummy_syssph_t, syssph_t, lambda_dsa);
	CHK_FIELD__(dummy_syssph_t, syssph_t, mu);
	CHK_FIELD__(dummy_syssph_t, syssph_t, gamma);
	CHK_FIELD__(dummy_syssph_t, syssph_t, gamma_mu);
	CHK_SIZE__(dummy_syssph_t, syssph_t);
	return (code == 299838451); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void syssph_t_ctor(struct syssph_t* p)/*auto*/
{
	int i; (void)i;
	assert(p != NULL);
	assert(syssph_t_chk_members(p,299838451));
	for (i=0; i < MAX_SYSPAR; ++i) {
		sphere_t_ctor(&p->lambda_sp[i]);
	}
	sphere_t_ctor(&p->lambda_dsa);
	sphere_t_ctor(&p->mu);
	sphere_t_ctor(&p->gamma);
	sphere_t_ctor(&p->gamma_mu);
}
/*----------------------------------------------------------------------------*/
void syssph_t_dtor(struct syssph_t* p)/*auto*/
{
	int i; (void)i;
	assert(p != NULL);
	assert(syssph_t_chk_members(p,299838451));
	sphere_t_dtor(&p->gamma_mu);
	sphere_t_dtor(&p->gamma);
	sphere_t_dtor(&p->mu);
	sphere_t_dtor(&p->lambda_dsa);
	for (i=0; i < MAX_SYSPAR; ++i) {
		sphere_t_dtor(&p->lambda_sp[i]);
	}
	(void)p;
}
/*----------------------------------------------------------------------------*/
void syssph_t_asg(struct syssph_t* p, const struct syssph_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(syssph_t_chk_members(p,299838451));
	if (p != o) {
		int i; (void)i;
		for (i=0; i < MAX_SYSPAR; ++i) {
			sphere_t_asg(&p->lambda_sp[i], &o->lambda_sp[i]);
		}
		sphere_t_asg(&p->lambda_dsa, &o->lambda_dsa);
		sphere_t_asg(&p->mu, &o->mu);
		sphere_t_asg(&p->gamma, &o->gamma);
		sphere_t_asg(&p->gamma_mu, &o->gamma_mu);
	}
}
/*----------------------------------------------------------------------------*/
void syssph_t_move(struct syssph_t* p, struct syssph_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(syssph_t_chk_members(p,299838451));
	if (p != o) {
		int i; (void)i;
		for (i=0; i < MAX_SYSPAR; ++i) {
			sphere_t_move(&p->lambda_sp[i], &o->lambda_sp[i]);
		}
		sphere_t_move(&p->lambda_dsa, &o->lambda_dsa);
		sphere_t_move(&p->mu, &o->mu);
		sphere_t_move(&p->gamma, &o->gamma);
		sphere_t_move(&p->gamma_mu, &o->gamma_mu);
	}
}
/*----------------------------------------------------------------------------*/
struct syssph_t* syssph_t_new()/*auto*/
{
	struct syssph_t* p = (struct syssph_t*)malloc(sizeof(struct syssph_t));
	if (p != NULL) {
		syssph_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct syssph_t* syssph_t_clone(const struct syssph_t* o)/*auto*/
{
	struct syssph_t* p = NULL;
	if (o != NULL) {
		p = (struct syssph_t*)malloc(sizeof(struct syssph_t));
		if (p != NULL) {
			syssph_t_ctor(p);
			syssph_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void syssph_t_delete(struct syssph_t* p)/*auto*/
{
	if (p != NULL) {
		syssph_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

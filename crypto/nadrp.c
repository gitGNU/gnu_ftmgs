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
#include "nadrp.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "bigint.h"
#include "random.h"
#include "birnd.h"
#include "syspar.h"
#include "sphere.h"
#include "sok.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
#define array_sz(array__) (sizeof(array__)/sizeof(*(array__)))
/*----------------------------------------------------------------------------*/
/*-- NON-ADAPTIVE DRAWING OF RANDOM POWERS -----------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 * -----------------------------------
 * SK{(x, r): C1 = g^x h^r ; C2 = y^r }
 * -----------------------------------
 * Vbles	{ x, r }
 * Spheres	[0..m-1] [0..n^2-1]
 * -----------------------------------
 * Objs		{ C1, C2, g, h, y } | MOD
 * -----------------------------------
 * Rels		{ -1,  0, x, r, 0 } | n
 *			{  0, -1, 0, 0, r } | n
 * -----------------------------------
 */
#define X	set_vble_idx(0)
#define R	set_vble_idx(1)
#define NADRP_SK1_NVBLES	2
#define NADRP_SK1_NOBJS		5
#define NADRP_SK1_NRELS		2
static const sok_relset_t NADRP_SK1 = {
	NADRP_SK1_NVBLES,	/* NVbles */
	NADRP_SK1_NOBJS,	/* NObjs */
	NADRP_SK1_NRELS,	/* NRels */
	{
		{ -1,  0,  X,  R,  0 },
		{  0, -1,  0,  0,  R }
	}
};
#undef X
#undef R
/*----------------------------------------------------------------------------*/
/*
 * ----------------------------------------------------
 * PK{(x, z, r): C1 g^yy = g^x g^{mz} h^r ; C2 = y^r ; C3 = a^x}
 * ----------------------------------------------------
 * Vbles	{ x, z, r }
 * Spheres	[0..m-1] [0..m-1] [0..n^2-1]
 * ----------------------------------------------------
 * Objs		{ C1 g^yy, C2, C3, g, g^m, h, a, y } | MOD
 * ----------------------------------------------------
 * Rels		{  -1,      0,  0, x,  z,  r, 0, 0 } | n
 *			{   0,     -1,  0, 0,  0,  0, 0, r } | n
 *			{   0,      0, -1, 0,  0,  0, x, 0 } | n
 * ----------------------------------------------------
 */
#define X	set_vble_idx(0)
#define Z	set_vble_idx(1)
#define R	set_vble_idx(2)
#define NADRP_SK2_NVBLES	3
#define NADRP_SK2_NOBJS		8
#define NADRP_SK2_NRELS		3
static const sok_relset_t NADRP_SK2 = {
	NADRP_SK2_NVBLES,	/* NVbles */
	NADRP_SK2_NOBJS,	/* NObjs */
	NADRP_SK2_NRELS,	/* NRels */
	{
		{ -1,  0,  0,  X,  Z,  R,  0,  0 },
		{  0, -1,  0,  0,  0,  0,  0,  R },
		{  0,  0, -1,  0,  0,  0,  X,  0 }
	}
};
#undef X
#undef Z
#undef R
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void nadrp_precomp(nadrp_precomp_t* prcmp,
				   const bigint_t n,		/* modulus */
				   const bigint_t a,		/* calculates x and a^x (mod n) */
				   const bigint_t g,
				   const bigint_t h,
				   const bigint_t y,
				   const syspar_t* sp,
				   const sphere_t* sph)		/* sphere for x */
{
	/*--------------------------------*/
	create_sph_theta(&prcmp->theta, sph);
	/*--------------------------------*/
	create_sph_omega(&prcmp->omega, n, sp);
	/*--------------------------------*/
	bi_powmod_sec(prcmp->nadrp_1_h_r_omega, h, prcmp->omega.center, n);
	bi_powmod_sec(prcmp->nadrp_1_y_r_omega, y, prcmp->omega.center, n);
	bi_asg(prcmp->nadrp_2_h_g_omega, prcmp->nadrp_1_h_r_omega);
	bi_asg(prcmp->nadrp_2_y_g_omega, prcmp->nadrp_1_y_r_omega);
	/*--------------------------------*/
	BEG_VAR_1_bigint_t(mm);
	bi_sph_ek_range(mm, &prcmp->theta);
	bi_powmod_sec(prcmp->nadrp_2_g_m_theta_ek_range, g, mm, n);
	END_VAR_1_bigint_t(mm);
	bi_powmod_sec(prcmp->nadrp_1_g_x_theta, g, prcmp->theta.center, n);
	bi_asg(prcmp->nadrp_2_g_a_theta, prcmp->nadrp_1_g_x_theta);
	bi_powmod_sec(prcmp->nadrp_2_gm_b_theta, prcmp->nadrp_2_g_m_theta_ek_range,
			  prcmp->theta.center, n);
	bi_powmod_sec(prcmp->nadrp_2_a_a_theta, a, prcmp->theta.center, n);
	/*--------------------------------*/
}
/*----------------------------------------------------------------------------*/
void nadrp_parms(nadrp_parms_t* p,
				 const bigint_t* n,
				 const bigint_t* a,
				 const bigint_t* g,
				 const bigint_t* h,
				 const bigint_t* y,
				 const syspar_t* sp,
				 const sphere_t* xsph
#ifdef PRECOMPUTATIONS__
				 , const nadrp_precomp_t* precomp
#endif
				 )
{
	BEG_BIGINT_CHK();
	/*-----------------------------*/
	p->n = n;
	p->a = a;
	p->g = g;
	p->h = h;
	p->y = y;
	/*-----------------------------*/
	p->sp = sp;
	p->sph = xsph;
	/*-----------------------------*/
#ifdef PRECOMPUTATIONS__
	assert(precomp != NULL);
	p->precomp = precomp;
	p->theta = &precomp->theta;
	p->omega = &precomp->omega;
#else
	create_sph_theta(&p->base_theta, p->sph);
	p->theta = &p->base_theta;
	/*--------------------------------*/
	create_sph_omega(&p->base_omega, *p->n, sp);
	p->omega = &p->base_omega;
#endif
	/*-----------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/*
 * As result, the user gets a random 'x' in Mu_ek and GM gets ax = a^x (mod n)
 */
bool_t nadrp_a_1(nadrp_a1prv_t* a1prv, /* Private */
				 nadrp_a1pbl_t* a1pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx)
{
	bool_t sk1;
	BEG_BIGINT_CHK();
	BEG_VAR_1(sok_params_t, sokp);
	/*--------------------------------*/
	/* User */
	bi_random_sph_ek(a1prv->xx, p->theta, rnd_ctx);
	bi_random_sph_ek(a1prv->rr, p->omega, rnd_ctx);
	bi_mul_powmod(a1pbl->C1, *p->g, a1prv->xx, *p->h, a1prv->rr, *p->n);
	bi_powmod_sec(a1pbl->C2, *p->y, a1prv->rr, *p->n);
	/*---------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, &((const nadrp_a1pbl_t*)a1pbl)->C1);
	sok_params_add_obj(&sokp, &((const nadrp_a1pbl_t*)a1pbl)->C2);
	sok_params_add_obj(&sokp, p->g);
	sok_params_add_obj(&sokp, p->h);
	sok_params_add_obj(&sokp, p->y);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, p->n);
	sok_params_add_mod(&sokp, p->n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &a1pbl->c);
	/*--------------------------------*/
	sok_params_add_vble(&sokp, &a1pbl->sx, p->theta,
						&((const nadrp_a1prv_t*)a1prv)->xx);
	sok_params_add_vble(&sokp, &a1pbl->sr, p->omega,
						&((const nadrp_a1prv_t*)a1prv)->rr);
	/*---------------------------------*/
	sk1 = sok(&NADRP_SK1, &sokp, NULL, 0, p->sp, rnd_ctx);
	/*--------------------------------*/
	END_VAR_1(sok_params_t, sokp);
	END_BIGINT_CHK();
	return sk1;
}
/*--------------------------------------------------------------------------*/
UNUSED__
static void nadrpb2_precomp(sok_precomp_t* precomp, const nadrp_precomp_t* np)
{
	sok_precomp_init(precomp, &NADRP_SK1);
	sok_set_precomp(precomp, &NADRP_SK1, 0, 2, &np->nadrp_1_g_x_theta);
	sok_set_precomp(precomp, &NADRP_SK1, 0, 3, &np->nadrp_1_h_r_omega);
	sok_set_precomp(precomp, &NADRP_SK1, 1, 4, &np->nadrp_1_y_r_omega);
}
/*--------------------------------------------------------------------------*/
bool_t nadrp_b_2(nadrp_b2pbl_t* b2pbl,
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx)
{
	bool_t sk1;
	BEG_BIGINT_CHK();
	BEG_VAR_1(sokv_params_t, sokp);
	/*---------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, &((const nadrp_a1pbl_t*)a1pbl)->C1);
	sok_params_add_obj(&sokp, &((const nadrp_a1pbl_t*)a1pbl)->C2);
	sok_params_add_obj(&sokp, p->g);
	sok_params_add_obj(&sokp, p->h);
	sok_params_add_obj(&sokp, p->y);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, p->n);
	sok_params_add_mod(&sokp, p->n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &a1pbl->c);
	/*--------------------------------*/
	sok_params_add_sph(&sokp, &a1pbl->sx, p->theta);
	sok_params_add_sph(&sokp, &a1pbl->sr, p->omega);
	/*--------------------------------*/
	/* GM */
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(sok_precomp_t, precomp);
	nadrpb2_precomp(&precomp, p->precomp);
#endif
	/*--------------------------------*/
	sk1 = sokv(&NADRP_SK1, &sokp, NULL, 0, p->sp
#ifdef PRECOMPUTATIONS__
			   , &precomp
#endif
			   );
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	END_VAR_1(sok_precomp_t, precomp);
#endif
	/*---------------------------------*/
	if (sk1) {
		bi_random_sph_ek(b2pbl->yy, p->theta, rnd_ctx);
	} else {
		DBG(fprintf(stderr, "NADRP SK1 Fails\n"););
	}
	/*--------------------------------*/
	END_VAR_1(sokv_params_t, sokp);
	END_BIGINT_CHK();
	return sk1;
}
/*--------------------------------------------------------------------------*/
bool_t nadrp_a_3(nadrp_a3prv_t* a3prv, 		/* Private */
				 nadrp_a3pbl_t* a3pbl,
				 const nadrp_a1prv_t* a1prv, /* Private */
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_b2pbl_t* b2pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx)
{
	bool_t sk2 = TRUE;
	BEG_BIGINT_CHK();
	BEG_VAR_1(sok_params_t, sokp);
	BEG_VAR_5_bigint_t(mm, x1, remainder, beta, C1gy);
	bi_sph_ek_range(mm, p->theta);		/* mm is range of theta_ek */
	/*--------------------------------*/
	/* User */
	if ( ! bi_belongs_sph_ek(b2pbl->yy, p->theta)) {
		DBG(fprintf(stderr, "Abort: yy not in range\n"););
		sk2 = FALSE;
	} else {
#ifdef PRECOMPUTATIONS__
		const bigint_t* gm = &p->precomp->nadrp_2_g_m_theta_ek_range;
		DBG(bi_powmod_dbg(x1, *p->g, mm, *p->n););
		assert(bi_equals(x1, p->precomp->nadrp_2_g_m_theta_ek_range));
#else
		BEG_VAR_1_bigint_t(gm_);
		const bigint_t* gm = (const bigint_t*)&gm_;
		bi_powmod_sec(gm_, *p->g, mm, *p->n);
#endif
		/*----------------------------*/
		bi_addmod(x1, a1prv->xx, b2pbl->yy, mm);
		bi_powmod_sec(a3pbl->C3, *p->a, x1, *p->n);
		/*----------------------------*/
		bi_add(beta, a1prv->xx, b2pbl->yy);
		bi_div(beta, beta, mm, remainder);
		bi_powmod_sec(C1gy, *p->g, b2pbl->yy, *p->n);
		bi_mulmod(C1gy, C1gy, a1pbl->C1, *p->n);
		/*----------------------------*/
		sok_params_init(&sokp);
		/*----------------------------*/
		sok_params_add_obj(&sokp, (const bigint_t*)&C1gy);
		sok_params_add_obj(&sokp, &a1pbl->C2);
		sok_params_add_obj(&sokp, &((const nadrp_a3pbl_t*)a3pbl)->C3);
		sok_params_add_obj(&sokp, p->g);
		sok_params_add_obj(&sokp, gm);
		sok_params_add_obj(&sokp, p->h);
		sok_params_add_obj(&sokp, p->a);
		sok_params_add_obj(&sokp, p->y);
		/*----------------------------*/
		sok_params_add_mod(&sokp, p->n);
		sok_params_add_mod(&sokp, p->n);
		sok_params_add_mod(&sokp, p->n);
		/*----------------------------*/
		sok_params_add_c(&sokp, &a3pbl->c);
		/*----------------------------*/
		sok_params_add_vble(&sokp, &a3pbl->sx, p->theta,(const bigint_t*)&x1);
		sok_params_add_vble(&sokp, &a3pbl->sz, p->theta,(const bigint_t*)&beta);
		sok_params_add_vble(&sokp, &a3pbl->sr, p->omega, &a1prv->rr);
		/*----------------------------*/
		sk2 = sok(&NADRP_SK2, &sokp, NULL, 0, p->sp, rnd_ctx);
		/*----------------------------*/
		bi_sph_ek_min(mm, p->sph);		/* reuse of mm for min of sphere */
		bi_add(a3prv->x, x1, mm);		/* reuse of mm for min of sphere */
#ifndef PRECOMPUTATIONS__
		END_VAR_1_bigint_t(gm_);
#endif
	}
	END_VAR_5_bigint_t(mm, x1, remainder, beta, C1gy);
	END_VAR_1(sok_params_t, sokp);
	END_BIGINT_CHK();
	return sk2;
}
/*--------------------------------------------------------------------------*/
UNUSED__
static void nadrpb4_precomp(sok_precomp_t* precomp, const nadrp_precomp_t* np)
{
	sok_precomp_init(precomp, &NADRP_SK2);
	sok_set_precomp(precomp, &NADRP_SK2, 0, 3, &np->nadrp_2_g_a_theta);
	sok_set_precomp(precomp, &NADRP_SK2, 0, 4, &np->nadrp_2_gm_b_theta);
	sok_set_precomp(precomp, &NADRP_SK2, 0, 5, &np->nadrp_2_h_g_omega);
	sok_set_precomp(precomp, &NADRP_SK2, 1, 7, &np->nadrp_2_y_g_omega);
	sok_set_precomp(precomp, &NADRP_SK2, 2, 6, &np->nadrp_2_a_a_theta);
}
/*--------------------------------------------------------------------------*/
bool_t nadrp_b_4(nadrp_b4pbl_t* b4pbl,
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_b2pbl_t* b2pbl,
				 const nadrp_a3pbl_t* a3pbl,
				 const nadrp_parms_t* p)
{
	bool_t sk2;
	BEG_BIGINT_CHK();
	/*---------------------------------*/
	BEG_VAR_1(sokv_params_t, sokp);
	BEG_VAR_1_bigint_t(C1gy);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const bigint_t* gm = &p->precomp->nadrp_2_g_m_theta_ek_range;
	DBG(BEG_VAR_1_bigint_t(mm););
	DBG(bi_sph_ek_range(mm, p->theta););
	DBG(bi_powmod_dbg(C1gy, *p->g, mm, *p->n););
	DBG(END_VAR_1_bigint_t(mm););
	assert(bi_equals(C1gy, p->precomp->nadrp_2_g_m_theta_ek_range));
#else
	BEG_VAR_2_bigint_t(mm, gm_);
	const bigint_t* gm = (const bigint_t*)&gm_;
	bi_sph_ek_range(mm, p->theta);			/* mm is range of theta_ek */
	bi_powmod_sec(gm_, *p->g, mm, *p->n);
#endif
	/*--------------------------------*/
	/* GM */
	bi_powmod_sec(C1gy, *p->g, b2pbl->yy, *p->n);
	bi_mulmod(C1gy, C1gy, a1pbl->C1, *p->n);
	/*--------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, (const bigint_t*)&C1gy);
	sok_params_add_obj(&sokp, &a1pbl->C2);
	sok_params_add_obj(&sokp, &((const nadrp_a3pbl_t*)a3pbl)->C3);
	sok_params_add_obj(&sokp, p->g);
	sok_params_add_obj(&sokp, gm);
	sok_params_add_obj(&sokp, p->h);
	sok_params_add_obj(&sokp, p->a);
	sok_params_add_obj(&sokp, p->y);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, p->n);
	sok_params_add_mod(&sokp, p->n);
	sok_params_add_mod(&sokp, p->n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &a3pbl->c);
	/*--------------------------------*/
	sok_params_add_sph(&sokp, &a3pbl->sx, p->theta);
	sok_params_add_sph(&sokp, &a3pbl->sz, p->theta);
	sok_params_add_sph(&sokp, &a3pbl->sr, p->omega);
	/*---------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(sok_precomp_t, precomp);
	nadrpb4_precomp(&precomp, p->precomp);
#endif
	/*--------------------------------*/
	sk2 = sokv(&NADRP_SK2, &sokp, NULL, 0, p->sp
#ifdef PRECOMPUTATIONS__
			   , &precomp
#endif
			   );
	/*---------------------------------*/
	if (sk2) {
		BEG_VAR_1_bigint_t(min);
		bi_sph_ek_min(min, p->sph);
		bi_powmod_sec(b4pbl->ax, *p->a, min, *p->n);
		bi_mulmod(b4pbl->ax, b4pbl->ax, a3pbl->C3, *p->n);
		END_VAR_1_bigint_t(min);
	} else {
		DBG(fprintf(stderr, "NADRP SK2 Fails\n"););
	}
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	END_VAR_1(sok_precomp_t, precomp);
#else
	END_VAR_2_bigint_t(mm, gm_);
#endif
	/*---------------------------------*/
	END_VAR_1_bigint_t(C1gy);
	END_VAR_1(sokv_params_t, sokp);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return sk2;
}
/*--------------------------------------------------------------------------*/
/*
 * As result, the user gets a random 'x' in Mu_ek and GM gets ax = a^x (mod n)
 */
bool_t nadrp_prtcl(bigint_t* ax,
				   bigint_t* x,
				   const bigint_t* n,
				   const bigint_t* a,
				   const bigint_t* g,
				   const bigint_t* h,
				   const bigint_t* y,
				   const syspar_t* sp,
				   const sphere_t* xsph,
				   rnd_ctx_t* rnd_ctx)
{
	bool_t ask1, ask2, bsk1, bsk2;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_7(nadrp_parms_t, p,
		   nadrp_a1prv_t, a1prv,	/* Private */
		   nadrp_a1pbl_t, a1pbl,	/* to be sent to B */
		   nadrp_b2pbl_t, b2pbl,	/* to be sent to A */
		   nadrp_a3prv_t, a3prv,	/* Private */
		   nadrp_a3pbl_t, a3pbl,	/* to be sent to B */
		   nadrp_b4pbl_t, b4pbl);	/* to be sent to A */
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(nadrp_precomp_t, precomp);
	nadrp_precomp(&precomp, *n, *a, *g, *h, *y, sp, xsph);
#endif
	/*--------------------------------*/
	nadrp_parms(&p, n, a, g, h, y, sp, xsph
#ifdef PRECOMPUTATIONS__
				, &precomp
#endif
				);
	{INIT_TIMER(NADRP);
	/*---------------------------------*/
	/* A: */
	{INIT_TIMER(ASk1);
	ask1 = nadrp_a_1(&a1prv, &a1pbl, &p, rnd_ctx);
	PRINT_TIMER(ASk1);}
	assert(ask1);
	/*---------------------------------*/
	/* B: */
	{INIT_TIMER(BSk1);
	bsk1 = ask1 && nadrp_b_2(&b2pbl, &a1pbl, &p, rnd_ctx);
	PRINT_TIMER(BSk1);}
	assert(bsk1);
	/*---------------------------------*/
	/* A: */
	{INIT_TIMER(ASk2);
	ask2 = bsk1 && nadrp_a_3(&a3prv, &a3pbl, &a1prv, &a1pbl, &b2pbl, &p,
							 rnd_ctx);
	PRINT_TIMER(ASk2);}
	assert(ask2);
	/*---------------------------------*/
	/* B: */
	{INIT_TIMER(BSk2);
	bsk2 = ask2 && nadrp_b_4(&b4pbl, &a1pbl, &b2pbl, &a3pbl, &p);
	PRINT_TIMER(BSk2);}
	assert(bsk2);
	/*--------------------------------*/
	PRINT_TIMER(NADRP);}
	/*---------------------------------*/
	bi_swap(*ax, b4pbl.ax);
	bi_swap(*x, a3prv.x);;
	/*-----------------------------*/
	assert(bi_belongs_sph_ek(*x, p.sph));
	DBG(BEG_VAR_1_bigint_t(ax__); );
	DBG(bi_powmod_dbg(ax__, *p.a, *x, *p.n););
	assert(bi_equals(ax__, *ax));
	DBG(fprintf(stderr, "\tNADRP OK\n"););
	DBG(END_VAR_1_bigint_t(ax__); );
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	END_VAR_1(nadrp_precomp_t, precomp);
#endif
	END_VAR_7(nadrp_parms_t, p,
		   nadrp_a1prv_t, a1prv,	/* Private */
		   nadrp_a1pbl_t, a1pbl,	/* to be sent to B */
		   nadrp_b2pbl_t, b2pbl,	/* to be sent to A */
		   nadrp_a3prv_t, a3prv,	/* Private */
		   nadrp_a3pbl_t, a3pbl,	/* to be sent to B */
		   nadrp_b4pbl_t, b4pbl);	/* to be sent to A */
	/*--------------------------------*/
	END_BIGINT_CHK();
	return bsk2;
}	
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_precomp_t_chk_members(const struct nadrp_precomp_t* p, int code)/*auto*/
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
	struct dummy_nadrp_precomp_t {
		sphere_t theta;
		sphere_t omega;
		bigint_t nadrp_1_g_x_theta;				/* zero */
		bigint_t nadrp_1_h_r_omega;				/* zero */
		bigint_t nadrp_1_y_r_omega;				/* zero */
		bigint_t nadrp_2_g_m_theta_ek_range;	/* zero */
		bigint_t nadrp_2_g_a_theta;				/* zero */
		bigint_t nadrp_2_h_g_omega;				/* zero */
		bigint_t nadrp_2_y_g_omega;				/* zero */
		bigint_t nadrp_2_gm_b_theta;			/* zero */
		bigint_t nadrp_2_a_a_theta;				/* zero */
	};
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, theta);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, omega);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_1_g_x_theta);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_1_h_r_omega);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_1_y_r_omega);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_g_m_theta_ek_range);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_g_a_theta);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_h_g_omega);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_y_g_omega);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_gm_b_theta);
	CHK_FIELD__(dummy_nadrp_precomp_t, nadrp_precomp_t, nadrp_2_a_a_theta);
	CHK_SIZE__(dummy_nadrp_precomp_t, nadrp_precomp_t);
	return (code == 19155497); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_ctor(struct nadrp_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_precomp_t_chk_members(p,19155497));
	sphere_t_ctor(&p->theta);
	sphere_t_ctor(&p->omega);
	bi_ctor(p->nadrp_1_g_x_theta);
	bi_ctor(p->nadrp_1_h_r_omega);
	bi_ctor(p->nadrp_1_y_r_omega);
	bi_ctor(p->nadrp_2_g_m_theta_ek_range);
	bi_ctor(p->nadrp_2_g_a_theta);
	bi_ctor(p->nadrp_2_h_g_omega);
	bi_ctor(p->nadrp_2_y_g_omega);
	bi_ctor(p->nadrp_2_gm_b_theta);
	bi_ctor(p->nadrp_2_a_a_theta);
}
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_dtor(struct nadrp_precomp_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_precomp_t_chk_members(p,19155497));
	bi_clear_zero(p->nadrp_2_a_a_theta);
	bi_dtor(p->nadrp_2_a_a_theta);
	bi_clear_zero(p->nadrp_2_gm_b_theta);
	bi_dtor(p->nadrp_2_gm_b_theta);
	bi_clear_zero(p->nadrp_2_y_g_omega);
	bi_dtor(p->nadrp_2_y_g_omega);
	bi_clear_zero(p->nadrp_2_h_g_omega);
	bi_dtor(p->nadrp_2_h_g_omega);
	bi_clear_zero(p->nadrp_2_g_a_theta);
	bi_dtor(p->nadrp_2_g_a_theta);
	bi_clear_zero(p->nadrp_2_g_m_theta_ek_range);
	bi_dtor(p->nadrp_2_g_m_theta_ek_range);
	bi_clear_zero(p->nadrp_1_y_r_omega);
	bi_dtor(p->nadrp_1_y_r_omega);
	bi_clear_zero(p->nadrp_1_h_r_omega);
	bi_dtor(p->nadrp_1_h_r_omega);
	bi_clear_zero(p->nadrp_1_g_x_theta);
	bi_dtor(p->nadrp_1_g_x_theta);
	sphere_t_dtor(&p->omega);
	sphere_t_dtor(&p->theta);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_asg(struct nadrp_precomp_t* p, const struct nadrp_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_precomp_t_chk_members(p,19155497));
	if (p != o) {
		sphere_t_asg(&p->theta, &o->theta);
		sphere_t_asg(&p->omega, &o->omega);
		bi_asg(p->nadrp_1_g_x_theta, o->nadrp_1_g_x_theta);
		bi_asg(p->nadrp_1_h_r_omega, o->nadrp_1_h_r_omega);
		bi_asg(p->nadrp_1_y_r_omega, o->nadrp_1_y_r_omega);
		bi_asg(p->nadrp_2_g_m_theta_ek_range, o->nadrp_2_g_m_theta_ek_range);
		bi_asg(p->nadrp_2_g_a_theta, o->nadrp_2_g_a_theta);
		bi_asg(p->nadrp_2_h_g_omega, o->nadrp_2_h_g_omega);
		bi_asg(p->nadrp_2_y_g_omega, o->nadrp_2_y_g_omega);
		bi_asg(p->nadrp_2_gm_b_theta, o->nadrp_2_gm_b_theta);
		bi_asg(p->nadrp_2_a_a_theta, o->nadrp_2_a_a_theta);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_move(struct nadrp_precomp_t* p, struct nadrp_precomp_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_precomp_t_chk_members(p,19155497));
	if (p != o) {
		sphere_t_move(&p->theta, &o->theta);
		sphere_t_move(&p->omega, &o->omega);
		bi_asg_si(p->nadrp_1_g_x_theta, 0);
		bi_swap(p->nadrp_1_g_x_theta, o->nadrp_1_g_x_theta);
		bi_asg_si(p->nadrp_1_h_r_omega, 0);
		bi_swap(p->nadrp_1_h_r_omega, o->nadrp_1_h_r_omega);
		bi_asg_si(p->nadrp_1_y_r_omega, 0);
		bi_swap(p->nadrp_1_y_r_omega, o->nadrp_1_y_r_omega);
		bi_asg_si(p->nadrp_2_g_m_theta_ek_range, 0);
		bi_swap(p->nadrp_2_g_m_theta_ek_range, o->nadrp_2_g_m_theta_ek_range);
		bi_asg_si(p->nadrp_2_g_a_theta, 0);
		bi_swap(p->nadrp_2_g_a_theta, o->nadrp_2_g_a_theta);
		bi_asg_si(p->nadrp_2_h_g_omega, 0);
		bi_swap(p->nadrp_2_h_g_omega, o->nadrp_2_h_g_omega);
		bi_asg_si(p->nadrp_2_y_g_omega, 0);
		bi_swap(p->nadrp_2_y_g_omega, o->nadrp_2_y_g_omega);
		bi_asg_si(p->nadrp_2_gm_b_theta, 0);
		bi_swap(p->nadrp_2_gm_b_theta, o->nadrp_2_gm_b_theta);
		bi_asg_si(p->nadrp_2_a_a_theta, 0);
		bi_swap(p->nadrp_2_a_a_theta, o->nadrp_2_a_a_theta);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_precomp_t* nadrp_precomp_t_new()/*auto*/
{
	struct nadrp_precomp_t* p = (struct nadrp_precomp_t*)malloc(sizeof(struct nadrp_precomp_t));
	if (p != NULL) {
		nadrp_precomp_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_precomp_t* nadrp_precomp_t_clone(const struct nadrp_precomp_t* o)/*auto*/
{
	struct nadrp_precomp_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_precomp_t*)malloc(sizeof(struct nadrp_precomp_t));
		if (p != NULL) {
			nadrp_precomp_t_ctor(p);
			nadrp_precomp_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_delete(struct nadrp_precomp_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_precomp_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_parms_t_chk_members(const struct nadrp_parms_t* p, int code)/*auto*/
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
	struct dummy_nadrp_parms_t {
		const bigint_t* n;			/* n: modulus */
		const bigint_t* a;			/* a: calculates x and a^x (mod n) */
		const bigint_t* g;
		const bigint_t* h;
		const bigint_t* y;
		const syspar_t* sp;
		const sphere_t* sph;		/* sph: sphere for x */
		const sphere_t* theta;		/* theta: outer sphere for Z_m */
		const sphere_t* omega;		/* omega: sphere for Z_{n^2} */
#ifdef PRECOMPUTATIONS__
		const nadrp_precomp_t* precomp;
#else
		sphere_t base_theta;
		sphere_t base_omega;
#endif
	};
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, n);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, a);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, g);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, h);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, y);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, sp);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, sph);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, theta);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, omega);
#ifdef PRECOMPUTATIONS__
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, precomp);
#else
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, base_theta);
	CHK_FIELD__(dummy_nadrp_parms_t, nadrp_parms_t, base_omega);
#endif
	CHK_SIZE__(dummy_nadrp_parms_t, nadrp_parms_t);
	return (code == 408632838); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_ctor(struct nadrp_parms_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_parms_t_chk_members(p,408632838));
	p->n = NULL;
	p->a = NULL;
	p->g = NULL;
	p->h = NULL;
	p->y = NULL;
	p->sp = NULL;
	p->sph = NULL;
	p->theta = NULL;
	p->omega = NULL;
#ifdef PRECOMPUTATIONS__
	p->precomp = NULL;
#else
	sphere_t_ctor(&p->base_theta);
	sphere_t_ctor(&p->base_omega);
#endif
}
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_dtor(struct nadrp_parms_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_parms_t_chk_members(p,408632838));
#ifndef PRECOMPUTATIONS__
	sphere_t_dtor(&p->base_omega);
	sphere_t_dtor(&p->base_theta);
#else
#endif
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_asg(struct nadrp_parms_t* p, const struct nadrp_parms_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_parms_t_chk_members(p,408632838));
	if (p != o) {
		p->n = o->n;
		p->a = o->a;
		p->g = o->g;
		p->h = o->h;
		p->y = o->y;
		p->sp = o->sp;
		p->sph = o->sph;
		p->theta = o->theta;
		p->omega = o->omega;
#ifdef PRECOMPUTATIONS__
		p->precomp = o->precomp;
#else
		sphere_t_asg(&p->base_theta, &o->base_theta);
		sphere_t_asg(&p->base_omega, &o->base_omega);
#endif
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_move(struct nadrp_parms_t* p, struct nadrp_parms_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_parms_t_chk_members(p,408632838));
	if (p != o) {
		p->n = o->n;
		o->n = NULL;
		p->a = o->a;
		o->a = NULL;
		p->g = o->g;
		o->g = NULL;
		p->h = o->h;
		o->h = NULL;
		p->y = o->y;
		o->y = NULL;
		p->sp = o->sp;
		o->sp = NULL;
		p->sph = o->sph;
		o->sph = NULL;
		p->theta = o->theta;
		o->theta = NULL;
		p->omega = o->omega;
		o->omega = NULL;
#ifdef PRECOMPUTATIONS__
		p->precomp = o->precomp;
		o->precomp = NULL;
#else
		sphere_t_move(&p->base_theta, &o->base_theta);
		sphere_t_move(&p->base_omega, &o->base_omega);
#endif
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_parms_t* nadrp_parms_t_new()/*auto*/
{
	struct nadrp_parms_t* p = (struct nadrp_parms_t*)malloc(sizeof(struct nadrp_parms_t));
	if (p != NULL) {
		nadrp_parms_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_parms_t* nadrp_parms_t_clone(const struct nadrp_parms_t* o)/*auto*/
{
	struct nadrp_parms_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_parms_t*)malloc(sizeof(struct nadrp_parms_t));
		if (p != NULL) {
			nadrp_parms_t_ctor(p);
			nadrp_parms_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_delete(struct nadrp_parms_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_parms_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_a1prv_t_chk_members(const struct nadrp_a1prv_t* p, int code)/*auto*/
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
	struct dummy_nadrp_a1prv_t {
		bigint_t xx;		/* zero */
		bigint_t rr;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_a1prv_t, nadrp_a1prv_t, xx);
	CHK_FIELD__(dummy_nadrp_a1prv_t, nadrp_a1prv_t, rr);
	CHK_SIZE__(dummy_nadrp_a1prv_t, nadrp_a1prv_t);
	return (code == 90011590); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_ctor(struct nadrp_a1prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a1prv_t_chk_members(p,90011590));
	bi_ctor(p->xx);
	bi_ctor(p->rr);
}
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_dtor(struct nadrp_a1prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a1prv_t_chk_members(p,90011590));
	bi_clear_zero(p->rr);
	bi_dtor(p->rr);
	bi_clear_zero(p->xx);
	bi_dtor(p->xx);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_asg(struct nadrp_a1prv_t* p, const struct nadrp_a1prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a1prv_t_chk_members(p,90011590));
	if (p != o) {
		bi_asg(p->xx, o->xx);
		bi_asg(p->rr, o->rr);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_move(struct nadrp_a1prv_t* p, struct nadrp_a1prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a1prv_t_chk_members(p,90011590));
	if (p != o) {
		bi_asg_si(p->xx, 0);
		bi_swap(p->xx, o->xx);
		bi_asg_si(p->rr, 0);
		bi_swap(p->rr, o->rr);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_a1prv_t* nadrp_a1prv_t_new()/*auto*/
{
	struct nadrp_a1prv_t* p = (struct nadrp_a1prv_t*)malloc(sizeof(struct nadrp_a1prv_t));
	if (p != NULL) {
		nadrp_a1prv_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_a1prv_t* nadrp_a1prv_t_clone(const struct nadrp_a1prv_t* o)/*auto*/
{
	struct nadrp_a1prv_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_a1prv_t*)malloc(sizeof(struct nadrp_a1prv_t));
		if (p != NULL) {
			nadrp_a1prv_t_ctor(p);
			nadrp_a1prv_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_delete(struct nadrp_a1prv_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_a1prv_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_a1pbl_t_chk_members(const struct nadrp_a1pbl_t* p, int code)/*auto*/
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
	struct dummy_nadrp_a1pbl_t {
		bigint_t C1;		/* zero */
		bigint_t C2;		/* zero */
		bigint_t c;			/* zero */
		bigint_t sx;		/* zero */
		bigint_t sr;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t, C1);
	CHK_FIELD__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t, C2);
	CHK_FIELD__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t, c);
	CHK_FIELD__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t, sx);
	CHK_FIELD__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t, sr);
	CHK_SIZE__(dummy_nadrp_a1pbl_t, nadrp_a1pbl_t);
	return (code == 9921207); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_ctor(struct nadrp_a1pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a1pbl_t_chk_members(p,9921207));
	bi_ctor(p->C1);
	bi_ctor(p->C2);
	bi_ctor(p->c);
	bi_ctor(p->sx);
	bi_ctor(p->sr);
}
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_dtor(struct nadrp_a1pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a1pbl_t_chk_members(p,9921207));
	bi_clear_zero(p->sr);
	bi_dtor(p->sr);
	bi_clear_zero(p->sx);
	bi_dtor(p->sx);
	bi_clear_zero(p->c);
	bi_dtor(p->c);
	bi_clear_zero(p->C2);
	bi_dtor(p->C2);
	bi_clear_zero(p->C1);
	bi_dtor(p->C1);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_asg(struct nadrp_a1pbl_t* p, const struct nadrp_a1pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a1pbl_t_chk_members(p,9921207));
	if (p != o) {
		bi_asg(p->C1, o->C1);
		bi_asg(p->C2, o->C2);
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
		bi_asg(p->sr, o->sr);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_move(struct nadrp_a1pbl_t* p, struct nadrp_a1pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a1pbl_t_chk_members(p,9921207));
	if (p != o) {
		bi_asg_si(p->C1, 0);
		bi_swap(p->C1, o->C1);
		bi_asg_si(p->C2, 0);
		bi_swap(p->C2, o->C2);
		bi_asg_si(p->c, 0);
		bi_swap(p->c, o->c);
		bi_asg_si(p->sx, 0);
		bi_swap(p->sx, o->sx);
		bi_asg_si(p->sr, 0);
		bi_swap(p->sr, o->sr);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_a1pbl_t* nadrp_a1pbl_t_new()/*auto*/
{
	struct nadrp_a1pbl_t* p = (struct nadrp_a1pbl_t*)malloc(sizeof(struct nadrp_a1pbl_t));
	if (p != NULL) {
		nadrp_a1pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_a1pbl_t* nadrp_a1pbl_t_clone(const struct nadrp_a1pbl_t* o)/*auto*/
{
	struct nadrp_a1pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_a1pbl_t*)malloc(sizeof(struct nadrp_a1pbl_t));
		if (p != NULL) {
			nadrp_a1pbl_t_ctor(p);
			nadrp_a1pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_delete(struct nadrp_a1pbl_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_a1pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_b2pbl_t_chk_members(const struct nadrp_b2pbl_t* p, int code)/*auto*/
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
	struct dummy_nadrp_b2pbl_t {
		bigint_t yy;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_b2pbl_t, nadrp_b2pbl_t, yy);
	CHK_SIZE__(dummy_nadrp_b2pbl_t, nadrp_b2pbl_t);
	return (code == 433789588); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_ctor(struct nadrp_b2pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_b2pbl_t_chk_members(p,433789588));
	bi_ctor(p->yy);
}
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_dtor(struct nadrp_b2pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_b2pbl_t_chk_members(p,433789588));
	bi_clear_zero(p->yy);
	bi_dtor(p->yy);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_asg(struct nadrp_b2pbl_t* p, const struct nadrp_b2pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_b2pbl_t_chk_members(p,433789588));
	if (p != o) {
		bi_asg(p->yy, o->yy);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_move(struct nadrp_b2pbl_t* p, struct nadrp_b2pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_b2pbl_t_chk_members(p,433789588));
	if (p != o) {
		bi_asg_si(p->yy, 0);
		bi_swap(p->yy, o->yy);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_b2pbl_t* nadrp_b2pbl_t_new()/*auto*/
{
	struct nadrp_b2pbl_t* p = (struct nadrp_b2pbl_t*)malloc(sizeof(struct nadrp_b2pbl_t));
	if (p != NULL) {
		nadrp_b2pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_b2pbl_t* nadrp_b2pbl_t_clone(const struct nadrp_b2pbl_t* o)/*auto*/
{
	struct nadrp_b2pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_b2pbl_t*)malloc(sizeof(struct nadrp_b2pbl_t));
		if (p != NULL) {
			nadrp_b2pbl_t_ctor(p);
			nadrp_b2pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_delete(struct nadrp_b2pbl_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_b2pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_a3prv_t_chk_members(const struct nadrp_a3prv_t* p, int code)/*auto*/
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
	struct dummy_nadrp_a3prv_t {
		bigint_t x;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_a3prv_t, nadrp_a3prv_t, x);
	CHK_SIZE__(dummy_nadrp_a3prv_t, nadrp_a3prv_t);
	return (code == 451805986); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_ctor(struct nadrp_a3prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a3prv_t_chk_members(p,451805986));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_dtor(struct nadrp_a3prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a3prv_t_chk_members(p,451805986));
	bi_clear_zero(p->x);
	bi_dtor(p->x);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_asg(struct nadrp_a3prv_t* p, const struct nadrp_a3prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a3prv_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_move(struct nadrp_a3prv_t* p, struct nadrp_a3prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a3prv_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg_si(p->x, 0);
		bi_swap(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_a3prv_t* nadrp_a3prv_t_new()/*auto*/
{
	struct nadrp_a3prv_t* p = (struct nadrp_a3prv_t*)malloc(sizeof(struct nadrp_a3prv_t));
	if (p != NULL) {
		nadrp_a3prv_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_a3prv_t* nadrp_a3prv_t_clone(const struct nadrp_a3prv_t* o)/*auto*/
{
	struct nadrp_a3prv_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_a3prv_t*)malloc(sizeof(struct nadrp_a3prv_t));
		if (p != NULL) {
			nadrp_a3prv_t_ctor(p);
			nadrp_a3prv_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_delete(struct nadrp_a3prv_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_a3prv_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_a3pbl_t_chk_members(const struct nadrp_a3pbl_t* p, int code)/*auto*/
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
	struct dummy_nadrp_a3pbl_t {
		bigint_t C3;		/* zero */
		bigint_t c;			/* zero */
		bigint_t sx;		/* zero */
		bigint_t sz;		/* zero */
		bigint_t sr;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t, C3);
	CHK_FIELD__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t, c);
	CHK_FIELD__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t, sx);
	CHK_FIELD__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t, sz);
	CHK_FIELD__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t, sr);
	CHK_SIZE__(dummy_nadrp_a3pbl_t, nadrp_a3pbl_t);
	return (code == 372483553); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_ctor(struct nadrp_a3pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a3pbl_t_chk_members(p,372483553));
	bi_ctor(p->C3);
	bi_ctor(p->c);
	bi_ctor(p->sx);
	bi_ctor(p->sz);
	bi_ctor(p->sr);
}
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_dtor(struct nadrp_a3pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_a3pbl_t_chk_members(p,372483553));
	bi_clear_zero(p->sr);
	bi_dtor(p->sr);
	bi_clear_zero(p->sz);
	bi_dtor(p->sz);
	bi_clear_zero(p->sx);
	bi_dtor(p->sx);
	bi_clear_zero(p->c);
	bi_dtor(p->c);
	bi_clear_zero(p->C3);
	bi_dtor(p->C3);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_asg(struct nadrp_a3pbl_t* p, const struct nadrp_a3pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a3pbl_t_chk_members(p,372483553));
	if (p != o) {
		bi_asg(p->C3, o->C3);
		bi_asg(p->c, o->c);
		bi_asg(p->sx, o->sx);
		bi_asg(p->sz, o->sz);
		bi_asg(p->sr, o->sr);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_move(struct nadrp_a3pbl_t* p, struct nadrp_a3pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_a3pbl_t_chk_members(p,372483553));
	if (p != o) {
		bi_asg_si(p->C3, 0);
		bi_swap(p->C3, o->C3);
		bi_asg_si(p->c, 0);
		bi_swap(p->c, o->c);
		bi_asg_si(p->sx, 0);
		bi_swap(p->sx, o->sx);
		bi_asg_si(p->sz, 0);
		bi_swap(p->sz, o->sz);
		bi_asg_si(p->sr, 0);
		bi_swap(p->sr, o->sr);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_a3pbl_t* nadrp_a3pbl_t_new()/*auto*/
{
	struct nadrp_a3pbl_t* p = (struct nadrp_a3pbl_t*)malloc(sizeof(struct nadrp_a3pbl_t));
	if (p != NULL) {
		nadrp_a3pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_a3pbl_t* nadrp_a3pbl_t_clone(const struct nadrp_a3pbl_t* o)/*auto*/
{
	struct nadrp_a3pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_a3pbl_t*)malloc(sizeof(struct nadrp_a3pbl_t));
		if (p != NULL) {
			nadrp_a3pbl_t_ctor(p);
			nadrp_a3pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_delete(struct nadrp_a3pbl_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_a3pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int nadrp_b4pbl_t_chk_members(const struct nadrp_b4pbl_t* p, int code)/*auto*/
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
	struct dummy_nadrp_b4pbl_t {
		bigint_t ax;		/* zero */
	};
	CHK_FIELD__(dummy_nadrp_b4pbl_t, nadrp_b4pbl_t, ax);
	CHK_SIZE__(dummy_nadrp_b4pbl_t, nadrp_b4pbl_t);
	return (code == 526706459); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_ctor(struct nadrp_b4pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_b4pbl_t_chk_members(p,526706459));
	bi_ctor(p->ax);
}
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_dtor(struct nadrp_b4pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(nadrp_b4pbl_t_chk_members(p,526706459));
	bi_clear_zero(p->ax);
	bi_dtor(p->ax);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_asg(struct nadrp_b4pbl_t* p, const struct nadrp_b4pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_b4pbl_t_chk_members(p,526706459));
	if (p != o) {
		bi_asg(p->ax, o->ax);
	}
}
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_move(struct nadrp_b4pbl_t* p, struct nadrp_b4pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(nadrp_b4pbl_t_chk_members(p,526706459));
	if (p != o) {
		bi_asg_si(p->ax, 0);
		bi_swap(p->ax, o->ax);
	}
}
/*----------------------------------------------------------------------------*/
struct nadrp_b4pbl_t* nadrp_b4pbl_t_new()/*auto*/
{
	struct nadrp_b4pbl_t* p = (struct nadrp_b4pbl_t*)malloc(sizeof(struct nadrp_b4pbl_t));
	if (p != NULL) {
		nadrp_b4pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct nadrp_b4pbl_t* nadrp_b4pbl_t_clone(const struct nadrp_b4pbl_t* o)/*auto*/
{
	struct nadrp_b4pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct nadrp_b4pbl_t*)malloc(sizeof(struct nadrp_b4pbl_t));
		if (p != NULL) {
			nadrp_b4pbl_t_ctor(p);
			nadrp_b4pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_delete(struct nadrp_b4pbl_t* p)/*auto*/
{
	if (p != NULL) {
		nadrp_b4pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

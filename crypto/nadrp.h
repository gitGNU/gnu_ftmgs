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
#ifndef umalccvbg_nadrp_h__
#define umalccvbg_nadrp_h__	1
#include "bigint.h"
#include "syspar.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
typedef struct nadrp_precomp_t {
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
} nadrp_precomp_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_parms_t {
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
} nadrp_parms_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_a1prv_t {
	bigint_t xx;		/* zero */
	bigint_t rr;		/* zero */
} nadrp_a1prv_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_a1pbl_t {
	bigint_t C1;		/* zero */
	bigint_t C2;		/* zero */
	bigint_t c;			/* zero */
	bigint_t sx;		/* zero */
	bigint_t sr;		/* zero */
} nadrp_a1pbl_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_b2pbl_t {
	bigint_t yy;		/* zero */
} nadrp_b2pbl_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_a3prv_t {
	bigint_t x;		/* zero */
} nadrp_a3prv_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_a3pbl_t {
	bigint_t C3;		/* zero */
	bigint_t c;			/* zero */
	bigint_t sx;		/* zero */
	bigint_t sz;		/* zero */
	bigint_t sr;		/* zero */
} nadrp_a3pbl_t;
/*----------------------------------------------------------------------------*/
typedef struct nadrp_b4pbl_t {
	bigint_t ax;		/* zero */
} nadrp_b4pbl_t;
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
				   const sphere_t* sph);			/* sphere for x */
/*---------------------------------*/
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
				 );
/*---------------------------------*/
USE_RESULT__
bool_t nadrp_a_1(nadrp_a1prv_t* a1prv, /* Private */
				 nadrp_a1pbl_t* a1pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx);
/*---------------------------------*/
USE_RESULT__
bool_t nadrp_b_2(nadrp_b2pbl_t* b2pbl,
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx);
/*---------------------------------*/
USE_RESULT__
bool_t nadrp_a_3(nadrp_a3prv_t* a3prv, 		/* Private */
				 nadrp_a3pbl_t* a3pbl,
				 const nadrp_a1prv_t* a1prv, /* Private */
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_b2pbl_t* b2pbl,
				 const nadrp_parms_t* p,
				 rnd_ctx_t* rnd_ctx);
/*---------------------------------*/
USE_RESULT__
bool_t nadrp_b_4(nadrp_b4pbl_t* b4pbl,
				 const nadrp_a1pbl_t* a1pbl,
				 const nadrp_b2pbl_t* b2pbl,
				 const nadrp_a3pbl_t* a3pbl,
				 const nadrp_parms_t* p);
/*---------------------------------*/
USE_RESULT__
bool_t nadrp_prtcl(bigint_t* ax,
				   bigint_t* x,
				   const bigint_t* n,
				   const bigint_t* a,
				   const bigint_t* g,
				   const bigint_t* h,
				   const bigint_t* y,
				   const syspar_t* sp,
				   const sphere_t* xsph,
				   rnd_ctx_t* rnd_ctx);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void nadrp_precomp_t_ctor(struct nadrp_precomp_t* p);
void nadrp_precomp_t_dtor(struct nadrp_precomp_t* p);
void nadrp_precomp_t_asg(struct nadrp_precomp_t* p, const struct nadrp_precomp_t* o);
void nadrp_precomp_t_move(struct nadrp_precomp_t* p, struct nadrp_precomp_t* o);
struct nadrp_precomp_t* nadrp_precomp_t_new();
struct nadrp_precomp_t* nadrp_precomp_t_clone(const struct nadrp_precomp_t* o);
void nadrp_precomp_t_delete(struct nadrp_precomp_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_parms_t_ctor(struct nadrp_parms_t* p);
void nadrp_parms_t_dtor(struct nadrp_parms_t* p);
void nadrp_parms_t_asg(struct nadrp_parms_t* p, const struct nadrp_parms_t* o);
void nadrp_parms_t_move(struct nadrp_parms_t* p, struct nadrp_parms_t* o);
struct nadrp_parms_t* nadrp_parms_t_new();
struct nadrp_parms_t* nadrp_parms_t_clone(const struct nadrp_parms_t* o);
void nadrp_parms_t_delete(struct nadrp_parms_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_a1prv_t_ctor(struct nadrp_a1prv_t* p);
void nadrp_a1prv_t_dtor(struct nadrp_a1prv_t* p);
void nadrp_a1prv_t_asg(struct nadrp_a1prv_t* p, const struct nadrp_a1prv_t* o);
void nadrp_a1prv_t_move(struct nadrp_a1prv_t* p, struct nadrp_a1prv_t* o);
struct nadrp_a1prv_t* nadrp_a1prv_t_new();
struct nadrp_a1prv_t* nadrp_a1prv_t_clone(const struct nadrp_a1prv_t* o);
void nadrp_a1prv_t_delete(struct nadrp_a1prv_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_a1pbl_t_ctor(struct nadrp_a1pbl_t* p);
void nadrp_a1pbl_t_dtor(struct nadrp_a1pbl_t* p);
void nadrp_a1pbl_t_asg(struct nadrp_a1pbl_t* p, const struct nadrp_a1pbl_t* o);
void nadrp_a1pbl_t_move(struct nadrp_a1pbl_t* p, struct nadrp_a1pbl_t* o);
struct nadrp_a1pbl_t* nadrp_a1pbl_t_new();
struct nadrp_a1pbl_t* nadrp_a1pbl_t_clone(const struct nadrp_a1pbl_t* o);
void nadrp_a1pbl_t_delete(struct nadrp_a1pbl_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_b2pbl_t_ctor(struct nadrp_b2pbl_t* p);
void nadrp_b2pbl_t_dtor(struct nadrp_b2pbl_t* p);
void nadrp_b2pbl_t_asg(struct nadrp_b2pbl_t* p, const struct nadrp_b2pbl_t* o);
void nadrp_b2pbl_t_move(struct nadrp_b2pbl_t* p, struct nadrp_b2pbl_t* o);
struct nadrp_b2pbl_t* nadrp_b2pbl_t_new();
struct nadrp_b2pbl_t* nadrp_b2pbl_t_clone(const struct nadrp_b2pbl_t* o);
void nadrp_b2pbl_t_delete(struct nadrp_b2pbl_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_a3prv_t_ctor(struct nadrp_a3prv_t* p);
void nadrp_a3prv_t_dtor(struct nadrp_a3prv_t* p);
void nadrp_a3prv_t_asg(struct nadrp_a3prv_t* p, const struct nadrp_a3prv_t* o);
void nadrp_a3prv_t_move(struct nadrp_a3prv_t* p, struct nadrp_a3prv_t* o);
struct nadrp_a3prv_t* nadrp_a3prv_t_new();
struct nadrp_a3prv_t* nadrp_a3prv_t_clone(const struct nadrp_a3prv_t* o);
void nadrp_a3prv_t_delete(struct nadrp_a3prv_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_a3pbl_t_ctor(struct nadrp_a3pbl_t* p);
void nadrp_a3pbl_t_dtor(struct nadrp_a3pbl_t* p);
void nadrp_a3pbl_t_asg(struct nadrp_a3pbl_t* p, const struct nadrp_a3pbl_t* o);
void nadrp_a3pbl_t_move(struct nadrp_a3pbl_t* p, struct nadrp_a3pbl_t* o);
struct nadrp_a3pbl_t* nadrp_a3pbl_t_new();
struct nadrp_a3pbl_t* nadrp_a3pbl_t_clone(const struct nadrp_a3pbl_t* o);
void nadrp_a3pbl_t_delete(struct nadrp_a3pbl_t* p);
/*----------------------------------------------------------------------------*/
void nadrp_b4pbl_t_ctor(struct nadrp_b4pbl_t* p);
void nadrp_b4pbl_t_dtor(struct nadrp_b4pbl_t* p);
void nadrp_b4pbl_t_asg(struct nadrp_b4pbl_t* p, const struct nadrp_b4pbl_t* o);
void nadrp_b4pbl_t_move(struct nadrp_b4pbl_t* p, struct nadrp_b4pbl_t* o);
struct nadrp_b4pbl_t* nadrp_b4pbl_t_new();
struct nadrp_b4pbl_t* nadrp_b4pbl_t_clone(const struct nadrp_b4pbl_t* o);
void nadrp_b4pbl_t_delete(struct nadrp_b4pbl_t* p);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

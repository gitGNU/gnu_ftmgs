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
#ifndef umalccvbg_sok_h__
#define umalccvbg_sok_h__	1
#include <assert.h>
#include "bigint.h"
#include "sphere.h"
#include "sha.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
#define array_sz(array__) (sizeof(array__)/sizeof(*(array__)))
/*----------------------------------------------------------------------------*/
#define MAX_RELATIONS	10
#define MAX_OBJECTS		20
#define MAX_VBLES		10
/*----------------------------------------------------------------------------*/
typedef struct sok_relset_t {
	unsigned n_vbles;
	unsigned n_objs;
	unsigned n_rels;
	int a[MAX_RELATIONS][MAX_OBJECTS]; /* -1, 0, +1 ; (n<10) ? n : vble[n-10]*/
} sok_relset_t;
/*------------------------------------*/
/*
 * Ctor & Dtor not defined to allow defining static constants
 *
 * #define sok_relset_t_ctor(sok_relset_t_ptr)	((void)0)
 * #define sok_relset_t_dtor(sok_relset_t_ptr)	((void)0)
 */
/*----------------------------------------------------------------------------*/
#define	is_vble(int_n)			((int_n) >= 10)
#define get_vble_idx(int_n)		((unsigned)((int_n) - 10))
#define set_vble_idx(uint_idx)	((int)((uint_idx) + 10))
/*----------------------------------------------------------------------------*/
typedef struct sok_params_t {
	unsigned n_vbles;
	unsigned n_objs;
	unsigned n_rels;
	bigint_t* c;
	bigint_t* s[MAX_VBLES];
	const bigint_t* objs[MAX_OBJECTS];
	const bigint_t* mod[MAX_RELATIONS];
	const sphere_t* sphs[MAX_VBLES];
	const bigint_t* vbles[MAX_VBLES];
} sok_params_t;
/*------------------------------------*/
#define sok_params_t_ctor(ptr)	((void)0)
#define sok_params_t_dtor(ptr)	((void)0)
/*------------------------------------*/
#define sok_params_init(ptr)					\
	do{											\
		(ptr)->n_vbles = 0;						\
		(ptr)->n_objs = 0;						\
		(ptr)->n_rels = 0;						\
		(ptr)->c = NULL;						\
	}while(0)
#define sok_params_add_obj(ptr,po)						\
	do{ (ptr)->objs[(ptr)->n_objs++] = (po); }while(0)
#define sok_params_add_mod(ptr,pm)						\
	do{ (ptr)->mod[(ptr)->n_rels++] = (pm); }while(0)
#define sok_params_add_c(ptr,pc)						\
	do{ (ptr)->c = (pc); }while(0)
#define sok_params_add_vble(ptr,ps,psh,pv)		\
	do{											\
		(ptr)->vbles[(ptr)->n_vbles] = (pv);	\
		(ptr)->s[(ptr)->n_vbles] = (ps);		\
		(ptr)->sphs[(ptr)->n_vbles++] = (psh);	\
	}while(0)
#define sok_params_add_sph(ptr,ps,psh)			\
	do{											\
		(ptr)->s[(ptr)->n_vbles] = (ps);		\
		(ptr)->sphs[(ptr)->n_vbles++] = (psh); 	\
	}while(0)
/*----------------------------------------------------------------------------*/
typedef struct sokv_params_t {
	unsigned n_vbles;
	unsigned n_objs;
	unsigned n_rels;
	const bigint_t* c;
	const bigint_t* s[MAX_VBLES];
	const bigint_t* objs[MAX_OBJECTS];
	const bigint_t* mod[MAX_RELATIONS];
	const sphere_t* sphs[MAX_VBLES];
} sokv_params_t;
/*------------------------------------*/
#define sokv_params_t_ctor(sokv_params_t_ptr)	((void)0)
#define sokv_params_t_dtor(sokv_params_t_ptr)	((void)0)
/*----------------------------------------------------------------------------*/
typedef struct sok_precomp_t {
	const bigint_t* v[MAX_RELATIONS][MAX_OBJECTS];
} sok_precomp_t;
/*------------------------------------*/
#define sok_precomp_t_ctor(sok_precomp_t_ptr)	((void)0)
#define sok_precomp_t_dtor(sok_precomp_t_ptr)	((void)0)
/*------------------------------------*/
void sok_precomp_init(sok_precomp_t* p, const sok_relset_t* rs);
/*------------------------------------*/
/* 
 * void sok_set_precomp(sok_precomp_t* p, const sok_relset_t* rs,
 * 					 unsigned i_rel, unsigned i_obj, const bigint_t* bi);
 */
#define sok_set_precomp(p, rs, i_rel, i_obj, bi)					\
	do{																\
		assert(((i_rel) < (rs)->n_rels)&&((i_obj) < (rs)->n_objs));	\
		assert((p)->v[i_rel][i_obj]==NULL);							\
		assert(is_vble((rs)->a[i_rel][i_obj]));						\
		(p)->v[i_rel][i_obj] = (bi);								\
	}while(0)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
USE_RESULT__
bool_t sok(const sok_relset_t* rs,		/* relation set */
		   const sok_params_t* prms,		/* sok params */
		   const void* dat_digest,
		   unsigned dat_digestlen,
		   const syspar_t* sp,
		   rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t sok_dat(unsigned which_sha,
			   const sok_relset_t* rs,		/* relation set */
			   const sok_params_t* prms,		/* sok params */
			   const void* dat,				/* message data */
			   unsigned datlen,
			   const syspar_t* sp,
			   rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t sokv(const sok_relset_t* rs,			/* relation set */
			const sokv_params_t* prms,		/* sokv params */
			const void* dat_digest,
			unsigned dat_digestlen,
			const syspar_t* sp
#ifdef PRECOMPUTATIONS__
			, const sok_precomp_t* precomp
#endif
			);
/*------------------------------------*/
USE_RESULT__
bool_t sokv_dat(unsigned which_sha,
				const sok_relset_t* rs,			/* relation set */
				const sokv_params_t* prms,		/* sokv params */
				const void* dat,				/* message data */
				unsigned datlen,
				const syspar_t* sp
#ifdef PRECOMPUTATIONS__
				, const sok_precomp_t* precomp
#endif
				);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 * SK{(x): y1 = g1^x (mod n1) ; y2 = g2^x (mod n1) ; ... }
 *--------------------------------
 * Objs     { g1, y1, g2, y2, ... } | MOD
 *-----------------------------------
 * Rels     {  x, -1,  0,  0, ... } | n1
 *          {  0,  0,  x, -1, ... } | n2
 *          { ................... } | ...
 */
/*------------------------------------*/
typedef struct dlog_p_t {
	const bigint_t* g;
	const bigint_t* y;		/* y = g^x (mod n) */
	const bigint_t* n;
} dlog_p_t;
/*------------------------------------*/
#define dlog_p_t_ctor(dlog_p_t_ptr)	((void)0)
#define dlog_p_t_dtor(dlog_p_t_ptr)	((void)0)
/*------------------------------------*/
USE_RESULT__
bool_t dlog_proof(bigint_t* c,
				bigint_t* sx,
				const void* dat_digest,
				unsigned dat_digestlen,
				unsigned nrel,				/* nelms(dlog[]) == nrel */
				const dlog_p_t dlog[],		/* nelms(dlog[]) == nrel */
				const sphere_t* sph,
				const bigint_t* x,
				const syspar_t* sp,
				rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t dlog_proof_dat(unsigned which_sha,
					bigint_t* c,
					bigint_t* sx,
					const void* dat,			/* message data */
					unsigned datlen,	
					unsigned nrel,				/* nelms(dlog[]) == nrel */
					const dlog_p_t dlog[],		/* nelms(dlog[]) == nrel */
					const sphere_t* sph,
					const bigint_t* x,
					const syspar_t* sp,
					rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
USE_RESULT__
bool_t dlog_vrfy(const bigint_t* c,
				 const bigint_t* sx,
				 const void* dat_digest,
				 unsigned dat_digestlen,
				 unsigned nrel,				/* nelms(dlog[]) == nrel */
				 const dlog_p_t dlog[],		/* nelms(dlog[]) == nrel */
				 const sphere_t* sph,
				 const syspar_t* sp);
/*------------------------------------*/
USE_RESULT__
bool_t dlog_vrfy_dat(unsigned which_sha,
					 const bigint_t* c,
					 const bigint_t* sx,
					 const void* dat,		/* message data */
					 unsigned datlen,
					 unsigned nrel,				/* nelms(dlog[]) == nrel */
					 const dlog_p_t dlog[],		/* nelms(dlog[]) == nrel */
					 const sphere_t* sph,
					 const syspar_t* sp);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

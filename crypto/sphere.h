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
#ifndef umalccvbg_sphere_h__
#define umalccvbg_sphere_h__	1
#include "bigint.h"
#include "birnd.h"
#include "random.h"
#include "syspar.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
typedef struct sphere_t {
	unsigned log2radius;
	unsigned log2radius_ek;
	bigint_t center;
} sphere_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void print_spheres(const char* lbl, const sphere_t* sph);
/*----------------------------------------------------------------------------*/
void create_sph_lambda(sphere_t* lambda, const syspar_t* sp);
void create_sph_lambda_dsa(sphere_t* lambda_dsa, const syspar_t* sp);
void create_sph_lambda_sp(sphere_t* lambda_sp,
						  const syspar_t* org_sp, const syspar_t* curr_sp);
void create_sph_mu(sphere_t* mu, const syspar_t* sp);
void create_sph_gamma(sphere_t* gamma, const syspar_t* sp);
void create_sph_gamma_mu(sphere_t* gamma_mu, const sphere_t* gamma,
						 const sphere_t* mu, const syspar_t* sp);
void create_sph_theta(sphere_t* theta, const sphere_t* xsph);
/*----------------------------------------------------------------------------*/
void create_sph_omega(sphere_t* omega, const bigint_t n, const syspar_t* sp);
void create_sph_upsilon(sphere_t* upsilon, const bigint_t n, const syspar_t*sp);
void create_sph_psi(sphere_t* psi, const bigint_t n, const syspar_t* sp);
/*----------------------------------------------------------------------------*/
bool_t bi_belongs_sph_ek(const bigint_t x, const sphere_t* sph);
bool_t bi_belongs_sph(const bigint_t x, const sphere_t* sph);
/*----------------------------------------------------------------------------*/
void bi_random_sph_ek(bigint_t x, const sphere_t* sph, rnd_ctx_t* rnd_ctx);
void bi_random_sph(bigint_t x, const sphere_t* sph, rnd_ctx_t* rnd_ctx);
void bi_random_prime_sph_ek(bigint_t x, const sphere_t* sph,
							int ntests, rnd_ctx_t* rnd_ctx);
void bi_random_prime_sph(bigint_t x, const sphere_t* sph,
						 int ntests, rnd_ctx_t* rnd_ctx);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void sphere_t_ctor(struct sphere_t* p);
void sphere_t_dtor(struct sphere_t* p);
void sphere_t_asg(struct sphere_t* p, const struct sphere_t* o);
struct sphere_t* sphere_t_new();
struct sphere_t* sphere_t_clone(const struct sphere_t* o);
void sphere_t_delete(struct sphere_t* p);
/*------------------------------------*/
void sphere_t_swap(struct sphere_t* sph_1, struct sphere_t* sph_2);
/*-------------------------------------------------------------*/
/*- MACROS-----------------------------------------------------*/
/*-------------------------------------------------------------*/
#define bi_sph_radius(bigint_rad, p_sph)			\
	do{												\
		bi_pow2k(bigint_rad, (p_sph)->log2radius);	\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_ek_radius(bigint_rad, p_sph)				\
	do{													\
		bi_pow2k(bigint_rad, (p_sph)->log2radius_ek);	\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_min(bigint_min, p_sph)						\
	do{														\
		bi_pow2k(bigint_min, (p_sph)->log2radius);			\
		bi_sub(bigint_min, (p_sph)->center, bigint_min);	\
		bi_add_ui(bigint_min, bigint_min, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_ek_min(bigint_min, p_sph)					\
	do{														\
		bi_pow2k(bigint_min, (p_sph)->log2radius_ek);		\
		bi_sub(bigint_min, (p_sph)->center, bigint_min);	\
		bi_add_ui(bigint_min, bigint_min, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_max(bigint_max, p_sph)						\
	do{														\
		bi_pow2k(bigint_max, (p_sph)->log2radius);			\
		bi_add(bigint_max, (p_sph)->center, bigint_max);	\
		bi_sub_ui(bigint_max, bigint_max, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_ek_max(bigint_max, p_sph)					\
	do{														\
		bi_pow2k(bigint_max, (p_sph)->log2radius_ek);		\
		bi_add(bigint_max, (p_sph)->center, bigint_max);	\
		bi_sub_ui(bigint_max, bigint_max, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_min_max(bigint_min, bigint_max, p_sph)		\
	do{														\
		bi_pow2k(bigint_max, (p_sph)->log2radius);			\
		bi_asg(bigint_min, bigint_max);						\
		bi_sub(bigint_min, (p_sph)->center, bigint_min);	\
		bi_add_ui(bigint_min, bigint_min, 1);				\
		bi_add(bigint_max, (p_sph)->center, bigint_max);	\
		bi_sub_ui(bigint_max, bigint_max, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_ek_min_max(bigint_min, bigint_max, p_sph)	\
	do{														\
		bi_pow2k(bigint_max, (p_sph)->log2radius_ek);		\
		bi_asg(bigint_min, bigint_max);						\
		bi_sub(bigint_min, (p_sph)->center, bigint_min);	\
		bi_add_ui(bigint_min, bigint_min, 1);				\
		bi_add(bigint_max, (p_sph)->center, bigint_max);	\
		bi_sub_ui(bigint_max, bigint_max, 1);				\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_range(bigint_rng, p_sph)				\
	do{												\
		bi_pow2k(bigint_rng, (p_sph)->log2radius);	\
		bi_mul2(bigint_rng, bigint_rng);			\
		bi_sub_ui(bigint_rng, bigint_rng, 1);		\
	}while(0)
/*-------------------------------------------------------------*/
#define bi_sph_ek_range(bigint_rng, p_sph)				\
	do{													\
		bi_pow2k(bigint_rng, (p_sph)->log2radius_ek);	\
		bi_mul2(bigint_rng, bigint_rng);				\
		bi_sub_ui(bigint_rng, bigint_rng, 1);			\
	}while(0)
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
END_EXTERN_C
#endif

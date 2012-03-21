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
#include "sphere.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include "syspar.h"
#include "cdtor.h"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static void print_sph(const char* lbl, const sphere_t* sph)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_min_max(min, max, sph);
	fprintf(stderr, "\t%s_Min: [%u] ", lbl, bi_nbits(min));
	bi_printf(stderr, min, 16); fprintf(stderr, "\n");
	fprintf(stderr, "\t%s_Cnt: [%u] ", lbl, bi_nbits(sph->center));
	bi_printf(stderr, sph->center, 16); fprintf(stderr, "\n");
	fprintf(stderr, "\t%s_Max: [%u] ", lbl, bi_nbits(max));
	bi_printf(stderr, max, 16); fprintf(stderr, "\n");
	END_VAR_2_bigint_t(min, max);
}
/*----------------------------------------------------------------------------*/
static void print_sph_ek(const char* lbl, const sphere_t* sph)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_ek_min_max(min, max, sph);
	fprintf(stderr, "\t%s_MinEk: [%u] ", lbl, bi_nbits(min));
	bi_printf(stderr, min, 16); fprintf(stderr, "\n");
	fprintf(stderr, "\t%s_CntEk: [%u] ", lbl, bi_nbits(sph->center));
	bi_printf(stderr, sph->center, 16); fprintf(stderr, "\n");
	fprintf(stderr, "\t%s_MaxEk: [%u] ", lbl, bi_nbits(max));
	bi_printf(stderr, max, 16); fprintf(stderr, "\n");
	END_VAR_2_bigint_t(min, max);
}
/*----------------------------------------------------------------------------*/
void print_spheres(const char* lbl, const sphere_t* sph)
{
	print_sph(lbl, sph);
	print_sph_ek(lbl, sph);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static void create_sphere(sphere_t* sph,
						  unsigned log2center,
						  unsigned log2radius,
						  const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sph->log2radius = log2radius;
	/*--------------------------------*/
	sph->log2radius_ek = radius_ek(sp, sph->log2radius);
	/*--------------------------------*/
	bi_pow2k(sph->center, log2center);
	/*--------------------------------*/
	assert( bi_is2k(sph->center) );
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
static void create_sphere_ek(sphere_t* sph,
							 unsigned log2center,
							 unsigned log2radius_ek,
							 const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sph->log2radius_ek = log2radius_ek;
	/*--------------------------------*/
	sph->log2radius = radius(sp, sph->log2radius_ek);
	/*--------------------------------*/
	bi_pow2k(sph->center, log2center);
	/*--------------------------------*/
	assert( bi_is2k(sph->center) );
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
static void create_sphere_shift_ek(sphere_t* sph, const sphere_t* a)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(min);
	bi_sph_ek_min(min, a);
	/*--------------------------------*/
	sph->log2radius = a->log2radius;
	/*--------------------------------*/
	sph->log2radius_ek = a->log2radius_ek;
	/*--------------------------------*/
	bi_sub(sph->center, a->center, min);
	/*--------------------------------*/
	END_VAR_1_bigint_t(min);
	/*--------------------------------*/
	/*DBG(print_sph_ek("@", a););*/
	/*DBG(print_sph_ek("S", sph););*/
	/*--------------------------------*/
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
UNUSED__
static void create_internal_sphere_ek(sphere_t* sph,
									  const bigint_t min, const bigint_t max,
									  const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(rad);
	/*--------------------------------*/
	bi_add(sph->center, max, min);
	bi_div2(sph->center, sph->center);
	/*--------------------------------*/
	bi_sub(rad, sph->center, min);
	bi_add_ui(rad, rad, 1);
	assert(bi_nbits(rad) > 1);
	sph->log2radius_ek = bi_nbits(rad)-1;
	/*--------------------------------*/
	sph->log2radius = radius(sp, sph->log2radius_ek);
	/*--------------------------------*/
	/*DBG(print_sph_ek("I", sph););*/
	/*--------------------------------*/
	DBG(BEG_VAR_2_bigint_t(sph_min, sph_max););
	DBG(bi_sph_ek_min_max(sph_min, sph_max, sph););
	assert(bi_greatereq(sph_min, min) && bi_lesseq(sph_max, max));
	DBG(END_VAR_2_bigint_t(sph_min, sph_max););
	/*--------------------------------*/
	END_VAR_1_bigint_t(rad);
	END_BIGINT_CHK();
}

/*----------------------------------------------------------------------------*/
static void create_external_sphere_ek(sphere_t* sph,
									  const bigint_t min, const bigint_t max,
									  const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(rad);
	/*--------------------------------*/
	bi_add(sph->center, max, min);
	bi_div2(sph->center, sph->center);
	/*--------------------------------*/
	bi_sub(rad, max, sph->center);
	bi_add_ui(rad, rad, 1);
	/*--------------------------------*/
	assert(bi_nbits(rad) > 1);
	sph->log2radius_ek = bi_nbits(rad)-1;
	if (! bi_is2k(rad)) {
		++sph->log2radius_ek;
	}
	/*--------------------------------*/
	sph->log2radius = radius(sp, sph->log2radius_ek);
	/*--------------------------------*/
	/*DBG(print_sph_ek("E", sph););*/
	/*--------------------------------*/
	DBG(BEG_VAR_2_bigint_t(sph_min, sph_max););
	DBG(bi_sph_ek_min_max(sph_min, sph_max, sph););
	assert(bi_lesseq(sph_min, min) && bi_greatereq(sph_max, max));
	DBG(END_VAR_2_bigint_t(sph_min, sph_max););
	/*--------------------------------*/
	END_VAR_1_bigint_t(rad);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
static void create_sphere_mul_ek(sphere_t* r,
								 const sphere_t* a, const sphere_t* b,
								 const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	BEG_VAR_4_bigint_t(a_min, a_max, b_min, b_max);
	bi_sph_ek_min_max(a_min, a_max, a);
	bi_sph_ek_min_max(b_min, b_max, b);
	bi_mul(a_min, a_min, b_min);
	bi_mul(a_max, a_max, b_max);
	create_external_sphere_ek(r, a_min, a_max, sp);
	END_VAR_4_bigint_t(a_min, a_max, b_min, b_max);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
/*
 * create_sphere_zeta
 * Sph in Z_n {0..n-1}
 * (bi_nbits(n)-1) guarantees that the sphere is in {0..n-1}
 * Therefore the radius is a half of that: (n.nbits()-2)
 */
static void create_sphere_zeta(sphere_t* sph, const bigint_t n,
							   const syspar_t* sp)
{
	assert(bi_nbits(n) > 2);
	create_sphere(sph, bi_nbits(n)-2, bi_nbits(n)-2, sp);
	DBG(BEG_VAR_2_bigint_t(min, max););
	DBG(bi_sph_min_max(min, max, sph););
	assert(bi_equals_ui(min, 1) && bi_less(max, n));
	DBG(BEG_VAR_1(sphere_t, aux););
	DBG(create_sphere(&aux, (bi_nbits(n)-1), (bi_nbits(n)-1), sp););
	DBG(bi_sph_min_max(min, max, &aux););
	assert(bi_equals_ui(min, 1) && bi_greatereq(max, n));
	DBG(END_VAR_1(sphere_t, aux););
	DBG(END_VAR_2_bigint_t(min, max););
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void create_sph_lambda(sphere_t* lambda, const syspar_t* sp)
{
	create_sphere(lambda, sp->nu/4-1, sp->nu/4-1, sp);
	/*DBG(print_spheres("L", lambda););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_lambda_dsa(sphere_t* lambda_dsa, const syspar_t* sp)
{
	enum {
		DSA_MAX_NBITS_Q = 256
	};
	create_sphere_ek(lambda_dsa, DSA_MAX_NBITS_Q-1, DSA_MAX_NBITS_Q-1, sp);
	/*DBG(print_spheres("LDSA", lambda_dsa););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_lambda_sp(sphere_t* lambda_sp,
						  const syspar_t* org_sp, const syspar_t* curr_sp)
{
	create_sphere_ek(lambda_sp,
					 org_sp->nu/4-1,
					 radius_ek(org_sp, org_sp->nu/4-1),
					 curr_sp);
	/*DBG(print_spheres("LSP", lambda_sp););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_mu(sphere_t* mu, const syspar_t* sp)
{
	create_sphere(mu, sp->nu/2-1, sp->nu/2-1, sp);
	/*DBG(print_spheres("M", mu););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_gamma(sphere_t* gamma, const syspar_t* sp)
{
	create_sphere(gamma, sp->nu*3/4-1, sp->nu/4-1, sp);
	/*DBG(print_spheres("G", gamma););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_gamma_mu(sphere_t* gamma_mu,
						 const sphere_t* gamma,
						 const sphere_t* mu,
						 const syspar_t* sp)
{
	create_sphere_mul_ek(gamma_mu, gamma, mu, sp);
	/*DBG(print_spheres("GM", gamma_mu););*/
}
/*----------------------------------------------------------------------------*/
void create_sph_theta(sphere_t* theta,
					  const sphere_t* xsph)
{
	create_sphere_shift_ek(theta, xsph);
	/*DBG(print_spheres("T", theta););*/
}
/*----------------------------------------------------------------------------*/
/* 
 * Omega in Z_{n^2} {0..n^2-1}
 */
void create_sph_omega(sphere_t* omega, const bigint_t n, const syspar_t* sp)
{
	BEG_VAR_1_bigint_t(n2);
	bi_sq(n2, n);
	create_sphere_zeta(omega, n2, sp);
	END_VAR_1_bigint_t(n2);
	/*DBG(print_spheres("O", omega););*/
}
/*----------------------------------------------------------------------------*/
/* 
 * Upsilon in Z_{N^2/4} {0..n^2/4-1}
 */
void create_sph_upsilon(sphere_t* upsilon, const bigint_t n, const syspar_t* sp)
{
	BEG_VAR_1_bigint_t(n24);
	bi_sq(n24, n);
	bi_div2k(n24, n24, 2);
	create_sphere_zeta(upsilon, n24, sp);
	END_VAR_1_bigint_t(n24);
	/*DBG(print_spheres("U", upsilon););*/
}
/*----------------------------------------------------------------------------*/
/* 
 * Psi in Z_{N/4} {0..N/4-1}
 */
void create_sph_psi(sphere_t* psi, const bigint_t n, const syspar_t* sp)
{
	BEG_VAR_1_bigint_t(n4);
	bi_div2k(n4, n, 2);
	create_sphere_zeta(psi, n4, sp);
	END_VAR_1_bigint_t(n4);
	/*DBG(print_spheres("P", psi););*/
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
bool_t bi_belongs_sph_ek(const bigint_t x, const sphere_t* sph)
{
	bool_t ok;
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_ek_min_max(min, max, sph);
	ok = bi_greatereq(x, min) && bi_lesseq(x, max);
	END_VAR_2_bigint_t(min, max);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_belongs_sph(const bigint_t x, const sphere_t* sph)
{
	bool_t ok;
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_min_max(min, max, sph);
	ok = bi_greatereq(x, min) && bi_lesseq(x, max);
	END_VAR_2_bigint_t(min, max);
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void bi_random_sph_ek(bigint_t x, const sphere_t* sph, rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_ek_min_max(min, max, sph);
	bi_random_lub(x, min, max, rnd_ctx);
	END_VAR_2_bigint_t(min, max);
}
/*--------------------------------*/
void bi_random_sph(bigint_t x, const sphere_t* sph, rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_min_max(min, max, sph);
	bi_random_lub(x, min, max, rnd_ctx);
	END_VAR_2_bigint_t(min, max);
}
/*--------------------------------*/
void bi_random_prime_sph_ek(bigint_t x, const sphere_t* sph,
							int ntests, rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_ek_min_max(min, max, sph);
	bi_random_prime_lub(x, min, max, ntests, rnd_ctx);
	END_VAR_2_bigint_t(min, max);
}
/*--------------------------------*/
void bi_random_prime_sph(bigint_t x, const sphere_t* sph,
						 int ntests, rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_2_bigint_t(min, max);
	bi_sph_min_max(min, max, sph);
	bi_random_prime_lub(x, min, max, ntests, rnd_ctx);
	END_VAR_2_bigint_t(min, max);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int sphere_t_chk_members(struct sphere_t* p, int code)/*auto*/
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
	struct dummy_sphere_t {
		unsigned log2radius;
		unsigned log2radius_ek;
		bigint_t center;
	};
	CHK_FIELD__(dummy_sphere_t, sphere_t, log2radius);
	CHK_FIELD__(dummy_sphere_t, sphere_t, log2radius_ek);
	CHK_FIELD__(dummy_sphere_t, sphere_t, center);
	CHK_SIZE__(dummy_sphere_t, sphere_t);
	return (p!=NULL)&&(code == 490945622);
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void sphere_t_swap(struct sphere_t* p1, struct sphere_t* p2)
{
	assert(p1 != NULL && p2 != NULL);
	assert(sphere_t_chk_members(p1,490945622));
	if (p1 != p2) {
		unsigned aux;
		aux = p1->log2radius;
		p1->log2radius = p2->log2radius;
		p2->log2radius = aux;
		/*----------------------------*/
		aux = p1->log2radius_ek;
		p1->log2radius_ek = p2->log2radius_ek;
		p2->log2radius_ek = aux;
		/*----------------------------*/
		bi_swap(p1->center, p2->center);
	}
}
/*----------------------------------------------------------------------------*/
void sphere_t_ctor(struct sphere_t* p)/*auto*/
{
	assert(p != NULL);
	assert(sphere_t_chk_members(p,490945622));
	p->log2radius = 0;
	p->log2radius_ek = 0;
	bi_ctor(p->center);
}
/*----------------------------------------------------------------------------*/
void sphere_t_dtor(struct sphere_t* p)/*auto*/
{
	assert(p != NULL);
	assert(sphere_t_chk_members(p,490945622));
	bi_dtor(p->center);
}
/*----------------------------------------------------------------------------*/
void sphere_t_asg(struct sphere_t* p, const struct sphere_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(sphere_t_chk_members(p,490945622));
	if (p != o) {
		p->log2radius = o->log2radius;
		p->log2radius_ek = o->log2radius_ek;
		bi_asg(p->center, o->center);
	}
}
/*----------------------------------------------------------------------------*/
struct sphere_t* sphere_t_new()/*auto*/
{
	struct sphere_t* p = (struct sphere_t*)malloc(sizeof(struct sphere_t));
	if (p != NULL) {
		sphere_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct sphere_t* sphere_t_clone(const struct sphere_t* o)/*auto*/
{
	struct sphere_t* p = NULL;
	if (o != NULL) {
		p = (struct sphere_t*)malloc(sizeof(struct sphere_t));
		if (p != NULL) {
			sphere_t_ctor(p);
			sphere_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void sphere_t_delete(struct sphere_t* p)/*auto*/
{
	if (p != NULL) {
		sphere_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

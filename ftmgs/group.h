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
#ifndef umalccvbg_group_h__
#define umalccvbg_group_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "birnd.h"
#include "random.h"
#include "syssph.h"
#include "nadrp.h"
#include "elgamal_thr.h"
#include "paillier_thr.h"
BEGIN_EXTERN_C
/*--------------------------------------------------------------------------- */
typedef struct join_precomp_t {
	bigint_t join_gg_r_psi;		/* gg^P_center (mod n^2) */
	bigint_t join_yy_r_psi;		/* yy^P_center (mod n^2) */
	bigint_t join_hh_x_mu;		/* hh^M_center (mod n^2) */
} join_precomp_t;
/*----------------------------------- */
typedef struct sign_precomp_t {
	bigint_t sign_g_r_mu;			/* g^M_center (mod n) */
	bigint_t sign_g_e_gamma;		/* g^G_center (mod n) */
	bigint_t sign_g_h1_gamma_mu;	/* g^GM_center (mod n) */
	bigint_t sign_h_r_mu;			/* h^M_center (mod n) */
	bigint_t sign_y_h1_gamma_mu;	/* y^GM_center (mod n) */
	bigint_t sign_a_x_mu;			/* a^M_center (mod n) */
} sign_precomp_t;
/*	bigint_t sign_b_x1_lambda; */	/* b^L_center (mod n) */
/* 
 * Lambda is now flexible (it is not fixed) to allow x from DSA
 * and from other groups
 */
/*--------------------------------------------------------------------------- */
struct ftmgs_fa_pbkey_t {
	paillier_thr_pbkey_t pbkey; 				/* n, g, y, nkeys */
};
/*----------------------------------- */
struct ftmgs_faj_pbkey_share_t {
	paillier_thr_pbkey_share_t pbkey_j;			/* nadrp & reveal */
};
/*----------------------------------- */
struct ftmgs_faj_prkey_t {
	paillier_thr_prkey_t prkey_j;				/* nadrp & reveal */
};
/*----------------------------------- */
struct ftmgs_pbkey_t {
	elgamal_thr_pbkey_t gmpk;	 				/* n, g, y, nkeys */
	bigint_t a0;
	bigint_t a;
	bigint_t b;
	bigint_t h;
	paillier_thr_pbkey_t fapk; 					/* n, g, y, nkeys */
#ifdef PRECOMPUTATIONS__
	syssph_t syssph;
	sign_precomp_t sign_precomp;
	join_precomp_t join_precomp;
	nadrp_precomp_t nadrp_precomp;
#endif
};
/*----------------------------------- */
struct ftmgs_prkey_t {
	elgamal_thr_modfact_t npq;					/* join */
};
/*----------------------------------- */
struct ftmgs_faj_gr_pbkey_share_t {
	elgamal_thr_pbkey_share_t pbkey_j;
	bigint_t hj;
};
/*----------------------------------- */
struct ftmgs_faj_gr_prkey_t {
	elgamal_thr_prkey_t prkey_j;				/* open */
};
/*----------------------------------------------------------------------------*/
#ifdef PRECOMPUTATIONS__
void ftmgs_precomp_group(ftmgs_pbkey_t* gpk);
#endif
/*--------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------- */
void join_precomp_t_ctor(struct join_precomp_t* p);
void join_precomp_t_dtor(struct join_precomp_t* p);
void join_precomp_t_asg(struct join_precomp_t* p, const struct join_precomp_t* o);
struct join_precomp_t* join_precomp_t_new();
struct join_precomp_t* join_precomp_t_clone(const struct join_precomp_t* o);
void join_precomp_t_delete(struct join_precomp_t* p);
/*--------------------------------------------------------------------------- */
void sign_precomp_t_ctor(struct sign_precomp_t* p);
void sign_precomp_t_dtor(struct sign_precomp_t* p);
void sign_precomp_t_asg(struct sign_precomp_t* p, const struct sign_precomp_t* o);
struct sign_precomp_t* sign_precomp_t_new();
struct sign_precomp_t* sign_precomp_t_clone(const struct sign_precomp_t* o);
void sign_precomp_t_delete(struct sign_precomp_t* p);
/*----------------------------------------------------------------------------*/
void ftmgs_fa_pbkey_t_ctor(struct ftmgs_fa_pbkey_t* p);
void ftmgs_fa_pbkey_t_dtor(struct ftmgs_fa_pbkey_t* p);
void ftmgs_fa_pbkey_t_asg(struct ftmgs_fa_pbkey_t* p, const struct ftmgs_fa_pbkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_faj_pbkey_share_t_ctor(struct ftmgs_faj_pbkey_share_t* p);
void ftmgs_faj_pbkey_share_t_dtor(struct ftmgs_faj_pbkey_share_t* p);
void ftmgs_faj_pbkey_share_t_asg(struct ftmgs_faj_pbkey_share_t* p, const struct ftmgs_faj_pbkey_share_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_faj_prkey_t_ctor(struct ftmgs_faj_prkey_t* p);
void ftmgs_faj_prkey_t_dtor(struct ftmgs_faj_prkey_t* p);
void ftmgs_faj_prkey_t_asg(struct ftmgs_faj_prkey_t* p, const struct ftmgs_faj_prkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_pbkey_t_ctor(struct ftmgs_pbkey_t* p);
void ftmgs_pbkey_t_dtor(struct ftmgs_pbkey_t* p);
void ftmgs_pbkey_t_asg(struct ftmgs_pbkey_t* p, const struct ftmgs_pbkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_prkey_t_ctor(struct ftmgs_prkey_t* p);
void ftmgs_prkey_t_dtor(struct ftmgs_prkey_t* p);
void ftmgs_prkey_t_asg(struct ftmgs_prkey_t* p, const struct ftmgs_prkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_pbkey_share_t_ctor(struct ftmgs_faj_gr_pbkey_share_t* p);
void ftmgs_faj_gr_pbkey_share_t_dtor(struct ftmgs_faj_gr_pbkey_share_t* p);
void ftmgs_faj_gr_pbkey_share_t_asg(struct ftmgs_faj_gr_pbkey_share_t* p, const struct ftmgs_faj_gr_pbkey_share_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_faj_gr_prkey_t_ctor(struct ftmgs_faj_gr_prkey_t* p);
void ftmgs_faj_gr_prkey_t_dtor(struct ftmgs_faj_gr_prkey_t* p);
void ftmgs_faj_gr_prkey_t_asg(struct ftmgs_faj_gr_prkey_t* p, const struct ftmgs_faj_gr_prkey_t* o);
/*--------------------------------------------------------------------------- */
/*--------------------------------------------------------------------------- */
/*--------------------------------------------------------------------------- */
END_EXTERN_C
#endif
/*--------------------------------------------------------------------------- */

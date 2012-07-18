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
#ifndef umalccvbg_join_h__
#define umalccvbg_join_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "birnd.h"
#include "random.h"
#include "paillier_thr.h"
#include "nadrp.h"
#include "group.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
#define get_Ui(UiVi) ((UiVi)->alpha)
#define get_Vi(UiVi) ((UiVi)->beta)
/*----------------------------------------------------------------------------*/
/*-- DSS-DSA -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
struct dss_parms_t {
	bigint_t p;		/* zero */
	bigint_t q;		/* zero */
	bigint_t g;		/* zero */
};
/*------------------------------------*/
/* y = g^x (mod p) */
struct dsa_pbkey_t {
	bigint_t y;		/* zero */
};
/*------------------------------------*/
/* y = g^x (mod p) */
struct dsa_prkey_t {
	bigint_t x;		/* zero */
};
/*----------------------------------------------------------------------------*/
	/* y = g^x (mod n) */
struct dlogx_t {
	bigint_t x;		/* zero */
};
/*------------------------------------*/
/* y = g^x (mod n) */
struct dlog_t {
	bigint_t g;		/* zero */
	bigint_t y;		/* zero */
	bigint_t n;		/* zero */
};
/*----------------------------------------------------------------------------*/
/*-- Join --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct join_proof_t {
	bigint_t c;			/* zero */
	bigint_t sx1;		/* zero */
	bigint_t sr;		/* zero */
	bigint_t sx;		/* zero */
} join_proof_t;
/*----------------------------------------------------------------------------*/
typedef struct join_u1prv_t {
	nadrp_a1prv_t nadrp_a1;
	bigint_t x1i;		/* zero */
} join_u1prv_t;
/*----------------------------------------------------------------------------*/
typedef struct join_u1pbl_t {
	nadrp_a1pbl_t nadrp_a1;
	bigint_t Ci;		/* zero */
} join_u1pbl_t;
/*----------------------------------------------------------------------------*/
typedef struct join_gm2pbl_t {
	nadrp_b2pbl_t nadrp_b2;
} join_gm2pbl_t;
/*----------------------------------------------------------------------------*/
typedef struct join_u3prv_t {
	nadrp_a3prv_t nadrp_a3;
	bigint_t Xi;		/* zero */
} join_u3prv_t;
/*----------------------------------------------------------------------------*/
typedef struct join_u3pbl_t {
	nadrp_a3pbl_t nadrp_a3;
	paillier_thr_ciphertext_t UiVi;	/* alpha: Ui ; beta: Vi */
	join_proof_t Eiproof;
} join_u3pbl_t;
/*----------------------------------------------------------------------------*/
/* 
 * typedef struct join_gm4prv_t {
 * 	nadrp_b4pbl_t nadrp_b4;
 * } join_gm4prv_t;
 */
/*----------------------------------------------------------------------------*/
typedef struct join_gm4pbl_t {
	bigint_t Ai;		/* zero */
	bigint_t ei;		/* zero */
} join_gm4pbl_t;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
struct ftmgs_join_prv_t {
	unsigned status;
	join_u1prv_t u1;
	join_u3prv_t u3;
};
/*----------------------------------------------------------------------------*/
struct ftmgs_join_pbl_t {
	unsigned status;
	join_u1pbl_t u1;
	join_gm2pbl_t gm2;
	join_u3pbl_t u3;
	join_gm4pbl_t gm4;
};
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_ref_t {
	bigint_t Ai;		/* zero */
	bigint_t ei;		/* zero */
	bigint_t Ci;		/* zero */
	bigint_t Xi;		/* zero */
	paillier_thr_ciphertext_t UiVi;	/* alpha: Ui ; beta: Vi */
	dlog_t user_auth;
	join_proof_t Eiproof;
};
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_prkey_t {
	bigint_t Ai;		/* zero */
	bigint_t ei;		/* zero */
	bigint_t xi;		/* zero */
	bigint_t x1i;		/* zero */
};
/*----------------------------------------------------------------------------*/
/*
 *					JOIN
 *		--------------------------------
 * PUBL:		<gpk,uauth>
 *		--------------------------------
 * PRIV: <umk>					<gsk>
 *		--------------------------------
 *		U:							GM:
 *		--------------------------------
 *		+---+
 *		| 1 |
 *		+---+
 *			  ----- Ci,NADRP1 ----->
 *									+---+
 *									| 2 |
 *									+---+
 *		  	 <------- NADRP2 ------
 *		+---+
 *		| 3 |
 *		+---+
 *			  -- Ui,Vi,EiP,NADRP3 ->
 *									+---+
 *									| 4 |
 *									+---+
 *		  	 <------- Ai,ei -------
 *		+---+
 *		| 5 |
 *		+---+
 *		------------------------------------------
 *		 <x1i,xi,Ai,ei>		<Ci,Xi,Ui,Vi,EiP,Ai,ei>
 *		------------------------------------------
 */
/*----------------------------------------------------------------------------*/
void ftmgs_hash_join_log(unsigned which_sha,
						 void* mr_digest,
						 unsigned* mr_digestlen,
						 const ftmgs_mbr_ref_t* mr);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void dlogx_t_ctor(struct dlogx_t* p);
void dlogx_t_dtor(struct dlogx_t* p);
void dlogx_t_asg(struct dlogx_t* p, const struct dlogx_t* o);
void dlogx_t_move(struct dlogx_t* p, struct dlogx_t* o);
/*----------------------------------------------------------------------------*/
void dlog_t_ctor(struct dlog_t* p);
void dlog_t_dtor(struct dlog_t* p);
void dlog_t_asg(struct dlog_t* p, const struct dlog_t* o);
void dlog_t_move(struct dlog_t* p, struct dlog_t* o);
/*----------------------------------------------------------------------------*/
void join_proof_t_ctor(struct join_proof_t* p);
void join_proof_t_dtor(struct join_proof_t* p);
void join_proof_t_asg(struct join_proof_t* p, const struct join_proof_t* o);
void join_proof_t_move(struct join_proof_t* p, struct join_proof_t* o);
struct join_proof_t* join_proof_t_new();
struct join_proof_t* join_proof_t_clone(const struct join_proof_t* o);
void join_proof_t_delete(struct join_proof_t* p);
/*----------------------------------------------------------------------------*/
void join_u1prv_t_ctor(struct join_u1prv_t* p);
void join_u1prv_t_dtor(struct join_u1prv_t* p);
void join_u1prv_t_asg(struct join_u1prv_t* p, const struct join_u1prv_t* o);
void join_u1prv_t_move(struct join_u1prv_t* p, struct join_u1prv_t* o);
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_ctor(struct join_u1pbl_t* p);
void join_u1pbl_t_dtor(struct join_u1pbl_t* p);
void join_u1pbl_t_asg(struct join_u1pbl_t* p, const struct join_u1pbl_t* o);
void join_u1pbl_t_move(struct join_u1pbl_t* p, struct join_u1pbl_t* o);
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_ctor(struct join_gm2pbl_t* p);
void join_gm2pbl_t_dtor(struct join_gm2pbl_t* p);
void join_gm2pbl_t_asg(struct join_gm2pbl_t* p, const struct join_gm2pbl_t* o);
void join_gm2pbl_t_move(struct join_gm2pbl_t* p, struct join_gm2pbl_t* o);
/*----------------------------------------------------------------------------*/
void join_u3prv_t_ctor(struct join_u3prv_t* p);
void join_u3prv_t_dtor(struct join_u3prv_t* p);
void join_u3prv_t_asg(struct join_u3prv_t* p, const struct join_u3prv_t* o);
void join_u3prv_t_move(struct join_u3prv_t* p, struct join_u3prv_t* o);
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_ctor(struct join_u3pbl_t* p);
void join_u3pbl_t_dtor(struct join_u3pbl_t* p);
void join_u3pbl_t_asg(struct join_u3pbl_t* p, const struct join_u3pbl_t* o);
void join_u3pbl_t_move(struct join_u3pbl_t* p, struct join_u3pbl_t* o);
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_ctor(struct join_gm4pbl_t* p);
void join_gm4pbl_t_dtor(struct join_gm4pbl_t* p);
void join_gm4pbl_t_asg(struct join_gm4pbl_t* p, const struct join_gm4pbl_t* o);
void join_gm4pbl_t_move(struct join_gm4pbl_t* p, struct join_gm4pbl_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_ctor(struct ftmgs_mbr_ref_t* p);
void ftmgs_mbr_ref_t_dtor(struct ftmgs_mbr_ref_t* p);
void ftmgs_mbr_ref_t_asg(struct ftmgs_mbr_ref_t* p, const struct ftmgs_mbr_ref_t* o);
void ftmgs_mbr_ref_t_move(struct ftmgs_mbr_ref_t* p, struct ftmgs_mbr_ref_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_ctor(struct ftmgs_mbr_prkey_t* p);
void ftmgs_mbr_prkey_t_dtor(struct ftmgs_mbr_prkey_t* p);
void ftmgs_mbr_prkey_t_asg(struct ftmgs_mbr_prkey_t* p, const struct ftmgs_mbr_prkey_t* o);
void ftmgs_mbr_prkey_t_move(struct ftmgs_mbr_prkey_t* p, struct ftmgs_mbr_prkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_ctor(struct ftmgs_join_prv_t* p);
void ftmgs_join_prv_t_dtor(struct ftmgs_join_prv_t* p);
void ftmgs_join_prv_t_asg(struct ftmgs_join_prv_t* p, const struct ftmgs_join_prv_t* o);
void ftmgs_join_prv_t_move(struct ftmgs_join_prv_t* p, struct ftmgs_join_prv_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_ctor(struct ftmgs_join_pbl_t* p);
void ftmgs_join_pbl_t_dtor(struct ftmgs_join_pbl_t* p);
void ftmgs_join_pbl_t_asg(struct ftmgs_join_pbl_t* p, const struct ftmgs_join_pbl_t* o);
void ftmgs_join_pbl_t_move(struct ftmgs_join_pbl_t* p, struct ftmgs_join_pbl_t* o);
/*----------------------------------------------------------------------------*/
join_u1prv_t* join_u1prv_t_new();
join_u1prv_t* join_u1prv_t_clone(const join_u1prv_t* o);
void join_u1prv_t_delete(join_u1prv_t* p);
/*--------------------------------*/
join_u1pbl_t* join_u1pbl_t_new();
join_u1pbl_t* join_u1pbl_t_clone(const join_u1pbl_t* o);
void join_u1pbl_t_delete(join_u1pbl_t* p);
/*--------------------------------*/
join_gm2pbl_t* join_gm2pbl_t_new();
join_gm2pbl_t* join_gm2pbl_t_clone(const join_gm2pbl_t* o);
void join_gm2pbl_t_delete(join_gm2pbl_t* p);
/*--------------------------------*/
join_u3prv_t* join_u3prv_t_new();
join_u3prv_t* join_u3prv_t_clone(const join_u3prv_t* o);
void join_u3prv_t_delete(join_u3prv_t* p);
/*--------------------------------*/
join_u3pbl_t* join_u3pbl_t_new();
join_u3pbl_t* join_u3pbl_t_clone(const join_u3pbl_t* o);
void join_u3pbl_t_delete(join_u3pbl_t* p);
/*--------------------------------*/
join_gm4pbl_t* join_gm4pbl_t_new();
join_gm4pbl_t* join_gm4pbl_t_clone(const join_gm4pbl_t* o);
void join_gm4pbl_t_delete(join_gm4pbl_t* p);
/*----------------------------------------------------------------------------*/
void dss_parms_t_ctor(struct dss_parms_t* p);
void dss_parms_t_dtor(struct dss_parms_t* p);
void dss_parms_t_asg(struct dss_parms_t* p, const struct dss_parms_t* o);
void dss_parms_t_move(struct dss_parms_t* p, struct dss_parms_t* o);
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_ctor(struct dsa_pbkey_t* p);
void dsa_pbkey_t_dtor(struct dsa_pbkey_t* p);
void dsa_pbkey_t_asg(struct dsa_pbkey_t* p, const struct dsa_pbkey_t* o);
void dsa_pbkey_t_move(struct dsa_pbkey_t* p, struct dsa_pbkey_t* o);
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_ctor(struct dsa_prkey_t* p);
void dsa_prkey_t_dtor(struct dsa_prkey_t* p);
void dsa_prkey_t_asg(struct dsa_prkey_t* p, const struct dsa_prkey_t* o);
void dsa_prkey_t_move(struct dsa_prkey_t* p, struct dsa_prkey_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

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
#include "join.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "iexport.h"
#include "sphere.h"
#include "sok.h"
#include "syssph.h"
#include "group.h"
#include "paillier_thr.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
enum {
	JOIN_ERROR = 1000, JOIN_OK
};
/*----------------------------------------------------------------------------*/
STATIC_ASSERT(JOIN_ERROR > 100
			  && JOIN_ERROR < JOIN_OK,
			  Bad_joincode_definition);
/*----------------------------------------------------------------------------*/
/*-- DSS-DSA -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void extract_dsa_from_umk(dsa_prkey_t* dsa_sk, const dlogx_t* x)
{
	bi_asg(dsa_sk->x, x->x);
}
/*----------------------------------------------------------------------------*/
void extract_umk_from_dsa(dlogx_t* x, const dsa_prkey_t* dsa_sk)
{
	bi_asg(x->x, dsa_sk->x);
}
/*----------------------------------------------------------------------------*/
void extract_uauth_from_dsa(dlog_t* uauth,
							const dsa_pbkey_t* dsa_pk,
							const dss_parms_t* dss_parms)
{
	bi_asg(uauth->n, dss_parms->p);
	bi_asg(uauth->g, dss_parms->g);
	bi_asg(uauth->y, dsa_pk->y);
}
/*----------------------------------------------------------------------------*/
void extract_umk_from_msk(dlogx_t* x, const ftmgs_mbr_prkey_t* msk)
{
	bi_asg(x->x, msk->x1i);
}
/*----------------------------------------------------------------------------*/
/*-- Join --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t ftmgs_join_user_1(join_u1prv_t* u1prv,
								join_u1pbl_t* u1pbl,
								const dlogx_t* umk,
								const ftmgs_pbkey_t* gpk,
								rnd_ctx_t* rnd_ctx)
{
	bool_t ask1;
	INIT_TIMER(Join1);
	BEG_VAR_1(nadrp_parms_t, p);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&gpk->syssph.mu,
				&gpk->nadrp_precomp);
	ask1 = nadrp_a_1(&u1prv->nadrp_a1, &u1pbl->nadrp_a1, &p, rnd_ctx);
	if (ask1) {
		if (umk == NULL) {
			bi_random_sph_ek(u1prv->x1i,
							 get_sphere_lambda(&gpk->gmpk.sp, &gpk->syssph),
							 rnd_ctx);
			bi_powmod_sec(u1pbl->Ci, gpk->b, u1prv->x1i, gpk->gmpk.n);
		} else if (bi_belongs_sph_ek(umk->x,
									 get_sphere_lambda_x(umk->x,
														 &gpk->syssph))) {
			bi_asg(u1prv->x1i, umk->x);
			bi_powmod_sec(u1pbl->Ci, gpk->b, u1prv->x1i, gpk->gmpk.n);
		} else {
			ask1 = FALSE;
		}
		assert((ask1 == FALSE)
			   ||bi_belongs_sph_ek(u1prv->x1i,
								   get_sphere_lambda_x(u1prv->x1i,
													   &gpk->syssph)));
	}
#else
	BEG_VAR_1(syssph_t, syssph);
	create_syssph(&syssph, &gpk->gmpk.sp);
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&syssph.mu);
	ask1 = nadrp_a_1(&u1prv->nadrp_a1, &u1pbl->nadrp_a1, &p, rnd_ctx);
	if (ask1) {
		if (umk == NULL) {
			bi_random_sph_ek(u1prv->x1i,
							 get_sphere_lambda(&gpk->gmpk.sp, &syssph),
							 rnd_ctx);
			bi_powmod_sec(u1pbl->Ci, gpk->b, u1prv->x1i, gpk->gmpk.n);
		} else if (bi_belongs_sph_ek(umk->x,
									 get_sphere_lambda_x(umk->x,
														 &syssph))) {
			bi_asg(u1prv->x1i, umk->x);
			bi_powmod_sec(u1pbl->Ci, gpk->b, u1prv->x1i, gpk->gmpk.n);
		} else {
			ask1 = FALSE;
		}
		assert((ask1 == FALSE)
			   ||bi_belongs_sph_ek(u1prv->x1i,
								   get_sphere_lambda_x(u1prv->x1i,
													   &syssph)));
	}
	END_VAR_1(syssph_t, syssph);
#endif
	/*--------------------------------*/
	END_VAR_1(nadrp_parms_t, p);
	PRINT_TIMER(Join1);
	assert(ask1);
	return ask1;
}
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t ftmgs_join_gm_2(join_gm2pbl_t* gm2pbl,
							  const join_u1pbl_t* u1pbl,
							  const ftmgs_pbkey_t* gpk,
							  rnd_ctx_t* rnd_ctx)
{
	bool_t bsk1;
	INIT_TIMER(Join2);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(nadrp_parms_t, p);
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&gpk->syssph.mu,
				&gpk->nadrp_precomp);
	bsk1 = nadrp_b_2(&gm2pbl->nadrp_b2, &u1pbl->nadrp_a1, &p, rnd_ctx);
	END_VAR_1(nadrp_parms_t, p);
#else
	BEG_VAR_2(nadrp_parms_t, p, sphere_t, mu);
	create_sph_mu(&mu, &gpk->gmpk.sp);
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&mu);
	bsk1 = nadrp_b_2(&gm2pbl->nadrp_b2, &u1pbl->nadrp_a1, &p, rnd_ctx);
	END_VAR_2(nadrp_parms_t, p, sphere_t, mu);
#endif
	/*--------------------------------*/
	PRINT_TIMER(Join2);
	assert(bsk1);
	return bsk1;
}
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t join_ei_proof(join_proof_t* Eiproof,
						  const bigint_t* x1,
						  const bigint_t* r,
						  const bigint_t* x,
						  const bigint_t* Ci,
						  const bigint_t* Xi,
						  const paillier_thr_ciphertext_t* UiVi,
						  const dlog_t* u_auth,
						  const ftmgs_pbkey_t* gpk,
						  rnd_ctx_t* rnd_ctx);
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t ftmgs_join_user_3(join_u3prv_t* u3prv,
								join_u3pbl_t* u3pbl,
								const dlog_t* u_auth,
								const join_gm2pbl_t* gm2pbl,
								const join_u1prv_t* u1prv,
								const join_u1pbl_t* u1pbl,
								const ftmgs_pbkey_t* gpk,
								rnd_ctx_t* rnd_ctx)
{
	bool_t ask2 = TRUE;
	INIT_TIMER(Join3);
	if (u_auth != NULL) {
		BEG_VAR_1_bigint_t(aux);
		bi_powmod_sec(aux, u_auth->g, u1prv->x1i, u_auth->n);
		if (! bi_equals(aux, u_auth->y)) {
			DBG(fprintf(stderr, "Umk is not Log_g(y) (mod n)\n"););
			ask2 = FALSE;
		}
		END_VAR_1_bigint_t(aux);
	}
	if (ask2) {
		/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
		BEG_VAR_1(nadrp_parms_t, p);
		nadrp_parms(&p,
					&gpk->gmpk.n,
					&gpk->a,
					&gpk->gmpk.g,
					&gpk->h,
					&gpk->gmpk.y,
					&gpk->gmpk.sp,
					&gpk->syssph.mu,
					&gpk->nadrp_precomp);
		ask2 = nadrp_a_3(&u3prv->nadrp_a3,
						 &u3pbl->nadrp_a3,
						 &u1prv->nadrp_a1,
						 &u1pbl->nadrp_a1,
						 &gm2pbl->nadrp_b2,
						 &p, rnd_ctx);
		END_VAR_1(nadrp_parms_t, p);
#else
		BEG_VAR_2(nadrp_parms_t, p, sphere_t, mu);
		create_sph_mu(&mu, &gpk->gmpk.sp);
		nadrp_parms(&p,
					&gpk->gmpk.n,
					&gpk->a,
					&gpk->gmpk.g,
					&gpk->h,
					&gpk->gmpk.y,
					&gpk->gmpk.sp,
					&mu);
		ask2 = nadrp_a_3(&u3prv->nadrp_a3,
						 &u3pbl->nadrp_a3,
						 &u1prv->nadrp_a1,
						 &u1pbl->nadrp_a1,
						 &gm2pbl->nadrp_b2,
						 &p, rnd_ctx);
		END_VAR_2(nadrp_parms_t, p, sphere_t, mu);
#endif
		/*--------------------------------*/
	}
	if (ask2) {
		BEG_VAR_1_bigint_t(r);
		paillier_thr_encrypt_k(&u3pbl->UiVi, r, u3prv->nadrp_a3.x, &gpk->fapk,
							   rnd_ctx);
		bi_powmod_sec(u3prv->Xi, gpk->a, u3prv->nadrp_a3.x, gpk->gmpk.n);
		ask2 = join_ei_proof(&u3pbl->Eiproof,
					  &u1prv->x1i,
					  (const bigint_t*)&r,
					  (const bigint_t*)&u3prv->nadrp_a3.x,
					  &u1pbl->Ci,
					  (const bigint_t*)&u3prv->Xi,
					  &u3pbl->UiVi,
					  u_auth,
					  gpk, rnd_ctx);
		END_VAR_1_bigint_t(r);
	}
	PRINT_TIMER(Join3);
	assert(ask2);
	return ask2;		
}
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t join_ei_vrfy(const join_proof_t* Eiproof,
						   const bigint_t* Ci,
						   const bigint_t* Xi,
						   const paillier_thr_ciphertext_t* UiVi,
						   const dlog_t* u_auth,
						   const ftmgs_pbkey_t* gpk);
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t ftmgs_join_gm_4(ftmgs_mbr_ref_t* mr,
							  join_gm4pbl_t* gm4pbl,
							  const dlog_t* u_auth,
							  const join_u3pbl_t* u3pbl,
							  const join_gm2pbl_t* gm2pbl,
							  const join_u1pbl_t* u1pbl,
							  const ftmgs_prkey_t* gsk,
							  const ftmgs_pbkey_t* gpk,
							  rnd_ctx_t* rnd_ctx)
{
	bool_t bsk2;
	INIT_TIMER(Join4);
	BEG_VAR_1(nadrp_b4pbl_t, nadrp_b4pbl);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(nadrp_parms_t, p);
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&gpk->syssph.mu,
				&gpk->nadrp_precomp);
	bsk2 = nadrp_b_4(&nadrp_b4pbl,
					 &u1pbl->nadrp_a1,
					 &gm2pbl->nadrp_b2,
					 &u3pbl->nadrp_a3,
					 &p);
	END_VAR_1(nadrp_parms_t, p);
#else
	BEG_VAR_2(nadrp_parms_t, p, sphere_t, mu);
	create_sph_mu(&mu, &gpk->gmpk.sp);
	nadrp_parms(&p,
				&gpk->gmpk.n,
				&gpk->a,
				&gpk->gmpk.g,
				&gpk->h,
				&gpk->gmpk.y,
				&gpk->gmpk.sp,
				&mu);
	bsk2 = nadrp_b_4(&nadrp_b4pbl,
					 &u1pbl->nadrp_a1,
					 &gm2pbl->nadrp_b2,
					 &u3pbl->nadrp_a3,
					 &p);
	END_VAR_2(nadrp_parms_t, p, sphere_t, mu);
#endif
	/*--------------------------------*/
	if (bsk2) {
		bsk2 = join_ei_vrfy(&u3pbl->Eiproof,
							&u1pbl->Ci,
							(const bigint_t*)&nadrp_b4pbl.ax,
							&u3pbl->UiVi,
							u_auth,
							gpk);
		if (! bsk2) {
			DBG(fprintf(stderr, "join_proof failure\n"););
		} else {
			bi_asg(mr->Ci, u1pbl->Ci);
			bi_asg(mr->Xi, nadrp_b4pbl.ax);
			paillier_thr_ciphertext_t_asg(&mr->UiVi, &u3pbl->UiVi);
			join_proof_t_asg(&mr->Eiproof, &u3pbl->Eiproof);
			if (u_auth == NULL) {
				bi_asg_ui(mr->user_auth.n, 0);
				bi_asg_ui(mr->user_auth.g, 0);
				bi_asg_ui(mr->user_auth.y, 0);
			} else {
				dlog_t_asg(&mr->user_auth, u_auth);
			}
			BEG_VAR_4_bigint_t(p_1, q_1, phi, inv_ei);
#ifdef PRECOMPUTATIONS__
			bi_random_prime_sph_ek(mr->ei, &gpk->syssph.gamma,DEF_NTESTS_LOWSEC,
								   rnd_ctx);
#else
			BEG_VAR_1(sphere_t, gamma);
			create_sph_gamma(&gamma, &gpk->gmpk.sp);
			bi_random_prime_sph_ek(mr->ei, &gamma, DEF_NTESTS_LOWSEC, rnd_ctx);
			END_VAR_1(sphere_t, gamma);
#endif
			bi_sub_ui(p_1, gsk->npq.p, 1);
			bi_sub_ui(q_1, gsk->npq.q, 1);
			bi_mul(phi, p_1, q_1);
			assert(bi_iscoprime(mr->ei, phi));
			bi_invmod(inv_ei, mr->ei, phi);
			bi_mulmod(mr->Ai, gpk->a0, mr->Xi, gpk->gmpk.n); /* Ai=a0*a^x */
			bi_mulmod(mr->Ai, mr->Ai, mr->Ci, gpk->gmpk.n); /* Ai=a0*a^x*b^x1 */
			bi_powmod_sec(mr->Ai, mr->Ai, inv_ei, gpk->gmpk.n); /* Ai=(Ai)^(1/ei) */
			/*------------------------*/
			bi_asg(gm4pbl->Ai, mr->Ai);
			bi_asg(gm4pbl->ei, mr->ei);
			/*------------------------*/
			END_VAR_4_bigint_t(p_1, q_1, phi, inv_ei);
		}
	}
	END_VAR_1(nadrp_b4pbl_t, nadrp_b4pbl);
	PRINT_TIMER(Join4);
	assert(bsk2);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
#define NPOWVRFYJOINLOG		10
#else
#define NPOWVRFYJOINLOG		10
#endif
	assert((dec_powmod_cnt(NPOWVRFYJOINLOG), ftmgs_vrfy_join_log(mr, gpk)));
#undef NPOWVRFYJOINLOG
	/*--------------------------------*/
	return bsk2;
}
/*----------------------------------------------------------------------------*/
USE_RESULT__
static bool_t ftmgs_join_user_5(ftmgs_mbr_prkey_t* msk,
								const join_gm4pbl_t* gm4pbl,
								const join_u3prv_t* u3prv,
								const join_u3pbl_t* u3pbl UNUSED__,
								const join_gm2pbl_t* gm2pbl UNUSED__,
								const join_u1prv_t* u1prv,
								const join_u1pbl_t* u1pbl,
								const ftmgs_pbkey_t* gpk,
								rnd_ctx_t* rnd_ctx UNUSED__)
{
	bool_t ok;
	INIT_TIMER(Join5);
	BEG_VAR_2_bigint_t(Aiei, a0_ax_bx1);
	bi_powmod_sec(Aiei, gm4pbl->Ai, gm4pbl->ei, gpk->gmpk.n);
	bi_mulmod(a0_ax_bx1, gpk->a0, u3prv->Xi, gpk->gmpk.n);
	bi_mulmod(a0_ax_bx1, a0_ax_bx1, u1pbl->Ci, gpk->gmpk.n);
	if (! bi_equals(Aiei, a0_ax_bx1)) {
		DBG(fprintf(stderr, "Aiei failure\n"););
		ok = FALSE;
	} else {
		ok = TRUE;
		bi_asg(msk->Ai, gm4pbl->Ai);
		bi_asg(msk->ei, gm4pbl->ei);
		bi_asg(msk->xi, u3prv->nadrp_a3.x);
		bi_asg(msk->x1i, u1prv->x1i);
	}
	END_VAR_2_bigint_t(Aiei, a0_ax_bx1);
	PRINT_TIMER(Join5);
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
bool_t ftmgs_vrfy_join_log(const ftmgs_mbr_ref_t* mr,
						   const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	INIT_TIMER(VrfyJoinLog);
	BEG_VAR_2_bigint_t(Aiei, a0_ax_bx1);
	bi_powmod_sec(Aiei, mr->Ai, mr->ei, gpk->gmpk.n);
	bi_mulmod(a0_ax_bx1, gpk->a0, mr->Xi, gpk->gmpk.n);
	bi_mulmod(a0_ax_bx1, a0_ax_bx1, mr->Ci, gpk->gmpk.n);
	if (! bi_equals(Aiei, a0_ax_bx1)) {
		ok = FALSE;
	} else {
		const dlog_t* u_auth = NULL;
		if ( ! bi_iszero(mr->user_auth.n)) {
			u_auth = &mr->user_auth;
		}
		ok = join_ei_vrfy(&mr->Eiproof,
						  &mr->Ci,
						  &mr->Xi,
						  &mr->UiVi,
						  u_auth,
						  gpk);
	}
	END_VAR_2_bigint_t(Aiei, a0_ax_bx1);
	PRINT_TIMER(VrfyJoinLog);
	return ok;
}
/*----------------------------------------------------------------------------*/
/* 
 * -------------------------------------------------
 * while ((rc = ftmgs_join_usr(...)) == UNFINISHED) {
 *	sent_data(...);
 *	recv_data(...);
 * }
 * if (rc != OK) { ERROR; }
 * -------------------------------------------------
 * do {
 * 	recv_data(...);
 * 	rc = ftmgs_join_gm(...);
 * 	if (rc != ERROR) {
 * 		send_data(...);
 * 	}
 * } while (rc == UNFINISHED);
 * if (rc != OK) { ERROR; }
 * -------------------------------------------------
 */
/*----------------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_join_usr(ftmgs_join_prv_t* prv,
							   ftmgs_join_pbl_t* pbl,
							   ftmgs_mbr_prkey_t* msk,
							   const ftmgs_pbkey_t* gpk,
							   const dlogx_t* umk,
							   const dlog_t* u_auth,
							   rnd_ctx_t* rnd_ctx)
{
	ftmgs_retcode_t rc = FTMGS_ERROR;
	switch (pbl->status) {
	case 0:
		if ((prv->status == 0)
			&& ftmgs_join_user_1(&prv->u1, &pbl->u1, umk, gpk, rnd_ctx)) {
			++prv->status;
			++pbl->status;
			rc = FTMGS_UNFINISHED;
		} else {
			prv->status = pbl->status = JOIN_ERROR;
		}
		break;
	case 2:
		if ((prv->status == 1)
			&& ftmgs_join_user_3(&prv->u3, &pbl->u3, u_auth, &pbl->gm2,
								 &prv->u1, &pbl->u1, gpk, rnd_ctx)) {
			++prv->status;
			++pbl->status;
			rc = FTMGS_UNFINISHED;
		} else {
			prv->status = pbl->status = JOIN_ERROR;
		}
		break;
	case 4:
		if ((prv->status == 2)
			&& ftmgs_join_user_5(msk, &pbl->gm4, &prv->u3, &pbl->u3,
								 &pbl->gm2, &prv->u1, &pbl->u1,
								 gpk, rnd_ctx)) {
			prv->status = pbl->status = JOIN_OK;
			rc = FTMGS_OK;
		} else {
			prv->status = pbl->status = JOIN_ERROR;
		}
		break;
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
ftmgs_retcode_t ftmgs_join_gm(ftmgs_join_pbl_t* pbl,
							  ftmgs_mbr_ref_t* mr,
							  const ftmgs_pbkey_t* gpk,
							  const ftmgs_prkey_t* gsk,
							  const dlog_t* u_auth,
							  rnd_ctx_t* rnd_ctx)
{
	ftmgs_retcode_t rc = FTMGS_ERROR;
	switch (pbl->status) {
	case 1:
		if (ftmgs_join_gm_2(&pbl->gm2, &pbl->u1, gpk, rnd_ctx)) {
			++pbl->status;
			rc = FTMGS_UNFINISHED;
		} else {
			pbl->status = JOIN_ERROR;
		}
		break;
	case 3:
		if (ftmgs_join_gm_4(mr, &pbl->gm4, u_auth, &pbl->u3, &pbl->gm2,
							&pbl->u1, gsk, gpk, rnd_ctx)) {
			++pbl->status;
			rc = FTMGS_OK;
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
#define NPOWVRFYJOINLOG		10
#else
#define NPOWVRFYJOINLOG		15
#endif
			assert((dec_powmod_cnt(NPOWVRFYJOINLOG), ftmgs_vrfy_join_log(mr, gpk)));
#undef NPOWVRFYJOINLOG
	/*--------------------------------*/
		} else {
			pbl->status = JOIN_ERROR;
		}
		break;
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
void ftmgs_hash_join_log(unsigned which_sha,
						 void* mr_digest,
						 unsigned* mr_digestlen,
						 const ftmgs_mbr_ref_t* mr)
{
	USHAContext sha_ctx;
	bi_sha_reset(&sha_ctx, which_sha);
	bi_sha_input(&sha_ctx, mr->Ai);
	bi_sha_input(&sha_ctx, mr->ei);
	bi_sha_input(&sha_ctx, mr->Ci);
	bi_sha_input(&sha_ctx, mr->Xi);
	bi_sha_input(&sha_ctx, get_Ui(&mr->UiVi));
	bi_sha_input(&sha_ctx, get_Vi(&mr->UiVi));
	bi_sha_input(&sha_ctx, mr->user_auth.g);
	bi_sha_input(&sha_ctx, mr->user_auth.y);
	bi_sha_input(&sha_ctx, mr->user_auth.n);
	bi_sha_input(&sha_ctx, mr->Eiproof.c);
	bi_sha_input(&sha_ctx, mr->Eiproof.sx1);
	bi_sha_input(&sha_ctx, mr->Eiproof.sr);
	bi_sha_input(&sha_ctx, mr->Eiproof.sx);
	sha_result(&sha_ctx, mr_digest, mr_digestlen);
}
/*----------------------------------------------------------------------------*/
bool_t ftmgs_join_prtcl(ftmgs_mbr_prkey_t* msk,
						ftmgs_mbr_ref_t* mr,
						const dlogx_t* umk,
						const dlog_t* u_auth,
						const ftmgs_prkey_t* gsk,
						const ftmgs_pbkey_t* gpk,
						rnd_ctx_t* rnd_ctx)
{
	ftmgs_retcode_t rcu, rcgm = FTMGS_UNFINISHED;
	INIT_TIMER(Join);
	BEG_VAR_2(ftmgs_join_prv_t, uprv,
			  ftmgs_join_pbl_t, pbl);
	/*--------------------------------*/
	/*Usr -> GM: <UsrId, u_auth>*/ 
	/*GM -> Usr: <gpk> */ 
	rcu = ftmgs_join_usr(&uprv, &pbl, msk, gpk, umk, u_auth, rnd_ctx);
	while (rcu == FTMGS_UNFINISHED) {
		/*Usr -> GM: <pbl> */ 
		rcgm = ftmgs_join_gm(&pbl, mr, gpk, gsk, u_auth, rnd_ctx);
		/*GM -> Usr:  <pbl> */ 
		rcu = ftmgs_join_usr(&uprv, &pbl, msk, gpk, umk, u_auth, rnd_ctx);
	}
	/* GM: db_store(UsrId, <gpk, mr>);*/
	/* Usr: db_store(UsrId, <gpk, msk>);*/
	/*--------------------------------*/
	END_VAR_2(ftmgs_join_prv_t, uprv,
			  ftmgs_join_pbl_t, pbl);
	/*--------------------------------*/
	PRINT_TIMER(Join);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
#define NPOWVRFYJOINLOG		10
#else
#define NPOWVRFYJOINLOG		15
#endif
	assert((dec_powmod_cnt(NPOWVRFYJOINLOG), ftmgs_vrfy_join_log(mr, gpk)));
#undef NPOWVRFYJOINLOG
	/*--------------------------------*/
	return ((rcu == FTMGS_OK) && (rcgm == FTMGS_OK));
}
/*----------------------------------------------------------------------------*/
#define array_sz(array__) (sizeof(array__)/sizeof(*(array__)))
/*----------------------------------------------------------------------------*/
/*
 *--------------------------------
 * PK{(x', r, x): Ci=b^x' (mod n);
 *				  Xi=a^x (mod n) ;
 *				  Ui=G^r (mod N^2) ;
 *				  Vi=Y^r H^x (mod N^2) }(msg)
 *--------------------------------
 * Vbles	{ x', r,  x  }
 * Spheres	 Lek Pek Mek 
 *----------------------------------------------------
 * Objs		{  X,  a,  U,  G,  V,  Y,  H,  C,  b  } | MOD
 *---------------------------------------------------------------
 * Rels		{ -1,  x,  0,  0,  0,  0,  0,  0,  0  } | n
 *			{  0,  0, -1,  r,  0,  0,  0,  0,  0  } | N^2
 *			{  0,  0,  0,  0, -1,  r,  x,  0,  0  } | N^2
 *			{  0,  0,  0,  0,  0,  0,  0, -1,  x' } | n
 *---------------------------------------------------------------
 */
#define X1	set_vble_idx(0)
#define R	set_vble_idx(1)
#define X	set_vble_idx(2)
#define JOIN_SK1_NVBLES		3
#define JOIN_SK1_NOBJS		9
#define JOIN_SK1_NRELS		4
static const sok_relset_t JOIN_SK1 = {
	JOIN_SK1_NVBLES,	/* NVbles */
	JOIN_SK1_NOBJS,		/* NObjs */
	JOIN_SK1_NRELS,		/* NRels */
	{
		{ -1,  X,  0,  0,  0,  0,  0,  0,  0  },
		{  0,  0, -1,  R,  0,  0,  0,  0,  0  },
		{  0,  0,  0,  0, -1,  R,  X,  0,  0  },
		{  0,  0,  0,  0,  0,  0,  0, -1, X1  }
	}
};
/*----------------------------------------------------------------------------*/
/*
 *--------------------------------
 * PK{(x', r, x): Ci=b^x' (mod n);
 *				  y=g^x' (mod P) ;
 *				  Xi=a^x (mod n) ;
 *				  Ui=G^r (mod N^2) ;
 *				  Vi=Y^r H^x (mod N^2) }(msg)
 *--------------------------------
 * Vbles	{ x', r,  x  }
 * Spheres	 Lek Pek Mek 
 *----------------------------------------------------
 * Objs		{  X,  a,  U,  G,  V,  Y,  H,  C,  b,  y,  g  } | MOD
 *---------------------------------------------------------------
 * Rels		{ -1,  x,  0,  0,  0,  0,  0,  0,  0,  0,  0  } | n
 *			{  0,  0, -1,  r,  0,  0,  0,  0,  0,  0,  0  } | N^2
 *			{  0,  0,  0,  0, -1,  r,  x,  0,  0,  0,  0  } | N^2
 *			{  0,  0,  0,  0,  0,  0,  0, -1,  x', 0,  0  } | n
 *			{  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  x' } | P
 *---------------------------------------------------------------
 */
#define JOIN_SK2_NVBLES		3
#define JOIN_SK2_NOBJS		11
#define JOIN_SK2_NRELS		5
static const sok_relset_t JOIN_SK2 = {
	JOIN_SK2_NVBLES,	/* NVbles */
	JOIN_SK2_NOBJS,		/* NObjs */
	JOIN_SK2_NRELS,		/* NRels */
	{
		{ -1,  X,  0,  0,  0,  0,  0,  0,  0,  0,  0  },
		{  0,  0, -1,  R,  0,  0,  0,  0,  0,  0,  0  },
		{  0,  0,  0,  0, -1,  R,  X,  0,  0,  0,  0  },
		{  0,  0,  0,  0,  0,  0,  0, -1, X1,  0,  0  },
		{  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, X1  }
	}
};
#undef X1
#undef R
#undef X
/*----------------------------------------------------------------------------*/
static bool_t join_ei_proof(join_proof_t* Eiproof,
						  const bigint_t* x1,
						  const bigint_t* r,
						  const bigint_t* x,
						  const bigint_t* Ci,
						  const bigint_t* Xi,
						  const paillier_thr_ciphertext_t* UiVi,
						  const dlog_t* u_auth,
						  const ftmgs_pbkey_t* gpk,
						  rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_VAR_1(sok_params_t, sokp);
	BEG_VAR_1_bigint_t(h);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	const sphere_t* x_sph = &gpk->syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* r_sph = &gpk->fapk.precomp.psi;
	const bigint_t* n2_ = &gpk->fapk.precomp.n2;
	if (u_auth == NULL) {
		x1_sph = get_sphere_lambda(&gpk->gmpk.sp, &gpk->syssph);
	} else {
		x1_sph = get_sphere_lambda_x(*x1, &gpk->syssph);
	}
#else
	BEG_VAR_1_bigint_t(n2);
	BEG_VAR_2(syssph_t, syssph, sphere_t, psi);
	const sphere_t* x_sph = &syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* r_sph = &psi;
	const bigint_t* n2_ = (const bigint_t*)&n2;
	create_syssph(&syssph, &gpk->gmpk.sp);
	create_sph_psi(&psi, gpk->fapk.n, &gpk->gmpk.sp);
	bi_sq(n2, gpk->fapk.n);
	if (u_auth == NULL) {
		x1_sph = get_sphere_lambda(&gpk->gmpk.sp, &syssph);
	} else {
		x1_sph = get_sphere_lambda_x(*x1, &syssph);
	}
#endif
	bi_add_ui(h, gpk->fapk.n, 1);
	/*---------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, Xi);
	sok_params_add_obj(&sokp, &gpk->a);
	sok_params_add_obj(&sokp, &get_Ui(UiVi));
	sok_params_add_obj(&sokp, &gpk->fapk.g);
	sok_params_add_obj(&sokp, &get_Vi(UiVi));
	sok_params_add_obj(&sokp, &gpk->fapk.y);
	sok_params_add_obj(&sokp, (const bigint_t*)&h);
	sok_params_add_obj(&sokp, Ci);
	sok_params_add_obj(&sokp, &gpk->b);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, n2_);
	sok_params_add_mod(&sokp, n2_);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &Eiproof->c);
	/*--------------------------------*/
	sok_params_add_vble(&sokp, &Eiproof->sx1, x1_sph, x1);
	sok_params_add_vble(&sokp, &Eiproof->sr, r_sph, r);
	sok_params_add_vble(&sokp, &Eiproof->sx, x_sph, x);
	/*--------------------------------*/
	if (u_auth == NULL) {
		ok = sok(&JOIN_SK1, &sokp, NULL, 0, &gpk->gmpk.sp, rnd_ctx);
	} else {
		sok_params_add_obj(&sokp, &u_auth->y);
		sok_params_add_obj(&sokp, &u_auth->g);
		/*----------------------------*/
		sok_params_add_mod(&sokp, &u_auth->n);
		/*----------------------------*/
		ok = sok(&JOIN_SK2, &sokp, NULL, 0, &gpk->gmpk.sp, rnd_ctx);
	}
#ifndef PRECOMPUTATIONS__
	END_VAR_2(syssph_t, syssph, sphere_t, psi);
	END_VAR_1_bigint_t(n2);
#endif
	END_VAR_1_bigint_t(h);
	END_VAR_1(sok_params_t, sokp);
	return ok;
}
/*----------------------------------------------------------------------------*/
UNUSED__
static void join_precomp_sk(sok_precomp_t* precomp,
							const join_precomp_t* join_precomp,
							const sign_precomp_t* sign_precomp)
{
	/* 
	 * this is valid for JOIN_SK1 and JOIN_SK2
	 *		whenever relations stay in the same order and relations in SK1 are
	 *		a subset of SK2
	 */
	sok_precomp_init(precomp, &JOIN_SK2);
	sok_set_precomp(precomp, &JOIN_SK2, 0, 1, &sign_precomp->sign_a_x_mu);
	sok_set_precomp(precomp, &JOIN_SK2, 1, 3, &join_precomp->join_gg_r_psi);
	sok_set_precomp(precomp, &JOIN_SK2, 2, 5, &join_precomp->join_yy_r_psi);
	sok_set_precomp(precomp, &JOIN_SK2, 2, 6, &join_precomp->join_hh_x_mu);
/*sok_set_precomp(precomp, &JOIN_SK2, 3, 8, &sign_precomp->sign_b_x1_lambda);*/
}
/*----------------------------------------------------------------------------*/
static bool_t join_ei_vrfy(const join_proof_t* Eiproof,
						   const bigint_t* Ci,
						   const bigint_t* Xi,
						   const paillier_thr_ciphertext_t* UiVi,
						   const dlog_t* u_auth,
						   const ftmgs_pbkey_t* gpk)
{
	bool_t ok;
	BEG_VAR_1(sokv_params_t, sokp);
	BEG_VAR_1_bigint_t(h);
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	BEG_VAR_1(sok_precomp_t, precomp);
	const sphere_t* x_sph = &gpk->syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* r_sph = &gpk->fapk.precomp.psi;
	const bigint_t* n2_ = &gpk->fapk.precomp.n2;
	join_precomp_sk(&precomp, &gpk->join_precomp, &gpk->sign_precomp);
	if (u_auth == NULL) {
		x1_sph = get_sphere_lambda(&gpk->gmpk.sp, &gpk->syssph);
	} else {
		x1_sph = get_sphere_lambda_sx(Eiproof->sx1, &gpk->syssph);
	}
#else
	BEG_VAR_1_bigint_t(n2);
	BEG_VAR_2(syssph_t, syssph, sphere_t, psi);
	const sphere_t* x_sph = &syssph.mu;
	const sphere_t* x1_sph;
	const sphere_t* r_sph = &psi;
	const bigint_t* n2_ = (const bigint_t*)&n2;
	create_syssph(&syssph, &gpk->gmpk.sp);
	create_sph_psi(&psi, gpk->fapk.n, &gpk->gmpk.sp);
	bi_sq(n2, gpk->fapk.n);
	if (u_auth == NULL) {
		x1_sph = get_sphere_lambda(&gpk->gmpk.sp, &syssph);
	} else {
		x1_sph = get_sphere_lambda_sx(Eiproof->sx1, &syssph);
	}
#endif
	bi_add_ui(h, gpk->fapk.n, 1);
	/*---------------------------------*/
	sok_params_init(&sokp);
	/*--------------------------------*/
	sok_params_add_obj(&sokp, Xi);
	sok_params_add_obj(&sokp, &gpk->a);
	sok_params_add_obj(&sokp, &get_Ui(UiVi));
	sok_params_add_obj(&sokp, &gpk->fapk.g);
	sok_params_add_obj(&sokp, &get_Vi(UiVi));
	sok_params_add_obj(&sokp, &gpk->fapk.y);
	sok_params_add_obj(&sokp, (const bigint_t*)&h);
	sok_params_add_obj(&sokp, Ci);
	sok_params_add_obj(&sokp, &gpk->b);
	/*--------------------------------*/
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	sok_params_add_mod(&sokp, n2_);
	sok_params_add_mod(&sokp, n2_);
	sok_params_add_mod(&sokp, &gpk->gmpk.n);
	/*--------------------------------*/
	sok_params_add_c(&sokp, &Eiproof->c);
	/*--------------------------------*/
	sok_params_add_sph(&sokp, &Eiproof->sx1, x1_sph);
	sok_params_add_sph(&sokp, &Eiproof->sr, r_sph);
	sok_params_add_sph(&sokp, &Eiproof->sx, x_sph);
	/*--------------------------------*/
	if (u_auth == NULL) {
		ok = sokv(&JOIN_SK1, &sokp, NULL, 0, &gpk->gmpk.sp
#ifdef PRECOMPUTATIONS__
				  , &precomp
#endif
				  );
	} else {
		sok_params_add_obj(&sokp, &u_auth->y);
		sok_params_add_obj(&sokp, &u_auth->g);
		/*----------------------------*/
		sok_params_add_mod(&sokp, &u_auth->n);
		/*----------------------------*/
		ok = sokv(&JOIN_SK2, &sokp, NULL, 0, &gpk->gmpk.sp
#ifdef PRECOMPUTATIONS__
				  , &precomp
#endif
				  );
	}
	/*--------------------------------*/
#ifdef PRECOMPUTATIONS__
	END_VAR_1(sok_precomp_t, precomp);
#else
	END_VAR_2(syssph_t, syssph, sphere_t, psi);
	END_VAR_1_bigint_t(n2);
#endif
	END_VAR_1_bigint_t(h);
	END_VAR_1(sokv_params_t, sokp);
	/*--------------------------------*/
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int dlogx_t_chk_members(const struct dlogx_t* p, int code)/*auto*/
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
	struct dummy_dlogx_t {
		bigint_t x;		/* zero */
	};
	CHK_FIELD__(dummy_dlogx_t, dlogx_t, x);
	CHK_SIZE__(dummy_dlogx_t, dlogx_t);
	return (code == 451805986); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void dlogx_t_ctor(struct dlogx_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dlogx_t_chk_members(p,451805986));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void dlogx_t_dtor(struct dlogx_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dlogx_t_chk_members(p,451805986));
	bi_clear_zero(p->x);
	bi_dtor(p->x);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void dlogx_t_asg(struct dlogx_t* p, const struct dlogx_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dlogx_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
void dlogx_t_move(struct dlogx_t* p, struct dlogx_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dlogx_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg_si(p->x, 0);
		bi_swap(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct dlogx_t* dlogx_t_new()/*auto*/
{
	struct dlogx_t* p = (struct dlogx_t*)malloc(sizeof(struct dlogx_t));
	if (p != NULL) {
		dlogx_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct dlogx_t* dlogx_t_clone(const struct dlogx_t* o)/*auto*/
{
	struct dlogx_t* p = NULL;
	if (o != NULL) {
		p = (struct dlogx_t*)malloc(sizeof(struct dlogx_t));
		if (p != NULL) {
			dlogx_t_ctor(p);
			dlogx_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void dlogx_t_delete(struct dlogx_t* p)/*auto*/
{
	if (p != NULL) {
		dlogx_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int dlog_t_chk_members(const struct dlog_t* p, int code)/*auto*/
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
	struct dummy_dlog_t {
		bigint_t g;		/* zero */
		bigint_t y;		/* zero */
		bigint_t n;		/* zero */
	};
	CHK_FIELD__(dummy_dlog_t, dlog_t, g);
	CHK_FIELD__(dummy_dlog_t, dlog_t, y);
	CHK_FIELD__(dummy_dlog_t, dlog_t, n);
	CHK_SIZE__(dummy_dlog_t, dlog_t);
	return (code == 469974750); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void dlog_t_ctor(struct dlog_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dlog_t_chk_members(p,469974750));
	bi_ctor(p->g);
	bi_ctor(p->y);
	bi_ctor(p->n);
}
/*----------------------------------------------------------------------------*/
void dlog_t_dtor(struct dlog_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dlog_t_chk_members(p,469974750));
	bi_clear_zero(p->n);
	bi_dtor(p->n);
	bi_clear_zero(p->y);
	bi_dtor(p->y);
	bi_clear_zero(p->g);
	bi_dtor(p->g);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void dlog_t_asg(struct dlog_t* p, const struct dlog_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dlog_t_chk_members(p,469974750));
	if (p != o) {
		bi_asg(p->g, o->g);
		bi_asg(p->y, o->y);
		bi_asg(p->n, o->n);
	}
}
/*----------------------------------------------------------------------------*/
void dlog_t_move(struct dlog_t* p, struct dlog_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dlog_t_chk_members(p,469974750));
	if (p != o) {
		bi_asg_si(p->g, 0);
		bi_swap(p->g, o->g);
		bi_asg_si(p->y, 0);
		bi_swap(p->y, o->y);
		bi_asg_si(p->n, 0);
		bi_swap(p->n, o->n);
	}
}
/*----------------------------------------------------------------------------*/
struct dlog_t* dlog_t_new()/*auto*/
{
	struct dlog_t* p = (struct dlog_t*)malloc(sizeof(struct dlog_t));
	if (p != NULL) {
		dlog_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct dlog_t* dlog_t_clone(const struct dlog_t* o)/*auto*/
{
	struct dlog_t* p = NULL;
	if (o != NULL) {
		p = (struct dlog_t*)malloc(sizeof(struct dlog_t));
		if (p != NULL) {
			dlog_t_ctor(p);
			dlog_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void dlog_t_delete(struct dlog_t* p)/*auto*/
{
	if (p != NULL) {
		dlog_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_proof_t_chk_members(const struct join_proof_t* p, int code)/*auto*/
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
	struct dummy_join_proof_t {
		bigint_t c;			/* zero */
		bigint_t sx1;		/* zero */
		bigint_t sr;		/* zero */
		bigint_t sx;		/* zero */
	};
	CHK_FIELD__(dummy_join_proof_t, join_proof_t, c);
	CHK_FIELD__(dummy_join_proof_t, join_proof_t, sx1);
	CHK_FIELD__(dummy_join_proof_t, join_proof_t, sr);
	CHK_FIELD__(dummy_join_proof_t, join_proof_t, sx);
	CHK_SIZE__(dummy_join_proof_t, join_proof_t);
	return (code == 67824370); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_proof_t_ctor(struct join_proof_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_proof_t_chk_members(p,67824370));
	bi_ctor(p->c);
	bi_ctor(p->sx1);
	bi_ctor(p->sr);
	bi_ctor(p->sx);
}
/*----------------------------------------------------------------------------*/
void join_proof_t_dtor(struct join_proof_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_proof_t_chk_members(p,67824370));
	bi_clear_zero(p->sx);
	bi_dtor(p->sx);
	bi_clear_zero(p->sr);
	bi_dtor(p->sr);
	bi_clear_zero(p->sx1);
	bi_dtor(p->sx1);
	bi_clear_zero(p->c);
	bi_dtor(p->c);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_proof_t_asg(struct join_proof_t* p, const struct join_proof_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_proof_t_chk_members(p,67824370));
	if (p != o) {
		bi_asg(p->c, o->c);
		bi_asg(p->sx1, o->sx1);
		bi_asg(p->sr, o->sr);
		bi_asg(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
void join_proof_t_move(struct join_proof_t* p, struct join_proof_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_proof_t_chk_members(p,67824370));
	if (p != o) {
		bi_asg_si(p->c, 0);
		bi_swap(p->c, o->c);
		bi_asg_si(p->sx1, 0);
		bi_swap(p->sx1, o->sx1);
		bi_asg_si(p->sr, 0);
		bi_swap(p->sr, o->sr);
		bi_asg_si(p->sx, 0);
		bi_swap(p->sx, o->sx);
	}
}
/*----------------------------------------------------------------------------*/
struct join_proof_t* join_proof_t_new()/*auto*/
{
	struct join_proof_t* p = (struct join_proof_t*)malloc(sizeof(struct join_proof_t));
	if (p != NULL) {
		join_proof_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_proof_t* join_proof_t_clone(const struct join_proof_t* o)/*auto*/
{
	struct join_proof_t* p = NULL;
	if (o != NULL) {
		p = (struct join_proof_t*)malloc(sizeof(struct join_proof_t));
		if (p != NULL) {
			join_proof_t_ctor(p);
			join_proof_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_proof_t_delete(struct join_proof_t* p)/*auto*/
{
	if (p != NULL) {
		join_proof_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_u1prv_t_chk_members(const struct join_u1prv_t* p, int code)/*auto*/
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
	struct dummy_join_u1prv_t {
		nadrp_a1prv_t nadrp_a1;
		bigint_t x1i;		/* zero */
	};
	CHK_FIELD__(dummy_join_u1prv_t, join_u1prv_t, nadrp_a1);
	CHK_FIELD__(dummy_join_u1prv_t, join_u1prv_t, x1i);
	CHK_SIZE__(dummy_join_u1prv_t, join_u1prv_t);
	return (code == 514983335); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_u1prv_t_ctor(struct join_u1prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u1prv_t_chk_members(p,514983335));
	nadrp_a1prv_t_ctor(&p->nadrp_a1);
	bi_ctor(p->x1i);
}
/*----------------------------------------------------------------------------*/
void join_u1prv_t_dtor(struct join_u1prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u1prv_t_chk_members(p,514983335));
	bi_clear_zero(p->x1i);
	bi_dtor(p->x1i);
	nadrp_a1prv_t_dtor(&p->nadrp_a1);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_u1prv_t_asg(struct join_u1prv_t* p, const struct join_u1prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u1prv_t_chk_members(p,514983335));
	if (p != o) {
		nadrp_a1prv_t_asg(&p->nadrp_a1, &o->nadrp_a1);
		bi_asg(p->x1i, o->x1i);
	}
}
/*----------------------------------------------------------------------------*/
void join_u1prv_t_move(struct join_u1prv_t* p, struct join_u1prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u1prv_t_chk_members(p,514983335));
	if (p != o) {
		nadrp_a1prv_t_move(&p->nadrp_a1, &o->nadrp_a1);
		bi_asg_si(p->x1i, 0);
		bi_swap(p->x1i, o->x1i);
	}
}
/*----------------------------------------------------------------------------*/
struct join_u1prv_t* join_u1prv_t_new()/*auto*/
{
	struct join_u1prv_t* p = (struct join_u1prv_t*)malloc(sizeof(struct join_u1prv_t));
	if (p != NULL) {
		join_u1prv_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_u1prv_t* join_u1prv_t_clone(const struct join_u1prv_t* o)/*auto*/
{
	struct join_u1prv_t* p = NULL;
	if (o != NULL) {
		p = (struct join_u1prv_t*)malloc(sizeof(struct join_u1prv_t));
		if (p != NULL) {
			join_u1prv_t_ctor(p);
			join_u1prv_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_u1prv_t_delete(struct join_u1prv_t* p)/*auto*/
{
	if (p != NULL) {
		join_u1prv_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_u1pbl_t_chk_members(const struct join_u1pbl_t* p, int code)/*auto*/
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
	struct dummy_join_u1pbl_t {
		nadrp_a1pbl_t nadrp_a1;
		bigint_t Ci;		/* zero */
	};
	CHK_FIELD__(dummy_join_u1pbl_t, join_u1pbl_t, nadrp_a1);
	CHK_FIELD__(dummy_join_u1pbl_t, join_u1pbl_t, Ci);
	CHK_SIZE__(dummy_join_u1pbl_t, join_u1pbl_t);
	return (code == 224042007); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_ctor(struct join_u1pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u1pbl_t_chk_members(p,224042007));
	nadrp_a1pbl_t_ctor(&p->nadrp_a1);
	bi_ctor(p->Ci);
}
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_dtor(struct join_u1pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u1pbl_t_chk_members(p,224042007));
	bi_clear_zero(p->Ci);
	bi_dtor(p->Ci);
	nadrp_a1pbl_t_dtor(&p->nadrp_a1);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_asg(struct join_u1pbl_t* p, const struct join_u1pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u1pbl_t_chk_members(p,224042007));
	if (p != o) {
		nadrp_a1pbl_t_asg(&p->nadrp_a1, &o->nadrp_a1);
		bi_asg(p->Ci, o->Ci);
	}
}
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_move(struct join_u1pbl_t* p, struct join_u1pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u1pbl_t_chk_members(p,224042007));
	if (p != o) {
		nadrp_a1pbl_t_move(&p->nadrp_a1, &o->nadrp_a1);
		bi_asg_si(p->Ci, 0);
		bi_swap(p->Ci, o->Ci);
	}
}
/*----------------------------------------------------------------------------*/
struct join_u1pbl_t* join_u1pbl_t_new()/*auto*/
{
	struct join_u1pbl_t* p = (struct join_u1pbl_t*)malloc(sizeof(struct join_u1pbl_t));
	if (p != NULL) {
		join_u1pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_u1pbl_t* join_u1pbl_t_clone(const struct join_u1pbl_t* o)/*auto*/
{
	struct join_u1pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct join_u1pbl_t*)malloc(sizeof(struct join_u1pbl_t));
		if (p != NULL) {
			join_u1pbl_t_ctor(p);
			join_u1pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_u1pbl_t_delete(struct join_u1pbl_t* p)/*auto*/
{
	if (p != NULL) {
		join_u1pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_gm2pbl_t_chk_members(const struct join_gm2pbl_t* p, int code)/*auto*/
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
	struct dummy_join_gm2pbl_t {
		nadrp_b2pbl_t nadrp_b2;
	};
	CHK_FIELD__(dummy_join_gm2pbl_t, join_gm2pbl_t, nadrp_b2);
	CHK_SIZE__(dummy_join_gm2pbl_t, join_gm2pbl_t);
	return (code == 388929167); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_ctor(struct join_gm2pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_gm2pbl_t_chk_members(p,388929167));
	nadrp_b2pbl_t_ctor(&p->nadrp_b2);
}
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_dtor(struct join_gm2pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_gm2pbl_t_chk_members(p,388929167));
	nadrp_b2pbl_t_dtor(&p->nadrp_b2);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_asg(struct join_gm2pbl_t* p, const struct join_gm2pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_gm2pbl_t_chk_members(p,388929167));
	if (p != o) {
		nadrp_b2pbl_t_asg(&p->nadrp_b2, &o->nadrp_b2);
	}
}
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_move(struct join_gm2pbl_t* p, struct join_gm2pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_gm2pbl_t_chk_members(p,388929167));
	if (p != o) {
		nadrp_b2pbl_t_move(&p->nadrp_b2, &o->nadrp_b2);
	}
}
/*----------------------------------------------------------------------------*/
struct join_gm2pbl_t* join_gm2pbl_t_new()/*auto*/
{
	struct join_gm2pbl_t* p = (struct join_gm2pbl_t*)malloc(sizeof(struct join_gm2pbl_t));
	if (p != NULL) {
		join_gm2pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_gm2pbl_t* join_gm2pbl_t_clone(const struct join_gm2pbl_t* o)/*auto*/
{
	struct join_gm2pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct join_gm2pbl_t*)malloc(sizeof(struct join_gm2pbl_t));
		if (p != NULL) {
			join_gm2pbl_t_ctor(p);
			join_gm2pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_gm2pbl_t_delete(struct join_gm2pbl_t* p)/*auto*/
{
	if (p != NULL) {
		join_gm2pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_u3prv_t_chk_members(const struct join_u3prv_t* p, int code)/*auto*/
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
	struct dummy_join_u3prv_t {
		nadrp_a3prv_t nadrp_a3;
		bigint_t Xi;		/* zero */
	};
	CHK_FIELD__(dummy_join_u3prv_t, join_u3prv_t, nadrp_a3);
	CHK_FIELD__(dummy_join_u3prv_t, join_u3prv_t, Xi);
	CHK_SIZE__(dummy_join_u3prv_t, join_u3prv_t);
	return (code == 123573108); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_u3prv_t_ctor(struct join_u3prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u3prv_t_chk_members(p,123573108));
	nadrp_a3prv_t_ctor(&p->nadrp_a3);
	bi_ctor(p->Xi);
}
/*----------------------------------------------------------------------------*/
void join_u3prv_t_dtor(struct join_u3prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u3prv_t_chk_members(p,123573108));
	bi_clear_zero(p->Xi);
	bi_dtor(p->Xi);
	nadrp_a3prv_t_dtor(&p->nadrp_a3);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_u3prv_t_asg(struct join_u3prv_t* p, const struct join_u3prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u3prv_t_chk_members(p,123573108));
	if (p != o) {
		nadrp_a3prv_t_asg(&p->nadrp_a3, &o->nadrp_a3);
		bi_asg(p->Xi, o->Xi);
	}
}
/*----------------------------------------------------------------------------*/
void join_u3prv_t_move(struct join_u3prv_t* p, struct join_u3prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u3prv_t_chk_members(p,123573108));
	if (p != o) {
		nadrp_a3prv_t_move(&p->nadrp_a3, &o->nadrp_a3);
		bi_asg_si(p->Xi, 0);
		bi_swap(p->Xi, o->Xi);
	}
}
/*----------------------------------------------------------------------------*/
struct join_u3prv_t* join_u3prv_t_new()/*auto*/
{
	struct join_u3prv_t* p = (struct join_u3prv_t*)malloc(sizeof(struct join_u3prv_t));
	if (p != NULL) {
		join_u3prv_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_u3prv_t* join_u3prv_t_clone(const struct join_u3prv_t* o)/*auto*/
{
	struct join_u3prv_t* p = NULL;
	if (o != NULL) {
		p = (struct join_u3prv_t*)malloc(sizeof(struct join_u3prv_t));
		if (p != NULL) {
			join_u3prv_t_ctor(p);
			join_u3prv_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_u3prv_t_delete(struct join_u3prv_t* p)/*auto*/
{
	if (p != NULL) {
		join_u3prv_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_u3pbl_t_chk_members(const struct join_u3pbl_t* p, int code)/*auto*/
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
	struct dummy_join_u3pbl_t {
		nadrp_a3pbl_t nadrp_a3;
		paillier_thr_ciphertext_t UiVi;	/* alpha: Ui ; beta: Vi */
		join_proof_t Eiproof;
	};
	CHK_FIELD__(dummy_join_u3pbl_t, join_u3pbl_t, nadrp_a3);
	CHK_FIELD__(dummy_join_u3pbl_t, join_u3pbl_t, UiVi);
	CHK_FIELD__(dummy_join_u3pbl_t, join_u3pbl_t, Eiproof);
	CHK_SIZE__(dummy_join_u3pbl_t, join_u3pbl_t);
	return (code == 447188112); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_ctor(struct join_u3pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u3pbl_t_chk_members(p,447188112));
	nadrp_a3pbl_t_ctor(&p->nadrp_a3);
	paillier_thr_ciphertext_t_ctor(&p->UiVi);
	join_proof_t_ctor(&p->Eiproof);
}
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_dtor(struct join_u3pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_u3pbl_t_chk_members(p,447188112));
	join_proof_t_dtor(&p->Eiproof);
	paillier_thr_ciphertext_t_dtor(&p->UiVi);
	nadrp_a3pbl_t_dtor(&p->nadrp_a3);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_asg(struct join_u3pbl_t* p, const struct join_u3pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u3pbl_t_chk_members(p,447188112));
	if (p != o) {
		nadrp_a3pbl_t_asg(&p->nadrp_a3, &o->nadrp_a3);
		paillier_thr_ciphertext_t_asg(&p->UiVi, &o->UiVi);
		join_proof_t_asg(&p->Eiproof, &o->Eiproof);
	}
}
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_move(struct join_u3pbl_t* p, struct join_u3pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_u3pbl_t_chk_members(p,447188112));
	if (p != o) {
		nadrp_a3pbl_t_move(&p->nadrp_a3, &o->nadrp_a3);
		paillier_thr_ciphertext_t_move(&p->UiVi, &o->UiVi);
		join_proof_t_move(&p->Eiproof, &o->Eiproof);
	}
}
/*----------------------------------------------------------------------------*/
struct join_u3pbl_t* join_u3pbl_t_new()/*auto*/
{
	struct join_u3pbl_t* p = (struct join_u3pbl_t*)malloc(sizeof(struct join_u3pbl_t));
	if (p != NULL) {
		join_u3pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_u3pbl_t* join_u3pbl_t_clone(const struct join_u3pbl_t* o)/*auto*/
{
	struct join_u3pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct join_u3pbl_t*)malloc(sizeof(struct join_u3pbl_t));
		if (p != NULL) {
			join_u3pbl_t_ctor(p);
			join_u3pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_u3pbl_t_delete(struct join_u3pbl_t* p)/*auto*/
{
	if (p != NULL) {
		join_u3pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int join_gm4pbl_t_chk_members(const struct join_gm4pbl_t* p, int code)/*auto*/
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
	struct dummy_join_gm4pbl_t {
		bigint_t Ai;		/* zero */
		bigint_t ei;		/* zero */
	};
	CHK_FIELD__(dummy_join_gm4pbl_t, join_gm4pbl_t, Ai);
	CHK_FIELD__(dummy_join_gm4pbl_t, join_gm4pbl_t, ei);
	CHK_SIZE__(dummy_join_gm4pbl_t, join_gm4pbl_t);
	return (code == 87237266); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_ctor(struct join_gm4pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_gm4pbl_t_chk_members(p,87237266));
	bi_ctor(p->Ai);
	bi_ctor(p->ei);
}
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_dtor(struct join_gm4pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(join_gm4pbl_t_chk_members(p,87237266));
	bi_clear_zero(p->ei);
	bi_dtor(p->ei);
	bi_clear_zero(p->Ai);
	bi_dtor(p->Ai);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_asg(struct join_gm4pbl_t* p, const struct join_gm4pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_gm4pbl_t_chk_members(p,87237266));
	if (p != o) {
		bi_asg(p->Ai, o->Ai);
		bi_asg(p->ei, o->ei);
	}
}
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_move(struct join_gm4pbl_t* p, struct join_gm4pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(join_gm4pbl_t_chk_members(p,87237266));
	if (p != o) {
		bi_asg_si(p->Ai, 0);
		bi_swap(p->Ai, o->Ai);
		bi_asg_si(p->ei, 0);
		bi_swap(p->ei, o->ei);
	}
}
/*----------------------------------------------------------------------------*/
struct join_gm4pbl_t* join_gm4pbl_t_new()/*auto*/
{
	struct join_gm4pbl_t* p = (struct join_gm4pbl_t*)malloc(sizeof(struct join_gm4pbl_t));
	if (p != NULL) {
		join_gm4pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct join_gm4pbl_t* join_gm4pbl_t_clone(const struct join_gm4pbl_t* o)/*auto*/
{
	struct join_gm4pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct join_gm4pbl_t*)malloc(sizeof(struct join_gm4pbl_t));
		if (p != NULL) {
			join_gm4pbl_t_ctor(p);
			join_gm4pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void join_gm4pbl_t_delete(struct join_gm4pbl_t* p)/*auto*/
{
	if (p != NULL) {
		join_gm4pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_mbr_ref_t_chk_members(const struct ftmgs_mbr_ref_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_mbr_ref_t {
		bigint_t Ai;		/* zero */
		bigint_t ei;		/* zero */
		bigint_t Ci;		/* zero */
		bigint_t Xi;		/* zero */
		paillier_thr_ciphertext_t UiVi;	/* alpha: Ui ; beta: Vi */
		dlog_t user_auth;
		join_proof_t Eiproof;
	};
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, Ai);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, ei);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, Ci);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, Xi);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, UiVi);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, user_auth);
	CHK_FIELD__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t, Eiproof);
	CHK_SIZE__(dummy_ftmgs_mbr_ref_t, ftmgs_mbr_ref_t);
	return (code == 119169697); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_ctor(struct ftmgs_mbr_ref_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mbr_ref_t_chk_members(p,119169697));
	bi_ctor(p->Ai);
	bi_ctor(p->ei);
	bi_ctor(p->Ci);
	bi_ctor(p->Xi);
	paillier_thr_ciphertext_t_ctor(&p->UiVi);
	dlog_t_ctor(&p->user_auth);
	join_proof_t_ctor(&p->Eiproof);
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_dtor(struct ftmgs_mbr_ref_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mbr_ref_t_chk_members(p,119169697));
	join_proof_t_dtor(&p->Eiproof);
	dlog_t_dtor(&p->user_auth);
	paillier_thr_ciphertext_t_dtor(&p->UiVi);
	bi_clear_zero(p->Xi);
	bi_dtor(p->Xi);
	bi_clear_zero(p->Ci);
	bi_dtor(p->Ci);
	bi_clear_zero(p->ei);
	bi_dtor(p->ei);
	bi_clear_zero(p->Ai);
	bi_dtor(p->Ai);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_asg(struct ftmgs_mbr_ref_t* p, const struct ftmgs_mbr_ref_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mbr_ref_t_chk_members(p,119169697));
	if (p != o) {
		bi_asg(p->Ai, o->Ai);
		bi_asg(p->ei, o->ei);
		bi_asg(p->Ci, o->Ci);
		bi_asg(p->Xi, o->Xi);
		paillier_thr_ciphertext_t_asg(&p->UiVi, &o->UiVi);
		dlog_t_asg(&p->user_auth, &o->user_auth);
		join_proof_t_asg(&p->Eiproof, &o->Eiproof);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_move(struct ftmgs_mbr_ref_t* p, struct ftmgs_mbr_ref_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mbr_ref_t_chk_members(p,119169697));
	if (p != o) {
		bi_asg_si(p->Ai, 0);
		bi_swap(p->Ai, o->Ai);
		bi_asg_si(p->ei, 0);
		bi_swap(p->ei, o->ei);
		bi_asg_si(p->Ci, 0);
		bi_swap(p->Ci, o->Ci);
		bi_asg_si(p->Xi, 0);
		bi_swap(p->Xi, o->Xi);
		paillier_thr_ciphertext_t_move(&p->UiVi, &o->UiVi);
		dlog_t_move(&p->user_auth, &o->user_auth);
		join_proof_t_move(&p->Eiproof, &o->Eiproof);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_ref_t* ftmgs_mbr_ref_t_new()/*auto*/
{
	struct ftmgs_mbr_ref_t* p = (struct ftmgs_mbr_ref_t*)malloc(sizeof(struct ftmgs_mbr_ref_t));
	if (p != NULL) {
		ftmgs_mbr_ref_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_ref_t* ftmgs_mbr_ref_t_clone(const struct ftmgs_mbr_ref_t* o)/*auto*/
{
	struct ftmgs_mbr_ref_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_mbr_ref_t*)malloc(sizeof(struct ftmgs_mbr_ref_t));
		if (p != NULL) {
			ftmgs_mbr_ref_t_ctor(p);
			ftmgs_mbr_ref_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_ref_t_delete(struct ftmgs_mbr_ref_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_mbr_ref_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_mbr_prkey_t_chk_members(const struct ftmgs_mbr_prkey_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_mbr_prkey_t {
		bigint_t Ai;		/* zero */
		bigint_t ei;		/* zero */
		bigint_t xi;		/* zero */
		bigint_t x1i;		/* zero */
	};
	CHK_FIELD__(dummy_ftmgs_mbr_prkey_t, ftmgs_mbr_prkey_t, Ai);
	CHK_FIELD__(dummy_ftmgs_mbr_prkey_t, ftmgs_mbr_prkey_t, ei);
	CHK_FIELD__(dummy_ftmgs_mbr_prkey_t, ftmgs_mbr_prkey_t, xi);
	CHK_FIELD__(dummy_ftmgs_mbr_prkey_t, ftmgs_mbr_prkey_t, x1i);
	CHK_SIZE__(dummy_ftmgs_mbr_prkey_t, ftmgs_mbr_prkey_t);
	return (code == 200381983); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_ctor(struct ftmgs_mbr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mbr_prkey_t_chk_members(p,200381983));
	bi_ctor(p->Ai);
	bi_ctor(p->ei);
	bi_ctor(p->xi);
	bi_ctor(p->x1i);
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_dtor(struct ftmgs_mbr_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_mbr_prkey_t_chk_members(p,200381983));
	bi_clear_zero(p->x1i);
	bi_dtor(p->x1i);
	bi_clear_zero(p->xi);
	bi_dtor(p->xi);
	bi_clear_zero(p->ei);
	bi_dtor(p->ei);
	bi_clear_zero(p->Ai);
	bi_dtor(p->Ai);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_asg(struct ftmgs_mbr_prkey_t* p, const struct ftmgs_mbr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mbr_prkey_t_chk_members(p,200381983));
	if (p != o) {
		bi_asg(p->Ai, o->Ai);
		bi_asg(p->ei, o->ei);
		bi_asg(p->xi, o->xi);
		bi_asg(p->x1i, o->x1i);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_move(struct ftmgs_mbr_prkey_t* p, struct ftmgs_mbr_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_mbr_prkey_t_chk_members(p,200381983));
	if (p != o) {
		bi_asg_si(p->Ai, 0);
		bi_swap(p->Ai, o->Ai);
		bi_asg_si(p->ei, 0);
		bi_swap(p->ei, o->ei);
		bi_asg_si(p->xi, 0);
		bi_swap(p->xi, o->xi);
		bi_asg_si(p->x1i, 0);
		bi_swap(p->x1i, o->x1i);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_prkey_t* ftmgs_mbr_prkey_t_new()/*auto*/
{
	struct ftmgs_mbr_prkey_t* p = (struct ftmgs_mbr_prkey_t*)malloc(sizeof(struct ftmgs_mbr_prkey_t));
	if (p != NULL) {
		ftmgs_mbr_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_mbr_prkey_t* ftmgs_mbr_prkey_t_clone(const struct ftmgs_mbr_prkey_t* o)/*auto*/
{
	struct ftmgs_mbr_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_mbr_prkey_t*)malloc(sizeof(struct ftmgs_mbr_prkey_t));
		if (p != NULL) {
			ftmgs_mbr_prkey_t_ctor(p);
			ftmgs_mbr_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_mbr_prkey_t_delete(struct ftmgs_mbr_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_mbr_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_join_prv_t_chk_members(const struct ftmgs_join_prv_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_join_prv_t {
		unsigned status;
		join_u1prv_t u1;
		join_u3prv_t u3;
	};
	CHK_FIELD__(dummy_ftmgs_join_prv_t, ftmgs_join_prv_t, status);
	CHK_FIELD__(dummy_ftmgs_join_prv_t, ftmgs_join_prv_t, u1);
	CHK_FIELD__(dummy_ftmgs_join_prv_t, ftmgs_join_prv_t, u3);
	CHK_SIZE__(dummy_ftmgs_join_prv_t, ftmgs_join_prv_t);
	return (code == 526712973); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_ctor(struct ftmgs_join_prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_join_prv_t_chk_members(p,526712973));
	p->status = 0;
	join_u1prv_t_ctor(&p->u1);
	join_u3prv_t_ctor(&p->u3);
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_dtor(struct ftmgs_join_prv_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_join_prv_t_chk_members(p,526712973));
	join_u3prv_t_dtor(&p->u3);
	join_u1prv_t_dtor(&p->u1);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_asg(struct ftmgs_join_prv_t* p, const struct ftmgs_join_prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_join_prv_t_chk_members(p,526712973));
	if (p != o) {
		p->status = o->status;
		join_u1prv_t_asg(&p->u1, &o->u1);
		join_u3prv_t_asg(&p->u3, &o->u3);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_move(struct ftmgs_join_prv_t* p, struct ftmgs_join_prv_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_join_prv_t_chk_members(p,526712973));
	if (p != o) {
		p->status = o->status;
		join_u1prv_t_move(&p->u1, &o->u1);
		join_u3prv_t_move(&p->u3, &o->u3);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_join_prv_t* ftmgs_join_prv_t_new()/*auto*/
{
	struct ftmgs_join_prv_t* p = (struct ftmgs_join_prv_t*)malloc(sizeof(struct ftmgs_join_prv_t));
	if (p != NULL) {
		ftmgs_join_prv_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_join_prv_t* ftmgs_join_prv_t_clone(const struct ftmgs_join_prv_t* o)/*auto*/
{
	struct ftmgs_join_prv_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_join_prv_t*)malloc(sizeof(struct ftmgs_join_prv_t));
		if (p != NULL) {
			ftmgs_join_prv_t_ctor(p);
			ftmgs_join_prv_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_prv_t_delete(struct ftmgs_join_prv_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_join_prv_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int ftmgs_join_pbl_t_chk_members(const struct ftmgs_join_pbl_t* p, int code)/*auto*/
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
	struct dummy_ftmgs_join_pbl_t {
		unsigned status;
		join_u1pbl_t u1;
		join_gm2pbl_t gm2;
		join_u3pbl_t u3;
		join_gm4pbl_t gm4;
	};
	CHK_FIELD__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t, status);
	CHK_FIELD__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t, u1);
	CHK_FIELD__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t, gm2);
	CHK_FIELD__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t, u3);
	CHK_FIELD__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t, gm4);
	CHK_SIZE__(dummy_ftmgs_join_pbl_t, ftmgs_join_pbl_t);
	return (code == 279795973); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_ctor(struct ftmgs_join_pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_join_pbl_t_chk_members(p,279795973));
	p->status = 0;
	join_u1pbl_t_ctor(&p->u1);
	join_gm2pbl_t_ctor(&p->gm2);
	join_u3pbl_t_ctor(&p->u3);
	join_gm4pbl_t_ctor(&p->gm4);
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_dtor(struct ftmgs_join_pbl_t* p)/*auto*/
{
	assert(p != NULL);
	assert(ftmgs_join_pbl_t_chk_members(p,279795973));
	join_gm4pbl_t_dtor(&p->gm4);
	join_u3pbl_t_dtor(&p->u3);
	join_gm2pbl_t_dtor(&p->gm2);
	join_u1pbl_t_dtor(&p->u1);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_asg(struct ftmgs_join_pbl_t* p, const struct ftmgs_join_pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_join_pbl_t_chk_members(p,279795973));
	if (p != o) {
		p->status = o->status;
		join_u1pbl_t_asg(&p->u1, &o->u1);
		join_gm2pbl_t_asg(&p->gm2, &o->gm2);
		join_u3pbl_t_asg(&p->u3, &o->u3);
		join_gm4pbl_t_asg(&p->gm4, &o->gm4);
	}
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_move(struct ftmgs_join_pbl_t* p, struct ftmgs_join_pbl_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(ftmgs_join_pbl_t_chk_members(p,279795973));
	if (p != o) {
		p->status = o->status;
		join_u1pbl_t_move(&p->u1, &o->u1);
		join_gm2pbl_t_move(&p->gm2, &o->gm2);
		join_u3pbl_t_move(&p->u3, &o->u3);
		join_gm4pbl_t_move(&p->gm4, &o->gm4);
	}
}
/*----------------------------------------------------------------------------*/
struct ftmgs_join_pbl_t* ftmgs_join_pbl_t_new()/*auto*/
{
	struct ftmgs_join_pbl_t* p = (struct ftmgs_join_pbl_t*)malloc(sizeof(struct ftmgs_join_pbl_t));
	if (p != NULL) {
		ftmgs_join_pbl_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct ftmgs_join_pbl_t* ftmgs_join_pbl_t_clone(const struct ftmgs_join_pbl_t* o)/*auto*/
{
	struct ftmgs_join_pbl_t* p = NULL;
	if (o != NULL) {
		p = (struct ftmgs_join_pbl_t*)malloc(sizeof(struct ftmgs_join_pbl_t));
		if (p != NULL) {
			ftmgs_join_pbl_t_ctor(p);
			ftmgs_join_pbl_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void ftmgs_join_pbl_t_delete(struct ftmgs_join_pbl_t* p)/*auto*/
{
	if (p != NULL) {
		ftmgs_join_pbl_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int dss_parms_t_chk_members(const struct dss_parms_t* p, int code)/*auto*/
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
	struct dummy_dss_parms_t {
		bigint_t p;		/* zero */
		bigint_t q;		/* zero */
		bigint_t g;		/* zero */
	};
	CHK_FIELD__(dummy_dss_parms_t, dss_parms_t, p);
	CHK_FIELD__(dummy_dss_parms_t, dss_parms_t, q);
	CHK_FIELD__(dummy_dss_parms_t, dss_parms_t, g);
	CHK_SIZE__(dummy_dss_parms_t, dss_parms_t);
	return (code == 117362936); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void dss_parms_t_ctor(struct dss_parms_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dss_parms_t_chk_members(p,117362936));
	bi_ctor(p->p);
	bi_ctor(p->q);
	bi_ctor(p->g);
}
/*----------------------------------------------------------------------------*/
void dss_parms_t_dtor(struct dss_parms_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dss_parms_t_chk_members(p,117362936));
	bi_clear_zero(p->g);
	bi_dtor(p->g);
	bi_clear_zero(p->q);
	bi_dtor(p->q);
	bi_clear_zero(p->p);
	bi_dtor(p->p);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void dss_parms_t_asg(struct dss_parms_t* p, const struct dss_parms_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dss_parms_t_chk_members(p,117362936));
	if (p != o) {
		bi_asg(p->p, o->p);
		bi_asg(p->q, o->q);
		bi_asg(p->g, o->g);
	}
}
/*----------------------------------------------------------------------------*/
void dss_parms_t_move(struct dss_parms_t* p, struct dss_parms_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dss_parms_t_chk_members(p,117362936));
	if (p != o) {
		bi_asg_si(p->p, 0);
		bi_swap(p->p, o->p);
		bi_asg_si(p->q, 0);
		bi_swap(p->q, o->q);
		bi_asg_si(p->g, 0);
		bi_swap(p->g, o->g);
	}
}
/*----------------------------------------------------------------------------*/
struct dss_parms_t* dss_parms_t_new()/*auto*/
{
	struct dss_parms_t* p = (struct dss_parms_t*)malloc(sizeof(struct dss_parms_t));
	if (p != NULL) {
		dss_parms_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct dss_parms_t* dss_parms_t_clone(const struct dss_parms_t* o)/*auto*/
{
	struct dss_parms_t* p = NULL;
	if (o != NULL) {
		p = (struct dss_parms_t*)malloc(sizeof(struct dss_parms_t));
		if (p != NULL) {
			dss_parms_t_ctor(p);
			dss_parms_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void dss_parms_t_delete(struct dss_parms_t* p)/*auto*/
{
	if (p != NULL) {
		dss_parms_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int dsa_pbkey_t_chk_members(const struct dsa_pbkey_t* p, int code)/*auto*/
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
	struct dummy_dsa_pbkey_t {
		bigint_t y;		/* zero */
	};
	CHK_FIELD__(dummy_dsa_pbkey_t, dsa_pbkey_t, y);
	CHK_SIZE__(dummy_dsa_pbkey_t, dsa_pbkey_t);
	return (code == 70046067); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_ctor(struct dsa_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dsa_pbkey_t_chk_members(p,70046067));
	bi_ctor(p->y);
}
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_dtor(struct dsa_pbkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dsa_pbkey_t_chk_members(p,70046067));
	bi_clear_zero(p->y);
	bi_dtor(p->y);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_asg(struct dsa_pbkey_t* p, const struct dsa_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dsa_pbkey_t_chk_members(p,70046067));
	if (p != o) {
		bi_asg(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_move(struct dsa_pbkey_t* p, struct dsa_pbkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dsa_pbkey_t_chk_members(p,70046067));
	if (p != o) {
		bi_asg_si(p->y, 0);
		bi_swap(p->y, o->y);
	}
}
/*----------------------------------------------------------------------------*/
struct dsa_pbkey_t* dsa_pbkey_t_new()/*auto*/
{
	struct dsa_pbkey_t* p = (struct dsa_pbkey_t*)malloc(sizeof(struct dsa_pbkey_t));
	if (p != NULL) {
		dsa_pbkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct dsa_pbkey_t* dsa_pbkey_t_clone(const struct dsa_pbkey_t* o)/*auto*/
{
	struct dsa_pbkey_t* p = NULL;
	if (o != NULL) {
		p = (struct dsa_pbkey_t*)malloc(sizeof(struct dsa_pbkey_t));
		if (p != NULL) {
			dsa_pbkey_t_ctor(p);
			dsa_pbkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void dsa_pbkey_t_delete(struct dsa_pbkey_t* p)/*auto*/
{
	if (p != NULL) {
		dsa_pbkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/
#ifndef NDEBUG
static int dsa_prkey_t_chk_members(const struct dsa_prkey_t* p, int code)/*auto*/
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
	struct dummy_dsa_prkey_t {
		bigint_t x;		/* zero */
	};
	CHK_FIELD__(dummy_dsa_prkey_t, dsa_prkey_t, x);
	CHK_SIZE__(dummy_dsa_prkey_t, dsa_prkey_t);
	return (code == 451805986); (void)p;
#undef STATIC_ASSERT__
#undef CHK_FIELD__
#undef CHK_SIZE__
}
#endif
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_ctor(struct dsa_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dsa_prkey_t_chk_members(p,451805986));
	bi_ctor(p->x);
}
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_dtor(struct dsa_prkey_t* p)/*auto*/
{
	assert(p != NULL);
	assert(dsa_prkey_t_chk_members(p,451805986));
	bi_clear_zero(p->x);
	bi_dtor(p->x);
	(void)p;
}
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_asg(struct dsa_prkey_t* p, const struct dsa_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dsa_prkey_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_move(struct dsa_prkey_t* p, struct dsa_prkey_t* o)/*auto*/
{
	assert(p != NULL && o != NULL);
	assert(dsa_prkey_t_chk_members(p,451805986));
	if (p != o) {
		bi_asg_si(p->x, 0);
		bi_swap(p->x, o->x);
	}
}
/*----------------------------------------------------------------------------*/
struct dsa_prkey_t* dsa_prkey_t_new()/*auto*/
{
	struct dsa_prkey_t* p = (struct dsa_prkey_t*)malloc(sizeof(struct dsa_prkey_t));
	if (p != NULL) {
		dsa_prkey_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
struct dsa_prkey_t* dsa_prkey_t_clone(const struct dsa_prkey_t* o)/*auto*/
{
	struct dsa_prkey_t* p = NULL;
	if (o != NULL) {
		p = (struct dsa_prkey_t*)malloc(sizeof(struct dsa_prkey_t));
		if (p != NULL) {
			dsa_prkey_t_ctor(p);
			dsa_prkey_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void dsa_prkey_t_delete(struct dsa_prkey_t* p)/*auto*/
{
	if (p != NULL) {
		dsa_prkey_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

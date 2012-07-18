/*----------------------------------------------------------------------------*/
/* Copyright (c) 2012 Vicente Benjumea, University of Malaga, Spain           */
/*                                                                            */
/* This program is free software: you can redistribute it and/or              */
/* modify it under the terms of the GNU General Public License as             */
/* published by the Free Software Foundation, either version 2 of             */
/* the License, or (at your option) any later version.                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               */
/* GNU General Public License for more details.                               */
/*                                                                            */
/* You should have received a copy of the GNU General Public                  */
/* License along with this program. If not, see                               */
/* <http://www.gnu.org/licenses/>.                                            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "elgamal.h"
#include "elgamal_thr.h"
#include "nadrp.h"
#include "paillier.h"
#include "paillier_thr.h"
#include "random.h"
#include "birnd.h"
#include "schnorr.h"
#include "sphere.h"
#include "syspar.h"

#include "cdtor.h"
/*----------------------------------------------------------------------------*/
#define NFAS	2
#define MSG1	"hello world"
#define MSG1LEN	strlen(MSG1)
#define MSG2	"HELLO WORLD"
#define MSG2LEN	strlen(MSG2)
/*----------------------------------------------------------------------------*/
void check_spheres(rnd_ctx_t* rnd_ctx)
{
	BEG_VAR_1_bigint_t(n);
	BEG_VAR_10(syspar_t, sp,
		   sphere_t, lambda,
		   sphere_t, lambda_dsa,
		   sphere_t, mu,
		   sphere_t, gamma,
		   sphere_t, gamma_mu,
		   sphere_t, theta,
		   sphere_t, omega,
		   sphere_t, upsilon,
		   sphere_t, psi);
	/*--------------------------------*/
	set_default_syspar(&sp);
	bi_random_nb(n, sp.nu, rnd_ctx);
	/*--------------------------------*/
	create_sph_lambda(&lambda, &sp);
	print_spheres("L", &lambda);
	/*--------------------------------*/
	create_sph_lambda_dsa(&lambda_dsa, &sp);
	print_spheres("LDSA", &lambda_dsa);
	/*--------------------------------*/
	create_sph_mu(&mu, &sp);
	print_spheres("M", &mu);
	/*--------------------------------*/
	create_sph_gamma(&gamma, &sp);
	print_spheres("G", &gamma);
	/*--------------------------------*/
	create_sph_gamma_mu(&gamma_mu, &gamma, &mu, &sp);
	print_spheres("GM", &gamma_mu);
	/*--------------------------------*/
	create_sph_theta(&theta, &mu);
	print_spheres("T", &theta);
	/*--------------------------------*/
	create_sph_omega(&omega, n, &sp);
	print_spheres("O", &omega);
	/*--------------------------------*/
	create_sph_upsilon(&upsilon, n, &sp);
	print_spheres("U", &upsilon);
	/*--------------------------------*/
	create_sph_psi(&psi, n, &sp);
	print_spheres("P", &psi);
	/*--------------------------------*/
	END_VAR_10(syspar_t, sp,
		   sphere_t, lambda,
		   sphere_t, lambda_dsa,
		   sphere_t, mu,
		   sphere_t, gamma,
		   sphere_t, gamma_mu,
		   sphere_t, theta,
		   sphere_t, omega,
		   sphere_t, upsilon,
		   sphere_t, psi);
	END_VAR_1_bigint_t(n);
}
/*----------------------------------------------------------------------------*/
void check_schnorr(rnd_ctx_t* rnd_ctx)
{
	bool_t ok0, ok1, ok2;
	BEG_BIGINT_CHK();
	BEG_VAR_4(schnorr_pbkey_t, pk,
		   schnorr_prkey_t, sk,
		   schnorr_sign_t, sg,
		   syspar_t, sp);
	BEG_VAR_1(sphere_t, lambda);
	/*--------------------------------*/
	printf("SCHNORR SIGNATURES\n");
	set_default_syspar(&sp);
	create_sph_lambda(&lambda, &sp);
	/*--------------------------------*/
	INIT_TIMER(Schnorr_Create_Keys);
	schnorr_create_keys(&pk, &sk, &lambda, sp.nu, rnd_ctx);
	PRINT_TIMER(Schnorr_Create_Keys);
	INIT_TIMER(Schnorr_Sign);
	ok0 = schnorr_sign_dat(sp.k, &sg, MSG1, MSG1LEN, &pk, &sk, &lambda,rnd_ctx);
	PRINT_TIMER(Schnorr_Sign);
	INIT_TIMER(Schnorr_Vrfy_1);
	ok1 = schnorr_vrfy_dat(sp.k, &sg, MSG1, MSG1LEN, &pk, &lambda);
	PRINT_TIMER(Schnorr_Vrfy_1);
	INIT_TIMER(Schnorr_Vrfy_2);
	ok2 = schnorr_vrfy_dat(sp.k, &sg, MSG2, MSG2LEN, &pk, &lambda);
	PRINT_TIMER(Schnorr_Vrfy_2);
	if (ok0 && ok1 && ! ok2) {
		printf("\tOK, schnorr signatures OK\n");
	} else {
		printf("\tERROR, schnorr signatures ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_1(sphere_t, lambda);
	END_VAR_4(schnorr_pbkey_t, pk,
		   schnorr_prkey_t, sk,
		   schnorr_sign_t, sg,
		   syspar_t, sp);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_elgamal(rnd_ctx_t* rnd_ctx)
{
	BEG_BIGINT_CHK();
	BEG_VAR_3_bigint_t(msg1, msg2, order);
	BEG_VAR_6(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_modord_t, p1q1,
		   elgamal_prkey_t, sk,
		   elgamal_ciphertext_t, ct,
		   syspar_t, sp);
	BEG_VAR_1(sphere_t, mu);
	/*--------------------------------*/
	printf("ELGAMAL ENCRYPTION\n");
	set_default_syspar(&sp);
	create_sph_mu(&mu, &sp);
	/*--------------------------------*/
	INIT_TIMER(ElGamal_Create_Keys);
	elgamal_create_keys(&pk, &sk, &npq, &p1q1, &mu, sp.nu, rnd_ctx);
	PRINT_TIMER(ElGamal_Create_Keys);
	bi_asg_ui(msg1, 12345);
	INIT_TIMER(ElGamal_Encrypt);
	elgamal_encrypt(&ct, msg1, &pk, &mu, rnd_ctx);
	PRINT_TIMER(ElGamal_Encrypt);
	INIT_TIMER(ElGamal_Decrypt);
	elgamal_decrypt(msg2, &ct, &pk, &sk);
	PRINT_TIMER(ElGamal_Decrypt);
	if ( bi_equals(msg1, msg2) ) {
		printf("\tOK, elgamal encryption OK\n");
	} else {
		printf("\tERROR, elgamal encryption ERROR\n");
	}
	/*--------------------------------*/
	fprintf(stderr, "\tNbits N: [%u]\n", bi_nbits(pk.n));
	fprintf(stderr, "\tNbits G: [%u]\n", bi_nbits(pk.g));
	fprintf(stderr, "\tNbits Y: [%u]\n", bi_nbits(pk.y));
	fprintf(stderr, "\tNbits X: [%u]\n", bi_nbits(sk.x));
	/*--------------------------------*/
	bi_mul(order, npq.p, npq.q);
	fprintf(stderr, "\tNbits PQ: [%u]\n", bi_nbits(order));
	bi_mul(order, p1q1.p1, p1q1.q1);
	fprintf(stderr, "\tNbits P1Q1: [%u]\n", bi_nbits(order));

	/*--------------------------------*/
	END_VAR_1(sphere_t, mu);
	END_VAR_6(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_modord_t, p1q1,
		   elgamal_prkey_t, sk,
		   elgamal_ciphertext_t, ct,
		   syspar_t, sp);
	END_VAR_3_bigint_t(msg1, msg2, order);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_elgamal_thr(rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	int i;
	BEG_BIGINT_CHK();
	BEG_VAR_2_bigint_t(msg1, msg2);
	BEG_VAR_6(elgamal_thr_pbkey_t, pk,
		   elgamal_thr_modfact_t, npq,
		   elgamal_thr_modord_t, p1q1,
		   elgamal_thr_ciphertext_t, ct,
		   elgamal_thr_dshare_acc_t, dac,
		   syspar_t, sp);
	BEG_VAR_A(elgamal_thr_pbkey_share_t, pkj, NFAS);
	BEG_VAR_A(elgamal_thr_prkey_t, skj, NFAS);
	BEG_VAR_A(elgamal_thr_decrypt_share_t, dsj, NFAS);
	BEG_VAR_1(sphere_t, mu);
	/*--------------------------------*/
	printf("ELGAMAL_THR ENCRYPTION\n");
 	set_default_syspar(&sp);
	create_sph_mu(&mu, &sp);
	/*--------------------------------*/
	INIT_TIMER(ElGamal_Thr_Create_PbKey);
	elgamal_thr_create_pbkey(&pk, &npq, &p1q1, sp.nu, rnd_ctx);
	PRINT_TIMER(ElGamal_Thr_Create_PbKey);
	for (i = 0; i < NFAS; ++i) {
		INIT_TIMER(ElGamal_Thr_Create_PrKey_j);
		elgamal_thr_create_prkey(&pkj[i], &skj[i], &pk, &mu, rnd_ctx);
		PRINT_TIMER(ElGamal_Thr_Create_PrKey_j);
		INIT_TIMER(ElGamal_Thr_Add_PbKeySh_j);
		elgamal_thr_add_pbkey_share(&pk, &pkj[i]);
		PRINT_TIMER(ElGamal_Thr_Add_PbKeySh_j);
	}
	bi_asg_ui(msg1, 12345);
	INIT_TIMER(ElGamal_Thr_Encrypt);
	elgamal_thr_encrypt(&ct, msg1, &pk, &mu, rnd_ctx);
	PRINT_TIMER(ElGamal_Thr_Encrypt);
	for (i = 0; (i < NFAS); ++i) {
		INIT_TIMER(ElGamal_Thr_Decrypt_Sh_j);
		ok = elgamal_thr_decrypt_share(&dsj[i], &ct, &pk, &pkj[i], &skj[i],
								  &mu, NULL, 0, rnd_ctx);
		PRINT_TIMER(ElGamal_Thr_Decrypt_Sh_j);
		INIT_TIMER(ElGamal_Thr_Add_DSh_j);
		ok = ok && elgamal_thr_add_decrypt_share(&dac, &dsj[i], &ct, &pk,
												 &pkj[i],
												 &mu, NULL, 0);
		PRINT_TIMER(ElGamal_Thr_Add_DSh_j);
		assert(ok);
	}
	INIT_TIMER(ElGamal_Thr_Decrypt);
	ok = elgamal_thr_decrypt(msg2, &ct, &pk, &dac);
	PRINT_TIMER(ElGamal_Thr_Decrypt);
	assert(ok);
	if ( bi_equals(msg1, msg2) ) {
		printf("\tOK, elgamal_thr encryption OK\n");
	} else {
		printf("\tERROR, elgamal_thr encryption ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_1(sphere_t, mu);
	END_VAR_A(elgamal_thr_decrypt_share_t, dsj, NFAS);
	END_VAR_A(elgamal_thr_prkey_t, skj, NFAS);
	END_VAR_A(elgamal_thr_pbkey_share_t, pkj, NFAS);
	END_VAR_6(elgamal_thr_pbkey_t, pk,
		   elgamal_thr_modfact_t, npq,
		   elgamal_thr_modord_t, p1q1,
		   elgamal_thr_ciphertext_t, ct,
		   elgamal_thr_dshare_acc_t, dac,
		   syspar_t, sp);
	END_VAR_2_bigint_t(msg1, msg2);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_paillier(rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	BEG_VAR_2_bigint_t(msg1, msg2);
	BEG_VAR_4(paillier_pbkey_t, pk,
		   paillier_prkey_t, sk,
		   paillier_ciphertext_t, ct,
		   syspar_t, sp);
	/*--------------------------------*/
	printf("PAILLIER ENCRYPTION\n");
 	set_default_syspar(&sp);
	/*--------------------------------*/
	INIT_TIMER(Paillier_Create_Keys);
	paillier_create_keys(&pk, &sk, sp.nu, rnd_ctx);
	PRINT_TIMER(Paillier_Create_Keys);
	bi_asg_ui(msg1, 12345);
	INIT_TIMER(Paillier_Encrypt);
	paillier_encrypt(&ct, msg1, &pk, rnd_ctx);
	PRINT_TIMER(Paillier_Encrypt);
	INIT_TIMER(Paillier_Decrypt);
	ok = paillier_decrypt(msg2, &ct, &pk, &sk);
	PRINT_TIMER(Paillier_Decrypt);
	assert(ok);
	if ( bi_equals(msg1, msg2) ) {
		printf("\tOK, paillier encryption OK\n");
	} else {
		printf("\tERROR, paillier encryption ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_4(paillier_pbkey_t, pk,
		   paillier_prkey_t, sk,
		   paillier_ciphertext_t, ct,
		   syspar_t, sp);
	END_VAR_2_bigint_t(msg1, msg2);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_paillier_thr(rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	int i;
	BEG_BIGINT_CHK();
	BEG_VAR_2_bigint_t(msg1, msg2);
	BEG_VAR_4(paillier_thr_pbkey_t, pk,
		   paillier_thr_ciphertext_t, ct,
		   paillier_thr_dshare_acc_t, dac,
		   syspar_t, sp);
	BEG_VAR_A(paillier_thr_pbkey_share_t, pkj, NFAS);
	BEG_VAR_A(paillier_thr_prkey_t, skj, NFAS);
	BEG_VAR_A(paillier_thr_decrypt_share_t, dsj, NFAS);
	/*--------------------------------*/
	printf("PAILLIER_THR ENCRYPTION\n");
 	set_default_syspar(&sp);
	/*--------------------------------*/
	INIT_TIMER(Paillier_Thr_Create_PbKey);
	paillier_thr_create_pbkey(&pk, sp.nu, rnd_ctx);
	PRINT_TIMER(Paillier_Thr_Create_PbKey);
	for (i = 0; i < NFAS; ++i) {
		INIT_TIMER(Paillier_Thr_Create_PrKey_j);
		paillier_thr_create_prkey(&pkj[i], &skj[i], &pk, rnd_ctx);
		PRINT_TIMER(Paillier_Thr_Create_PrKey_j);
		INIT_TIMER(Paillier_Thr_Add_PbKeySh_j);
		paillier_thr_add_pbkey_share(&pk, &pkj[i]);
		PRINT_TIMER(Paillier_Thr_Add_PbKeySh_j);
	}
	bi_asg_ui(msg1, 12345);
	INIT_TIMER(Paillier_Thr_Encrypt);
	paillier_thr_encrypt(&ct, msg1, &pk, rnd_ctx);
	PRINT_TIMER(Paillier_Thr_Encrypt);
	for (i = 0; i < NFAS; ++i) {
		INIT_TIMER(Paillier_Thr_Decrypt_Sh_j);
		ok = paillier_thr_decrypt_share(&dsj[i], &ct, &pk, &pkj[i], &skj[i],
								   NULL, 0, rnd_ctx);
		PRINT_TIMER(Paillier_Thr_Decrypt_Sh_j);
		INIT_TIMER(Paillier_Thr_Add_DSh_j);
		ok = ok && paillier_thr_add_decrypt_share(&dac, &dsj[i], &ct, &pk,
												  &pkj[i], NULL, 0);
		PRINT_TIMER(Paillier_Thr_Add_DSh_j);
		assert(ok);
	}
	INIT_TIMER(Paillier_Thr_Decrypt);
	ok = paillier_thr_decrypt(msg2, &ct, &pk, &dac);
	PRINT_TIMER(Paillier_Thr_Decrypt);
	assert(ok);
	if ( bi_equals(msg1, msg2) ) {
		printf("\tOK, paillier_thr encryption OK\n");
	} else {
		printf("\tERROR, paillier_thr encryption ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_A(paillier_thr_decrypt_share_t, dsj, NFAS);
	END_VAR_A(paillier_thr_prkey_t, skj, NFAS);
	END_VAR_A(paillier_thr_pbkey_share_t, pkj, NFAS);
	END_VAR_4(paillier_thr_pbkey_t, pk,
		   paillier_thr_ciphertext_t, ct,
		   paillier_thr_dshare_acc_t, dac,
		   syspar_t, sp);
	END_VAR_2_bigint_t(msg1, msg2);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_nadrp(rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_4(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_prkey_t, sk,
		   syspar_t, sp);
	BEG_VAR_4_bigint_t(ax, x, a, h);
	BEG_VAR_1(sphere_t, mu);
	/*--------------------------------*/
	printf("NADRP\n");
	set_default_syspar(&sp);
	create_sph_mu(&mu, &sp);
	/*--------------------------------*/
	elgamal_create_keys(&pk, &sk, NULL, NULL, &mu, sp.nu, rnd_ctx);
	/*--------------------------------*/
	bi_random_nb(a, sp.nu/2, rnd_ctx);
	bi_powmod_sec(a, pk.g, a, pk.n);
	bi_random_nb(h, sp.nu/2, rnd_ctx);
	bi_powmod_sec(h, pk.g, h, pk.n);
	ok = nadrp_prtcl(&ax, &x,
					 (const bigint_t*)&pk.n,
					 (const bigint_t*)&a,
					 (const bigint_t*)&pk.g,
					 (const bigint_t*)&h,
					 (const bigint_t*)&pk.y,
					 &sp, &mu, rnd_ctx);
	bi_powmod_sec(h, a, x, pk.n);
	if ( bi_equals(ax, h) && bi_belongs_sph_ek(x, &mu) ) {
		printf("\tOK, nadrp OK\n");
	} else {
		printf("\tERROR, nadrp ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_1(sphere_t, mu);
	END_VAR_4_bigint_t(ax, x, a, h);
	END_VAR_4(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_prkey_t, sk,
		   syspar_t, sp);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
void check_nadrp_x(rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	BEG_VAR_4(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_prkey_t, sk,
		   syspar_t, sp);
	BEG_VAR_4_bigint_t(ax, x, a, h);
	BEG_VAR_1(sphere_t, mu);
	/*--------------------------------*/
	printf("NADRP\n");
	set_default_syspar(&sp);
	sp.nu = 24;
	sp.k = 8;
	create_sph_mu(&mu, &sp);
	/*--------------------------------*/
	elgamal_create_keys(&pk, &sk, NULL, NULL, &mu, sp.nu, rnd_ctx);
	/*--------------------------------*/
	bi_random_nb(a, sp.nu/2, rnd_ctx);
	bi_powmod_sec(a, pk.g, a, pk.n);
	bi_random_nb(h, sp.nu/2, rnd_ctx);
	bi_powmod_sec(h, pk.g, h, pk.n);
	/*--------------------------------*/
	fprintf(stderr, "XXXXXXXXX\n");
	bi_asg_ui(pk.n, 2773);
	bi_asg_ui(a, 2149);
	bi_asg_ui(pk.g, 670);
	bi_asg_ui(h, 1760);
	bi_asg_ui(pk.y, 1614);
	sp.nu = 12;
	sp.k = 2;
	mu.log2radius = 8;
	mu.log2radius_ek = 4;
	bi_asg_ui(mu.center, 256);
	fprintf(stderr, "ZZZZZZZZZZ\n");
	/*--------------------------------*/
	ok = nadrp_prtcl(&ax, &x,
					 (const bigint_t*)&pk.n,
					 (const bigint_t*)&a,
					 (const bigint_t*)&pk.g,
					 (const bigint_t*)&h,
					 (const bigint_t*)&pk.y,
					 &sp, &mu, rnd_ctx);
	bi_powmod_sec(h, a, x, pk.n);
	if ( bi_equals(ax, h) && bi_belongs_sph_ek(x, &mu) ) {
		printf("\tOK, nadrp OK\n");
	} else {
		printf("\tERROR, nadrp ERROR\n");
	}
	/*--------------------------------*/
	END_VAR_1(sphere_t, mu);
	END_VAR_4_bigint_t(ax, x, a, h);
	END_VAR_4(elgamal_pbkey_t, pk,
		   elgamal_modfact_t, npq,
		   elgamal_prkey_t, sk,
		   syspar_t, sp);
	END_BIGINT_CHK();
}
/*----------------------------------------------------------------------------*/
int main()
{
	BEG_BIGINT_CHK();
	struct rndctx_t* rnd_ctx = rndctx_t_new();
	random_seed(rnd_ctx, PSEUDO_ENTROPY);
    /*random_seed(rnd_ctx, NO_ENTROPY);*/

	check_spheres(rnd_ctx);
	check_schnorr(rnd_ctx);
	check_elgamal(rnd_ctx);
	check_elgamal_thr(rnd_ctx);
	check_paillier(rnd_ctx);
	check_paillier_thr(rnd_ctx);
	check_nadrp(rnd_ctx);
/* 
 * 	check_nadrp_x(rnd_ctx);
 */
	rndctx_t_delete(rnd_ctx);
	END_BIGINT_CHK();
	return 0;
}
/*----------------------------------------------------------------------------*/

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
#include <time.h>
#include "ftmgs.h"
/*----------------------------------------------------------------------------*/
#define NU			Nu1
#define SHA_MODE	Sha1
/*------------------------------------*/
#define MSG1	"hello world"
#define MSG2	"Hello world"
#define MSG3	"hello World"
#define MSG4	"Hello World"
/*----------------------------------------------------------------------------*/
#define INIT_TIMER(name)								\
	do{{												\
	clock_t time__##name = clock()
#define PRINT_TIMER(name)											\
	do{																\
		time__##name=clock()-(time__##name);						\
		fprintf(stderr, "\t%s Time: %.8f seconds", #name,			\
			   ((double)(time__##name)/CLOCKS_PER_SEC));			\
		fprintf(stderr,"\n");										\
	}while(0);														\
	}}while(0)
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifdef NCHECK_ASN1__
#define GENERATE_ASN1_CHECK(FUNC_NAME, TYPE_NAME)							\
	static void asn1_check_##FUNC_NAME(TYPE_NAME* p) { (void)p; }
#define GENERATE_ASN1_CHECK2(FUNC_NAME, TYPE_NAME)							\
	static void asn1_check_##FUNC_NAME(TYPE_NAME** p1, TYPE_NAME** p2)	\
	{ (void)p1; (void)p2; }
#else
/*----------------------------------------------------------------------------*/
static int buffer_t_cmp(const buffer_t* b1, const buffer_t* b2)
{
	int ok = buffer_size(b1) == buffer_size(b2);
	unsigned i;
	if (ok) {
		for (i = 0; (i < buffer_size(b1))
				 &&(buffer_data(b1)[i] == buffer_data(b1)[i]); ++i) {
			/* empty */
		}
		ok = (i >= buffer_size(b1));
	}
	return ok;
}
/*----------------------------------------------------------------------------*/
#define GENERATE_ASN1_CHECK(FUNC_NAME, TYPE_NAME)						\
	static void asn1_check_##FUNC_NAME(TYPE_NAME* p)					\
	{																	\
		extern const char* asn1_strerror(asn1_retcode_t rc);			\
		asn1_retcode_t rc;												\
		buffer_t* buff1 = buffer_t_new();								\
		buffer_t* buff2 = buffer_t_new();								\
		/*--------------------------------*/							\
		rc = asn1_enc_##FUNC_NAME(buff1, p);							\
		if (rc != ASN1_Success) {										\
			fprintf(stderr, "Error: ASN1 encoding (%d) [%s]\n",			\
					rc, asn1_strerror(rc));								\
		} else {														\
			/*--------------------------------*/						\
			rc = asn1_dec_##FUNC_NAME(p, buffer_data(buff1),			\
									  (int)buffer_size(buff1));			\
			if (rc != ASN1_Success) {									\
				fprintf(stderr, "Error: ASN1 decoding (%d) [%s]\n",		\
						rc, asn1_strerror(rc));							\
			} else {													\
				rc = asn1_enc_##FUNC_NAME(buff2, p);					\
				if (rc != ASN1_Success) {								\
					fprintf(stderr, "Error: ASN1 encoding2 (%d) [%s]\n", \
							rc, asn1_strerror(rc));						\
				} else if (! buffer_t_cmp(buff1, buff2)) {				\
					fprintf(stderr, "Error: ASN1 encoding/decoding\n");	\
				} else {												\
					fprintf(stderr, "\tASN1 DER encoding size: %s: %u\n", \
							#TYPE_NAME, buffer_size(buff1));			\
				}														\
			}															\
		}																\
		/*--------------------------------*/							\
		buffer_t_delete(buff2);											\
		buffer_t_delete(buff1);											\
	}
#define GENERATE_ASN1_CHECK2(FUNC_NAME, TYPE_NAME)						\
	static void asn1_check_##FUNC_NAME(TYPE_NAME** p1, TYPE_NAME** p2) \
	{																	\
		extern const char* asn1_strerror(asn1_retcode_t rc);			\
		asn1_retcode_t rc;												\
		buffer_t* buff1 = buffer_t_new();								\
		buffer_t* buff2 = buffer_t_new();								\
		/*--------------------------------*/							\
		rc = asn1_enc_##FUNC_NAME(buff1, *p1);							\
		if (rc != ASN1_Success) {										\
			fprintf(stderr, "Error: ASN1 encoding (%d) [%s]\n",			\
					rc, asn1_strerror(rc));								\
		} else {														\
			/*--------------------------------*/						\
			rc = asn1_dec_##FUNC_NAME(*p2, buffer_data(buff1),			\
									  (int)buffer_size(buff1));			\
			if (rc != ASN1_Success) {									\
				fprintf(stderr, "Error: ASN1 decoding (%d) [%s]\n",		\
						rc, asn1_strerror(rc));							\
			} else {													\
				rc = asn1_enc_##FUNC_NAME(buff2, *p2);					\
				if (rc != ASN1_Success) {								\
					fprintf(stderr, "Error: ASN1 encoding2 (%d) [%s]\n", \
							rc, asn1_strerror(rc));						\
				} else if (! buffer_t_cmp(buff1, buff2)) {				\
					fprintf(stderr, "Error: ASN1 encoding/decoding\n");	\
				} else {												\
					TYPE_NAME* px = *p1; *p1 = *p2; *p2 = px;			\
					fprintf(stderr, "\tASN1 DER encoding size: %s: %u\n", \
							#TYPE_NAME, buffer_size(buff1));			\
				}														\
			}															\
		}																\
		/*--------------------------------*/							\
		buffer_t_delete(buff2);											\
		buffer_t_delete(buff1);											\
	}
#endif
/*------------------------------------*/
GENERATE_ASN1_CHECK(dssparms, dss_parms_t)
GENERATE_ASN1_CHECK(dsapbkey, dsa_pbkey_t)
GENERATE_ASN1_CHECK(dsaprkey, dsa_prkey_t)
GENERATE_ASN1_CHECK(fapbkey, ftmgs_fa_pbkey_t)
GENERATE_ASN1_CHECK(grpbkey, ftmgs_pbkey_t)
GENERATE_ASN1_CHECK(grprkey, ftmgs_prkey_t)
GENERATE_ASN1_CHECK(fapbkeysh, ftmgs_faj_pbkey_share_t)
GENERATE_ASN1_CHECK(faprkey, ftmgs_faj_prkey_t)
GENERATE_ASN1_CHECK(fagrpbkeysh, ftmgs_faj_gr_pbkey_share_t)
GENERATE_ASN1_CHECK(fagrprkey, ftmgs_faj_gr_prkey_t)
GENERATE_ASN1_CHECK(dlog, dlog_t)
GENERATE_ASN1_CHECK(dlogx, dlogx_t)
GENERATE_ASN1_CHECK(mbrref, ftmgs_mbr_ref_t)
GENERATE_ASN1_CHECK(mbrprkey, ftmgs_mbr_prkey_t)
GENERATE_ASN1_CHECK(sign, ftmgs_sign_t)
GENERATE_ASN1_CHECK(open, ftmgs_open_t)
GENERATE_ASN1_CHECK(openshare, ftmgs_opensharej_t)
GENERATE_ASN1_CHECK(openacc, ftmgs_openacc_t)
GENERATE_ASN1_CHECK(mtkey, ftmgs_mtkey_t)
GENERATE_ASN1_CHECK(mtkeyshare, ftmgs_mtkey_sharej_t)
GENERATE_ASN1_CHECK(mtkeyacc, ftmgs_mtkey_acc_t)
GENERATE_ASN1_CHECK(claim, ftmgs_claim_t)
GENERATE_ASN1_CHECK(link, ftmgs_link_t)
GENERATE_ASN1_CHECK2(joinpbl, ftmgs_join_pbl_t)
GENERATE_ASN1_CHECK2(joinprv, ftmgs_join_prv_t)
/*----------------------------------------------------------------------------*/
/* 
	P: 153264416848194960527208940181356886994393289566996712615363676133692\
785284360759914535230382832505083471115520282621731340253537970607128539\
774703299245631294533018825099572122728057495798551203036789968670998730\
806268118546327624398287562933627024118237164087800262190498468927451654\
429569208006149330269991
	Q: 885906245263438157777387837829435020701245353167
	G: 720177377161567663364428972564320627595231762802048403915882173153292304\
002358091549015955592414548505126975908884454609122540228898450560360051\
895605282540761658359833696832113359554841246450528465008694066369865515\
840762644854724650759890440925149673939758691690398733257180349239579718\
44549872709414657647
	Y: 127476657220676638266209208806434436903927190056318596274414907370827\
162153213597627229380146744024390966328630740611982373129741494556249303\
410946062184111855191662141163191642601518732112473792599308733560559789\
868503244991802535761471118808028420380874004101206479578324690764006462\
260490895932276930762829
	X: 621781776391842983413715080838583528160221323122
 */
/*----------------------------------------------------------------------------*/
static unsigned char dss_params[] = {
	0x30, 0x82, 0x01, 0x1E, 0x02, 0x81, 0x81, 0x00, 0xDA, 0x41,
	0x7A, 0x6C, 0xF1, 0x0E, 0x2E, 0x1E, 0x10, 0x0F, 0xBC, 0x74,
	0x0D, 0xE6, 0x1E, 0x9E, 0x0E, 0x08, 0xB7, 0xAA, 0xDD, 0x6A,
	0xD7, 0xEE, 0x06, 0xD8, 0x33, 0x02, 0xF4, 0x05, 0xCB, 0x81,
	0x4A, 0xBA, 0x25, 0x66, 0xB4, 0xED, 0x9F, 0x53, 0xB5, 0xC6,
	0x30, 0x7E, 0xDC, 0xE5, 0x53, 0xB5, 0x9D, 0xDB, 0xF5, 0x40,
	0xCA, 0x57, 0x7C, 0x4B, 0xD8, 0x62, 0xBA, 0xE1, 0x2F, 0x8B,
	0x41, 0xFC, 0x99, 0xD0, 0x32, 0xA6, 0x6F, 0x75, 0x2D, 0xBD,
	0xE7, 0x9C, 0x7D, 0xEF, 0x76, 0x68, 0x1B, 0x3C, 0xF3, 0x9A,
	0x87, 0x89, 0xD0, 0x6D, 0x5E, 0xA4, 0x8F, 0x72, 0xD7, 0x4B,
	0x42, 0xB4, 0x35, 0xE6, 0xA8, 0x03, 0x06, 0x14, 0xCC, 0xD2,
	0xC9, 0xFA, 0x5F, 0x83, 0x52, 0x97, 0xAE, 0xE5, 0x41, 0x0B,
	0xE5, 0xB5, 0x58, 0xA5, 0x54, 0x5F, 0xC8, 0xAA, 0x54, 0x52,
	0xA2, 0x66, 0x87, 0x51, 0x77, 0x27, 0x02, 0x15, 0x00, 0x9B,
	0x2D, 0x68, 0xF2, 0xA9, 0xD3, 0xD6, 0x73, 0xF3, 0xB7, 0x5B,
	0x07, 0x1B, 0x9B, 0x36, 0x87, 0x9A, 0x9F, 0xAC, 0xCF, 0x02,
	0x81, 0x80, 0x66, 0x8E, 0x81, 0x73, 0x22, 0x22, 0xC2, 0x36,
	0x76, 0x67, 0x55, 0xEC, 0xBE, 0x11, 0xD3, 0x33, 0xB3, 0x18,
	0x63, 0x69, 0x32, 0x58, 0xA8, 0x70, 0xAC, 0xE7, 0xB9, 0x62,
	0x9A, 0xD3, 0xFE, 0x52, 0x7D, 0x3D, 0xFD, 0xFC, 0x4A, 0xBE,
	0x7D, 0xA0, 0x97, 0x5F, 0x4B, 0x7C, 0x23, 0xAD, 0x35, 0x66,
	0x7F, 0x2C, 0x5D, 0x09, 0x56, 0x41, 0xF3, 0x4A, 0x77, 0x81,
	0x8B, 0xE0, 0x1A, 0xB3, 0xC9, 0x78, 0x32, 0x0C, 0x6C, 0x2A,
	0x0E, 0x6B, 0x5B, 0x2E, 0x04, 0x82, 0x27, 0x42, 0x77, 0xB9,
	0x19, 0x82, 0xBE, 0xD8, 0x09, 0xBA, 0x99, 0x6C, 0x28, 0x96,
	0xF1, 0x1F, 0x1D, 0x27, 0xDF, 0x4B, 0xF0, 0xFF, 0x99, 0x04,
	0x79, 0x0C, 0x6D, 0x21, 0xA7, 0x20, 0x78, 0xCB, 0xDB, 0x77,
	0x0E, 0x22, 0x17, 0x18, 0x5E, 0xB4, 0x71, 0xD4, 0x54, 0x87,
	0x5E, 0xA2, 0x4F, 0x38, 0xA5, 0x03, 0xF7, 0xE7, 0x9E, 0x6F
};
static unsigned char dsa_pbkey[] = {
	0x02, 0x81, 0x81, 0x00, 0xB5, 0x88, 0x64, 0xA3, 0xA5, 0xA5,
	0x87, 0x30, 0x99, 0xBB, 0x7C, 0xD7, 0xAE, 0x75, 0xCD, 0x65,
	0xBA, 0x76, 0x13, 0x9E, 0x5D, 0x6A, 0x1E, 0x4B, 0xDE, 0x16,
	0x69, 0xC8, 0x10, 0x95, 0x0B, 0xF3, 0x35, 0xA9, 0x4A, 0x0B,
	0x67, 0xB7, 0x31, 0xB3, 0x6B, 0x15, 0x2A, 0xD0, 0x7F, 0x9B,
	0x64, 0x9B, 0xBA, 0x8E, 0x42, 0x5C, 0xF4, 0x42, 0x1F, 0x42,
	0x68, 0x98, 0x58, 0x83, 0x66, 0x1C, 0xBC, 0xBF, 0x73, 0xE3,
	0xC4, 0x16, 0xFB, 0x08, 0x47, 0x1A, 0xE0, 0x23, 0x18, 0x5E,
	0x57, 0xF4, 0x5B, 0xB8, 0x8A, 0x4A, 0x0C, 0x33, 0x47, 0x22,
	0x24, 0xB7, 0xB1, 0xC5, 0xA1, 0x76, 0x21, 0xA6, 0x47, 0xBA,
	0xA2, 0x97, 0x80, 0x5B, 0xDF, 0x62, 0xBA, 0x4F, 0x35, 0x10,
	0x9D, 0xB4, 0x39, 0xCC, 0xA9, 0x7C, 0x65, 0x61, 0x2F, 0xD1,
	0x50, 0x75, 0x67, 0x9B, 0x35, 0x6D, 0x66, 0x0E, 0x33, 0x2B,
	0xD8, 0x4D
};
static unsigned char dsa_prkey[] = {
	0x02, 0x14, 0x6C, 0xE9, 0xA8, 0xBD, 0xFB, 0x6D, 0xE3, 0x37,
	0xC7, 0x21, 0xC5, 0x9F, 0xE0, 0x06, 0x8E, 0x73, 0x58, 0xC5,
	0xDF, 0x72
};
/*----------------------------------------------------------------------------*/
void create_ukey_from_dsa(dlogx_t* umk, dlog_t* u_auth)
{
	/*--------------------------------*/
	asn1_retcode_t rc;
	/*--------------------------------*/
	struct dss_parms_t* dsa_p = dss_parms_t_new();
	struct dsa_pbkey_t* dsa_pk = dsa_pbkey_t_new();
	struct dsa_prkey_t* dsa_sk = dsa_prkey_t_new();
	/*--------------------------------*/
	rc = asn1_dec_dssparms(dsa_p, dss_params,
						   sizeof(dss_params)/sizeof(*dss_params));
	if (rc != ASN1_Success) {
		fprintf(stderr, "Error: ASN1 dss_params decoding [%d]\n", rc);
	}
	/*--------------------------------*/
	rc = asn1_dec_dsapbkey(dsa_pk, dsa_pbkey,
						   sizeof(dsa_pbkey)/sizeof(*dsa_pbkey));
	if (rc != ASN1_Success) {
		fprintf(stderr, "Error: ASN1 dsa_pbkey decoding [%d]\n", rc);
	}
	/*--------------------------------*/
	rc = asn1_dec_dsaprkey(dsa_sk, dsa_prkey,
						   sizeof(dsa_prkey)/sizeof(*dsa_prkey));
	if (rc != ASN1_Success) {
		fprintf(stderr, "Error: ASN1 dsa_prkey decoding [%d]\n", rc);
	}
	/*--------------------------------*/
	extract_uauth_from_dsa(u_auth, dsa_pk, dsa_p);
	extract_umk_from_dsa(umk, dsa_sk);
	/*--------------------------------*/
	asn1_check_dssparms(dsa_p);
	asn1_check_dsapbkey(dsa_pk);
	asn1_check_dsaprkey(dsa_sk);
	asn1_check_dlogx(umk);
	asn1_check_dlog(u_auth);
	/*--------------------------------*/
	dss_parms_t_delete(dsa_p);
	dsa_pbkey_t_delete(dsa_pk);
	dsa_prkey_t_delete(dsa_sk);
}
/*----------------------------------------------------------------------------*/
void check(bool_t ok, const char* msg)
{
	if (!ok) {
		fprintf(stderr, "%s\n", msg);
		abort();
	}
}
/*----------------------------------------------------------------------------*/
void random_numbers(rndctx_t* rnd_ctx)
{
	struct data_t {
		unsigned x;
		unsigned y;
	};
	struct data_t d;
	unsigned n;
	fprintf(stderr, "-Random-Numbers-----------------\n");
	n = random_uint(1000, rnd_ctx);
	random_bytes(&d, sizeof(d), rnd_ctx);
	fprintf(stderr, "N: %u\tX: 0x%08x\tY: 0x%08x\n", n, d.x, d.y);
	n = random_uint(1000, rnd_ctx);
	random_bytes(&d, sizeof(d), rnd_ctx);
	fprintf(stderr, "N: %u\tX: 0x%08x\tY: 0x%08x\n", n, d.x, d.y);
	n = random_uint(1000, rnd_ctx);
	random_bytes(&d, sizeof(d), rnd_ctx);
	fprintf(stderr, "N: %u\tX: 0x%08x\tY: 0x%08x\n", n, d.x, d.y);
	n = random_uint(1000, rnd_ctx);
	random_bytes(&d, sizeof(d), rnd_ctx);
	fprintf(stderr, "N: %u\tX: 0x%08x\tY: 0x%08x\n", n, d.x, d.y);
}
/*----------------------------------------------------------------------------*/
void create_group(ftmgs_pbkey_t* gpk,
				  ftmgs_prkey_t* gsk,
				  unsigned nfas,
				  ftmgs_faj_pbkey_share_t* faj_pk[],
				  ftmgs_faj_prkey_t* faj_sk[],
				  ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
				  ftmgs_faj_gr_prkey_t* faj_gsk[],
				  unsigned nu,
				  rndctx_t* rnd_ctx)
{
	unsigned i;
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	ftmgs_pbkey_t* gpk_preimage = ftmgs_pbkey_t_new();
	ftmgs_pbkey_t* gpk_aux = ftmgs_pbkey_t_new();
	ftmgs_fa_pbkey_t* fa_pk_preimage = ftmgs_fa_pbkey_t_new();
	ftmgs_faj_pbkey_share_t** faj_pk_preimage
		= malloc(nfas*sizeof(ftmgs_faj_pbkey_share_t*));
	ftmgs_faj_gr_pbkey_share_t** faj_gpk_preimage
		= malloc(nfas*sizeof(ftmgs_faj_gr_pbkey_share_t*));
	for (i = 0; i < nfas; ++i) {
		faj_pk_preimage[i] = ftmgs_faj_pbkey_share_t_new();
		faj_gpk_preimage[i] = ftmgs_faj_gr_pbkey_share_t_new();
	}
	fprintf(stderr, "-Create-Group-------------------\n");
	INIT_TIMER(GroupSetup);
	/*- FA0 ------------------------------*/
	ftmgs_fa0_setup_mono(fa_pk_preimage, nu, rnd_ctx);
	/*- FAj ------------------------------*/
	for (i = 0; i < nfas; ++i) {
		/* FA0 -> FAj: <fa_pk_preimage> */
		ftmgs_faj_setup(faj_pk[i], faj_pk_preimage[i], faj_sk[i],
						fa_pk_preimage, rnd_ctx);
	}
	/*- GM -------------------------------*/
	ftmgs_gm_init_setup(gpk, gsk, nu, rnd_ctx);
	/*- FAj ------------------------------*/
	for (i = 0; i < nfas; ++i) {
		/* GM -> FAj: <gpk> */
		ftmgs_faj_group_setup(faj_gpk[i], faj_gpk_preimage[i], faj_gsk[i],
							  gpk, rnd_ctx);
	}
	/*- GM -------------------------------*/
	for (i = 0; (i < nfas)&&(rc == FTMGS_UNFINISHED); ++i) {
		/* FAj -> GM: <fa_pk_preimage, faj_pk_preimage, faj_gpk_preimage> */
		rc = ftmgs_gm_group_setup(gpk, gpk_preimage, nfas, fa_pk_preimage,
								  faj_pk_preimage[i], faj_gpk_preimage[i]);
	}
	PRINT_TIMER(GroupSetup);
	check((rc == FTMGS_OK), "Group Creation Failure (1)");
	/* GM: db_store(gpk, gpk_preimage); */
	/* FAj: db_store(gpk-old, faj_pk, faj_gpk, faj_sk, faj_gsk); */
	/* FA0: db_store(fa_pk_preimage); */
	/*-ASN1---------------------------*/
	if (rc == FTMGS_OK) {
		asn1_check_fapbkey(fa_pk_preimage);
		asn1_check_grpbkey(gpk);
		asn1_check_grprkey(gsk);
		for (i = 0; i < nfas; ++i) {
			asn1_check_fapbkeysh(faj_pk[i]);
			asn1_check_faprkey(faj_sk[i]);
			asn1_check_fagrpbkeysh(faj_gpk[i]);
			asn1_check_fagrprkey(faj_gsk[i]);
		}
	}
	/*----------------------------*/
	if (rc == FTMGS_OK) {
		rc = ftmgs_check_gpk(gpk,gpk_preimage)? FTMGS_UNFINISHED : FTMGS_ERROR;
		check((rc != FTMGS_ERROR), "Group Creation Failure (2)");
		/*----------------------------*/
		for (i = 0; (i < nfas)&&(rc == FTMGS_UNFINISHED); ++i) {
			rc = ftmgs_check_gpk_fa(gpk_aux, gpk, fa_pk_preimage,
									faj_pk[i], faj_gpk[i]);
		}
		check((rc == FTMGS_OK), "Group Creation Failure (3)");
	}
	/*----------------------------*/
	for (i = 0; i < nfas; ++i) {
		ftmgs_faj_pbkey_share_t_delete(faj_pk_preimage[i]);
		ftmgs_faj_gr_pbkey_share_t_delete(faj_gpk_preimage[i]);
	}
	free(faj_pk_preimage);
	free(faj_gpk_preimage);
	/*--------------------------------*/
	ftmgs_fa_pbkey_t_delete(fa_pk_preimage);
	ftmgs_pbkey_t_delete(gpk_aux);
	ftmgs_pbkey_t_delete(gpk_preimage);
	/*--------------------------------*/
	check((rc == FTMGS_OK), "Group Creation Failure (4)");
}
/*----------------------------------------------------------------------------*/
void join_new_member(ftmgs_mbr_prkey_t* msk,
					 ftmgs_mbr_ref_t* mr,
					 const dlogx_t* umk,
					 const dlog_t* u_auth,
					 const ftmgs_prkey_t* gsk,
					 const ftmgs_pbkey_t* gpk,
					 rndctx_t* rnd_ctx)
{
	bool_t ok;
	ftmgs_retcode_t rcu, rcgm = FTMGS_UNFINISHED;
	ftmgs_join_prv_t* uprv1 = ftmgs_join_prv_t_new();
	ftmgs_join_prv_t* uprv2 = ftmgs_join_prv_t_new();
	ftmgs_join_pbl_t* pbl1 = ftmgs_join_pbl_t_new();
	ftmgs_join_pbl_t* pbl2 = ftmgs_join_pbl_t_new();
	fprintf(stderr, "-Join-New-Member----------------\n");
	/*--------------------------------*/
	INIT_TIMER(Join);
	/*--------------------------------*/
	/*Usr -> GM: <UsrId, u_auth>*/ 
	/*GM -> Usr: <gpk> */ 
	rcu = ftmgs_join_usr(uprv1, pbl1, msk, gpk, umk, u_auth, rnd_ctx);
	while (rcu == FTMGS_UNFINISHED) {
		/*Usr -> GM: <pbl> */ 
		asn1_check_joinpbl(&pbl1, &pbl2);
		rcgm = ftmgs_join_gm(pbl1, mr, gpk, gsk, u_auth, rnd_ctx);
		/*GM -> Usr:  <pbl> */ 
		asn1_check_joinpbl(&pbl1, &pbl2);
		asn1_check_joinprv(&uprv1, &uprv2);
		rcu = ftmgs_join_usr(uprv1, pbl1, msk, gpk, umk, u_auth, rnd_ctx);
	}
	/*--------------------------------*/
	PRINT_TIMER(Join);
	/*--------------------------------*/
	asn1_check_mbrref(mr);
	asn1_check_mbrprkey(msk);
	ok = (rcu == FTMGS_OK) && (rcgm == FTMGS_OK);
	/* GM: db_store(UsrId, <gpk, mr>);*/
	/* Usr: db_store(UsrId, <gpk, msk>);*/
	/*--------------------------------*/
	check((rcu == FTMGS_OK), "Join Failure (1)");
	check((rcgm == FTMGS_OK), "Join Failure (2)");
	/*--------------------------------*/
	INIT_TIMER(VrfyJoinLog);
	ok = ftmgs_vrfy_join_log(mr, gpk);
	PRINT_TIMER(VrfyJoinLog);
	check(ok, "VerifyJoinLog Failure (1)");
	/*--------------------------------*/
	ftmgs_join_prv_t_delete(uprv2);
	ftmgs_join_prv_t_delete(uprv1);
	ftmgs_join_pbl_t_delete(pbl2);
	ftmgs_join_pbl_t_delete(pbl1);
	/*--------------------------------*/
}
/*----------------------------------------------------------------------------*/
void sign_verify(ftmgs_sign_t* sg,
				 const ftmgs_mbr_prkey_t* msk,
				 const ftmgs_pbkey_t* gpk,
				 const char* msg1,
				 const char* msg2,
				 rndctx_t* rnd_ctx)
{
	bool_t ok;
	/*assert(strcmp(msg1, msg2)!=0);*/
	fprintf(stderr, "-Sign-Verify--------------------\n");
	INIT_TIMER(SignVerify);
	INIT_TIMER(Sign);
	ok = ftmgs_sign_dat(SHA_MODE, sg, msg1, (unsigned)strlen(msg1), gpk, msk,
						rnd_ctx);  
	PRINT_TIMER(Sign);
	check(ok, "Signature (1)");
	asn1_check_sign(sg);
	INIT_TIMER(Verify);
	ok = ok && ftmgs_vrfy_dat(SHA_MODE, sg, msg1, (unsigned)strlen(msg1), gpk);
	PRINT_TIMER(Verify);
	PRINT_TIMER(SignVerify);
	check(ok, "Signature & Verify Failure (1)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_dat(SHA_MODE, sg, msg2, (unsigned)strlen(msg2), gpk);  
	check( ! ok, "Signature & Verify Failure (2)");
	/*--------------------------------*/
	{
		unsigned digestlen = ShaMaxSize;
		char msg_digest[ShaMaxSize];
		shactx_t* sha_ctx = shactx_t_new();
		sha_reset(sha_ctx, SHA_MODE);
		sha_input(sha_ctx, msg1, (unsigned)strlen(msg1));
		sha_result(sha_ctx, msg_digest, &digestlen);
		shactx_t_delete(sha_ctx);
		ok = ftmgs_vrfy_dgst(sg, msg_digest, digestlen, gpk);
		check(ok, "Signature & Verify & Sha Failure (3)");
	}
	/*--------------------------------*/
}
/*----------------------------------------------------------------------------*/
void open_and_check(const ftmgs_sign_t* sg,
					const ftmgs_pbkey_t* gpk,
					unsigned nfas,
					const ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
					const ftmgs_faj_gr_prkey_t* faj_gsk[],
					const ftmgs_mbr_ref_t* mr1,
					const ftmgs_mbr_ref_t* mr2,
					rndctx_t* rnd_ctx)
{
	bool_t ok = (nfas == ftmgs_get_nfas_open(gpk));
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	unsigned i;
	ftmgs_open_t* op = ftmgs_open_t_new();
	ftmgs_openacc_t* osa = ftmgs_openacc_t_new();
	ftmgs_opensharej_t* osj = ftmgs_opensharej_t_new();
	fprintf(stderr, "-Open-Check---------------------\n");
	check(ok, "FA-Keys Failure (O)");
	INIT_TIMER(OpenCheck);
	INIT_TIMER(Open);
	/*Any -> Judge: <gpk, sg> */ 
	for (i = 0; ok && (rc == FTMGS_UNFINISHED) && (i < nfas); ++i) {
		/*Judge -> FAj: <gpk, sg> */
		/* FAj */
		ok = ftmgs_open_dshare_j(osj, sg, faj_gpk[i], faj_gsk[i], gpk, rnd_ctx);
		check(ok, "Open DShare Failure");
		asn1_check_openshare(osj);
		/*FAj -> Judge: <osj> */ 
		/* Judge */
		if (ok) {
			rc = ftmgs_open_sign(op, osa, sg, osj, faj_gpk[i], gpk);
			asn1_check_openacc(osa);
		} else {
			rc = FTMGS_ERROR;
		}
	}
	PRINT_TIMER(Open);
	asn1_check_open(op);
	ok = (rc == FTMGS_OK);
	/*Judge -> GM: <gpk, op> */
	/*GM: db_foreach(UsrId, <gpk, mr>) DO ftmgs_open_ckeck(op, mr, gpk);*/
	/*GM -> Judge: <gpk, op, mr, UsrId> */
	check(ok, "Open Failure");
	INIT_TIMER(Check);
	ok = ftmgs_open_check(op, mr1, gpk);
	PRINT_TIMER(Check);
	PRINT_TIMER(OpenCheck);
	check(ok, "Open & Check Failure (1)");
	/*--------------------------------*/
	ok = ftmgs_open_check(op, mr2, gpk);
	check( ! ok, "Open & Check Failure (2)");
	/*--------------------------------*/
	ftmgs_opensharej_t_delete(osj);
	ftmgs_openacc_t_delete(osa);
	ftmgs_open_t_delete(op);
}
/*----------------------------------------------------------------------------*/
void reveal_and_trace(const ftmgs_mbr_ref_t* mr,
					  const ftmgs_pbkey_t* gpk,
					  unsigned nfas,
					  const ftmgs_faj_pbkey_share_t* faj_pk[],
					  const ftmgs_faj_prkey_t* faj_sk[],
					  const ftmgs_sign_t* sg1,
					  const ftmgs_sign_t* sg2,
					  rndctx_t* rnd_ctx)
{
	bool_t ok = (nfas == ftmgs_get_nfas_reveal(gpk));
	ftmgs_retcode_t rc = FTMGS_UNFINISHED;
	unsigned i;
	ftmgs_mtkey_t* mtk = ftmgs_mtkey_t_new();
	ftmgs_mtkey_sharej_t* mtk_shj = ftmgs_mtkey_sharej_t_new();
	ftmgs_mtkey_acc_t* mtka = ftmgs_mtkey_acc_t_new();
	fprintf(stderr, "-Reveal-Trace-------------------\n");
	check(ok, "FA-Keys Failure (R)");
	INIT_TIMER(RevTrace);
	INIT_TIMER(Reveal);
	/*Judge -> GM: <UsrId> */
	/*GM: db_search(UsrId, <gpk, mr>);*/
	/*GM -> Judge: <gpk, mr> */ 
	for (i = 0; ok && (rc == FTMGS_UNFINISHED) && (i < nfas); ++i) {
		/*Judge -> FAj: <gpk, mr> */
		/* FAj */
		ok =  ftmgs_reveal_dshare_j(mtk_shj, mr, faj_pk[i], faj_sk[i],
									gpk, rnd_ctx);
		asn1_check_mtkeyshare(mtk_shj);
		/*FAj -> Judge: <mtk_shj> */ 
		/* Judge */
		if (ok) {
			rc = ftmgs_reveal_mtkey(mtk, mtka, mtk_shj, mr, faj_pk[i], gpk);
			asn1_check_mtkeyacc(mtka);
		} else {
			rc = FTMGS_ERROR;
		}
	}
	PRINT_TIMER(Reveal);
	asn1_check_mtkey(mtk);
	/*Judge -> TAj: <mtk> */
	ok = (rc == FTMGS_OK);
	check(ok, "Reveal Failure");
	INIT_TIMER(Trace);
	/* TAj */
	ok = ftmgs_trace(sg1, mtk);
	PRINT_TIMER(Trace);
	PRINT_TIMER(RevTrace);
	check(ok, "Reveal & Trace Failure (1)");
	/*--------------------------------*/
	/* TAj */
	ok = ftmgs_trace(sg2, mtk);
	check( ! ok, "Reveal & Trace Failure (2)");
	/*--------------------------------*/
	ftmgs_mtkey_acc_t_delete(mtka);
	ftmgs_mtkey_sharej_t_delete(mtk_shj);
	ftmgs_mtkey_t_delete(mtk);
}
/*----------------------------------------------------------------------------*/
void claim_and_verify(const ftmgs_sign_t* sg1,
					  const ftmgs_sign_t* sg2,
					  const char* msg1,
					  const char* msg2,
					  const ftmgs_mbr_prkey_t* msk,
					  const ftmgs_pbkey_t* gpk,
					  rndctx_t* rnd_ctx)
{
	bool_t ok;
	ftmgs_claim_t* clm = ftmgs_claim_t_new();
	fprintf(stderr, "-Claim-Verify-------------------\n");
	INIT_TIMER(ClaimVerify);
	INIT_TIMER(Claim);
	ok = ftmgs_claim_dat(SHA_MODE, clm, sg1, msg1, (unsigned)strlen(msg1), gpk,
						 msk, rnd_ctx);
	PRINT_TIMER(Claim);
	check(ok, "Claim Failure");
	asn1_check_claim(clm);
	INIT_TIMER(Verify);
	ok = ftmgs_vrfy_claim_dat(SHA_MODE, clm, sg1, msg1, (unsigned)strlen(msg1),
							  gpk);
	PRINT_TIMER(Verify);
	PRINT_TIMER(ClaimVerify);
	check(ok, "Claim & Verify Failure (1)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_claim_dat(SHA_MODE, clm, sg1, msg2, (unsigned)strlen(msg1),
							  gpk);
	check( ! ok, "Claim & Verify Failure (2)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_claim_dat(SHA_MODE, clm, sg2, msg1, (unsigned)strlen(msg1),
							  gpk);
	check( ! ok, "Claim & Verify Failure (3)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_claim_dat(SHA_MODE, clm, sg2, msg2, (unsigned)strlen(msg1),
							  gpk);
	check( ! ok, "Claim & Verify Failure (4)");
	/*--------------------------------*/
	{
		unsigned digestlen = ShaMaxSize;
		char msg_digest[ShaMaxSize];
		shactx_t* sha_ctx = shactx_t_new();
		sha_reset(sha_ctx, SHA_MODE);
		sha_input(sha_ctx, msg1, (unsigned)strlen(msg1));
		sha_result(sha_ctx, msg_digest, &digestlen);
		shactx_t_delete(sha_ctx);
		ok = ftmgs_vrfy_claim_dgst(clm, sg1, msg_digest, digestlen, gpk);
		check(ok, "Claim & Verify & Sha Failure (5)");
	}
	/*--------------------------------*/
	ftmgs_claim_t_delete(clm);
}
/*----------------------------------------------------------------------------*/
void link_and_verify(const ftmgs_sign_t* sg1,
					 const ftmgs_sign_t* sg2,
					 const ftmgs_sign_t* sg3,
					 const char* msg1,
					 const char* msg2,
					 const ftmgs_mbr_prkey_t* msk,
					 const ftmgs_pbkey_t* gpk,
					 rndctx_t* rnd_ctx)
{
	bool_t ok;
	ftmgs_link_t* lnk = ftmgs_link_t_new();
	fprintf(stderr, "-Link-Verify--------------------\n");
	INIT_TIMER(LinkVerify);
	INIT_TIMER(Link);
	ok = ftmgs_link_dat(SHA_MODE, lnk, msg1, (unsigned)strlen(msg1),
						sg1, gpk, sg2, gpk, msk, rnd_ctx);
	PRINT_TIMER(Link);
	check(ok, "Link Failure");
	asn1_check_link(lnk);
	INIT_TIMER(Verify);
	ok = ftmgs_vrfy_link_dat(SHA_MODE, lnk, msg1, (unsigned)strlen(msg1),
							 sg1, gpk, sg2, gpk);
	PRINT_TIMER(Verify);
	PRINT_TIMER(LinkVerify);
	check(ok, "Link & Verify Failure (1)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_link_dat(SHA_MODE, lnk, msg2, (unsigned)strlen(msg2),
							 sg1, gpk, sg2, gpk);
	check( ! ok, "Link & Verify Failure (2)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_link_dat(SHA_MODE, lnk, msg1, (unsigned)strlen(msg1),
							 sg2, gpk, sg1, gpk);
	check( ! ok, "Link & Verify Failure (3)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_link_dat(SHA_MODE, lnk, msg1, (unsigned)strlen(msg1),
							 sg1, gpk, sg3, gpk);
	check( ! ok, "Link & Verify Failure (4)");
	/*--------------------------------*/
	ok = ftmgs_vrfy_link_dat(SHA_MODE, lnk, msg1, (unsigned)strlen(msg1),
							 sg2, gpk, sg3, gpk);
	check( ! ok, "Link & Verify Failure (5)");
	/*--------------------------------*/
	{
		unsigned digestlen = ShaMaxSize;
		char msg_digest[ShaMaxSize];
		shactx_t* sha_ctx = shactx_t_new();
		sha_reset(sha_ctx, SHA_MODE);
		sha_input(sha_ctx, msg1, (unsigned)strlen(msg1));
		sha_result(sha_ctx, msg_digest, &digestlen);
		shactx_t_delete(sha_ctx);
		ok = ftmgs_vrfy_link_dgst(lnk, msg_digest, digestlen, sg1, gpk, sg2, gpk);
		check(ok, "Link & Verify & Sha Failure (6)");
	}
	/*--------------------------------*/
	ftmgs_link_t_delete(lnk);
}
/*----------------------------------------------------------------------------*/
void print_system_options(unsigned nu, unsigned nfas)
{
	fprintf(stderr, "--------------------------------\n");
	fprintf(stderr, "\tFTMGS");
	fprintf(stderr, "\tGMP");
#ifndef NDEBUG
	fprintf(stderr, "\tDebug-Mode");
#else
	fprintf(stderr, "\tOptimized-Mode");
#endif
#ifdef PRECOMPUTATIONS__
	fprintf(stderr, "\tPrecomputations");
#endif

#ifdef __linux__
	fprintf(stderr, "\tLinux");
#elif defined __WIN32__
	fprintf(stderr, "\tWindows-32");
#elif defined __WIN64__
	fprintf(stderr, "\tWindows-64");
#endif

#if defined(__sparc) || defined(__sparc__)
	fprintf(stderr, "\tSparc");
#elif defined(__alpha__)
	fprintf(stderr, "\tAlpha");
#elif defined(_POWER) || defined(__powerpc__) || defined(__ppc__)
	fprintf(stderr, "\tPowerPC");
#elif defined(__i386__)
	fprintf(stderr, "\tIntel386");
#elif defined(__i586__)
	fprintf(stderr, "\tIntel586");
#elif defined(__ia64__) || defined(__ia64)
	fprintf(stderr, "\tIntel64");
#elif defined(__x86_64) || defined(__x86_64__)
	fprintf(stderr, "\tIntel64");
#elif defined(__amd64) || defined(__amd64__)
	fprintf(stderr, "\tAmd64");
#endif
	fprintf(stderr, "\n");
	
#ifdef __linux__
	fprintf(stderr, "\t");
	system("grep 'model name' /proc/cpuinfo");
	fprintf(stderr, "\t");
	system("grep 'cpu MHz' /proc/cpuinfo");
#endif
	fprintf(stderr, "\tSecPar (Nu): %d\n", nu);
	fprintf(stderr, "\tNumber of Fairness-Authorities: %d\n", nfas);
}
/*----------------------------------------------------------------------------*/
void ftmgs(unsigned nu, unsigned nfas)
{
	unsigned i;
	rndctx_t* rnd_ctx = rndctx_t_new();
	ftmgs_pbkey_t* gpk = ftmgs_pbkey_t_new();
	ftmgs_prkey_t* gsk = ftmgs_prkey_t_new();
	ftmgs_mbr_prkey_t* msk1 = ftmgs_mbr_prkey_t_new();
	ftmgs_mbr_prkey_t* msk2 = ftmgs_mbr_prkey_t_new();
	ftmgs_mbr_ref_t* mr1 = ftmgs_mbr_ref_t_new();
	ftmgs_mbr_ref_t* mr2 = ftmgs_mbr_ref_t_new();
	ftmgs_sign_t* sg11 = ftmgs_sign_t_new();
	ftmgs_sign_t* sg12 = ftmgs_sign_t_new();
	ftmgs_sign_t* sg21 = ftmgs_sign_t_new();
	ftmgs_sign_t* sg22 = ftmgs_sign_t_new();

	dlogx_t* umk = dlogx_t_new();
	dlog_t* u_auth = dlog_t_new();

	ftmgs_faj_pbkey_share_t**faj_pk =
		(ftmgs_faj_pbkey_share_t**)malloc(nfas*sizeof(ftmgs_faj_pbkey_share_t*));
	ftmgs_faj_prkey_t**faj_sk =
		(ftmgs_faj_prkey_t**)malloc(nfas*sizeof(ftmgs_faj_prkey_t*));
	ftmgs_faj_gr_pbkey_share_t**faj_gpk =
		(ftmgs_faj_gr_pbkey_share_t**)malloc(nfas*sizeof(ftmgs_faj_gr_pbkey_share_t*));
	ftmgs_faj_gr_prkey_t**faj_gsk =
		(ftmgs_faj_gr_prkey_t**)malloc(nfas*sizeof(ftmgs_faj_gr_prkey_t*));
	for (i = 0; i < nfas; ++i) {
		faj_pk[i] = ftmgs_faj_pbkey_share_t_new();
		faj_sk[i] = ftmgs_faj_prkey_t_new();
		faj_gpk[i] = ftmgs_faj_gr_pbkey_share_t_new();
		faj_gsk[i] = ftmgs_faj_gr_prkey_t_new();
	}
	/*------------------------------------------------------------------------*/
	print_system_options(nu, nfas);
	/*------------------------------------------------------------------------*/
	/*random_seed(rnd_ctx, TrueEntropy);*/
	random_seed(rnd_ctx, PseudoEntropy);

	random_numbers(rnd_ctx);

	create_group(gpk, gsk, nfas, faj_pk, faj_sk, faj_gpk, faj_gsk, nu, rnd_ctx);

	join_new_member(msk1, mr1, NULL, NULL, gsk, gpk, rnd_ctx);
	/*join_new_member(msk2, mr2, NULL, NULL, gsk, gpk, rnd_ctx);*/

	create_ukey_from_dsa(umk, u_auth);
	join_new_member(msk2, mr2, umk, u_auth, gsk, gpk, rnd_ctx);
	
	sign_verify(sg11, msk1, gpk, MSG1, MSG2, rnd_ctx);
	sign_verify(sg12, msk1, gpk, MSG2, MSG1, rnd_ctx);
	sign_verify(sg21, msk2, gpk, MSG1, MSG2, rnd_ctx);
	sign_verify(sg22, msk2, gpk, MSG2, MSG1, rnd_ctx);

	open_and_check(sg11, gpk, nfas,
				   (const ftmgs_faj_gr_pbkey_share_t**)faj_gpk,
				   (const ftmgs_faj_gr_prkey_t**)faj_gsk,
				   mr1, mr2, rnd_ctx);
	open_and_check(sg12, gpk, nfas,
				   (const ftmgs_faj_gr_pbkey_share_t**)faj_gpk,
				   (const ftmgs_faj_gr_prkey_t**)faj_gsk,
				   mr1, mr2, rnd_ctx);
	open_and_check(sg21, gpk, nfas,
				   (const ftmgs_faj_gr_pbkey_share_t**)faj_gpk,
				   (const ftmgs_faj_gr_prkey_t**)faj_gsk,
				   mr2, mr1, rnd_ctx);
	open_and_check(sg22, gpk, nfas,
				   (const ftmgs_faj_gr_pbkey_share_t**)faj_gpk,
				   (const ftmgs_faj_gr_prkey_t**)faj_gsk,
				   mr2, mr1, rnd_ctx);

	reveal_and_trace(mr1, gpk, nfas,
					 (const ftmgs_faj_pbkey_share_t**)faj_pk,
					 (const ftmgs_faj_prkey_t**)faj_sk,
					 sg11, sg21, rnd_ctx);
	reveal_and_trace(mr1, gpk, nfas,
					 (const ftmgs_faj_pbkey_share_t**)faj_pk,
					 (const ftmgs_faj_prkey_t**)faj_sk,
					 sg12, sg22, rnd_ctx);
	reveal_and_trace(mr2, gpk, nfas,
					 (const ftmgs_faj_pbkey_share_t**)faj_pk,
					 (const ftmgs_faj_prkey_t**)faj_sk,
					 sg21, sg11, rnd_ctx);
	reveal_and_trace(mr2, gpk, nfas,
					 (const ftmgs_faj_pbkey_share_t**)faj_pk,
					 (const ftmgs_faj_prkey_t**)faj_sk,
					 sg22, sg12, rnd_ctx);

	claim_and_verify(sg11, sg21, MSG3, MSG4, msk1, gpk, rnd_ctx);
	claim_and_verify(sg12, sg22, MSG4, MSG3, msk1, gpk, rnd_ctx);
	claim_and_verify(sg21, sg11, MSG3, MSG4, msk2, gpk, rnd_ctx);
	claim_and_verify(sg22, sg12, MSG4, MSG3, msk2, gpk, rnd_ctx);

	link_and_verify(sg11, sg12, sg21, MSG3, MSG4, msk1, gpk, rnd_ctx);
	link_and_verify(sg21, sg22, sg12, MSG4, MSG3, msk2, gpk, rnd_ctx);
	/*------------------------------------------------------------------------*/
	for (i = 0; i < nfas; ++i) {
		ftmgs_faj_gr_prkey_t_delete(faj_gsk[i]);
		ftmgs_faj_gr_pbkey_share_t_delete(faj_gpk[i]);
		ftmgs_faj_prkey_t_delete(faj_sk[i]);
		ftmgs_faj_pbkey_share_t_delete(faj_pk[i]);
	}
	free(faj_gsk);
	free(faj_gpk);
	free(faj_sk);
	free(faj_pk);

	dlogx_t_delete(umk);
	dlog_t_delete(u_auth);

	ftmgs_sign_t_delete(sg22);
	ftmgs_sign_t_delete(sg21);
	ftmgs_sign_t_delete(sg12);
	ftmgs_sign_t_delete(sg11);
	ftmgs_mbr_ref_t_delete(mr2);
	ftmgs_mbr_ref_t_delete(mr1);
	ftmgs_mbr_prkey_t_delete(msk2);
	ftmgs_mbr_prkey_t_delete(msk1);
	ftmgs_prkey_t_delete(gsk);
	ftmgs_pbkey_t_delete(gpk);
	rndctx_t_delete(rnd_ctx);
	fprintf(stderr, "--------------------------------\n");
}
/*----------------------------------------------------------------------------*/
int main()
{
	/* undocumented from API. Only if compiled with -DNDEBUG  */
	/* extern int bi_cdtor_cntdiff(); */
	ftmgs(NU, 1);
	ftmgs(NU, 4);
	ftmgs(NU, 8);
	/* assert(bi_cdtor_cntdiff() == 0); */
	return 0;
}
/*----------------------------------------------------------------------------*/

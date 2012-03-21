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
#ifndef umalccvbg_ftmgs_h__
#define umalccvbg_ftmgs_h__	1
/*----------------------------------------------------------------------------*/
#ifndef USE_RESULT__
#ifdef __GNUC__
#define USE_RESULT__ __attribute__ ((warn_unused_result))
#else
#define USE_RESULT__
#endif
#endif
/*----------------------------------------------------------------------------*/
# ifndef FTMGS_API__
#  if defined FTMGS_BUILDING__ && defined HAVE_VISIBILITY && HAVE_VISIBILITY
#   define FTMGS_API__	extern __attribute__((__visibility__("default")))
#  elif defined FTMGS_BUILDING__ && defined _MSC_VER && ! defined FTMGS_STATIC__
#   define FTMGS_API__	extern __declspec(dllexport)
#  elif defined _MSC_VER && ! defined FTMGS_STATIC__
#   define FTMGS_API__	extern __declspec(dllimport)
#  else
#   define FTMGS_API__	extern
#  endif
# endif

/*----------------------------------------------------------------------------*/
#ifndef BOOL_T_DEFINED__
#define BOOL_T_DEFINED__	1
typedef char bool_t;
#endif
#ifndef TRUE
#define TRUE	1
#elif TRUE == 0
#error "Bad definition of symbol TRUE"
#endif
#ifndef FALSE
#define FALSE	0
#elif FALSE != 0
#error "Bad definition of symbol FALSE"
#endif
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
	/*------------------------------------------------------------------------*/
	/*-- Version -------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/* 
	 * Version Major: changes in public API and functionality
	 * Version Minor: internal changes that do not affect the public behavior
	 * Revision: bug fixes
	 * defined as macros, to allow its use by the preprocessor
	 */
#define FTMGS_VERSION_MAJOR		0
#define FTMGS_VERSION_MINOR		1
#define FTMGS_REVISION			0
	/*------------------------------------------------------------------------*/
	/*-- Security Parameter --------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	enum secpar_t {
		Nu1 = 1024, Nu2 = 2048, Nu3 = 3072
	};
	/*------------------------------------------------------------------------*/
	/*-- Return Codes --------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef enum ftmgs_retcode_t {
		FTMGS_ERROR, FTMGS_OK, FTMGS_UNFINISHED
	} ftmgs_retcode_t;
	/*--------------------------------*/
	typedef enum asn1_retcode_t {
		ASN1_Success,
		ASN1_File_Not_Found,
		ASN1_Element_Not_Found,
		ASN1_Identifier_Not_Found,
		ASN1_Der_Error,
		ASN1_Value_Not_Found,
		ASN1_Generic_Error,
		ASN1_Value_Not_Valid,
		ASN1_Tag_Error,
		ASN1_Tag_Implicit,
		ASN1_Error_Type_Any,
		ASN1_Syntax_Error,
		ASN1_Mem_Error,
		ASN1_Mem_Alloc_Error,
		ASN1_Der_Overflow,
		ASN1_Name_Too_Long,
		ASN1_Array_Error,
		ASN1_Element_Not_Empty
	} asn1_retcode_t;
	/*------------------------------------------------------------------------*/
	/*-- Hash ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	enum sha_mode_t {
		Sha1, Sha224, Sha256, Sha384, Sha512
	};
	enum sha_size_t {
		Sha1Size = 20, Sha224Size = 28, Sha256Size = 32,
		Sha384Size = 48, Sha512Size = 64, ShaMaxSize = Sha512Size
	};
	typedef struct USHAContext shactx_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	bool_t sha_reset(shactx_t* sha_ctx, unsigned sha_mode);
	FTMGS_API__
	bool_t sha_input(shactx_t* sha_ctx, const void* dat, unsigned datlen);
	FTMGS_API__
	bool_t sha_result(shactx_t* sha_ctx, void* dat_digest, unsigned* digestlen);
	/*------------------------------------------------------------------------*/
	/*-- Random --------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/* 
	 * TrueEntropy		it blocks until there are enough real random bits 
	 * PseudoEntropy	real entropy, if not enough bits, then pseudo 
	 * NoEntropy		no entropy, always start with the same seed
	 */
	enum entropy_src_t {
		TrueEntropy, PseudoEntropy, NoEntropy
	};
	typedef struct rndctx_t rndctx_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	unsigned random_seed(rndctx_t* rnd_ctx, unsigned entropy_src);
	FTMGS_API__
	unsigned random_reseed(rndctx_t* rnd_ctx, unsigned entropy_src);
	FTMGS_API__
	void random_bytes(void* buf, unsigned buflen, rndctx_t* rnd_ctx);
	FTMGS_API__
	unsigned random_uint(unsigned max, rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/*-- Group Creation ------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_fa_pbkey_t ftmgs_fa_pbkey_t;
	typedef struct ftmgs_faj_pbkey_share_t ftmgs_faj_pbkey_share_t;
	typedef struct ftmgs_faj_prkey_t ftmgs_faj_prkey_t;
	typedef struct ftmgs_pbkey_t ftmgs_pbkey_t;
	typedef struct ftmgs_prkey_t ftmgs_prkey_t;
	typedef struct ftmgs_faj_gr_pbkey_share_t ftmgs_faj_gr_pbkey_share_t;
	typedef struct ftmgs_faj_gr_prkey_t ftmgs_faj_gr_prkey_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void ftmgs_fa0_setup_mono(ftmgs_fa_pbkey_t* fa_pk_preimage,
							  unsigned nu,
							  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void ftmgs_faj_setup(ftmgs_faj_pbkey_share_t* faj_pk,
						 ftmgs_faj_pbkey_share_t* faj_pk_preimage,
						 ftmgs_faj_prkey_t* faj_sk,
						 const ftmgs_fa_pbkey_t* fa_pk,
						 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void ftmgs_gm_init_setup(ftmgs_pbkey_t* gpk_preimage,
							 ftmgs_prkey_t* gsk,
							 unsigned nu,
							 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void ftmgs_faj_group_setup(ftmgs_faj_gr_pbkey_share_t* faj_gpk,
							   ftmgs_faj_gr_pbkey_share_t* faj_gpk_preimage,
							   ftmgs_faj_gr_prkey_t* faj_gsk,
							   const ftmgs_pbkey_t* gpk_preimage,
							   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_gm_group_setup(ftmgs_pbkey_t* gpk,
										 ftmgs_pbkey_t* gpk_preimage,
										 unsigned nfas,
										 const ftmgs_fa_pbkey_t* fa_pk,
										 const ftmgs_faj_pbkey_share_t* faj_pk,
										 const ftmgs_faj_gr_pbkey_share_t* faj_gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_check_gpk(const ftmgs_pbkey_t* gpk,
						   const ftmgs_pbkey_t* gpk_preimage);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_check_gpk_fa(ftmgs_pbkey_t* gpk_aux,
									   const ftmgs_pbkey_t* gpk,
									   const ftmgs_fa_pbkey_t* fa_pk,
									   const ftmgs_faj_pbkey_share_t* faj_pk,
									   const ftmgs_faj_gr_pbkey_share_t* faj_gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	unsigned ftmgs_get_nfas_reveal(const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	unsigned ftmgs_get_nfas_open(const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_group_setup_prtcl(ftmgs_pbkey_t* gpk,
								   ftmgs_prkey_t* gsk,
								   unsigned nfas,
								   ftmgs_faj_pbkey_share_t* faj_pk[],
								   ftmgs_faj_prkey_t* faj_sk[],
								   ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
								   ftmgs_faj_gr_prkey_t* faj_gsk[],
								   unsigned nu,
								   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/*-- DSS-DSA -------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct dss_parms_t dss_parms_t;
	typedef struct dsa_pbkey_t dsa_pbkey_t;
	typedef struct dsa_prkey_t dsa_prkey_t;
	typedef struct dlogx_t dlogx_t;
	typedef struct dlog_t dlog_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void extract_umk_from_dsa(dlogx_t* x, const dsa_prkey_t* dsa_sk);
	FTMGS_API__
	void extract_uauth_from_dsa(dlog_t* uauth,
								const dsa_pbkey_t* dsa_pk,
								const dss_parms_t* dss_parms);
	FTMGS_API__
	void extract_dsa_from_umk(dsa_prkey_t* dsa_sk, const dlogx_t* x);
	/*------------------------------------------------------------------------*/
	/*-- Join ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_join_prv_t ftmgs_join_prv_t;
	typedef struct ftmgs_join_pbl_t ftmgs_join_pbl_t;
	typedef struct ftmgs_mbr_ref_t ftmgs_mbr_ref_t;
	typedef struct ftmgs_mbr_prkey_t ftmgs_mbr_prkey_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_join_usr(ftmgs_join_prv_t* prv,
								   ftmgs_join_pbl_t* pbl,
								   ftmgs_mbr_prkey_t* msk,
								   const ftmgs_pbkey_t* gpk,
								   const dlogx_t* umk,
								   const dlog_t* u_auth,
								   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_join_gm(ftmgs_join_pbl_t* pbl,
								  ftmgs_mbr_ref_t* mr,
								  const ftmgs_pbkey_t* gpk,
								  const ftmgs_prkey_t* gsk,
								  const dlog_t* u_auth,
								  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_join_log(const ftmgs_mbr_ref_t* mr,
							   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_join_prtcl(ftmgs_mbr_prkey_t* msk,
							ftmgs_mbr_ref_t* mr,
							const dlogx_t* umk,
							const dlog_t* u_auth,
							const ftmgs_prkey_t* gsk,
							const ftmgs_pbkey_t* gpk,
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void extract_umk_from_msk(dlogx_t* x, const ftmgs_mbr_prkey_t* msk);
	/*------------------------------------------------------------------------*/
	/*-- Sign ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_sign_t ftmgs_sign_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_sign_dat(unsigned which_sha,
						ftmgs_sign_t* sg,
						const void* dat, unsigned datlen,
						const ftmgs_pbkey_t* gpk,
						const ftmgs_mbr_prkey_t* msk,
						rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_dat(unsigned which_sha,
						  const ftmgs_sign_t* sg,
						  const void* dat, unsigned datlen,
						  const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_sign_dgst(ftmgs_sign_t* sg,
						 const void* dat_digest,
						 unsigned dat_digestlen,
						 const ftmgs_pbkey_t* gpk,
						 const ftmgs_mbr_prkey_t* msk,
						 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_dgst(const ftmgs_sign_t* sg,
						   const void* dat_digest,
						   unsigned dat_digestlen,
						   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__
	void extract_uauth_from_sg(dlog_t* uauth,
							   const ftmgs_sign_t* sg,
							   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/*-- Open ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_open_t ftmgs_open_t;
	typedef struct ftmgs_opensharej_t ftmgs_opensharej_t;
	typedef struct ftmgs_openacc_t ftmgs_openacc_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_dshare_j(ftmgs_opensharej_t* osj,
							 const ftmgs_sign_t* sg,
							 const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
							 const ftmgs_faj_gr_prkey_t* faj_gsk,
							 const ftmgs_pbkey_t* gpk,
							 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_open_sign(ftmgs_open_t* op,
									ftmgs_openacc_t* osa,
									const ftmgs_sign_t* sg,
									const ftmgs_opensharej_t* osj,
									const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
									const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_check(const ftmgs_open_t* op,
							const ftmgs_mbr_ref_t* mr,
							const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_prtcl(ftmgs_open_t* op,
							const ftmgs_sign_t* sg,
							const ftmgs_pbkey_t* gpk,
							unsigned nfas,
							const ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
							const ftmgs_faj_gr_prkey_t* faj_gsk[],
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/*-- Reveal&Trace --------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_mtkey_t ftmgs_mtkey_t;
	typedef struct ftmgs_mtkey_sharej_t ftmgs_mtkey_sharej_t;
	typedef struct ftmgs_mtkey_acc_t ftmgs_mtkey_acc_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_reveal_dshare_j(ftmgs_mtkey_sharej_t* mtk_shj,
								 const ftmgs_mbr_ref_t* mr,
								 const ftmgs_faj_pbkey_share_t* faj_pk,
								 const ftmgs_faj_prkey_t* faj_sk,
								 const ftmgs_pbkey_t* gpk,
								 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_reveal_mtkey(ftmgs_mtkey_t* mtk,
									   ftmgs_mtkey_acc_t* mtka,
									   const ftmgs_mtkey_sharej_t* mtk_shj,
									   const ftmgs_mbr_ref_t* mr,
									   const ftmgs_faj_pbkey_share_t* faj_pk,
									   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_trace(const ftmgs_sign_t* sg, const ftmgs_mtkey_t* mtk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_reveal_prtcl(ftmgs_mtkey_t* mtk,
							  const ftmgs_mbr_ref_t* mr,
							  const ftmgs_pbkey_t* gpk,
							  unsigned nfas,
							  const ftmgs_faj_pbkey_share_t* faj_pk[],
							  const ftmgs_faj_prkey_t* faj_sk[],
							  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/*-- Claim ---------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_claim_t ftmgs_claim_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_claim_dat(unsigned which_sha,
						   ftmgs_claim_t* clm,
						   const ftmgs_sign_t* sg,
						   const void* dat,
						   unsigned datlen,
						   const ftmgs_pbkey_t* gpk,
						   const ftmgs_mbr_prkey_t* msk,
						   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_claim_dat(unsigned which_sha,
								const ftmgs_claim_t* clm,
								const ftmgs_sign_t* sg,
								const void* dat,
								unsigned datlen,
								const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_claim_dgst(ftmgs_claim_t* clm,
							const ftmgs_sign_t* sg,
							const void* dat_digest,
							unsigned dat_digestlen,
							const ftmgs_pbkey_t* gpk,
							const ftmgs_mbr_prkey_t* msk,
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_claim_dgst(const ftmgs_claim_t* clm,
								 const ftmgs_sign_t* sg,
								 const void* dat_digest,
								 unsigned dat_digestlen,
								 const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/*-- Link ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_link_t ftmgs_link_t;
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_link_dat(unsigned which_sha,
						  ftmgs_link_t* lnk,
						  const void* dat, unsigned datlen,
						  const ftmgs_sign_t* sg0,
						  const ftmgs_pbkey_t* gpk0,
						  const ftmgs_sign_t* sg1,
						  const ftmgs_pbkey_t* gpk1,
						  const ftmgs_mbr_prkey_t* msk,
						  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_dat(unsigned which_sha,
							   const ftmgs_link_t* lnk,
							   const void* dat, unsigned datlen,
							   const ftmgs_sign_t* sg0,
							   const ftmgs_pbkey_t* gpk0,
							   const ftmgs_sign_t* sg1,
							   const ftmgs_pbkey_t* gpk1);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_link_m_dat(unsigned which_sha,
							ftmgs_link_t* lnk,
							const void* dat,
							unsigned datlen,
							unsigned nsg,
							const ftmgs_sign_t* sg[],
							const ftmgs_pbkey_t* gpk[],
							const ftmgs_mbr_prkey_t* msk,
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_m_dat(unsigned which_sha,
								 const ftmgs_link_t* lnk,
								 const void* dat,
								 unsigned datlen,
								 unsigned nsg,
								 const ftmgs_sign_t* sg[],
								 const ftmgs_pbkey_t* gpk[]);
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_link_dgst(ftmgs_link_t* lnk,
						   const void* dat_digest,
						   unsigned dat_digestlen,
						   const ftmgs_sign_t* sg0,
						   const ftmgs_pbkey_t* gpk0,
						   const ftmgs_sign_t* sg1,
						   const ftmgs_pbkey_t* gpk1,
						   const ftmgs_mbr_prkey_t* msk,
						   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_dgst(const ftmgs_link_t* lnk,
								const void* dat_digest,
								unsigned dat_digestlen,
								const ftmgs_sign_t* sg0,
								const ftmgs_pbkey_t* gpk0,
								const ftmgs_sign_t* sg1,
								const ftmgs_pbkey_t* gpk1);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_link_m_dgst(ftmgs_link_t* lnk,
							 const void* dat_digest,
							 unsigned dat_digestlen,
							 unsigned nsg,
							 const ftmgs_sign_t* sg[],
							 const ftmgs_pbkey_t* gpk[],
							 const ftmgs_mbr_prkey_t* msk,
							 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_m_dgst(const ftmgs_link_t* lnk,
								  const void* dat_digest,
								  unsigned dat_digestlen,
								  unsigned nsg,
								  const ftmgs_sign_t* sg[],
								  const ftmgs_pbkey_t* gpk[]);
	/*------------------------------------------------------------------------*/
	/*-- ASN1-Buffer ---------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	typedef struct buffer_t buffer_t;
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	char* buffer_data(const buffer_t* buff);
	FTMGS_API__ USE_RESULT__
	unsigned buffer_size(const buffer_t* buff);
	FTMGS_API__ USE_RESULT__
	unsigned buffer_maxsize(const buffer_t* buff);
	/*--------------------------------*/
	FTMGS_API__
	void buffer_reserve(buffer_t* buff, unsigned maxsz);
	FTMGS_API__
	void buffer_shrink(buffer_t* buff);
	FTMGS_API__
	void buffer_reset(buffer_t* buff);
	/*--------------------------------*/
	FTMGS_API__
	void buffer_clear(buffer_t* buff);
	FTMGS_API__
	void buffer_resize(buffer_t* buff, unsigned newsz);
	FTMGS_API__
	void buffer_push(buffer_t* buff, const void* dat, unsigned datlen);
	FTMGS_API__
	void buffer_push_str(buffer_t* buff, const char* str);
	/*--------------------------------*/
#ifndef buffer_push_buf
#define buffer_push_buf(buffer, b)							\
	buffer_push((buffer), buffer_data(b), buffer_size(b))
#define buffer_resize_add(buffer, sz)					\
	buffer_resize((buffer), buffer_size(buffer)+(sz))
#define buffer_reserve_add(buffer, sz)						\
	buffer_reserve((buffer), buffer_size(buffer)+(sz)+1)
#endif
	/*------------------------------------------------------------------------*/
	/*-- ASN1-Conversion -----------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_dssparms(buffer_t* buff, const dss_parms_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_dssparms(dss_parms_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_dsapbkey(buffer_t* buff, const dsa_pbkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_dsapbkey(dsa_pbkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_dsaprkey(buffer_t* buff, const dsa_prkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_dsaprkey(dsa_prkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_fapbkey(buffer_t* buff, const ftmgs_fa_pbkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_fapbkey(ftmgs_fa_pbkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_grpbkey(buffer_t* buff, const ftmgs_pbkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_grpbkey(ftmgs_pbkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_grprkey(buffer_t* buff, const ftmgs_prkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_grprkey(ftmgs_prkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_fapbkeysh(buffer_t* buff, const ftmgs_faj_pbkey_share_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_fapbkeysh(ftmgs_faj_pbkey_share_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_faprkey(buffer_t* buff, const ftmgs_faj_prkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_faprkey(ftmgs_faj_prkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_fagrpbkeysh(buffer_t* buff, const ftmgs_faj_gr_pbkey_share_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_fagrpbkeysh(ftmgs_faj_gr_pbkey_share_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_fagrprkey(buffer_t* buff, const ftmgs_faj_gr_prkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_fagrprkey(ftmgs_faj_gr_prkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_dlog(buffer_t* buff, const dlog_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_dlog(dlog_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_dlogx(buffer_t* buff, const dlogx_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_dlogx(dlogx_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_joinpbl(buffer_t* buff, const ftmgs_join_pbl_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_joinpbl(ftmgs_join_pbl_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_joinprv(buffer_t* buff, const ftmgs_join_prv_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_joinprv(ftmgs_join_prv_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_mbrref(buffer_t* buff, const ftmgs_mbr_ref_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_mbrref(ftmgs_mbr_ref_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_mbrprkey(buffer_t* buff, const ftmgs_mbr_prkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_mbrprkey(ftmgs_mbr_prkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_sign(buffer_t* buff, const ftmgs_sign_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_sign(ftmgs_sign_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_open(buffer_t* buff, const ftmgs_open_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_open(ftmgs_open_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_openshare(buffer_t* buff, const ftmgs_opensharej_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_openshare(ftmgs_opensharej_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_openacc(buffer_t* buff, const ftmgs_openacc_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_openacc(ftmgs_openacc_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_mtkey(buffer_t* buff, const ftmgs_mtkey_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_mtkey(ftmgs_mtkey_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_mtkeyshare(buffer_t* buff, const ftmgs_mtkey_sharej_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_mtkeyshare(ftmgs_mtkey_sharej_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_mtkeyacc(buffer_t* buff, const ftmgs_mtkey_acc_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_mtkeyacc(ftmgs_mtkey_acc_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_claim(buffer_t* buff, const ftmgs_claim_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_claim(ftmgs_claim_t* p, const void* buff, int len);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_enc_link(buffer_t* buff, const ftmgs_link_t* p);
	FTMGS_API__ USE_RESULT__
	asn1_retcode_t asn1_dec_link(ftmgs_link_t* p, const void* buff, int len);
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/*-- CONSTRUCTORS AND DESTRUCTORS ----------------------------------------*/
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/*-- Hash ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	shactx_t* shactx_t_new();
	FTMGS_API__ USE_RESULT__
	shactx_t* shactx_t_clone(const shactx_t* o);
	FTMGS_API__
	void shactx_t_delete(shactx_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Random --------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	rndctx_t* rndctx_t_new();
	FTMGS_API__ USE_RESULT__
	rndctx_t* rndctx_t_clone(const rndctx_t* o);
	FTMGS_API__
	void rndctx_t_delete(rndctx_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Group Creation ------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_fa_pbkey_t* ftmgs_fa_pbkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_fa_pbkey_t* ftmgs_fa_pbkey_t_clone(const ftmgs_fa_pbkey_t* o);
	FTMGS_API__
	void ftmgs_fa_pbkey_t_delete(ftmgs_fa_pbkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_pbkey_share_t* ftmgs_faj_pbkey_share_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_pbkey_share_t* ftmgs_faj_pbkey_share_t_clone(const ftmgs_faj_pbkey_share_t* o);
	FTMGS_API__
	void ftmgs_faj_pbkey_share_t_delete(ftmgs_faj_pbkey_share_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_prkey_t* ftmgs_faj_prkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_prkey_t* ftmgs_faj_prkey_t_clone(const ftmgs_faj_prkey_t* o);
	FTMGS_API__
	void ftmgs_faj_prkey_t_delete(ftmgs_faj_prkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_pbkey_t* ftmgs_pbkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_pbkey_t* ftmgs_pbkey_t_clone(const ftmgs_pbkey_t* o);
	FTMGS_API__
	void ftmgs_pbkey_t_delete(ftmgs_pbkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_prkey_t* ftmgs_prkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_prkey_t* ftmgs_prkey_t_clone(const ftmgs_prkey_t* o);
	FTMGS_API__
	void ftmgs_prkey_t_delete(ftmgs_prkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_gr_pbkey_share_t* ftmgs_faj_gr_pbkey_share_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_gr_pbkey_share_t* ftmgs_faj_gr_pbkey_share_t_clone(const ftmgs_faj_gr_pbkey_share_t* o);
	FTMGS_API__
	void ftmgs_faj_gr_pbkey_share_t_delete(ftmgs_faj_gr_pbkey_share_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_gr_prkey_t* ftmgs_faj_gr_prkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_faj_gr_prkey_t* ftmgs_faj_gr_prkey_t_clone(const ftmgs_faj_gr_prkey_t* o);
	FTMGS_API__
	void ftmgs_faj_gr_prkey_t_delete(ftmgs_faj_gr_prkey_t* p);
	/*------------------------------------------------------------------------*/
	/*-- DSS-DSA -------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	dss_parms_t* dss_parms_t_new();
	FTMGS_API__ USE_RESULT__
	dss_parms_t* dss_parms_t_clone(const dss_parms_t* o);
	FTMGS_API__
	void dss_parms_t_delete(dss_parms_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	dsa_pbkey_t* dsa_pbkey_t_new();
	FTMGS_API__ USE_RESULT__
	dsa_pbkey_t* dsa_pbkey_t_clone(const dsa_pbkey_t* o);
	FTMGS_API__
	void dsa_pbkey_t_delete(dsa_pbkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	dsa_prkey_t* dsa_prkey_t_new();
	FTMGS_API__ USE_RESULT__
	dsa_prkey_t* dsa_prkey_t_clone(const dsa_prkey_t* o);
	FTMGS_API__
	void dsa_prkey_t_delete(dsa_prkey_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Join ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	dlogx_t* dlogx_t_new();
	FTMGS_API__ USE_RESULT__
	dlogx_t* dlogx_t_clone(const dlogx_t* o);
	FTMGS_API__
	void dlogx_t_delete(dlogx_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	dlog_t* dlog_t_new();
	FTMGS_API__ USE_RESULT__
	dlog_t* dlog_t_clone(const dlog_t* o);
	FTMGS_API__
	void dlog_t_delete(dlog_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_join_prv_t* ftmgs_join_prv_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_join_prv_t* ftmgs_join_prv_t_clone(const ftmgs_join_prv_t* o);
	FTMGS_API__
	void ftmgs_join_prv_t_delete(ftmgs_join_prv_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_join_pbl_t* ftmgs_join_pbl_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_join_pbl_t* ftmgs_join_pbl_t_clone(const ftmgs_join_pbl_t* o);
	FTMGS_API__
	void ftmgs_join_pbl_t_delete(ftmgs_join_pbl_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_mbr_ref_t* ftmgs_mbr_ref_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_mbr_ref_t* ftmgs_mbr_ref_t_clone(const ftmgs_mbr_ref_t* o);
	FTMGS_API__
	void ftmgs_mbr_ref_t_delete(ftmgs_mbr_ref_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_mbr_prkey_t* ftmgs_mbr_prkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_mbr_prkey_t* ftmgs_mbr_prkey_t_clone(const ftmgs_mbr_prkey_t* o);
	FTMGS_API__
	void ftmgs_mbr_prkey_t_delete(ftmgs_mbr_prkey_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Sign ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_sign_t* ftmgs_sign_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_sign_t* ftmgs_sign_t_clone(const ftmgs_sign_t* o);
	FTMGS_API__
	void ftmgs_sign_t_delete(ftmgs_sign_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Open ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_open_t* ftmgs_open_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_open_t* ftmgs_open_t_clone(const ftmgs_open_t* o);
	FTMGS_API__
	void ftmgs_open_t_delete(ftmgs_open_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_opensharej_t* ftmgs_opensharej_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_opensharej_t* ftmgs_opensharej_t_clone(const ftmgs_opensharej_t* o);
	FTMGS_API__
	void ftmgs_opensharej_t_delete(ftmgs_opensharej_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_openacc_t* ftmgs_openacc_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_openacc_t* ftmgs_openacc_t_clone(const ftmgs_openacc_t* o);
	FTMGS_API__
	void ftmgs_openacc_t_delete(ftmgs_openacc_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Reveal&Trace --------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_t* ftmgs_mtkey_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_t* ftmgs_mtkey_t_clone(const ftmgs_mtkey_t* o);
	FTMGS_API__
	void ftmgs_mtkey_t_delete(ftmgs_mtkey_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_sharej_t* ftmgs_mtkey_sharej_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_sharej_t* ftmgs_mtkey_sharej_t_clone(const ftmgs_mtkey_sharej_t* o);
	FTMGS_API__
	void ftmgs_mtkey_sharej_t_delete(ftmgs_mtkey_sharej_t* p);
	/*--------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_acc_t* ftmgs_mtkey_acc_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_mtkey_acc_t* ftmgs_mtkey_acc_t_clone(const ftmgs_mtkey_acc_t* o);
	FTMGS_API__
	void ftmgs_mtkey_acc_t_delete(ftmgs_mtkey_acc_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Claim ---------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_claim_t* ftmgs_claim_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_claim_t* ftmgs_claim_t_clone(const ftmgs_claim_t* o);
	FTMGS_API__
	void ftmgs_claim_t_delete(ftmgs_claim_t* p);
	/*------------------------------------------------------------------------*/
	/*-- Link ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	ftmgs_link_t* ftmgs_link_t_new();
	FTMGS_API__ USE_RESULT__
	ftmgs_link_t* ftmgs_link_t_clone(const ftmgs_link_t* o);
	FTMGS_API__
	void ftmgs_link_t_delete(ftmgs_link_t* p);
	/*------------------------------------------------------------------------*/
	/*-- ASN1-Buffer ---------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	FTMGS_API__ USE_RESULT__
	buffer_t* buffer_t_new();
	FTMGS_API__ USE_RESULT__
	buffer_t* buffer_t_clone(const buffer_t* o);
	FTMGS_API__
	void buffer_t_delete(buffer_t* p);
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

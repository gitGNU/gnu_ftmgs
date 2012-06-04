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
/**
 * @file ftmgs.h
 *
 * FTMGS (Fair Traceable Multi-Group Signatures) public API
 *
 * References: 
 *    V. Benjumea, S. G. Choi, J. Lopez, and M. Yung.
 *    Fair Traceable Multi-Group Signatures.
 *    In G. Tsudik, editor, FC'08: 12th. Intl. Conf. on Financial
 *    Cryptography and Data Security, volume 5143 of Lecture Notes in
 *    Computer Science, pages 231--246. Springer-Verlag, Jan. 2008.
 *    Full Version: http://eprint.iacr.org/2008/047
 *
 * Source files that make use of the facilities provided by the FTMGS
 * library should include the header file @c ftmgs.h, where the
 * public API is defined. This header file is installed in the system
 * include directory by the installation process, thus the file should be
 * included by the following directive:
 * @code
 * #include <ftmgs.h>
 * @endcode
 * The object files should be linked with the FTMGS library
 * (@c libftmgs.a or @c libftmgs.so in unix like systems):
 * @code
 * $ gcc -o test test.c -lftmgs
 * @endcode
 */
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
	/**
	 * @defgroup version Version Number
	 * Version Number.
	 * - @c FTMGS_VERSION_MAJOR: changes in public API and functionality
	 * - @c FTMGS_VERSION_MINOR: internal changes that do not affect the public API
	 * - @c FTMGS_REVISION: bug fixes
	 * 
	 * Defined as macros, to allow its use by the preprocessor
	 * @{
	 */
#define FTMGS_VERSION_MAJOR		0
#define FTMGS_VERSION_MINOR		1
#define FTMGS_REVISION			0
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Hash ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup hash SHA Hash API
	 * The SHA hash API may be used to create the digest of some data that
	 * is not directly available as an array of contiguous data bytes.
	 * @{
	 */
	/**
	 * These values allow to choose the operation mode for the SHA
	 * hash engine.
	 */
	enum sha_mode_t {
		Sha1, Sha224, Sha256, Sha384, Sha512
	};
	/**
	 * These values define the length (in bytes) of the hash digest.
	 */
	enum sha_size_t {
		Sha1Size = 20, Sha224Size = 28, Sha256Size = 32,
		Sha384Size = 48, Sha512Size = 64, ShaMaxSize = Sha512Size
	};
	/**
	 * Holds the internal context of the SHA engine.
	 */
	typedef struct USHAContext shactx_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It resets the context for the SHA engine, as selected by
	 * @a sha_mode from the aforementioned values.
	 */
	FTMGS_API__
	bool_t sha_reset(shactx_t* sha_ctx, unsigned sha_mode);
	/**
	 * It incrementally incorporates a sequence of @a datlen bytes stored
	 * in @a dat to the SHA context.
	 */
	FTMGS_API__
	bool_t sha_input(shactx_t* sha_ctx, const void* dat, unsigned datlen);
	/**
	 * It generates a @e digest of the data previously incorporated into
	 * the SHA context. This digest is stored into @a dat_digest, which
	 * should have enough allocated memory to hold the result, which is
	 * specified by @a digestlen (a maximum value requests the length
	 * specified for the previously selected SHA engine, see aforementioned
	 * values for sha sizes). As result, @a digestlen also holds the
	 * length of the generated digest.
	 */
	FTMGS_API__
	bool_t sha_result(shactx_t* sha_ctx, void* dat_digest, unsigned* digestlen);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Random --------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup random Random Bytes Generator API
	 * @{
	 */
	/**
	 * Random Bytes Generator. Source of Entropy:
	 * - @c TrueEntropy: it seeds the random number generator with
	 *   real random bits from a source of entropy that uses random
	 *   noise from internal devices. If there is not enough real
	 *   random bits, as required, from the source of entropy, then
	 *   the operation blocks until there are enough real random bits
	 *   available. This mode is @b required for secure cryptographic
	 *   use.
	 * - @c PseudoEntropy: it seeds the random number generator with
	 *   random bits from a source of entropy that uses random noise
	 *   from internal devices. If there is not enough real random
	 *   bits, as required, from the source of entropy, then the rest
	 *   of required bits are generated internally following some
	 *   pseudo-random number generation. This mode is @b not @b valid
	 *   for secure cryptographic use, but it can be used for other
	 *   less secure requirement scenarios.
	 * - @c NoEntropy: it does not use any source of entropy for
	 *   seeding. It seeds the random number generator with the same
	 *   fixed seed, so it is useful for debugging purposes, since, as
	 *   it starts with the same seed, it repeats the same sequence of
	 *   generated random numbers.
	 */
	enum entropy_src_t {
		TrueEntropy, PseudoEntropy, NoEntropy
	};
	/**
	 * Holds the internal context of the random number generator.
	 */
	typedef struct rndctx_t rndctx_t;
	/*------------------------------------------------------------------------*/
	/**
	 * Seeds the random bytes generator.
	 *
	 * Initializes and seeds the context for the random bytes
	 * generator. This function must be called once before using the
	 * generator context in any other function. It returns the amount of
	 * bytes used from the entropy source.
	 * 
	 * @param  rnd_ctx      Random bytes generator context
	 * @param  entropy_src  Entropy source: TrueEntropy, PseudoEntropy, NoEntropy
	 * @return length (in bytes) of entropy and nonce bytes used for seeding
	 * @pre    Already constructed ADT (@a rnd_ctx)
	 */
	FTMGS_API__
	unsigned random_seed(rndctx_t* rnd_ctx, unsigned entropy_src);
	/**
	 * Reseeds the random bytes generator.
	 * 
	 * Reseeds the context for the random bytes generator. It returns the
	 * amount of bytes used from the entropy source.
	 * 
	 * @param  rnd_ctx      Random bytes generator context
	 * @param  entropy_src  Entropy source: TrueEntropy, PseudoEntropy, NoEntropy
	 * @return length (in bytes) of entropy and nonce bytes used for seeding
	 * @pre    Already constructed ADT (@a rnd_ctx)
	 */
	FTMGS_API__
	unsigned random_reseed(rndctx_t* rnd_ctx, unsigned entropy_src);
	/**
	 * Generates random bytes.
	 *
	 * Generates @a buflen random bytes that will be stored in the memory
	 * pointed to by @a buf, by using the previously seeded random context
	 * @a rnd_ctx. The memory pointed by @a buf must have been previously
	 * allocated with enough room to hold @a buflen bytes.
	 * 
	 * @param[out]     data     output data buffer to hold the random bytes
	 * @param[in]      nbytes   requested number of random bytes
	 * @param[in,out]  rnd_ctx  random bytes generator context
	 * @return void
	 * @pre    Already constructed ADT (@a rnd_ctx)
	 */
	FTMGS_API__
	void random_bytes(void* buf, unsigned buflen, rndctx_t* rnd_ctx);
	/**
	 * Generates a random unsigned number less than @a max (0 <= r < max)
	 * 
	 * Returns a random @c unsigned number between @e 0 and @a max
	 * (exclusive), by using the previously seeded random context 
	 * @a rnd_ctx.
	 * 
	 * @param[in]      max      outer limit for the generated random number
	 * @param[in,out]  rnd_ctx  random bytes generator context
	 * @return a random unsigned number less than @a max (0 <= r < max)
	 * @pre    Already constructed ADT (@a rnd_ctx)
	 */
	FTMGS_API__
	unsigned random_uint(unsigned max, rndctx_t* rnd_ctx);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- FTMGS ---------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs FTMGS API
	 * @{
	 */
	/**
	 * FTMGS return codes.
	 *
	 * Defines the return codes for incremental (iterative) functions,
	 * which can be error (@c FTMGS_ERROR), success (@c FTMGS_OK), or
	 * still unfinished (@c FTMGS_UNFINISHED), which means that the
	 * (iterative) operation is still unfinished and it needs to be
	 * executed some more times, till either error or success is
	 * found.
	 */
	typedef enum ftmgs_retcode_t {
		FTMGS_ERROR, FTMGS_OK, FTMGS_UNFINISHED
	} ftmgs_retcode_t;
	/*------------------------------------------------------------------------*/
	/*-- Group Creation ------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_group FTMGS Group Setup:
	 * @{
	 */
	/*------------------------------------------------------------------------*/
	/*-- Security Parameter --------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * FTMGS security parameter.
	 *
	 * These values define the security parameters used when creating
	 * a FTMGS group:
	 *
	 * - @c Nu1 = @e 1024
	 * - @c Nu2 = @e 2048
	 * - @c Nu3 = @e 3072
	 */
	enum secpar_t {
		Nu1 = 1024, Nu2 = 2048, Nu3 = 3072
	};
	/*------------------------------------------------------------------------*/
	/**
	 * Holds the modulus @f$<\hat{n}>@f$ and generator @f$<\hat{g}>@f$
	 * for the paillier encryption scheme.
	 */
	typedef struct ftmgs_fa_pbkey_t ftmgs_fa_pbkey_t;
	/**
	 * Holds the fairness authority public key share @f$<\hat{y}_j>@f$
	 * for the paillier encryption scheme.
	 */
	typedef struct ftmgs_faj_pbkey_share_t ftmgs_faj_pbkey_share_t;
	/**
	 * Holds the fairness authority private key @f$<\hat{o}_j>@f$ for
	 * the paillier encryption scheme. It allows to recover the
	 * member's tracing key.
	 */
	typedef struct ftmgs_faj_prkey_t ftmgs_faj_prkey_t;
	/**
	 * Holds the group public key
	 * @f$<n,a,a_o,b,g,h,y,\hat{n},\hat{g},\hat{y}>@f$. It allows to
	 * verify group signatures, as well as provides support for all
	 * operations dealing with the group.
	 */
	typedef struct ftmgs_pbkey_t ftmgs_pbkey_t;
	/**
	 * Holds the group manager private key, the prime factors of the
	 * group modulus @f$<p,q>@f$. It allows to join new members to the
	 * group.
	 */
	typedef struct ftmgs_prkey_t ftmgs_prkey_t;
	/**
	 * Holds the fairness authority public key share @f$<y_j,h_j>@f$
	 * for the el-gamal encryption scheme for a given group.
	 */
	typedef struct ftmgs_faj_gr_pbkey_share_t ftmgs_faj_gr_pbkey_share_t;
	/**
	 * Holds the fairness authority private key @f$<o_j>@f$ for the
	 * el-gamal encryption scheme for a given group. It allows to open
	 * signatures.
	 */
	typedef struct ftmgs_faj_gr_prkey_t ftmgs_faj_gr_prkey_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It allows to create an initial public key modulus and generator
	 * for the fairness authorities, where the security parameter
	 * @a nu may have any of the aforementioned values. It is necessary
	 * that the random bytes generator context @c rnd_ctx had been
	 * previously seeded.
	 * 
	 * @b Note: This function create the public key modulus and
	 * generator for the fairness authorities for the @e paillier
	 * encryption scheme. It is necessary to be aware that the
	 * security of the @e paillier encryption scheme relies on the
	 * unknown factorization of this public modulus, and in this case,
	 * the entity that creates this modulus is able to know such
	 * factorization, and it must be, therefore, a trusted authority
	 * with an overall power over the others. Therefore, in order to
	 * improve the security of the scheme, in next versions of this
	 * library, new functions for creating such a public modulus in a
	 * distributed and collaborative manner will be incorporated.
	 * 
	 * @f$\diamond@f$ In a next version of this library, a
	 * @c ftmgs_fa0_setup() function will be incorporate to
	 * implement a protocol for the distributed generation of the RSA
	 * modulus, to be played among the involved fairness
	 * authorities.
	 */
	FTMGS_API__
	void ftmgs_fa0_setup_mono(ftmgs_fa_pbkey_t* fa_pk_preimage,
							  unsigned nu,
							  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by each fairness authority to generate the public
	 * and private keys (@c faj_pk, and @c faj_sk respectively) for a
	 * given public modulus and generator (@c fa_pk) created by using
	 * @c ftmgs_fa0_setup(). These keys are used to deal with
	 * encryption/decryption of @e member @e tracing @e keys.
	 * 
	 * Note that @c faj_pk_preimage may be NULL, but otherwise it will
	 * hold the preimage for the public key @c faj_pk, that is @c
	 * faj_pk @f$\equiv@f$ @c faj_pk_preimage@f$^2@f$. This preimage
	 * is used by the @e Group @e Manager when creating the group.
	 * 
	 * Note that by squaring the preimage, it is assured that
	 * @f$\hat{y}_j\in\mathit{QR}(n)@f$.
	 */
	FTMGS_API__
	void ftmgs_faj_setup(ftmgs_faj_pbkey_share_t* faj_pk,
						 ftmgs_faj_pbkey_share_t* faj_pk_preimage,
						 ftmgs_faj_prkey_t* faj_sk,
						 const ftmgs_fa_pbkey_t* fa_pk,
						 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by the @e Group @e Manager (GM) to compute the
	 * modulus, generator and related terms (a preimage of the group
	 * public key @c gpk), as well as the group private key that
	 * allows the GM to join new members to the group. The security
	 * parameter @a nu may have the aforementioned values.
	 */
	FTMGS_API__
	void ftmgs_gm_init_setup(ftmgs_pbkey_t* gpk_preimage,
							 ftmgs_prkey_t* gsk,
							 unsigned nu,
							 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by each fairness authority to generate the public
	 * and private keys (@c faj_gpk, and @c faj_gsk respectively) for
	 * a given group public key (@c gpk_preimage) created by using @c
	 * ftmgs_gm_init_setup(). These keys are used to deal with
	 * encryption/decryption for @e opening signatures.
	 * 
	 * Note that @c faj_gpk_preimage may be NULL, but otherwise it
	 * will hold the preimage for the public key @c faj_gpk, that is
	 * @c faj_gpk @f$\equiv@f$ @c faj_gpk_preimage@f$^2@f$. This
	 * preimage is used by the @e Group @e Manager when creating the
	 * group.
	 * 
	 * Note that by squaring the preimage, it is assured that
	 * @f$y_j\in\mathit{QR}(n)@f$ and @f$h_j\in\mathit{QR}(n)@f$.
	 */
	FTMGS_API__
	void ftmgs_faj_group_setup(ftmgs_faj_gr_pbkey_share_t* faj_gpk,
							   ftmgs_faj_gr_pbkey_share_t* faj_gpk_preimage,
							   ftmgs_faj_gr_prkey_t* faj_gsk,
							   const ftmgs_pbkey_t* gpk_preimage,
							   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by the @e Group @e Manager (GM) to incorporate into
	 * the group public key the public key preimages for each fairness
	 * authority that will supervise @e opening and @e revealing
	 * operations for the group, where @c nfas specifies the total
	 * number of required fairness authorities in the process.
	 * 
	 * This function belongs to an iterative process to incorporate
	 * the public keys for all the fairness authorities, and therefore
	 * it returns @c FTMGS_UNFINISHED while the number of incorporated
	 * fairness authority's public keys is lower that the amount of
	 * required fairness authorities (@c nfas). When the process is
	 * over, then either @c FTMGS_OK or @c FTMGS_ERROR is returned to
	 * indicate success or failure in the operation.
	 * 
	 * Note that @c gpk_preimage may be NULL, but otherwise it will
	 * hold the preimage for the group public key @c gpk, that is 
	 * @c gpk @f$\equiv@f$ @c gpk_preimage@f$^2@f$. This preimage is
	 * used by any entity to check that the group members have the right
	 * order. Note that by squaring the preimage, it is assured that group
	 * members belong to @f$\mathit{QR}(n)@f$.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_gm_group_setup(ftmgs_pbkey_t* gpk,
										 ftmgs_pbkey_t* gpk_preimage,
										 unsigned nfas,
										 const ftmgs_fa_pbkey_t* fa_pk,
										 const ftmgs_faj_pbkey_share_t* faj_pk,
										 const ftmgs_faj_gr_pbkey_share_t* faj_gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns @c TRUE (@c 1) if the group public key (@a gpk) is
	 * well formed with respect to the preimage (@a gpk_preimage),
	 * which means that all members have the right order and belong to
	 * @f$\mathit{QR}(n)@f$. It returns @c FALSE (@c 0) otherwise.
	 * 
	 * @f$\diamond@f$ In a next version of this library, this function
	 * will also check that the moduli in the group public key do not
	 * have small prime factors.
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_check_gpk(const ftmgs_pbkey_t* gpk,
						   const ftmgs_pbkey_t* gpk_preimage);
	/*------------------------------------------------------------------------*/
	/**
	 * This function belongs to an iterative process to check that the public
	 * keys for all the fairness authorities have been incorporated to the
	 * group public key, and therefore it returns @c FTMGS_UNFINISHED
	 * while the number of checked fairness authority's public keys is lower
	 * that the amount of required fairness authorities previously
	 * incorporated to the group public key. When the process is over, then
	 * either @c FTMGS_OK or @c FTMGS_ERROR is returned to indicate
	 * success or failure in the operation.
	 * 
	 * Note that the fairness authority's public keys are used,
	 * instead of their preimages. Also note that @a gpk_aux is used
	 * to temporarily hold the incorporation of checked public keys
	 * while the iterative process is being carried out.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_check_gpk_fa(ftmgs_pbkey_t* gpk_aux,
									   const ftmgs_pbkey_t* gpk,
									   const ftmgs_fa_pbkey_t* fa_pk,
									   const ftmgs_faj_pbkey_share_t* faj_pk,
									   const ftmgs_faj_gr_pbkey_share_t* faj_gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns the number of fairness authorities required to
	 * reveal a @e member @e tracing @e key.
	 */
	FTMGS_API__ USE_RESULT__
	unsigned ftmgs_get_nfas_reveal(const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns the number of fairness authorities required to open
	 * a signature.
	 */
	FTMGS_API__ USE_RESULT__
	unsigned ftmgs_get_nfas_open(const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It plays the group setup protocol locally (internally) for
	 * testing or demonstration purposes
	 */
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
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- DSS-DSA -------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup dsa FTMGS DSS Interaction:
	 * @{
	 */
	/**
	 * Holds the DSS parameters @f$<p, q, g>@f$, used by the user when
	 * authenticated to join the group.
	 */
	typedef struct dss_parms_t dss_parms_t;
	/**
	 * Holds the DSA user's public key @f$<y>@f$, used by the user
	 * when authenticated to join the group.
	 */
	typedef struct dsa_pbkey_t dsa_pbkey_t;
	/**
	 * Holds the DSA user's private key @f$<x>@f$, used by the user
	 * when authenticated to join the group.
	 */
	typedef struct dsa_prkey_t dsa_prkey_t;
	/**
	 * Holds a user's master private key @f$<x>@f$, used by the user
	 * when authenticated to join the group. It can come from the
	 * user's DSA private key, or from a group member's private key.
	 */
	typedef struct dlogx_t dlogx_t;
	/**
	 * Holds a user's master public key @f$<n, g, y>@f$, used by the
	 * user when authenticated to join the group. It can come from the
	 * user's DSA public key and DSS parameters, or from a group
	 * signature issued by the member when authenticated in joining a
	 * new group.
	 */
	typedef struct dlog_t dlog_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It is used to extract the user's master key from a DSA private
	 * key that have been used (a DSA signature used as authentication
	 * method) when the user was authorized to join the group. This
	 * user's master key will be used in the joining process, and it
	 * will be embedded into the user's member private key.
	 */
	FTMGS_API__
	void extract_umk_from_dsa(dlogx_t* x, const dsa_prkey_t* dsa_sk);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used to extract the user's authentication from a DSA
	 * public key that have been used (a DSA signature used as
	 * authentication method) when the user was authorized to join the
	 * group. This user's authentication will be used in the joining
	 * process, and it will be embedded into the member's reference
	 * with non-repudiation purposes.
	 */
	FTMGS_API__
	void extract_uauth_from_dsa(dlog_t* uauth,
								const dsa_pbkey_t* dsa_pk,
								const dss_parms_t* dss_parms);
	/*------------------------------------------------------------------------*/
	/**
	 * This function is used to extract (and create) a DSA private key
	 * from a user's master key (which can also be extracted from a
	 * member's private key), in this way it discourages the user from
	 * sharing her member's private keys with other users, since in
	 * this case, they will be able to recover her DSA private key and
	 * it will allow them to impersonate the user in a world-wide
	 * manner.
	 */
	FTMGS_API__
	void extract_dsa_from_umk(dsa_prkey_t* dsa_sk, const dlogx_t* x);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Join ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_join FTMGS Join:
	 * @{
	 */
	/**
	 * Holds temporal user's private data generated while the
	 * iterative process of joining to a group.
	 */
	typedef struct ftmgs_join_prv_t ftmgs_join_prv_t;
	/**
	 * Holds temporal public data generated while the iterative
	 * process of joining a new member to a group.
	 */
	typedef struct ftmgs_join_pbl_t ftmgs_join_pbl_t;
	/**
	 * Holds the member's reference
	 * @f$<A_i,e_i,C_i,X_i,U_i,V_i,E^{\wp}_i,\mathcal{A}_u>@f$ (join
	 * log) kept by the group manager for each member of the group. It
	 * holds information to allow recovery, with the collaboration of
	 * the fairness authorities, of tracing keys and opening
	 * signatures, as well as non-repudiable bindings with the user.
	 */
	typedef struct ftmgs_mbr_ref_t ftmgs_mbr_ref_t;
	/**
	 * Holds the member's private key @f$<A_i,e_i,x_i,x'_i>@f$ that
	 * allows the member to issue anonymous and unlinkable group
	 * signatures. It holds, among other data, the user's master key.
	 */
	typedef struct ftmgs_mbr_prkey_t ftmgs_mbr_prkey_t;
	/*------------------------------------------------------------------------*/
	/**
	 * This function belongs to an iterative protocol, at the user's
	 * side, to join a new member to a FTMGS group, and therefore it
	 * returns @c FTMGS_UNFINISHED while the protocol is still
	 * unfinished. When the protocol is over, then either @c FTMGS_OK
	 * or @c FTMGS_ERROR is returned to indicate success or failure in
	 * the operation.
	 * 
	 * The protocol starts at the user's side, then the private and
	 * public outcome (@a prv and @a pbl) are stored for the next
	 * iteration, and the public outcome (@a pbl) is sent (usually its
	 * ASN.1 DER encoding) to the group manager, which plays its part
	 * or the protocol, then the public outcome (@a pbl) is stored for
	 * the next iteration, and sent (usually its ASN.1 DER encoding)
	 * again to the user, which plays again this protocol till the
	 * end. Note that the user's side of the protocol is the starting
	 * and ending points of the protocol.
	 * 
	 * As result, if everything was fine, the user gets her member's
	 * private key (@a msk) that allows her to issue FTMGS group
	 * signatures, and therefore, being authenticated as a member of
	 * the group.
	 * 
	 * If the user was authorized (and authenticated) to join the
	 * group by any external means, then the user's master key (@a
	 * umk) and user's authentication (@a u_auth) should be extracted
	 * from the external authentication, otherwise they should be @c
	 * NULL. Note that both, user and group manager, should follow the
	 * same conventions for these cases in order to play a valid join
	 * protocol.
	 * 
	 * Note that @a prv and @a pbl are used to temporarily hold
	 * respectively the private and public data between iterations
	 * while the iterative protocol is being carried out.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_join_usr(ftmgs_join_prv_t* prv,
								   ftmgs_join_pbl_t* pbl,
								   ftmgs_mbr_prkey_t* msk,
								   const ftmgs_pbkey_t* gpk,
								   const dlogx_t* umk,
								   const dlog_t* u_auth,
								   rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * This function belongs to an iterative protocol, at the group
	 * manager's side, to join a new member to a FTMGS group, and
	 * therefore it returns @c FTMGS_UNFINISHED while the protocol is
	 * still unfinished. When the protocol is over, then either @c
	 * FTMGS_OK or @c FTMGS_ERROR is returned to indicate success or
	 * failure in the operation.
	 * 
	 * The protocol starts at the user's side, then the private and
	 * public outcome (@a prv and @a pbl) are stored for the next
	 * iteration, and the public outcome (@a pbl) is sent (usually its
	 * ASN.1 DER encoding) to the group manager, which plays its part
	 * or the protocol, then the public outcome (@a pbl) is stored for
	 * the next iteration, and sent (usually its ASN.1 DER encoding)
	 * again to the user, which plays again this protocol till the
	 * end. Note that the user's side of the protocol is the starting
	 * and ending points of the protocol.
	 * 
	 * As result, if everything was fine, the group manager gets the
	 * member's reference (@a mr) that allows the Judge (with
	 * collaboration of GM and FAs) opening and reveal operations.
	 * 
	 * If the user was authorized (and authenticated) to join the
	 * group by any external means, then the user's authentication (@a
	 * u_auth) should be extracted from the external authentication,
	 * otherwise it should be @c NULL. Note that both, user and group
	 * manager, should follow the same conventions for these cases in
	 * order to play a valid join protocol.
	 * 
	 * Note that @a pbl is used to temporarily hold the public data
	 * between iterations while the iterative protocol is being
	 * carried out.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_join_gm(ftmgs_join_pbl_t* pbl,
								  ftmgs_mbr_ref_t* mr,
								  const ftmgs_pbkey_t* gpk,
								  const ftmgs_prkey_t* gsk,
								  const dlog_t* u_auth,
								  rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns @c TRUE (@c 1) if the member's reference
	 * (@a mr) is consistent and well formed, which means that the
	 * non-repudiable bindings and proofs still hold, and therefore
	 * the record has not been manipulated. It returns @c FALSE
	 * (@c 0) otherwise.
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_join_log(const ftmgs_mbr_ref_t* mr,
							   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It plays the join protocol locally (internally) for testing or
	 * demonstration purposes
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_join_prtcl(ftmgs_mbr_prkey_t* msk,
							ftmgs_mbr_ref_t* mr,
							const dlogx_t* umk,
							const dlog_t* u_auth,
							const ftmgs_prkey_t* gsk,
							const ftmgs_pbkey_t* gpk,
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used to extract the user's master key from a FTMGS group
	 * member's private key that have been used (a FTMGS signature
	 * used as authentication method) when the user was authorized to
	 * join the group. This user's master key will be used in the
	 * joining process, and it will be embedded into the user's member
	 * private key.
	 */
	FTMGS_API__
	void extract_umk_from_msk(dlogx_t* x, const ftmgs_mbr_prkey_t* msk);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Sign ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_sign FTMGS Sign:
	 * @{
	 */
	/**
	 * Holds a FTMGS signature @f$<T_1,\cdots,T_7,\sigma^{\wp}>@f$
	 * issued by a member of the group.
	 */
	/*------------------------------------------------------------------------*/
	typedef struct ftmgs_sign_t ftmgs_sign_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It issues a FTMGS signature (@a sg) for group public key
	 * (@a gpk) with member's private key (@a msk). The SHA engine,
	 * as selected by @a which_sha from aforementioned values, is applied
	 * to some given data bytes (@a dat) of length (@a datlen).
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the signature, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the signature it is issued over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_sign_dat(unsigned which_sha,
						ftmgs_sign_t* sg,
						const void* dat, unsigned datlen,
						const ftmgs_pbkey_t* gpk,
						const ftmgs_mbr_prkey_t* msk,
						rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS signature (@a sg) can
	 * be verified for group public key (@a gpk), and returns
	 * @c FALSE (@c 0) otherwise. The SHA engine, as selected by
	 * @a which_sha from aforementioned values, is applied to some given
	 * data bytes (@a dat) of length (@a datlen).
	 * 
	 * Compatibility note: when verifying a signature, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the verification it is applied over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_dat(unsigned which_sha,
						  const ftmgs_sign_t* sg,
						  const void* dat, unsigned datlen,
						  const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * It issues a FTMGS signature (@a sg) for group public key
	 * (@a gpk) with member's private key (@a msk). The signature is
	 * applied to some given data digest (@a dat_digest) of length
	 * (@a dat_digestlen) that has been generated by some hashing
	 * functions.
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the signature, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{digest}||B_1||\cdots||B_6||\underbrace{n||\cdots||n}_6||g||h||T_2^{-1}||T_5||T_7||y||T_1^{-1}||a||b||a_0||T_3||T_4||T_6)@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_sign_dgst(ftmgs_sign_t* sg,
						 const void* dat_digest,
						 unsigned dat_digestlen,
						 const ftmgs_pbkey_t* gpk,
						 const ftmgs_mbr_prkey_t* msk,
						 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS signature (@a sg) can
	 * be verified for group public key (@a gpk) , and returns
	 * @c FALSE (@c 0) otherwise. The verification is applied to some
	 * given data digest (@a dat_digest) of length (@a dat_digestlen)
	 * that has been generated by some hashing functions.
	 * 
	 * Compatibility note: when verifying a signature, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the
	 * following data in the same order as specified (numbers are
	 * represented as a @e big-endian byte sequence):
	 * @f[c' = \mathrm{SHA}_k(\mathit{digest}||B_1||\cdots||B_6||\underbrace{n||\cdots||n}_6||g||h||T_2^{-1}||T_5||T_7||y||T_1^{-1}||a||b||a_0||T_3||T_4||T_6)@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_dgst(const ftmgs_sign_t* sg,
						   const void* dat_digest,
						   unsigned dat_digestlen,
						   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It is used to extract the user's authentication from a FTMGS
	 * signature that have been used (a FTMGS signature used as
	 * authentication method) when the user was authorized to join the
	 * group. This user's authentication will be used in the joining
	 * process, and it will be embedded into the member's reference
	 * with non-repudiation purposes.
	 */
	FTMGS_API__
	void extract_uauth_from_sg(dlog_t* uauth,
							   const ftmgs_sign_t* sg,
							   const ftmgs_pbkey_t* gpk);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Open ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_open FTMGS Open:
	 * @{
	 */
	/**
	 * Holds the member's reference @f$<\omega_{\sigma}>@f$ result of
	 * opening a signature.
	 */
	typedef struct ftmgs_open_t ftmgs_open_t;
	/**
	 * Holds a decryption share
	 * @f$<\hat{\omega}_{j\sigma},\hat{\omega}^{\wp}_{j\sigma}>@f$ of
	 * the opening of a signature.
	 */
	typedef struct ftmgs_opensharej_t ftmgs_opensharej_t;
	/**
	 * Holds the incremental product of the opening decryption shares.
	 */
	typedef struct ftmgs_openacc_t ftmgs_openacc_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by a fairness authority (with key-pair @a faj_gsk and
	 * @a faj_gpk) to generate a decryption share (@a osj) of the
	 * opening of a FTMGS signature (@a sg) for group public key
	 * @a gpk.
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the proof of correctness, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{digest}||B_1||B_2||n||n||g||y_j||T_2||\hat{\omega}_{j\sigma})@f]
	 * where 
	 * @f[\mathit{digest} = \mathrm{SHA}_k(T_1||\cdots||T_7||c||s_x||s_{x'}||s_e||s_r||s_{h'})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_dshare_j(ftmgs_opensharej_t* osj,
							 const ftmgs_sign_t* sg,
							 const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
							 const ftmgs_faj_gr_prkey_t* faj_gsk,
							 const ftmgs_pbkey_t* gpk,
							 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * This function belongs to an iterative process to incrementally open a
	 * FTMGS signature (@a sg) for all the fairness authority's opening
	 * decryption shares (@a osj), and therefore it returns
	 * @c FTMGS_UNFINISHED while the number of added decryption shares is
	 * lower that the amount of required fairness authorities. When the
	 * process is over, then either @c FTMGS_OK or @c FTMGS_ERROR is
	 * returned to indicate success or failure in the operation.
	 * 
	 * In case of success, @a op holds the outcome of the opening
	 * operation, and @a osa is a temporary value that must be held while
	 * the iterative process is not finished. Note that if the decryption
	 * share is not properly constructed with respect to the fairness
	 * authority's public key (@a faj_gpk) and group public key
	 * (@a gpk), then the operation will fail.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_open_sign(ftmgs_open_t* op,
									ftmgs_openacc_t* osa,
									const ftmgs_sign_t* sg,
									const ftmgs_opensharej_t* osj,
									const ftmgs_faj_gr_pbkey_share_t* faj_gpk,
									const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It checks if the outcome of the opening of a signature (@a op)
	 * matches a given member's reference (@a mr), and in such case, it
	 * also checks that this member's reference is consistent
	 * (@c ftmgs_vrfy_join_log()).
	 * 
	 * It returns @c TRUE (@c 1) if both match, and returns
	 * @c FALSE (@c 0) otherwise.
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_check(const ftmgs_open_t* op,
							const ftmgs_mbr_ref_t* mr,
							const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It plays the open protocol locally (internally) for testing or
	 * demonstration purposes
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_open_prtcl(ftmgs_open_t* op,
							const ftmgs_sign_t* sg,
							const ftmgs_pbkey_t* gpk,
							unsigned nfas,
							const ftmgs_faj_gr_pbkey_share_t* faj_gpk[],
							const ftmgs_faj_gr_prkey_t* faj_gsk[],
							rndctx_t* rnd_ctx);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Reveal&Trace --------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_reveal_trace FTMGS Reveal and Trace:
	 * @{
	 */
	/**
	 * Holds the member's tracing key @f$<\tau_{i}>@f$ result of
	 * opening a signature.
	 */
	typedef struct ftmgs_mtkey_t ftmgs_mtkey_t;
	/**
	 * Holds a decryption share
	 * @f$<\hat{\tau}_{ji},\hat{\tau}^{\wp}_{ji}>@f$ of the revealing
	 * of a member's tracing key.
	 */
	typedef struct ftmgs_mtkey_sharej_t ftmgs_mtkey_sharej_t;
	/**
	 * Holds the incremental product of the revealing decryption
	 * shares.
	 */
	typedef struct ftmgs_mtkey_acc_t ftmgs_mtkey_acc_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It is used by a fairness authority (with key-pair @a faj_sk and
	 * @a faj_pk) to generate a decryption share (@a mtk_shj) of the
	 * revealing the member's tracing key of a member's reference (@a mr)
	 * for group public key @a gpk. It also checks that this member's
	 * reference is consistent (@c ftmgs_vrfy_join_log()).
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the proof of correctness, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{digest}||B_1||B_2||\hat{n}^2||\hat{n}^2||\hat{g}||\hat{y}_j||U_i||\hat{\tau}_{ji})@f]
	 * where 
	 * @f[\mathit{digest} = \mathrm{SHA}_k(A_i||e_i||C_i||X_i||U_i||V_i||g||y||n||c||s_{x'}||s_r||s_x)@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_reveal_dshare_j(ftmgs_mtkey_sharej_t* mtk_shj,
								 const ftmgs_mbr_ref_t* mr,
								 const ftmgs_faj_pbkey_share_t* faj_pk,
								 const ftmgs_faj_prkey_t* faj_sk,
								 const ftmgs_pbkey_t* gpk,
								 rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * This function belongs to an iterative process to incrementally reveal
	 * a member's tracing key for a given member's reference (@a mr) for
	 * all the fairness authority's revealing decryption shares (@a mtk_shj),
	 * and therefore it returns @c FTMGS_UNFINISHED while the number of
	 * added decryption shares is lower that the amount of required fairness
	 * authorities. When the process is over, then either @c FTMGS_OK or
	 * @c FTMGS_ERROR is returned to indicate success or failure in the
	 * operation.
	 * 
	 * In case of success, @a mtk holds the outcome of the reveal
	 * operation, and @a mtka is a temporary value that must be held while
	 * the iterative process is not finished. Note that if the decryption
	 * share is not properly constructed with respect to the fairness
	 * authority's public key (@a faj_pk) and group public key
	 * (@a gpk), then the operation will fail.
	 */
	FTMGS_API__ USE_RESULT__
	ftmgs_retcode_t ftmgs_reveal_mtkey(ftmgs_mtkey_t* mtk,
									   ftmgs_mtkey_acc_t* mtka,
									   const ftmgs_mtkey_sharej_t* mtk_shj,
									   const ftmgs_mbr_ref_t* mr,
									   const ftmgs_faj_pbkey_share_t* faj_pk,
									   const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It checks if the outcome of revealing a member's tracing key (@a mtk)
	 * matches a given group signature (@a sg).
	 * 
	 * It returns @c TRUE (@c 1) if both match, and returns
	 * @c FALSE (@c 0) otherwise.
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_trace(const ftmgs_sign_t* sg, const ftmgs_mtkey_t* mtk);
	/*------------------------------------------------------------------------*/
	/**
	 * It plays the reveal protocol locally (internally) for testing or
	 * demonstration purposes
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_reveal_prtcl(ftmgs_mtkey_t* mtk,
							  const ftmgs_mbr_ref_t* mr,
							  const ftmgs_pbkey_t* gpk,
							  unsigned nfas,
							  const ftmgs_faj_pbkey_share_t* faj_pk[],
							  const ftmgs_faj_prkey_t* faj_sk[],
							  rndctx_t* rnd_ctx);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Claim ---------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_claim FTMGS Claim:
	 * @{
	 */
	/**
	 * Holds the proof @f$<\pi^{\wp}>@f$ of the claiming of a
	 * signature authorship.
	 */
	typedef struct ftmgs_claim_t ftmgs_claim_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It issues a FTMGS claim (@a clm) for group public key (@a gpk)
	 * with member's private key (@a msk) for a signature
	 * (@a sg). The SHA engine, as selected by @a which_sha from
	 * aforementioned values, is applied to some given data bytes
	 * (@a dat) of length (@a datlen).
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the claim, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the claim it is issued over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
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
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS claim (@a clm) can be
	 * verified for signature (@a sg) and group public key (@a gpk),
	 * and returns @c FALSE (@c 0) otherwise. The SHA engine, as
	 * selected by @a which_sha from aforementioned values, is applied to
	 * some given data bytes (@a dat) of length (@a datlen).
	 * 
	 * Compatibility note: when verifying a claim, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the verification it is applied over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_claim_dat(unsigned which_sha,
								const ftmgs_claim_t* clm,
								const ftmgs_sign_t* sg,
								const void* dat,
								unsigned datlen,
								const ftmgs_pbkey_t* gpk);
	/*------------------------------------------------------------------------*/
	/**
	 * It issues a FTMGS claim (@a clm) for group public key (@a gpk)
	 * with member's private key (@a msk) for a signature (@a sg).
	 * The claim is applied to some given data digest (@a dat_digest) of
	 * length (@a dat_digestlen) that has been generated by some hashing
	 * functions.
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the claim, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{dgst}||B_1||n||T_7||T_6)@f]
	 * where 
	 * @f[\mathit{dgst} = \mathrm{SHA}_k(\mathit{digest}||T_1||\cdots||T_7||c||s_x||s_{x'}||s_e||s_r||s_{h'})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_claim_dgst(ftmgs_claim_t* clm,
							const ftmgs_sign_t* sg,
							const void* dat_digest,
							unsigned dat_digestlen,
							const ftmgs_pbkey_t* gpk,
							const ftmgs_mbr_prkey_t* msk,
							rndctx_t* rnd_ctx);
	/*------------------------------------------------------------------------*/
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS claim (@a clm) can be
	 * verified for signature (@a sg) and group public key (@a gpk),
	 * and returns @c FALSE (@c 0) otherwise.  The verification is
	 * applied to some given data digest (@a dat_digest) of length
	 * (@a dat_digestlen) that has been generated by some hashing
	 * functions.
	 * 
	 * Compatibility note: when verifying the claim, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{dgst}||B_1||n||T_7||T_6)@f]
	 * where 
	 * @f[\mathit{dgst} = \mathrm{SHA}_k(\mathit{digest}||T_1||\cdots||T_7||c||s_x||s_{x'}||s_e||s_r||s_{h'})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_claim_dgst(const ftmgs_claim_t* clm,
								 const ftmgs_sign_t* sg,
								 const void* dat_digest,
								 unsigned dat_digestlen,
								 const ftmgs_pbkey_t* gpk);
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- Link ----------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup ftmgs_link FTMGS Link:
	 * @{
	 */
	/**
	 * Holds the proof @f$<\lambda^{\wp}>@f$ of the linking of several
	 * FTMGS signatures.
	 */
	typedef struct ftmgs_link_t ftmgs_link_t;
	/*------------------------------------------------------------------------*/
	/**
	 * It issues a FTMGS link (@a lnk) with member's private key
	 * (@a msk) for two signatures (@a sg0, @a sg1) for group public keys
	 * (@a gpk0, @a gpk1). The SHA engine, as selected by
	 * @a which_sha from aforementioned values, is applied to some given
	 * data bytes (@a dat) of length (@a datlen).
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the link, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the link it is issued over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
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
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS link (@a lnk) can be
	 * verified for signatures (@a sg0, @a sg1) and group public keys
	 * (@a gpk0, @a gpk1), and returns @c FALSE (@c 0)
	 * otherwise. The SHA engine, as selected by @a which_sha from
	 * aforementioned values, is applied to some given data bytes
	 * (@a dat) of length (@a datlen).
	 * 
	 * Compatibility note: when verifying a link, the @e digest is
	 * calculated by applying the selected SHA engine to the user's data, and
	 * then the verification it is applied over this digest as specified in
	 * the next functions:
	 * @f[\mathit{digest} = \mathrm{SHA}_w(\underbrace{\mathit{dat}}_{\mathit{datlen}})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_dat(unsigned which_sha,
							   const ftmgs_link_t* lnk,
							   const void* dat, unsigned datlen,
							   const ftmgs_sign_t* sg0,
							   const ftmgs_pbkey_t* gpk0,
							   const ftmgs_sign_t* sg1,
							   const ftmgs_pbkey_t* gpk1);
	/*------------------------------------------------------------------------*/
	/**
	 * It is the same as previous @c ftmgs_link_dat(), but applied to an
	 * array of signatures.
	 */
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
	/**
	 * It is the same as previous @c ftmgs_vrfy_link_dat(), but applied to an
	 * array of signatures.
	 */
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
	/**
	 * It issues a FTMGS link (@a lnk) with member's private key
	 * (@a msk) for two signatures (@a sg0, @a sg1) for group
	 * public keys (@a gpk0, @a gpk1). The link is applied to some
	 * given data digest (@a dat_digest) of length (@a dat_digestlen)
	 * that has been generated by some hashing functions.
	 * 
	 * It returns @c TRUE (@c 1) if everything was fine, and returns
	 * @c FALSE (@c 0) otherwise.
	 * 
	 * Compatibility note: when issuing the link, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{dgst}||B_1||B_2||n_1||n_2||T_{17}||T_{16}||T_{27}||T_{26})@f]
	 * where 
	 * @f[\mathit{dgst} = \mathrm{SHA}_k(\mathit{digest}||T_{11}||\cdots||T_{17}||c_1||s_{1x}||s_{1x'}||s_{1e}||s_{1r}||s_{1h'}||T_{21}||\cdots||T_{27}||c_2||s_{2x}||s_{2x'}||s_{2e}||s_{2r}||s_{2h'})@f]
	 */
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
	/**
	 * It returns @c TRUE (@c 1) if a FTMGS link (@a lnk) can be
	 * verified for signatures (@a sg0, @a sg1) and group public keys
	 * (@a gpk0, @a gpk1), and returns @c FALSE (@c 0)
	 * otherwise. The verification is applied to some given data digest
	 * (@a dat_digest) of length (@a dat_digestlen) that has been
	 * generated by some hashing functions.
	 * 
	 * Compatibility note: when verifying the link, the SHA hash
	 * (truncated to security parameter @e k bits) is applied to the following
	 * data in the same order as specified (numbers are represented as a
	 * @e big-endian byte sequence):
	 * @f[c = \mathrm{SHA}_k(\mathit{dgst}||B_1||B_2||n_1||n_2||T_{17}||T_{16}||T_{27}||T_{26})@f]
	 * where 
	 * @f[\mathit{dgst} = \mathrm{SHA}_k(\mathit{digest}||T_{11}||\cdots||T_{17}||c_1||s_{1x}||s_{1x'}||s_{1e}||s_{1r}||s_{1h'}||T_{21}||\cdots||T_{27}||c_2||s_{2x}||s_{2x'}||s_{2e}||s_{2r}||s_{2h'})@f]
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_dgst(const ftmgs_link_t* lnk,
								const void* dat_digest,
								unsigned dat_digestlen,
								const ftmgs_sign_t* sg0,
								const ftmgs_pbkey_t* gpk0,
								const ftmgs_sign_t* sg1,
								const ftmgs_pbkey_t* gpk1);
	/*------------------------------------------------------------------------*/
	/**
	 * It is the same as previous @c ftmgs_link_m_dgst(), but applied to an
	 * array of signatures.
	 */
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
	/**
	 * It is the same as previous @c ftmgs_vrfy_link_m_dgst(), but applied to an
	 * array of signatures.
	 */
	FTMGS_API__ USE_RESULT__
	bool_t ftmgs_vrfy_link_m_dgst(const ftmgs_link_t* lnk,
								  const void* dat_digest,
								  unsigned dat_digestlen,
								  unsigned nsg,
								  const ftmgs_sign_t* sg[],
								  const ftmgs_pbkey_t* gpk[]);
	/** @} */
	/*------------------------------------------------------------------------*/
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- ASN1-Buffer ---------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup buffer ASN1 encoding buffer
	 * @{
	 */
	/**
	 * Holds a buffer of bytes representing the ASN.1 DER encoding of
	 * some abstract data type.
	 */
	typedef struct buffer_t buffer_t;
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the pointer to the allocated buffer that holds the data bytes
	 * 
	 * @param  buff  actual data buffer structure
	 * @return the pointer to the allocated buffer that holds the data bytes
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	FTMGS_API__ USE_RESULT__
	char* buffer_data(const buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Returns the number of bytes actually stored in the buffer
	 * 
	 * @param  buff  actual data buffer structure
	 * @return the number of bytes actually stored in the buffer
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	FTMGS_API__ USE_RESULT__
	unsigned buffer_size(const buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space for @a sz bytes in the data buffer
	 * 
	 * @param  buff  actual data buffer structure
	 * @param  sz    size of allocated space
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	FTMGS_API__
	void buffer_reserve(buffer_t* buff, unsigned sz);
	/*------------------------------------------------------------------------*/
	/**
	 * Resets the buffer structure, removes stored data and deallocates
	 * buffer memory
	 * 
	 * @param  buff  actual data buffer structure
	 * @return void
	 * @pre    already constructed ADT buffer (@a buff)
	 */
	FTMGS_API__
	void buffer_reset(buffer_t* buff);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space and copy @a datlen bytes from @a dat into data buffer
	 * 
	 * @param  buff   actual data structure
	 * @param  dat    input data to be copied
	 * @param  datlen length of input data
	 * @return void
	 * @pre    already constructed ADT data (@a buff)
	 */
	void buffer_copy(struct buffer_t* buff, void* dat, unsigned datlen);
	/*------------------------------------------------------------------------*/
	/**
	 * Allocates space and copy bytes from a data buffer
	 * 
	 * @param  data   actual data structure
	 * @param  b      input buffer to be copied
	 * @return void
	 * @pre    already constructed ADT data (@a data)
	 */
#ifndef buffer_copy_buf
#define buffer_copy_buf(data, b)	data_copy(data, (b)->data_, (b)->size)
#endif
	/** @} */
	/*------------------------------------------------------------------------*/
	/*-- ASN1-Conversion -----------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup asn1der ASN1 DER encoding and decoding functions.
	 * 
	 * These functions allow to encode in ASN.1 DER each of the FTMGS
	 * abstract data types, storing the result in a buffer of bytes
	 * (of type @c buffer_t). They also allow to decode from a buffer
	 * of bytes in ASN.1 DER to any of the FTMGS abstract data types:
	 *
	 * @{
	 */
	/*------------------------------------------------------------------------*/
	/**
	 * ASN1 return codes.
	 *
	 * The type @c asn1_retcode_t defines the return codes for
	 * ASN1 encoding functions.
	 */
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
	/** @} */
	/*------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------*/
	/*-- CONSTRUCTORS AND DESTRUCTORS ----------------------------------------*/
	/*------------------------------------------------------------------------*/
	/**
	 * @defgroup adt ADT: Constructors (New), Destructors (Delete), and Cloners
	 *
	 * All defined data in the FTMGS library, except for enumerations,
	 * are Abstract Data Types (ADT), and therefore their internal
	 * representation is hidden (and protected). This fact has several
	 * advantages:
	 * 
	 * - The internal representation is hidden, so there is no need to
	 *   expose the internal data types and internal implementation.
	 * - As the internal representation is hidden, it diminishes the
	 *   possibility (and temptation) to bypass the API and dealing
	 *   with the internal data representation.
	 * - It improves the possibility of internal modifications that do
	 *   not affect to the external public API.
	 * - It improves the binary compatibility of the library, since
	 *   all data are defined as pointers to the internal
	 *   representation, which can change without affecting to the
	 *   external pointers.
	 * 
	 * As abstract data types, except for enumerations, any variable
	 * to deal with data must be declared as a @b pointer to the
	 * hidden data representation. Each of them will be created
	 * through a function (the @b constructor) that allocates memory
	 * space to hold the internal representation, and initializes the
	 * data to a known initial state. Moreover, when such data is not
	 * useful anymore, then it is necessary to call a function (the @b
	 * destructor) to free the allocated resources associated with the
	 * internal representation. Additionally, there is also a function
	 * (the @b cloner) that allows to clone the internal
	 * representation of the abstract data.
	 * 
	 * For each type in the library, these functions are named by the
	 * name of the type followed by the word @b new, @b delete and @b
	 * clone respectively. For example, for the type @c rndctx_t, the
	 * following code defines a variable to point to the internal
	 * representation, calls the constructors, other functions from
	 * the API, cloning and finally calls the destructors:
	 *
	 * @code
	 * #include <ftmgs.h>
	 * int main()
	 * {
	 *     unsigned x;
	 *     rndctx_t* rctx;
	 *     rndctx_t* random_context = rndctx_t_new();
	 *     ...
	 *     bi_random_seed(random_context, PseudoEntropy);
	 *     ...
	 *     x = bi_random_ui(10, random_context);
	 *     ...
	 *     rctx = rndctx_t_clone(random_context);
	 *     ... 
	 *     rndctx_t_delete(random_context);
	 *     rndctx_t_delete(rctx);
	 *     return 0;
	 * }
	 * @endcode
	 *
	 * @{
	 */
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
	/** @} */
	/*------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

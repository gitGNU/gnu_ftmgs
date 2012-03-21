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
#include "sok.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "syspar.h"
#include "iexport.h"
#include "birnd.h"
#include "random.h"
#include "cdtor.h"
/*----------------------------------------------------------------------------*/
/*-- Signatures of Knowledge -------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void sok_precomp_init(sok_precomp_t* p, const sok_relset_t* rs)
{
	unsigned i, j;
	for (i = 0; i < rs->n_rels; ++i) {
		for (j = 0; j < rs->n_objs; ++j) {
			p->v[i][j] = NULL;
		}
	}
}
/*----------------------------------------------------------------------------*/
/* 
 *  HASH( msg, {bi}, {ni}, {oi} )
 */
static void sok_hash(bigint_t c,
					 const void* dat_digest,
					 unsigned dat_digestlen,
					 const sok_relset_t* rs,
					 const bigint_t bi[],
					 const bigint_t* const mod[],
					 const bigint_t* const objs[],
					 const syspar_t* sp)
{
	unsigned i;
	USHAContext sha_ctx;
	/*--------------------------------*/
	bi_sha_reset(&sha_ctx, sp->k);
	sha_input(&sha_ctx, dat_digest, dat_digestlen);
	for (i = 0; i < rs->n_rels; ++i) {
		bi_sha_input(&sha_ctx, bi[i]);
	}
	for (i = 0; i < rs->n_rels; ++i) {
		bi_sha_input(&sha_ctx, *mod[i]);
	}
	for (i = 0; i < rs->n_objs; ++i) {
		bi_sha_input(&sha_ctx, *objs[i]);
	}
	bi_sha_result_len(&sha_ctx, c, sp->k/CHAR_BIT);
	/*--------------------------------*/
}
/*----------------------------------------------------------------------------*/
bool_t sok(const sok_relset_t* rs,		/* relation set */
		   const sok_params_t* prms,		/* sok params */
		   const void* dat_digest,
		   unsigned dat_digestlen,
		   const syspar_t* sp,
		   rnd_ctx_t* rnd_ctx)
{
	bool_t ok = TRUE;
	BEG_BIGINT_CHK();
	unsigned i, j;
	BEG_VAR_1_bigint_t(b);
	BEG_VAR_A_bigint_t(bi, MAX_RELATIONS);
	DBG(BEG_VAR_1_bigint_t(aux1););
	/*--------------------------------*/
	assert((rs->n_vbles <= MAX_VBLES) && (rs->n_objs <= MAX_OBJECTS)
		   && (rs->n_rels <= MAX_RELATIONS));
	assert((rs->n_vbles == prms->n_vbles) && (rs->n_objs == prms->n_objs)
		   && (rs->n_rels <= prms->n_rels));
	for (i = 0; ok && (i < rs->n_vbles); ++i) {
		ok = bi_belongs_sph_ek(*prms->vbles[i], prms->sphs[i]);
		/* tw in +-{0,1}^(eps*(mu+k)) */
		bi_random_nb(*prms->s[i], prms->sphs[i]->log2radius, rnd_ctx);
		/* adjust sign */
		if ((bi_bit(*prms->s[i], prms->sphs[i]->log2radius/2)
			 ^ bi_bit(*prms->s[i], prms->sphs[i]->log2radius/4)) == 1) {
			bi_negate(*prms->s[i], *prms->s[i]);
		}
		assert(bi_nbits(*prms->s[i]) == prms->sphs[i]->log2radius);/* <= */
	}
	if (ok) {
		for (i = 0; (i < rs->n_rels); ++i) {
			bi_asg_ui(bi[i], 1);
			DBG(bi_asg_ui(aux1, 1););
			for (j = 0; j < rs->n_objs; ++j) {
				assert(is_vble(rs->a[i][j])
					   || ((rs->a[i][j] >= -1) && (rs->a[i][j] <= +1))); 
				if (is_vble(rs->a[i][j])) {
					/*  Bi *= Aj^tw */
					unsigned vidx = get_vble_idx(rs->a[i][j]);
					assert(vidx < rs->n_vbles);
					bi_powmod(b, *prms->objs[j], *prms->s[vidx], *prms->mod[i]);
					bi_mulmod(bi[i], bi[i], b, *prms->mod[i]);
					/*-----------------*/
#ifndef NDEBUG
					DBG(bi_powmod_dbg(b, *prms->objs[j], *prms->vbles[vidx],
									  *prms->mod[i]););
					DBG(bi_mulmod(aux1, aux1, b, *prms->mod[i]););
				} else if (rs->a[i][j] == -1) {
					/*  Bi *= Aj^aij */
					/*bi_powmod_ui(b,*prms->objs[j],rs->a[i][j],*prms->mod[i]);*/
					/*bi_mulmod(bi[i], bi[i], b, *prms->mod[i]); */
					DBG(bi_invmod(b, *prms->objs[j], *prms->mod[i]););
					DBG(bi_mulmod(aux1, aux1, b, *prms->mod[i]););
				} else if (rs->a[i][j] == +1) {
					/*  Bi *= Aj^aij */
					/*bi_powmod_ui(b,*prms->objs[j],rs->a[i][j],*prms->mod[i]);*/
					/*bi_mulmod(bi[i], bi[i], b, *prms->mod[i]); */
					DBG(bi_mulmod(aux1, aux1, *prms->objs[j], *prms->mod[i]););
#endif
					/*-----------------*/
				}
			}
			assert(bi_equals_ui(aux1, 1));
		}
		sok_hash(*prms->c, dat_digest, dat_digestlen, rs, (const bigint_t*)bi,
				 prms->mod, prms->objs, sp);
		for (i = 0; i < rs->n_vbles; ++i) {
			/*  sw = tw - c(xw - 2lw) */
			assert(bi_belongs_sph_ek(*prms->vbles[i], prms->sphs[i]));
			bi_sub(b, *prms->vbles[i], prms->sphs[i]->center);
			bi_mul(b, b, *prms->c);
			bi_sub(*prms->s[i], *prms->s[i], b);
			/* sw in +-{0,1}^(eps*(mu+k)+1) */
			assert(bi_nbits(*prms->s[i]) >= prms->sphs[i]->log2radius);
			assert(bi_nbits(*prms->s[i]) <= prms->sphs[i]->log2radius+1);
		}
	} else {
		bi_asg_ui(*prms->c, 0);
		for (i = 0; i < rs->n_vbles; ++i) {
			bi_asg_ui(*prms->s[i], 0);
		}
	}
	/*--------------------------------*/
	DBG(END_VAR_1_bigint_t(aux1););
	END_VAR_A_bigint_t(bi, MAX_RELATIONS);
	END_VAR_1_bigint_t(b);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t sok_dat(unsigned which_sha,
			   const sok_relset_t* rs,		/* relation set */
			   const sok_params_t* prms,		/* sok params */
			   const void* dat,				/* message data */
			   unsigned datlen,
			   const syspar_t* sp,
			   rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = sok(rs, prms, dat_digest, dat_digestlen, sp, rnd_ctx);
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t sokv(const sok_relset_t* rs,			/* relation set */
			const sokv_params_t* prms,		/* sokv params */
			const void* dat_digest,
			unsigned dat_digestlen,
			const syspar_t* sp
#ifdef PRECOMPUTATIONS__
			, const sok_precomp_t* precomp
#endif
			)
{
	BEG_BIGINT_CHK();
	bool_t ok = TRUE;
	unsigned i, j;
	assert((rs->n_vbles <= MAX_VBLES) && (rs->n_objs <= MAX_OBJECTS)
		   && (rs->n_rels <= MAX_RELATIONS));
	assert((rs->n_vbles == prms->n_vbles) && (rs->n_objs == prms->n_objs)
		   && (rs->n_rels <= prms->n_rels));
	if (bi_nbits(*prms->c) > sp->k) {
		DBG(fprintf(stderr, "Failure: nbits(c)=[%u] out of range [%u]\n",
					bi_nbits(*prms->c), sp->k););/*DBG */
		ok = FALSE;
	}
	for (i = 0; ok && i < rs->n_vbles; ++i) {
		/* sw in +-{0,1}^(eps*(mu+k)+1) */
		if ((bi_nbits(*prms->s[i]) > prms->sphs[i]->log2radius+1)
			||(bi_nbits(*prms->s[i]) < prms->sphs[i]->log2radius)) {
			DBG(fprintf(stderr,
						"Failure: nbits(s[%u])=[%u] out of range [%u]\n",
						i, bi_nbits(*prms->s[i]),
						prms->sphs[i]->log2radius););/*DBG */
			ok = FALSE;
		}
	}
	if (ok) {
		BEG_VAR_2_bigint_t(b, c1);
		BEG_VAR_A_bigint_t(bi, MAX_RELATIONS);
		bi_negate(c1, *prms->c);
		for (i = 0; i < rs->n_rels; ++i) {
			bi_asg_ui(bi[i], 1);
			for (j = 0; j < rs->n_objs; ++j) {
				assert(is_vble(rs->a[i][j])
					   || ((rs->a[i][j] >= -1) && (rs->a[i][j] <= +1))); 
				if (is_vble(rs->a[i][j])) {
					/*  Bi *= Aj^2lw */
					unsigned vidx = get_vble_idx(rs->a[i][j]);
					assert(vidx < rs->n_vbles);
					/* NOTE: OPTIMIZATION: This value may be precomputed */
#ifdef PRECOMPUTATIONS__
					if ((precomp != NULL) && (precomp->v[i][j] != NULL)) {
						DBG(bi_powmod_dbg(b, *prms->objs[j],
										  prms->sphs[vidx]->center,
										  *prms->mod[i]););
						DBG(if (!bi_equals(b, *precomp->v[i][j])) {
								fprintf(stderr,"PreCompError i: %d j: %d\n",
										i, j);});
						assert(bi_equals(b, *precomp->v[i][j]));
						bi_mulmod(bi[i], bi[i], *precomp->v[i][j],
								  *prms->mod[i]);
					} else 
#endif
						{
							bi_powmod_sec(b, *prms->objs[j],
										  prms->sphs[vidx]->center,
										  *prms->mod[i]);
							bi_mulmod(bi[i], bi[i], b, *prms->mod[i]);
						}
				} else if (rs->a[i][j] == -1) {
					/*  Bi *= Aj^aij */
					/*bi_powmod_ui(b,*prms->objs[j],rs->a[i][j],*prms->mod[i]);*/
					bi_invmod(b, *prms->objs[j], *prms->mod[i]);
					bi_mulmod(bi[i], bi[i], b, *prms->mod[i]);
				} else if (rs->a[i][j] == +1) {
					/*  Bi *= Aj^aij */
					/*bi_powmod_ui(b,*prms->objs[j],rs->a[i][j],*prms->mod[i]);*/
					/*bi_mulmod(bi[i], bi[i], b, *prms->mod[i]); */
					bi_mulmod(bi[i], bi[i], *prms->objs[j], *prms->mod[i]);
				}
			}
			/*  Bi = Bi^-c */
			bi_powmod(bi[i], bi[i], c1, *prms->mod[i]);
			for (j = 0; j < rs->n_objs; ++j) {
				if (is_vble(rs->a[i][j])) {
					/*  Bi *= Aj^sw */
					unsigned vidx = get_vble_idx(rs->a[i][j]);
					assert(vidx < rs->n_vbles);
					bi_powmod(b, *prms->objs[j], *prms->s[vidx], *prms->mod[i]);
					bi_mulmod(bi[i], bi[i], b, *prms->mod[i]);
				}
			}
		}
		sok_hash(c1, dat_digest, dat_digestlen, rs, (const bigint_t*)bi,
				 prms->mod, prms->objs,
				 sp);
		ok = bi_equals(c1, *prms->c);
		END_VAR_A_bigint_t(bi, MAX_RELATIONS);
		END_VAR_2_bigint_t(b, c1);
	}
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------*/
bool_t sokv_dat(unsigned which_sha,
				const sok_relset_t* rs,			/* relation set */
				const sokv_params_t* prms,		/* sokv params */
				const void* dat,				/* message data */
				unsigned datlen,
				const syspar_t* sp
#ifdef PRECOMPUTATIONS__
				, const sok_precomp_t* precomp
#endif
				)
{
	bool_t ok = TRUE;
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	BEG_BIGINT_CHK();
	/*--------------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = sokv(rs, prms, dat_digest, dat_digestlen, sp
#ifdef PRECOMPUTATIONS__
					  , precomp
#endif
				 );
	/*--------------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------*/
/*---------------------------------*/
/* SK{(x): y1 = g1^x (mod n1) ; y2 = g2^x (mod n1) ; ... } */
/*---------------------------------*/
/* Vble    { x } */
/* Sphere   */
/*------------------------------------*/
/* Objs     { g1, y1, g2, y2, ... } | MOD */
/*------------------------------------*/
/* Rels     {  x, -1,  0,  0, ... } | n1 */
/*          {  0,  0,  x, -1, ... } | n2 */
/*          { ................... } | ... */
/*------------------------------------*/
bool_t dlog_proof(bigint_t* c,
				  bigint_t* sx,
				  const void* dat_digest,
				  unsigned dat_digestlen,
				  unsigned nrel,				/* nelms(dlog[]) == nrel */
				  const dlog_p_t dlog[],	/* nelms(dlog[]) == nrel */
				  const sphere_t* sph,
				  const bigint_t* x,
				  const syspar_t* sp,
				  rnd_ctx_t* rnd_ctx)
{
	bool_t ok = TRUE;
	BEG_BIGINT_CHK();
	unsigned i;
	USHAContext sha_ctx;
	BEG_VAR_2_bigint_t(b, bi);
	/*-----------------------------*/
	ok = bi_belongs_sph_ek(*x, sph);
	if (ok) {
		/* tw in +-{0,1}^(eps*(mu+k)) */
		bi_random_nb(*sx, sph->log2radius, rnd_ctx);
		/* adjust sign */
		if ((bi_bit(*sx, sph->log2radius/2)
			 ^ bi_bit(*sx, sph->log2radius/4)) == 1) {
			bi_negate(*sx, *sx);
		}
		assert(bi_nbits(*sx) == sph->log2radius);/* <= */
		/*---------------------------------*/
		/* HASH( msg, {bi}, {ni}, {oi} ) ; same order as sok_hash */
		/*---------------------------------*/
		bi_sha_reset(&sha_ctx, sp->k);
		sha_input(&sha_ctx, dat_digest, dat_digestlen);
		for (i = 0; i < nrel; ++i) {
			DBG(bi_powmod_dbg(b, *dlog[i].g, *x, *dlog[i].n););
			assert(bi_equals(b, *dlog[i].y));
			bi_powmod(bi, *dlog[i].g, *sx, *dlog[i].n);
			bi_sha_input(&sha_ctx, bi);
		}
		for (i = 0; i < nrel; ++i) {
			bi_sha_input(&sha_ctx, *dlog[i].n);
		}
		for (i = 0; i < nrel; ++i) {
			bi_sha_input(&sha_ctx, *dlog[i].g);
			bi_sha_input(&sha_ctx, *dlog[i].y);
		}
		bi_sha_result_len(&sha_ctx, *c, sp->k/CHAR_BIT);
		/*-----------------------------*/
		assert(bi_belongs_sph_ek(*x, sph));
		bi_sub(b, *x, sph->center);
		bi_mul(b, b, *c);
		bi_sub(*sx, *sx, b);
		/* sw in +-{0,1}^(eps*(mu+k)+1) */
		assert(bi_nbits(*sx) >= sph->log2radius);
		assert(bi_nbits(*sx) <= sph->log2radius+1);
	} else {
		bi_asg_ui(*c, 0);
		bi_asg_ui(*sx, 0);
	}
	/*-----------------------------*/
	END_VAR_2_bigint_t(b, bi);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------*/
bool_t dlog_proof_dat(unsigned which_sha,
					  bigint_t* c,
					  bigint_t* sx,
					  const void* dat,
					  unsigned datlen,
					  unsigned nrel,			/* nelms(dlog[]) == nrel */
					  const dlog_p_t dlog[],	/* nelms(dlog[]) == nrel */
					  const sphere_t* sph,
					  const bigint_t* x,
					  const syspar_t* sp,
					  rnd_ctx_t* rnd_ctx)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	/*-----------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = dlog_proof(c, sx, dat_digest, dat_digestlen, nrel, dlog, sph, x, sp,
					rnd_ctx);
	/*-----------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------*/
bool_t dlog_vrfy(const bigint_t* c,
				 const bigint_t* sx,
				 const void* dat_digest,
				 unsigned dat_digestlen,
				 unsigned nrel,				/* nelms(dlog[]) == nrel */
				 const dlog_p_t dlog[],	/* nelms(dlog[]) == nrel */
				 const sphere_t* sph,
				 const syspar_t* sp)
{
	BEG_BIGINT_CHK();
	bool_t ok = TRUE;
	unsigned i;
	USHAContext sha_ctx;
	/*--------------------------------*/
	if (bi_nbits(*c) > sp->k) {
		DBG(fprintf(stderr, "Failure: nbits(c)=[%u] out of range [%u]\n",
					bi_nbits(*c), sp->k););/*DBG */
		ok = FALSE;
	} else if ((bi_nbits(*sx) > sph->log2radius+1)
			   ||(bi_nbits(*sx) < sph->log2radius)){
		/* sw in +-{0,1}^(eps*(mu+k)+1) */
		DBG(fprintf(stderr,
					"Failure: nbits(sx)=[%u] out of range [%u]\n",
					bi_nbits(*sx), sph->log2radius););/*DBG */
		ok = FALSE;
	}
	if (ok) {
		BEG_VAR_3_bigint_t(b, c1, bi);
		bi_negate(c1, *c);
		/*---------------------------------*/
		/* HASH( msg, {bi}, {ni}, {oi} ) ; same order as sok_hash */
		/*---------------------------------*/
		bi_sha_reset(&sha_ctx, sp->k);
		sha_input(&sha_ctx, dat_digest, dat_digestlen);
		/*-----------------------------*/
		for (i = 0; i < nrel; ++i) {
			bi_powmod_sec(bi, *dlog[i].g, sph->center, *dlog[i].n);
			bi_invmod(b, *dlog[i].y, *dlog[i].n);
			bi_mulmod(bi, bi, b, *dlog[i].n);
			bi_powmod(bi, bi, c1, *dlog[i].n);
			bi_powmod(b, *dlog[i].g, *sx, *dlog[i].n);
			bi_mulmod(bi, bi, b, *dlog[i].n);
			bi_sha_input(&sha_ctx, bi);
		}
		for (i = 0; i < nrel; ++i) {
			bi_sha_input(&sha_ctx, *dlog[i].n);
		}
		for (i = 0; i < nrel; ++i) {
			bi_sha_input(&sha_ctx, *dlog[i].g);
			bi_sha_input(&sha_ctx, *dlog[i].y);
		}
		bi_sha_result_len(&sha_ctx, c1, sp->k/CHAR_BIT);
		ok = bi_equals(c1, *c);
		END_VAR_3_bigint_t(b, c1, bi);
	}
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------*/
bool_t dlog_vrfy_dat(unsigned which_sha,
					 const bigint_t* c,
					 const bigint_t* sx,
					 const void* dat,
					 unsigned datlen,
					 unsigned nrel,			/* nelms(dlog[]) == nrel */
					 const dlog_p_t dlog[],	/* nelms(dlog[]) == nrel */
					 const sphere_t* sph,
					 const syspar_t* sp)
{
	bool_t ok = TRUE;
	BEG_BIGINT_CHK();
	unsigned dat_digestlen = USHAMaxHashSize;
	char dat_digest[USHAMaxHashSize];
	/*-----------------------------*/
	sha_data(which_sha, dat_digest, &dat_digestlen, dat, datlen);
	ok = dlog_vrfy(c, sx, dat_digest, dat_digestlen, nrel, dlog, sph, sp);
	/*-----------------------------*/
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/

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
/** @file */
#ifndef umalccvbg_random_h__
#define umalccvbg_random_h__	1
#ifdef __cplusplus
extern "C" {
#endif
/*-------------------------------------------------------------*/
/**
 * Entropy source:
 * - TrueEntropy	it blocks until there are enough real random bits 
 * - PseudoEntropy	real entropy, if not enough bits, then pseudo entropy
 * - NoEntropy		no entropy, always start with the same seed
 *
 * Do Not change the asigned values (the order)
 *	[ it must match with another definition ]
 */
typedef enum Entropy_src_t {
	TRUE_ENTROPY, PSEUDO_ENTROPY, NO_ENTROPY
} entropy_src_t;
/*--------------------------------*/
/**
 * Random bytes generator context
 */
typedef struct rndctx_t rnd_ctx_t;
/*-------------------------------------------------------------*/
/**
 * @defgroup random_adt
 * Random Context ADT: Constructor (New), Destructor (Delete), Clone
 * @{
 */
struct rndctx_t* rndctx_t_new();
struct rndctx_t* rndctx_t_clone(const struct rndctx_t* o);
void rndctx_t_delete(struct rndctx_t* p);
/** @} */
/*-------------------------------------------------------------*/
/**
 * Seeds the random bytes generator
 * 
 * @param  rnd_ctx      Random bytes generator context
 * @param  entropy_src  Entropy source: TRUE_ENTROPY, PSEUDO_ENTROPY, NO_ENTROPY
 * @return length (in bytes) of entropy and nonce bytes used for seeding
 * @pre    Already constructed ADT (@a rnd_ctx)
 */
unsigned random_seed(rnd_ctx_t* rnd_ctx, unsigned entropy_src);
/*-------------------------------------------------------------*/
/**
 * Reseeds the random bytes generator
 * 
 * @param  rnd_ctx      Random bytes generator context
 * @param  entropy_src  Entropy source: TRUE_ENTROPY, PSEUDO_ENTROPY, NO_ENTROPY
 * @return length (in bytes) of entropy and nonce bytes used for seeding
 * @pre    Already constructed ADT (@a rnd_ctx)
 */
unsigned random_reseed(rnd_ctx_t* rnd_ctx, unsigned entropy_src);
/*-------------------------------------------------------------*/
/**
 * Generates random bytes
 * 
 * @param[out]     data     output data buffer to hold the random bytes
 * @param[in]      nbytes   requested number of random bytes
 * @param[in,out]  rnd_ctx  random bytes generator context
 * @return void
 * @pre    Already constructed ADT (@a rnd_ctx)
 */
void random_bytes(void* data, unsigned nbytes, rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
/**
 * Generates a random unsigned number less than @a max (0 <= r < max)
 * 
 * @param[in]      max      outer limit for the generated random number
 * @param[in,out]  rnd_ctx  random bytes generator context
 * @return a random unsigned number less than @a max (0 <= r < max)
 * @pre    Already constructed ADT (@a rnd_ctx)
 */
unsigned random_uint(unsigned max, rnd_ctx_t* rnd_ctx);
/*-------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

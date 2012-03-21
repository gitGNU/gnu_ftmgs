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
#ifndef umalccvbg_random_h__
#define umalccvbg_random_h__	1
#ifdef __cplusplus
extern "C" {
#endif
/*-------------------------------------------------------------*/
/*
 * Do Not change the asigned values (the order)
 *	[ it must mach with another definition ]
 *
 * TrueEntropy		it blocks until there are enough real random bits 
 * PseudoEntropy	real entropy, if not enough bits, then pseudo 
 * NoEntropy		no entropy, always start with the same seed
 */
typedef enum Entropy_src_t {
	TRUE_ENTROPY, PSEUDO_ENTROPY, NO_ENTROPY
} entropy_src_t;
/* 
 * NISTSP80090. 10.1.1.1. pg: 35
 */
enum {
	RANDOM_CTX_SEEDLEN = 55
};
typedef struct rndctx_t {
	char v[RANDOM_CTX_SEEDLEN];
	char c[RANDOM_CTX_SEEDLEN];
	unsigned counter;
} rnd_ctx_t;
/*-------------------------------------------------------------*/
void rndctx_t_ctor(struct rndctx_t* p);
void rndctx_t_dtor(struct rndctx_t* p);
void rndctx_t_asg(struct rndctx_t* d, const struct rndctx_t* o);
struct rndctx_t* rndctx_t_new();
struct rndctx_t* rndctx_t_clone(const struct rndctx_t* o);
void rndctx_t_delete(struct rndctx_t* p);
/*-------------------------------------------------------------*/
unsigned random_seed(rnd_ctx_t* rnd_ctx, unsigned entropy_src);
unsigned random_reseed(rnd_ctx_t* rnd_ctx, unsigned entropy_src);
/*-------------------------------------------------------------*/
void random_bytes(void* data, unsigned nbytes, rnd_ctx_t* rnd_ctx);
/*------------------------------------*/
/* r rnd such that 0 <= r < max */
unsigned random_uint(unsigned max, rnd_ctx_t* rnd_ctx);
/*-------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

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
#ifndef umalccvbg_generator_h__
#define umalccvbg_generator_h__		1
#include "bigint.h"
#include "random.h"
#include "birnd.h"
BEGIN_EXTERN_C
/*------------------------------------------------------------------------*/
void zpq_generator(bigint_t g, const bigint_t p, const bigint_t q,
				   rnd_ctx_t* rndctx);
void gqzp_generator(bigint_t g, const bigint_t p, const bigint_t q,
					rnd_ctx_t* rndctx);
void qrn_generator(bigint_t g, const bigint_t n, const bigint_t order,
				   const bigint_t p1, const bigint_t q1,
				   rnd_ctx_t* rndctx);
void qrn_generator_2(bigint_t g, const bigint_t n,
					 const bigint_t p, const bigint_t p1,
					 const bigint_t q, const bigint_t q1,
					 rnd_ctx_t* rndctx);
/*------------------------------------------------------------------------*/
END_EXTERN_C
#endif

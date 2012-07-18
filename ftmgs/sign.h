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
#ifndef umalccvbg_sign_h__
#define umalccvbg_sign_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "iexport.h"
#include "birnd.h"
#include "random.h"
#include "elgamal_thr.h"
#include "group.h"
#include "join.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
#define get_T1(sg)	((sg)->T1_T2.beta)
#define get_T2(sg)	((sg)->T1_T2.alpha)
/*----------------------------------------------------------------------------*/
struct ftmgs_sign_t {
	elgamal_thr_ciphertext_t T1_T2; /* T1: beta ; T2: alpha */
	bigint_t T3;		/* zero */
	bigint_t T4;		/* zero */
	bigint_t T5;		/* zero */
	bigint_t T6;		/* zero */
	bigint_t T7;		/* zero */
	bigint_t c;			/* zero */
	bigint_t sx;		/* zero */
	bigint_t sx1;		/* zero */
	bigint_t se;		/* zero */
	bigint_t sr;		/* zero */
	bigint_t sh1;		/* zero */
};
/*----------------------------------------------------------------------------*/
void ftmgs_hash_sign(unsigned which_sha,
					 void* sg_digest,
					 unsigned* sg_digestlen,
					 const ftmgs_sign_t* sg);
/*------------------------------------*/
void ftmgs_hash_add_sign(USHAContext* sha_ctx, const ftmgs_sign_t* sg);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ftmgs_sign_t_ctor(struct ftmgs_sign_t* p);
void ftmgs_sign_t_dtor(struct ftmgs_sign_t* p);
void ftmgs_sign_t_asg(struct ftmgs_sign_t* p, const struct ftmgs_sign_t* o);
void ftmgs_sign_t_move(struct ftmgs_sign_t* p, struct ftmgs_sign_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

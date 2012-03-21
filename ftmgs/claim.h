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
#ifndef umalccvbg_claim_h__
#define umalccvbg_claim_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "group.h"
#include "join.h"
#include "sign.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
struct ftmgs_claim_t {
	bigint_t c;
	bigint_t sx;
};
/*----------------------------------------------------------------------------*/
void ftmgs_claim_t_ctor(struct ftmgs_claim_t* p);
void ftmgs_claim_t_dtor(struct ftmgs_claim_t* p);
void ftmgs_claim_t_asg(struct ftmgs_claim_t* p, const struct ftmgs_claim_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

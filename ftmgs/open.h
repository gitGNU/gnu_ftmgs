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
#ifndef umalccvbg_open_h__
#define umalccvbg_open_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "group.h"
#include "join.h"
#include "sign.h"
#include "elgamal_thr.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
struct ftmgs_open_t {
	bigint_t A;		/* zero */
};
/*----------------------------------------------------------------------------*/
struct ftmgs_opensharej_t {
	elgamal_thr_decrypt_share_t dsharej;
};
/*----------------------------------------------------------------------------*/
struct ftmgs_openacc_t {
	elgamal_thr_dshare_acc_t dshacc;
};
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ftmgs_open_t_ctor(struct ftmgs_open_t* p);
void ftmgs_open_t_dtor(struct ftmgs_open_t* p);
void ftmgs_open_t_asg(struct ftmgs_open_t* p, const struct ftmgs_open_t* o);
void ftmgs_open_t_move(struct ftmgs_open_t* p, struct ftmgs_open_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_opensharej_t_ctor(struct ftmgs_opensharej_t* p);
void ftmgs_opensharej_t_dtor(struct ftmgs_opensharej_t* p);
void ftmgs_opensharej_t_asg(struct ftmgs_opensharej_t* p, const struct ftmgs_opensharej_t* o);
void ftmgs_opensharej_t_move(struct ftmgs_opensharej_t* p, struct ftmgs_opensharej_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_openacc_t_ctor(struct ftmgs_openacc_t* p);
void ftmgs_openacc_t_dtor(struct ftmgs_openacc_t* p);
void ftmgs_openacc_t_asg(struct ftmgs_openacc_t* p, const struct ftmgs_openacc_t* o);
void ftmgs_openacc_t_move(struct ftmgs_openacc_t* p, struct ftmgs_openacc_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

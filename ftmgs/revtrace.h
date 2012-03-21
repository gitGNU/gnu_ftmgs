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
#ifndef umalccvbg_revtrace_h__
#define umalccvbg_revtrace_h__	1
#include "ftmgs.h"
#include "bigint.h"
#include "group.h"
#include "join.h"
#include "sign.h"
#include "paillier_thr.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_t {
	bigint_t n;
	bigint_t x;
};
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_sharej_t {
	paillier_thr_decrypt_share_t dsharej;
};
/*----------------------------------------------------------------------------*/
struct ftmgs_mtkey_acc_t {
    paillier_thr_dshare_acc_t dshacc;
};
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_t_ctor(struct ftmgs_mtkey_t* p);
void ftmgs_mtkey_t_dtor(struct ftmgs_mtkey_t* p);
void ftmgs_mtkey_t_asg(struct ftmgs_mtkey_t* p, const struct ftmgs_mtkey_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_sharej_t_ctor(struct ftmgs_mtkey_sharej_t* p);
void ftmgs_mtkey_sharej_t_dtor(struct ftmgs_mtkey_sharej_t* p);
void ftmgs_mtkey_sharej_t_asg(struct ftmgs_mtkey_sharej_t* p, const struct ftmgs_mtkey_sharej_t* o);
/*----------------------------------------------------------------------------*/
void ftmgs_mtkey_acc_t_ctor(struct ftmgs_mtkey_acc_t* p);
void ftmgs_mtkey_acc_t_dtor(struct ftmgs_mtkey_acc_t* p);
void ftmgs_mtkey_acc_t_asg(struct ftmgs_mtkey_acc_t* p, const struct ftmgs_mtkey_acc_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

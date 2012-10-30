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
#ifndef umalccvbg_link_h__
#define umalccvbg_link_h__	1
#include "ftmgs.h"
#include "bigintop.h"
#include "group.h"
#include "join.h"
#include "sign.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
struct ftmgs_link_t {
	bigint_t c;		/* zero */
	bigint_t sx;	/* zero */
};
/*----------------------------------------------------------------------------*/
void ftmgs_link_t_ctor(struct ftmgs_link_t* p);
void ftmgs_link_t_dtor(struct ftmgs_link_t* p);
void ftmgs_link_t_asg(struct ftmgs_link_t* p, const struct ftmgs_link_t* o);
void ftmgs_link_t_move(struct ftmgs_link_t* p, struct ftmgs_link_t* o);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

/*--------------------------------------------------------------------------- */
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
/*--------------------------------------------------------------------------- */
#ifndef umalccvbg_syssph_h__
#define umalccvbg_syssph_h__	1
#include "bigintop.h"
#include "syspar.h"
#include "sphere.h"
BEGIN_EXTERN_C
/*--------------------------------------------------------------------------- */
typedef struct syssph_t {
	sphere_t lambda_sp[MAX_SYSPAR];
	sphere_t lambda_dsa;
	sphere_t mu;
	sphere_t gamma;
	sphere_t gamma_mu;
} syssph_t;
/*----------------------------------------------------------------------------*/
void create_syssph(syssph_t* p, const syspar_t* sp);
/*--------------------------------------------------------------------------- */
const sphere_t* get_sphere_lambda_x(const bigint_t x, const syssph_t* ss);
const sphere_t* get_sphere_lambda_sx(const bigint_t sx, const syssph_t* ss);
const sphere_t* get_sphere_lambda(const syspar_t* sp, const syssph_t* ss);
const sphere_t* get_sphere_mu(const syssph_t* ss);
const sphere_t* get_sphere_gamma(const syssph_t* ss);
const sphere_t* get_sphere_gamma_mu(const syssph_t* ss);
/*----------------------------------------------------------------------------*/
void syssph_t_ctor(struct syssph_t* p);
void syssph_t_dtor(struct syssph_t* p);
void syssph_t_asg(struct syssph_t* p, const struct syssph_t* o);
void syssph_t_move(struct syssph_t* p, struct syssph_t* o);
struct syssph_t* syssph_t_new();
struct syssph_t* syssph_t_clone(const struct syssph_t* o);
void syssph_t_delete(struct syssph_t* p);
/*--------------------------------------------------------------------------- */
/*--------------------------------------------------------------------------- */
END_EXTERN_C
#endif

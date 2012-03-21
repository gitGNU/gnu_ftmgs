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
#ifndef umalccvbg_syspar_h__
#define umalccvbg_syspar_h__	1
#include "sha.h"
#ifdef __cplusplus
extern "C" {
#endif
	/*-----------------------------------------------------------------*/
#ifndef BOOL_T_DEFINED__
#define BOOL_T_DEFINED__	1
	typedef char bool_t;
#endif
#ifndef TRUE
#define TRUE	1
#elif TRUE == 0
#error "Bad definition of symbol TRUE"
#endif
#ifndef FALSE
#define FALSE	0
#elif FALSE != 0
#error "Bad definition of symbol FALSE"
#endif
	/*-------------------------------------------------------------*/
	enum { MAX_SYSPAR = 3 };
	/*-------------------------------------------------------------*/
	enum security_par_nu_t {
		NU_1 = 1024, NU_2 = 2048, NU_3 = 3072
	};
	/*-------------------------------------------------------------*/
	typedef struct syspar_t {
		unsigned nu;	/* security parameter: nbits of modulus */
		unsigned k;		/* security parameter ZK proofs 		*/
	} syspar_t;
	/*-------------------------------------------------------------*/
	/* 
	 * Currently, the implementation only supports the following
	 * values for NU: 1024, 2048, 3072
	 */
	/*-------------------------------------------------------------*/
	void set_default_syspar(syspar_t* sp);
	bool_t set_syspar(syspar_t* sp, unsigned nu);
	bool_t get_syspar(syspar_t* sp, unsigned idx);
	unsigned radius_ek(const syspar_t* sp, unsigned nbits);
	unsigned radius(const syspar_t* sp, unsigned nbits_ek);
	/*------------------------------------------------------------------------*/
#define syspar_t_dtor(syspar_t_ptr) ((void)0)
#define syspar_t_asg(d, o)			((void)(*(d) = *(o)))
	/*------------------------------------------------------------------------*/
	void syspar_t_swap(struct syspar_t* p1, struct syspar_t* p2);
	void syspar_t_ctor(struct syspar_t* p);
	/*void syspar_t_dtor(struct syspar_t* p);*/
	/*void syspar_t_asg(struct syspar_t* p, const struct syspar_t* o);*/
	struct syspar_t* syspar_t_new();
	struct syspar_t* syspar_t_clone(const struct syspar_t* o);
	void syspar_t_delete(struct syspar_t* p);
	/*-------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif

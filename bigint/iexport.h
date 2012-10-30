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
#ifndef umalccvbg_iexport_h__
#define umalccvbg_iexport_h__	1
#include <stdio.h>
#include "bigintop.h"
#include "sha.h"
BEGIN_EXTERN_C
/*----------------------------------------------------------------------------*/
#define shactx_t_ctor(p)		((void)0)
#define shactx_t_asg(p, o)		((void)(*(p) = *(o)))
#define shactx_t_dtor(p)		((void)0)
USHAContext* shactx_t_new();
USHAContext* shactx_t_clone(const USHAContext* o);
void shactx_t_delete(USHAContext* p);
/*----------------------------------------------------------------------------*/
/* 
 * which_sha may be a value
 * 1.- from the enumeration SHAversion: SHA1, SHA224, SHA256, SHA384, SHA512
 * 2.- it may also be a requested number of bytes
 * 3.- it may also be a requested number of bits
 */
bool_t sha_reset(USHAContext* sha_context, unsigned which_sha);
bool_t sha_input(USHAContext* sha_context, const void* dat, unsigned datlen);
bool_t sha_result(USHAContext* sha_context, void* digest, unsigned* digestlen);
/*------------------------------------*/
bool_t sha_data(unsigned which_sha, void* digest, unsigned* digestlen,
			  const void* dat, unsigned datlen);
/*----------------------------------------------------------------------------*/
/* 
 * which_sha may be a value
 * 1.- from the enumeration SHAversion: SHA1, SHA224, SHA256, SHA384, SHA512
 * 2.- it may also be a requested number of bytes
 * 3.- it may also be a requested number of bits
 */
bool_t bi_sha_reset(USHAContext* sha_context, unsigned which_sha);
bool_t bi_sha_input(USHAContext* sha_context, const bigint_t dat);
bool_t bi_sha_result(USHAContext* sha_context, bigint_t digest);
bool_t bi_sha_result_len(USHAContext* sha_context, bigint_t digest,
					   unsigned digestlen);
/*------------------------------------*/
bool_t bi_sha_data(unsigned which_sha, bigint_t digest,
				 const void* dat, unsigned datlen);
bool_t bi_sha_data_len(unsigned which_sha, bigint_t digest, unsigned digestlen,
					 const void* dat, unsigned datlen);
/*------------------------------------*/
bool_t bi_sha(unsigned which_sha, bigint_t digest, const bigint_t dat);
bool_t bi_sha_len(unsigned which_sha, bigint_t digest, unsigned digest_len,
				const bigint_t dat);
/*----------------------------------------------------------------------------*/
void* bi_export(const bigint_t n, void* buf, unsigned* len);
void bi_import(bigint_t n, void* buf, unsigned buflen);
void bi_print_bytes(FILE* stream, const void* buf, unsigned buflen);
/*----------------------------------------------------------------------------*/
END_EXTERN_C
#endif

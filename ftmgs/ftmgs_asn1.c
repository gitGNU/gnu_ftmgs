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
/*
 * asn1Parser ftmgs.asn1
 * gcc -o ftmgs_asn1 ftmgs_asn1.c ftmgs_asn1_tab.c -ltasn1
 */
/*----------------------------------------------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bigintop.h"
#include "iexport.h"
#include "buffer.h"
#include "vector.h"
#include "system_endian.h"
#include "ftmgs.h"
#include "group.h"
#include "join.h"
#include "sign.h"
#include "open.h"
#include "revtrace.h"
#include "claim.h"
#include "link.h"
/*----------------------------------------------------------------------------*/
#include <libtasn1.h>
extern const ASN1_ARRAY_TYPE ftmgs_asn1_tab[];
/*----------------------------------------------------------------------------*/
STATIC_ASSERT(ASN1_SUCCESS == ASN1_Success &&
			  ASN1_FILE_NOT_FOUND == ASN1_File_Not_Found &&
			  ASN1_ELEMENT_NOT_FOUND == ASN1_Element_Not_Found &&
			  ASN1_IDENTIFIER_NOT_FOUND == ASN1_Identifier_Not_Found &&
			  ASN1_DER_ERROR == ASN1_Der_Error &&
			  ASN1_VALUE_NOT_FOUND == ASN1_Value_Not_Found &&
			  ASN1_GENERIC_ERROR == ASN1_Generic_Error &&
			  ASN1_VALUE_NOT_VALID == ASN1_Value_Not_Valid &&
			  ASN1_TAG_ERROR == ASN1_Tag_Error &&
			  ASN1_TAG_IMPLICIT == ASN1_Tag_Implicit &&
			  ASN1_ERROR_TYPE_ANY == ASN1_Error_Type_Any &&
			  ASN1_SYNTAX_ERROR == ASN1_Syntax_Error &&
			  ASN1_MEM_ERROR == ASN1_Mem_Error &&
			  ASN1_MEM_ALLOC_ERROR == ASN1_Mem_Alloc_Error &&
			  ASN1_DER_OVERFLOW == ASN1_Der_Overflow &&
			  ASN1_NAME_TOO_LONG == ASN1_Name_Too_Long &&
			  ASN1_ARRAY_ERROR == ASN1_Array_Error &&
			  ASN1_ELEMENT_NOT_EMPTY == ASN1_Element_Not_Empty
			  , Bad_definition_of_Asn1_retcode_values);
/*----------------------------------------------------------------------------*/
typedef struct choice_name_t {
	char name[ASN1_MAX_NAME_SIZE];
} choice_name_t;
/*----------------------------------------------------------------------------*/
/*-- Asn1-Name ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static void init(struct vector_t* name)
{
	vector_reserve(name, 128);
	vector_clear(name);
	vector_data(name)[0] = '\0';
}
/*------------------------------------*/
static bool_t concat(struct vector_t* name, const char* member)
{
	bool_t ok = TRUE;
	size_t member_len = strlen(member);
	assert((name != NULL)&&(member != NULL));
	if (member_len > 0) {
		if (vector_size(name) > 0) {
			vector_push_str(name, ".");
		}
		vector_push_str(name, member);
		ok = (vector_data(name) != NULL);
	}
	assert(vector_size(name) == strlen(vector_data(name)));
	return ok;
}
/*------------------------------------*/
static void resize(struct vector_t* name, unsigned len)
{
	assert((name != NULL)&&(len < vector_maxsize(name)));
	vector_resize(name, len);
	vector_data(name)[len] = '\0';
	assert(vector_size(name) == strlen(vector_data(name)));
}
/*----------------------------------------------------------------------------*/
/*-- Low level data IO -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_write_int(ASN1_TYPE root,
								   struct vector_t* name,
								   const char* member,
								   int int_val)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		/*----------------------------*/
		int_val = int2bigendian(int_val);
		rc = asn1_write_value(root, vector_data(name), &int_val, sizeof(int));
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 write int (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_read_int(ASN1_TYPE root,
								  struct vector_t* name,
								  const char* member,
								  int* int_val)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	int i;
	int int_sz = sizeof(int);
	unsigned char val = 0;
	unsigned char* p = (unsigned char*)int_val;
	*int_val = 0;
	if (concat(name, member)) {
		/*----------------------------*/
		rc = asn1_read_value(root, vector_data(name), int_val, &int_sz);
		if ((rc == ASN1_SUCCESS)
			&& (int_sz > 0) && (int_sz < (int)sizeof(int))) {
			if ((p[0] & 0x080U) != 0) {
				val = 0x0FFU;
			}
			/*------------------------*/
			for (i = sizeof(int)-1; i > (int)sizeof(int)-1-int_sz; --i) {
				p[i] = p[i-((int)sizeof(int)-int_sz)];
			}
			for (i = 0; i < (int)sizeof(int)-int_sz; ++i) {
				p[i] = val;
			}
			/*------------------------*/
			*int_val = bigendian2int(*int_val);
		}
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 read int (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_write_uint(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  unsigned uint_val)
{
	return asn1_write_int(root, name, member, (int)uint_val);
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_read_uint(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 unsigned* uint_val)
{
	return asn1_read_int(root, name, member, (int*)uint_val);
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_write_bigint(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  const bigint_t bigint_val)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	unsigned len = 0;
	void* buff = NULL;
	if (concat(name, member)) {
		/*----------------------------*/
		buff = bi_export(bigint_val, NULL, &len);
		if (buff != NULL) {
			rc = asn1_write_value(root, vector_data(name), buff, (int)len);
			free(buff);
		}
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 write bigint (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_read_bigint(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 bigint_t bigint_val)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	int len = 0;
	void* buff = NULL;
	bi_asg_ui(bigint_val, 0);
	if (concat(name, member)) {
		/*----------------------------*/
		rc = asn1_read_value(root, vector_data(name), buff, &len);
		if (rc == ASN1_MEM_ERROR) {
			buff = (char*)malloc((unsigned)len * sizeof(char));
			if (buff != NULL) {
				rc = asn1_read_value(root, vector_data(name), buff, &len);
			}
		}
		if (rc == ASN1_SUCCESS) {
			bi_import(bigint_val, buff, (unsigned)len);
		}
		free(buff);
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 read bigint (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_write_choice(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										const char* choice)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		/*----------------------------*/
		rc = asn1_write_value(root, vector_data(name), choice,
							  (int)strlen(choice)+1);
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 write choice (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
static asn1_retcode_t asn1_read_choice(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   choice_name_t* choice)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	int len = sizeof(*choice);
	if (concat(name, member)) {
		/*----------------------------*/
		rc = asn1_read_value(root, vector_data(name), choice->name, &len);
		/*----------------------------*/
#ifndef NDEBUG
		if (rc != ASN1_SUCCESS) {
			fprintf(stderr, "ASN1 read choice (%s) [%s]\n",
					vector_data(name), asn1_strerror(rc));
		}
#endif
		/*----------------------------*/
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/*-- DSS-DSA -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* DSS-Parms */
static asn1_retcode_t asn1_write_dssparms(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const dss_parms_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "p", p->p);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "q", p->q);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "g", p->g);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DSS-Parms */
static asn1_retcode_t asn1_read_dssparms(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										dss_parms_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "p", p->p);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "q", p->q);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "g", p->g);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DSAPublicKey */
static asn1_retcode_t asn1_write_dsapbkey(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										const dsa_pbkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "", p->y);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DSAPublicKey */
static asn1_retcode_t asn1_read_dsapbkey(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   dsa_pbkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "", p->y);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DSAPrivateKey */
static asn1_retcode_t asn1_write_dsaprkey(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										const dsa_prkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "", p->x);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DSAPrivateKey */
static asn1_retcode_t asn1_read_dsaprkey(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   dsa_prkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "", p->x);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/*-- Embedded Structures -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* SysPar */
static asn1_retcode_t asn1_write_syspar(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  const syspar_t* sp)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_uint(root, name, "nu", sp->nu);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_uint(root, name, "k", sp->k);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* SysPar */
static asn1_retcode_t asn1_read_syspar(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 syspar_t* sp)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_uint(root, name, "nu", &sp->nu);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_uint(root, name, "k", &sp->k);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAPbKey */
static asn1_retcode_t asn1_write_papbkey(ASN1_TYPE root,
										   struct vector_t* name,
										   const char* member,
										   const paillier_thr_pbkey_t* fapk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_syspar(root, name, "sp", &fapk->sp);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_uint(root, name, "nkeys", fapk->nkeys);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "n", fapk->n);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "g", fapk->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "y", fapk->y);
		}
		resize(name, name_len);
	}
	return rc;
}
/*------------------------------------*/
static asn1_retcode_t asn1_write_fapbkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_fa_pbkey_t* fapk)
{
	return asn1_write_papbkey(root, name, member, &fapk->pbkey);
}
/*----------------------------------------------------------------------------*/
/* FAPbKey */
static asn1_retcode_t asn1_read_papbkey(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  paillier_thr_pbkey_t* fapk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_syspar(root, name, "sp", &fapk->sp);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_uint(root, name, "nkeys", &fapk->nkeys);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "n", fapk->n);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "g", fapk->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "y", fapk->y);
		}
#ifdef PRECOMPUTATIONS__
		paillier_thr_pbkey_precomp(&fapk->precomp, fapk);
#endif
		resize(name, name_len);
	}
	return rc;
}
/*------------------------------------*/
static asn1_retcode_t asn1_read_fapbkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 ftmgs_fa_pbkey_t* fapk)
{
	return asn1_read_papbkey(root, name, member, &fapk->pbkey);
}
/*----------------------------------------------------------------------------*/
/* FAGrPbKey */
static asn1_retcode_t asn1_write_fagrpbkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const elgamal_thr_pbkey_t* gmpk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_syspar(root, name, "sp", &gmpk->sp);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_uint(root, name, "nkeys", gmpk->nkeys);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "n", gmpk->n);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "g", gmpk->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "y", gmpk->y);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAGrPbKey */
static asn1_retcode_t asn1_read_fagrpbkey(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										elgamal_thr_pbkey_t* gmpk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_syspar(root, name, "sp", &gmpk->sp);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_uint(root, name, "nkeys", &gmpk->nkeys);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "n", gmpk->n);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "g", gmpk->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "y", gmpk->y);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* GrPbKey */
static asn1_retcode_t asn1_write_grpbkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_pbkey_t* gpk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_fagrpbkey(root, name, "gmpk", &gpk->gmpk);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "a0", gpk->a0);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "a", gpk->a);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "b", gpk->b);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "h", gpk->h);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_papbkey(root, name, "fapk", &gpk->fapk);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* GrPbKey */
static asn1_retcode_t asn1_read_grpbkey(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_pbkey_t* gpk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_fagrpbkey(root, name, "gmpk", &gpk->gmpk);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "a0", gpk->a0);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "a", gpk->a);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "b", gpk->b);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "h", gpk->h);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_papbkey(root, name, "fapk", &gpk->fapk);
		}
#ifdef PRECOMPUTATIONS__
		ftmgs_precomp_group(gpk);
#endif
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* GrPrKey */
static asn1_retcode_t asn1_write_grprkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_prkey_t* gsk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "p", gsk->npq.p);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "q", gsk->npq.q);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* GrPrKey */
static asn1_retcode_t asn1_read_grprkey(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_prkey_t* gsk)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "p", gsk->npq.p);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "q", gsk->npq.q);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAPbKeyShare */
static asn1_retcode_t asn1_write_fapbkeysh(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_faj_pbkey_share_t* pkj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "yj", pkj->pbkey_j.yj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAPbKeyShare */
static asn1_retcode_t asn1_read_fapbkeysh(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_faj_pbkey_share_t* pkj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "yj", pkj->pbkey_j.yj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAPrKey */
static asn1_retcode_t asn1_write_faprkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_faj_prkey_t* skj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "xj", skj->prkey_j.xj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAPrKey */
static asn1_retcode_t asn1_read_faprkey(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   ftmgs_faj_prkey_t* skj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "xj", skj->prkey_j.xj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAGrPbKeyShare */
static asn1_retcode_t asn1_write_fagrpbkeysh(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_faj_gr_pbkey_share_t* pkj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "yj", pkj->pbkey_j.yj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "hj", pkj->hj);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAGrPbKeyShare */
static asn1_retcode_t asn1_read_fagrpbkeysh(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_faj_gr_pbkey_share_t* pkj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "yj", pkj->pbkey_j.yj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "hj", pkj->hj);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAGrPrKey */
static asn1_retcode_t asn1_write_fagrprkey(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_faj_gr_prkey_t* skj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "xj", skj->prkey_j.xj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* FAGrPrKey */
static asn1_retcode_t asn1_read_fagrprkey(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   ftmgs_faj_gr_prkey_t* skj)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "xj", skj->prkey_j.xj);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DLog */
static asn1_retcode_t asn1_write_dlog(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const dlog_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "n", p->n);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "g", p->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "y", p->y);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DLog */
static asn1_retcode_t asn1_read_dlog(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										dlog_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "n", p->n);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "g", p->g);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "y", p->y);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DLogX */
static asn1_retcode_t asn1_write_dlogx(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const dlogx_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "x", p->x);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* DLogX */
static asn1_retcode_t asn1_read_dlogx(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   dlogx_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "x", p->x);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinProof */
static asn1_retcode_t asn1_write_joinproof(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_proof_t* jp)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "c", jp->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx1", jp->sx1);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sr", jp->sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", jp->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinProof */
static asn1_retcode_t asn1_read_joinproof(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										join_proof_t* jp)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "c", jp->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx1", jp->sx1);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sr", jp->sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", jp->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU1Prv */
static asn1_retcode_t asn1_write_joinu1prv(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_u1prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "nadrp-xx", p->nadrp_a1.xx);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-rr", p->nadrp_a1.rr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "x1i", p->x1i);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU1Prv */
static asn1_retcode_t asn1_read_joinu1prv(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										join_u1prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "nadrp-xx", p->nadrp_a1.xx);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-rr", p->nadrp_a1.rr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "x1i", p->x1i);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU1Pbl */
static asn1_retcode_t asn1_write_joinu1pbl(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_u1pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "nadrp-C1", p->nadrp_a1.C1);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-C2", p->nadrp_a1.C2);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-c", p->nadrp_a1.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-sx", p->nadrp_a1.sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-sr", p->nadrp_a1.sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ci", p->Ci);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU1Pbl */
static asn1_retcode_t asn1_read_joinu1pbl(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										join_u1pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "nadrp-C1", p->nadrp_a1.C1);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-C2", p->nadrp_a1.C2);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-c", p->nadrp_a1.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-sx", p->nadrp_a1.sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-sr", p->nadrp_a1.sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ci", p->Ci);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinGM2Pbl */
static asn1_retcode_t asn1_write_joingm2pbl(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_gm2pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "nadrp-yy", p->nadrp_b2.yy);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinGM2Pbl */
static asn1_retcode_t asn1_read_joingm2pbl(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   join_gm2pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "nadrp-yy", p->nadrp_b2.yy);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU3Prv */
static asn1_retcode_t asn1_write_joinu3prv(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_u3prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "nadrp-x", p->nadrp_a3.x);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "xi", p->Xi);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU3Prv */
static asn1_retcode_t asn1_read_joinu3prv(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										join_u3prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "nadrp-x", p->nadrp_a3.x);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "xi", p->Xi);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU3Pbl */
static asn1_retcode_t asn1_write_joinu3pbl(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const join_u3pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "nadrp-C3", p->nadrp_a3.C3);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-c", p->nadrp_a3.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-sx", p->nadrp_a3.sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-sz", p->nadrp_a3.sz);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "nadrp-sr", p->nadrp_a3.sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ui", get_Ui(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "vi", get_Vi(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_joinproof(root, name, "eiproof", &p->Eiproof);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinU3Pbl */
static asn1_retcode_t asn1_read_joinu3pbl(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										join_u3pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "nadrp-C3", p->nadrp_a3.C3);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-c", p->nadrp_a3.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-sx", p->nadrp_a3.sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-sz", p->nadrp_a3.sz);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "nadrp-sr", p->nadrp_a3.sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ui", get_Ui(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "vi", get_Vi(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_joinproof(root, name, "eiproof", &p->Eiproof);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinGM4Pbl */
static asn1_retcode_t asn1_write_joingm4pbl(ASN1_TYPE root,
										  struct vector_t* name,
										  const char* member,
										  const join_gm4pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ei", p->ei);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinGM4Pbl */
static asn1_retcode_t asn1_read_joingm4pbl(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 join_gm4pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ei", p->ei);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinPbl */
static asn1_retcode_t asn1_write_joinpbl(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   const ftmgs_join_pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_uint(root, name, "status", p->status);
		if (rc == ASN1_SUCCESS) {
			switch (p->status) {
			case 1:
				rc = asn1_write_choice(root, name, "data" , "u1");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joinu1pbl(root, name, "data.u1", &p->u1);
				}
				break;
			case 2:
				rc = asn1_write_choice(root, name, "data" , "gm2");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joingm2pbl(root, name, "data.gm2", &p->gm2);
				}
				break;
			case 3:
				rc = asn1_write_choice(root, name, "data" , "u3");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joinu3pbl(root, name, "data.u3", &p->u3);
				}
				break;
			case 4:
				rc = asn1_write_choice(root, name, "data" , "gm4");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joingm4pbl(root, name, "data.gm4", &p->gm4);
				}
				break;
			default:
				rc = asn1_write_choice(root, name, "data" , "error");
				if (rc == ASN1_SUCCESS) {
					rc = ASN1_VALUE_NOT_VALID;
				}
				break;
			}
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinPbl */
static asn1_retcode_t asn1_read_joinpbl(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_join_pbl_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	unsigned st = 0;
	choice_name_t choice;
	if (concat(name, member)) {
		rc = asn1_read_uint(root, name, "status", &st);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_choice(root, name, "data" , &choice);
		}
		if (rc == ASN1_SUCCESS) {
			if ((st < 5)&&(st > p->status)) {
				p->status = st;
				switch (p->status) {
				case 1:
					if (strcmp(choice.name, "u1")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joinu1pbl(root, name, "data.u1", &p->u1);
					}
					break;
				case 2:
					if (strcmp(choice.name, "gm2")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joingm2pbl(root, name, "data.gm2",
												  &p->gm2);
					}
					break;
				case 3:
					if (strcmp(choice.name, "u3")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joinu3pbl(root, name, "data.u3", &p->u3);
					}
					break;
				case 4:
					if (strcmp(choice.name, "gm4")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joingm4pbl(root, name, "data.gm4",
												  &p->gm4);
					}
					break;
				default:
					rc = ASN1_VALUE_NOT_VALID;
					break;
				}
			} else {
				rc = ASN1_VALUE_NOT_VALID;
			}
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinPrv */
static asn1_retcode_t asn1_write_joinprv(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 const ftmgs_join_prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_uint(root, name, "status", p->status);
		if (rc == ASN1_SUCCESS) {
			switch (p->status) {
			case 1:
				rc = asn1_write_choice(root, name, "data" , "u1");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joinu1prv(root, name, "data.u1", &p->u1);
				}
				break;
			case 2:
				rc = asn1_write_choice(root, name, "data" , "u3");
				if (rc == ASN1_SUCCESS) {
					rc = asn1_write_joinu3prv(root, name, "data.u3", &p->u3);
				}
				break;
			default:
				rc = asn1_write_choice(root, name, "data" , "error");
				if (rc == ASN1_SUCCESS) {
					rc = ASN1_VALUE_NOT_VALID;
				}
				break;
			}
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* JoinPrv */
static asn1_retcode_t asn1_read_joinprv(ASN1_TYPE root,
										struct vector_t* name,
										const char* member,
										ftmgs_join_prv_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	unsigned st = 0;
	choice_name_t choice;
	if (concat(name, member)) {
		rc = asn1_read_uint(root, name, "status", &st);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_choice(root, name, "data" , &choice);
		}
		if (rc == ASN1_SUCCESS) {
			if ((st < 3)&&(st > p->status)) {
				p->status = st;
				switch (p->status) {
				case 1:
					if (strcmp(choice.name, "u1")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joinu1prv(root, name, "data.u1", &p->u1);
					}
					break;
				case 2:
					if (strcmp(choice.name, "u3")!=0) {
						rc = ASN1_VALUE_NOT_VALID;
					} else {
						rc = asn1_read_joinu3prv(root, name, "data.u3", &p->u3);
					}
					break;
				default:
					rc = ASN1_VALUE_NOT_VALID;
					break;
				}
			} else {
				rc = ASN1_VALUE_NOT_VALID;
			}
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MbrRef */
static asn1_retcode_t asn1_write_mbrref(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  const ftmgs_mbr_ref_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ei", p->ei);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ci", p->Ci);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "xi", p->Xi);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ui", get_Ui(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "vi", get_Vi(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_dlog(root, name, "uauth", &p->user_auth);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_joinproof(root, name, "eiproof", &p->Eiproof);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MbrRef */
static asn1_retcode_t asn1_read_mbrref(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 ftmgs_mbr_ref_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ei", p->ei);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ci", p->Ci);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "xi", p->Xi);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ui", get_Ui(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "vi", get_Vi(&p->UiVi));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_dlog(root, name, "uauth", &p->user_auth);
			if (rc == ASN1_VALUE_NOT_FOUND) {
				bi_asg_ui(p->user_auth.n, 0);
				bi_asg_ui(p->user_auth.g, 0);
				bi_asg_ui(p->user_auth.y, 0);
				rc = ASN1_SUCCESS;	
			}
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_joinproof(root, name, "eiproof", &p->Eiproof);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MbrPrKey */
static asn1_retcode_t asn1_write_mbrprkey(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  const ftmgs_mbr_prkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "ei", p->ei);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "xi", p->xi);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "x1i", p->x1i);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MbrPrKey */
static asn1_retcode_t asn1_read_mbrprkey(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 ftmgs_mbr_prkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "ai", p->Ai);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "ei", p->ei);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "xi", p->xi);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "x1i", p->x1i);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Sign */
static asn1_retcode_t asn1_write_sign(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  const ftmgs_sign_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "t1", get_T1(p));
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t2", get_T2(p));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t3", p->T3);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t4", p->T4);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t5", p->T5);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t6", p->T6);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "t7", p->T7);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "c", p->c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", p->sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx1", p->sx1);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "se", p->se);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sr", p->sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sh1", p->sh1);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Sign */
static asn1_retcode_t asn1_read_sign(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 ftmgs_sign_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "t1", get_T1(p));
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t2", get_T2(p));
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t3", p->T3);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t4", p->T4);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t5", p->T5);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t6", p->T6);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "t7", p->T7);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "c", p->c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", p->sx);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx1", p->sx1);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "se", p->se);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sr", p->sr);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sh1", p->sh1);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Open */
static asn1_retcode_t asn1_write_open(ASN1_TYPE root,
									struct vector_t* name,
									const char* member,
									const ftmgs_open_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "a", p->A);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Open */
static asn1_retcode_t asn1_read_open(ASN1_TYPE root,
								   struct vector_t* name,
								   const char* member,
								   ftmgs_open_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "a", p->A);
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* OpenShare */
static asn1_retcode_t asn1_write_openshare(ASN1_TYPE root,
										  struct vector_t* name,
										  const char* member,
										  const ftmgs_opensharej_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "alphax", p->dsharej.alpha_xj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "c", p->dsharej.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", p->dsharej.sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* OpenShare */
static asn1_retcode_t asn1_read_openshare(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 ftmgs_opensharej_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "alphax", p->dsharej.alpha_xj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "c", p->dsharej.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", p->dsharej.sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* OpenAcc */
static asn1_retcode_t asn1_write_openacc(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   const ftmgs_openacc_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_uint(root, name, "nshares", p->dshacc.nshares);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "a", p->dshacc.a);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* OpenAcc */
static asn1_retcode_t asn1_read_openacc(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  ftmgs_openacc_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_uint(root, name, "nshares", &p->dshacc.nshares);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "a", p->dshacc.a);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKey */
static asn1_retcode_t asn1_write_mtkey(ASN1_TYPE root,
									struct vector_t* name,
									const char* member,
									const ftmgs_mtkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "n", p->n);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "x", p->x);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKey */
static asn1_retcode_t asn1_read_mtkey(ASN1_TYPE root,
								   struct vector_t* name,
								   const char* member,
								   ftmgs_mtkey_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "n", p->n);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "x", p->x);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKeyShare */
static asn1_retcode_t asn1_write_mtkeyshare(ASN1_TYPE root,
										  struct vector_t* name,
										  const char* member,
										  const ftmgs_mtkey_sharej_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "alphax", p->dsharej.alpha_xj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "c", p->dsharej.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", p->dsharej.sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKeyShare */
static asn1_retcode_t asn1_read_mtkeyshare(ASN1_TYPE root,
										 struct vector_t* name,
										 const char* member,
										 ftmgs_mtkey_sharej_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "alphax", p->dsharej.alpha_xj);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "c", p->dsharej.c);
		}
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", p->dsharej.sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKeyAcc */
static asn1_retcode_t asn1_write_mtkeyacc(ASN1_TYPE root,
									   struct vector_t* name,
									   const char* member,
									   const ftmgs_mtkey_acc_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_uint(root, name, "nshares", p->dshacc.nshares);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "a", p->dshacc.a);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* MTKeyAcc */
static asn1_retcode_t asn1_read_mtkeyacc(ASN1_TYPE root,
									  struct vector_t* name,
									  const char* member,
									  ftmgs_mtkey_acc_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_uint(root, name, "nshares", &p->dshacc.nshares);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "a", p->dshacc.a);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Claim */
static asn1_retcode_t asn1_write_claim(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 const ftmgs_claim_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "c", p->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", p->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Claim */
static asn1_retcode_t asn1_read_claim(ASN1_TYPE root,
									struct vector_t* name,
									const char* member,
									ftmgs_claim_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "c", p->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", p->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Link */
static asn1_retcode_t asn1_write_link(ASN1_TYPE root,
									 struct vector_t* name,
									 const char* member,
									 const ftmgs_link_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_write_bigint(root, name, "c", p->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_write_bigint(root, name, "sx", p->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/* Link */
static asn1_retcode_t asn1_read_link(ASN1_TYPE root,
									struct vector_t* name,
									const char* member,
									ftmgs_link_t* p)
{
	asn1_retcode_t rc = ASN1_VALUE_NOT_VALID;
	unsigned name_len = vector_size(name);
	if (concat(name, member)) {
		rc = asn1_read_bigint(root, name, "c", p->c);
		if (rc == ASN1_SUCCESS) {
			rc = asn1_read_bigint(root, name, "sx", p->sx);
		}
		resize(name, name_len);
	}
	return rc;
}
/*----------------------------------------------------------------------------*/
/*- FTMGS structures ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define GENERATE_ASN1_ENC_DEC(FUNC_NAME, TYPE_NAME, ASN1_NAME)			\
	asn1_retcode_t asn1_enc_##FUNC_NAME(buffer_t* buff, const TYPE_NAME* p) \
	{																	\
		asn1_retcode_t rc;												\
		int buflen = 0;													\
		ASN1_TYPE definitions = ASN1_TYPE_EMPTY;						\
		char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];			\
		ASN1_TYPE element = ASN1_TYPE_EMPTY;							\
		struct vector_t name;											\
		vector_t_ctor(&name);											\
		init(&name);													\
		rc = asn1_array2tree(ftmgs_asn1_tab, &definitions, errorDescription); \
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_create_element(definitions, ASN1_NAME, &element);	\
		}																\
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_write_##FUNC_NAME(element, &name, "", p); \
		}																\
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_der_coding(element, "", NULL, &buflen, errorDescription);	\
			if (rc == ASN1_MEM_ERROR) {									\
				buffer_reserve(buff, (unsigned)buflen);					\
				if (buffer_data(buff) != NULL) {						\
					rc = asn1_der_coding(element, "",					\
										 buffer_data(buff),				\
										 &buflen,						\
										 errorDescription);				\
					if (rc != ASN1_SUCCESS) {							\
						buffer_reset(buff);								\
					}													\
				}														\
			}															\
		}																\
		asn1_delete_structure(&element);								\
		asn1_delete_structure(&definitions);							\
		vector_t_dtor(&name);											\
		return rc;														\
	}																	\
	asn1_retcode_t asn1_dec_##FUNC_NAME(TYPE_NAME* p, const void* buff, int len) \
	{																	\
		asn1_retcode_t rc;												\
		ASN1_TYPE definitions = ASN1_TYPE_EMPTY;						\
		char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];			\
		ASN1_TYPE element = ASN1_TYPE_EMPTY;							\
		struct vector_t name;											\
		vector_t_ctor(&name);											\
		init(&name);													\
		rc = asn1_array2tree(ftmgs_asn1_tab, &definitions, errorDescription); \
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_create_element(definitions, ASN1_NAME, &element);	\
		}																\
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_der_decoding(&element, buff, len,					\
								   errorDescription);					\
		}																\
		if (rc == ASN1_SUCCESS) {										\
			rc = asn1_read_##FUNC_NAME(element, &name, "", p);			\
		}																\
		asn1_delete_structure(&element);								\
		asn1_delete_structure(&definitions);							\
		vector_t_dtor(&name);											\
		return rc;														\
	}
/*----------------------------------------------------------------------------*/
/* 
 * GENERATE_ASN1_ENC_DEC(syspar, syspar_t, "Ftmgs.SysPar")
 * GENERATE_ASN1_ENC_DEC(fagrpbkey, elgamal_thr_pbkey_t, "Ftmgs.FAGrPbKey")
 * GENERATE_ASN1_ENC_DEC(joinproof, join_proof_t, "Ftmgs.JoinProof")
 * GENERATE_ASN1_ENC_DEC(joinu1prv, join_u1prv_t, "Ftmgs.JoinU1Prv")
 * GENERATE_ASN1_ENC_DEC(joinu1pbl, join_u1pbl_t, "Ftmgs.JoinU1Pbl")
 * GENERATE_ASN1_ENC_DEC(joingm2pbl, join_gm2pbl_t, "Ftmgs.JoinGM2Pbl")
 * GENERATE_ASN1_ENC_DEC(joinu3prv, join_u3prv_t, "Ftmgs.JoinU3Prv")
 * GENERATE_ASN1_ENC_DEC(joinu3pbl, join_u3pbl_t, "Ftmgs.JoinU3Pbl")
 * GENERATE_ASN1_ENC_DEC(joingm4pbl, join_gm4pbl_t, "Ftmgs.JoinGM4Pbl")
 */
GENERATE_ASN1_ENC_DEC(dssparms, dss_parms_t, "Ftmgs.Dss-Parms")
GENERATE_ASN1_ENC_DEC(dsapbkey, dsa_pbkey_t, "Ftmgs.DSAPublicKey")
GENERATE_ASN1_ENC_DEC(dsaprkey, dsa_prkey_t, "Ftmgs.DSAPrivateKey")
GENERATE_ASN1_ENC_DEC(fapbkey, ftmgs_fa_pbkey_t, "Ftmgs.FAPbKey")
GENERATE_ASN1_ENC_DEC(grpbkey, ftmgs_pbkey_t, "Ftmgs.GrPbKey")
GENERATE_ASN1_ENC_DEC(grprkey, ftmgs_prkey_t, "Ftmgs.GrPrKey")
GENERATE_ASN1_ENC_DEC(fapbkeysh, ftmgs_faj_pbkey_share_t, "Ftmgs.FAPbKeyShare")
GENERATE_ASN1_ENC_DEC(faprkey, ftmgs_faj_prkey_t, "Ftmgs.FAPrKey")
GENERATE_ASN1_ENC_DEC(fagrpbkeysh, ftmgs_faj_gr_pbkey_share_t, "Ftmgs.FAGrPbKeyShare")
GENERATE_ASN1_ENC_DEC(fagrprkey, ftmgs_faj_gr_prkey_t, "Ftmgs.FAGrPrKey")
GENERATE_ASN1_ENC_DEC(dlog, dlog_t, "Ftmgs.DLog")
GENERATE_ASN1_ENC_DEC(dlogx, dlogx_t, "Ftmgs.DLogX")
GENERATE_ASN1_ENC_DEC(joinpbl, ftmgs_join_pbl_t, "Ftmgs.JoinPbl")
GENERATE_ASN1_ENC_DEC(joinprv, ftmgs_join_prv_t, "Ftmgs.JoinPrv")
GENERATE_ASN1_ENC_DEC(mbrref, ftmgs_mbr_ref_t, "Ftmgs.MbrRef")
GENERATE_ASN1_ENC_DEC(mbrprkey, ftmgs_mbr_prkey_t, "Ftmgs.MbrPrKey")
GENERATE_ASN1_ENC_DEC(sign, ftmgs_sign_t, "Ftmgs.Sign")
GENERATE_ASN1_ENC_DEC(open, ftmgs_open_t, "Ftmgs.Open")
GENERATE_ASN1_ENC_DEC(openshare, ftmgs_opensharej_t, "Ftmgs.OpenShare")
GENERATE_ASN1_ENC_DEC(openacc, ftmgs_openacc_t, "Ftmgs.OpenAcc")
GENERATE_ASN1_ENC_DEC(mtkey, ftmgs_mtkey_t, "Ftmgs.MTKey")
GENERATE_ASN1_ENC_DEC(mtkeyshare, ftmgs_mtkey_sharej_t, "Ftmgs.MTKeyShare")
GENERATE_ASN1_ENC_DEC(mtkeyacc, ftmgs_mtkey_acc_t, "Ftmgs.MTKeyAcc")
GENERATE_ASN1_ENC_DEC(claim, ftmgs_claim_t, "Ftmgs.Claim")
GENERATE_ASN1_ENC_DEC(link, ftmgs_link_t, "Ftmgs.Link")
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define GENERATE_ASN1_ENC_DEC_PROTO(FUNC_NAME, TYPE_NAME, ASN1_NAME)	\
	asn1_retcode_t asn1_enc_##FUNC_NAME(buffer_t* buff, const TYPE_NAME* p); \
	asn1_retcode_t asn1_dec_##FUNC_NAME(TYPE_NAME* p, const void* buff, int len);
/*----------------------------------------------------------------------------*/
/* 
 * GENERATE_ASN1_ENC_DEC_PROTO(syspar, syspar_t, "Ftmgs.SysPar")
 * GENERATE_ASN1_ENC_DEC_PROTO(fagrpbkey, elgamal_thr_pbkey_t, "Ftmgs.FAGrPbKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinproof, join_proof_t, "Ftmgs.JoinProof")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinu1prv, join_u1prv_t, "Ftmgs.JoinU1Prv")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinu1pbl, join_u1pbl_t, "Ftmgs.JoinU1Pbl")
 * GENERATE_ASN1_ENC_DEC_PROTO(joingm2pbl, join_gm2pbl_t, "Ftmgs.JoinGM2Pbl")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinu3prv, join_u3prv_t, "Ftmgs.JoinU3Prv")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinu3pbl, join_u3pbl_t, "Ftmgs.JoinU3Pbl")
 * GENERATE_ASN1_ENC_DEC_PROTO(joingm4pbl, join_gm4pbl_t, "Ftmgs.JoinGM4Pbl")
 */
/* 
 * GENERATE_ASN1_ENC_DEC_PROTO(dssparms, dss_parms_t, "Ftmgs.Dss-Parms")
 * GENERATE_ASN1_ENC_DEC_PROTO(dsapbkey, dsa_pbkey_t, "Ftmgs.DSAPublicKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(dsaprkey, dsa_prkey_t, "Ftmgs.DSAPrivateKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(fapbkey, ftmgs_fa_pbkey_t, "Ftmgs.FAPbKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(grpbkey, ftmgs_pbkey_t, "Ftmgs.GrPbKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(grprkey, ftmgs_prkey_t, "Ftmgs.GrPrKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(fapbkeysh, ftmgs_faj_pbkey_share_t, "Ftmgs.FAPbKeyShare")
 * GENERATE_ASN1_ENC_DEC_PROTO(faprkey, ftmgs_faj_prkey_t, "Ftmgs.FAPrKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(fagrpbkeysh, ftmgs_faj_gr_pbkey_share_t, "Ftmgs.FAGrPbKeyShare")
 * GENERATE_ASN1_ENC_DEC_PROTO(fagrprkey, ftmgs_faj_gr_prkey_t, "Ftmgs.FAGrPrKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(dlog, dlog_t, "Ftmgs.DLog")
 * GENERATE_ASN1_ENC_DEC_PROTO(dlogx, dlogx_t, "Ftmgs.DLogX")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinpbl, ftmgs_join_pbl_t, "Ftmgs.JoinPbl")
 * GENERATE_ASN1_ENC_DEC_PROTO(joinprv, ftmgs_join_prv_t, "Ftmgs.JoinPrv")
 * GENERATE_ASN1_ENC_DEC_PROTO(mbrref, ftmgs_mbr_ref_t, "Ftmgs.MbrRef")
 * GENERATE_ASN1_ENC_DEC_PROTO(mbrprkey, ftmgs_mbr_prkey_t, "Ftmgs.MbrPrKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(sign, ftmgs_sign_t, "Ftmgs.Sign")
 * GENERATE_ASN1_ENC_DEC_PROTO(open, ftmgs_open_t, "Ftmgs.Open")
 * GENERATE_ASN1_ENC_DEC_PROTO(openshare, ftmgs_opensharej_t, "Ftmgs.OpenShare")
 * GENERATE_ASN1_ENC_DEC_PROTO(openacc, ftmgs_openacc_t, "Ftmgs.OpenAcc")
 * GENERATE_ASN1_ENC_DEC_PROTO(mtkey, ftmgs_mtkey_t, "Ftmgs.MTKey")
 * GENERATE_ASN1_ENC_DEC_PROTO(mtkeyshare, ftmgs_mtkey_sharej_t, "Ftmgs.MTKeyShare")
 * GENERATE_ASN1_ENC_DEC_PROTO(mtkeyacc, ftmgs_mtkey_acc_t, "Ftmgs.MTKeyAcc")
 * GENERATE_ASN1_ENC_DEC_PROTO(claim, ftmgs_claim_t, "Ftmgs.Claim")
 * GENERATE_ASN1_ENC_DEC_PROTO(link, ftmgs_link_t, "Ftmgs.Link")
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifdef NUNCA_DEFINIDO_
static asn1_retcode_t asn1_init(ASN1_TYPE* definitions, char* errorDescription,
							  struct vector_t* name)
{
	asn1_retcode_t rc;
	*definitions = ASN1_TYPE_EMPTY;
	rc = asn1_array2tree(ftmgs_asn1_tab, definitions, errorDescription);
	init(name);
	return rc;
}
static void asn1_end(ASN1_TYPE* definitions)
{
	asn1_delete_structure(definitions);
}
/*----------------------------------------------------------------------------*/
asn1_retcode_t asn1_syspar_enc(buffer_t* buff, const syspar_t* sp)
{
	asn1_retcode_t rc;
	int buflen = 0;
	ASN1_TYPE definitions = ASN1_TYPE_EMPTY;
	char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
	ASN1_TYPE element = ASN1_TYPE_EMPTY;
	vector_t name;
	vector_t_ctor(&name);
	init(&name);
	rc = asn1_array2tree(ftmgs_asn1_tab, &definitions, errorDescription);
	if (rc == ASN1_SUCCESS) {
		rc = asn1_create_element(definitions, "Ftmgs.SysPar", &element);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_write_uint(element, &name, "nu", sp->nu);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_write_uint(element, &name, "k", sp->k);
	}
	if (rc == ASN1_SUCCESS) {
/* 
 * 		rc = asn1_der_coding(definitions, "Ftmgs.SysPar",
 * 							 buff, buflen, errorDescription);
 */
		rc = asn1_der_coding(element, "", NULL, &buflen, errorDescription);
		if (rc == ASN1_MEM_ERROR) {
			buffer_reserve(buff, (unsigned)buflen);
			if (buffer_data(buff) != NULL) {
				rc = asn1_der_coding(element, "",
									 buffer_data(buff),
									 &buflen,
									 errorDescription);
				if (rc != ASN1_SUCCESS) {
					buffer_reset(buff);
				}
			}
		}
	}	
	if (rc != ASN1_SUCCESS) {
		fprintf(stderr, "ASN.1 DER Encoding Error: %s [%s]\n",
				asn1_strerror(rc), errorDescription);
	}
	asn1_delete_structure(&element);
	asn1_delete_structure(&definitions);
	vector_t_dtor(&name);
	return rc;
}
/*----------------------------------------------------------------------------*/
asn1_retcode_t asn1_syspar_dec(syspar_t* sp, const void* buff, int buflen)
{
	asn1_retcode_t rc;
	ASN1_TYPE definitions = ASN1_TYPE_EMPTY;
	char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
	ASN1_TYPE element = ASN1_TYPE_EMPTY;
	vector_t name;
	vector_t_ctor(&name);
	init(&name);
	rc = asn1_array2tree(ftmgs_asn1_tab, &definitions, errorDescription);
	if (rc == ASN1_SUCCESS) {
		rc = asn1_create_element(definitions, "Ftmgs.SysPar", &element);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_der_decoding(&element, buff, buflen, errorDescription);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_read_uint(element, &name, "nu", &sp->nu);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_read_uint(element, &name, "k", &sp->k);
	}
	if (rc != ASN1_SUCCESS) {
		fprintf(stderr, "ASN.1 DER Decoding Error: %s [%s]\n",
				asn1_strerror(rc), errorDescription);
	}
	asn1_delete_structure(&element);
	asn1_delete_structure(&definitions);
	vector_t_dtor(&name);
	return rc;
}
/*----------------------------------------------------------------------------*/
asn1_retcode_t asn1_dssparms_enc(buffer_t* buff, const dss_parms_t* p)
{
	asn1_retcode_t rc;
	int buflen = 0;
	ASN1_TYPE definitions = ASN1_TYPE_EMPTY;
	char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];
	ASN1_TYPE element = ASN1_TYPE_EMPTY;
	vector_t name;
	vector_t_ctor(&name);
	init(&name);
	rc = asn1_array2tree(ftmgs_asn1_tab, &definitions, errorDescription);
	if (rc == ASN1_SUCCESS) {
		rc = asn1_create_element(definitions, "Ftmgs.Dss-Parms", &element);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_write_bigint(element, &name, "p", p->p);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_write_bigint(element, &name, "q", p->q);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_write_bigint(element, &name, "g", p->g);
	}
	if (rc == ASN1_SUCCESS) {
		rc = asn1_der_coding(element, "", NULL, &buflen, errorDescription);
		if (rc == ASN1_MEM_ERROR) {
			buffer_reserve(buff, (unsigned)buflen);
			if (buffer_data(buff) != NULL) {
				rc = asn1_der_coding(element, "",
									 buffer_data(buff),
									 &buflen,
									 errorDescription);
				if (rc != ASN1_SUCCESS) {
					buffer_reset(buff);
				}
			}
		}
	}	
	if (rc != ASN1_SUCCESS) {
		fprintf(stderr, "ASN.1 DER Encoding Error: %s [%s]\n",
				asn1_strerror(rc), errorDescription);
	}
	asn1_delete_structure(&element);
	asn1_delete_structure(&definitions);
	vector_t_dtor(&name);
	return rc;
}
#endif
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

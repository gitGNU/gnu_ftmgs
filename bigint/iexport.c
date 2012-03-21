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
#include "iexport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*----------------------------------------------------------------------------*/
STATIC_ASSERT(sizeof(uint8_t) == sizeof(char),
			  Sizeof_uint8_t_differs_from_sizeof_char);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define cucp(p)	((const unsigned char*)(p))
#define ucp(p)	((unsigned char*)(p))
/*----------------------------------------------------------------------------*/
static SHAversion select_sha(unsigned which_sha)
{
	SHAversion shv;
	if (which_sha <= SHA512) {
		shv = (SHAversion)which_sha;
	} else if (which_sha <= SHA1HashSize) {
		shv = SHA1;
	} else if (which_sha <= SHA256HashSize) {
		shv = SHA256;
	} else if (which_sha <= SHA512HashSize) {
		shv = SHA512;
	} else if (which_sha <= SHA1HashSizeBits) {
		shv = SHA1;
	} else if (which_sha <= SHA256HashSizeBits) {
		shv = SHA256;
	} else if (which_sha <= SHA512HashSizeBits) {
		shv = SHA512;
	} else {
		shv = SHA256;
	}
	return shv;
}
/*----------------------------------------------------------------------------*/
STATIC_ASSERT((shaSuccess == 0), Success_not_zero);
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* 
 * which_sha may be a value
 * 1.- from the enumeration SHAversion: SHA1, SHA224, SHA256, SHA384, SHA512
 * 2.- it may also be a requested number of bytes
 * 3.- it may also be a requested number of bits
 */
bool_t sha_reset(USHAContext* sha_context, unsigned which_sha)
{
	bool_t ok = ! USHAReset(sha_context, select_sha(which_sha));
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t sha_input(USHAContext* sha_context, const void* dat, unsigned datlen)
{
	bool_t ok = ! USHAInput(sha_context, (const uint8_t*)dat, datlen);
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t sha_result(USHAContext* sha_context, void* digest, unsigned* digestlen)
{
	bool_t ok = TRUE;
	if (*digestlen >= (unsigned)USHAHashSize(sha_context->whichSha)) {
		*digestlen = (unsigned)USHAHashSize(sha_context->whichSha);
		ok = ! USHAResult(sha_context, (uint8_t*)digest);
	} else if (*digestlen > 0) {
		uint8_t msg_digest[USHAMaxHashSize];
		ok = ! USHAResult(sha_context, msg_digest);
		memcpy(digest, msg_digest, *digestlen);
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t sha_data(unsigned which_sha, void* digest, unsigned* digestlen,
			  const void* dat, unsigned datlen)
{
	bool_t ok = TRUE;
	if (*digestlen > 0) {
		USHAContext sha_context;
		ok = ! USHAReset(&sha_context, select_sha(which_sha));
		ok = ok && ! USHAInput(&sha_context, (const uint8_t*)dat, datlen);
		if (*digestlen >= (unsigned)USHAHashSize(sha_context.whichSha)) {
			*digestlen = (unsigned)USHAHashSize(sha_context.whichSha);
			ok = ok && ! USHAResult(&sha_context, (uint8_t*)digest);
		} else if (*digestlen > 0) {
			uint8_t msg_digest[USHAMaxHashSize];
			ok = ok && ! USHAResult(&sha_context, msg_digest);
			if (ok) {
				memcpy(digest, msg_digest, *digestlen);
			}
		}
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* 
 * which_sha may be a value
 * 1.- from the enumeration SHAversion: SHA1, SHA224, SHA256, SHA384, SHA512
 * 2.- it may also be a requested number of bytes
 * 3.- it may also be a requested number of bits
 */
bool_t bi_sha_reset(USHAContext* sha_context, unsigned which_sha)
{
	bool_t ok = ! USHAReset(sha_context, select_sha(which_sha));
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_sha_input(USHAContext* sha_context, const bigint_t dat)
{
	unsigned len = 0;
	void* databits = bi_export(dat, NULL, &len);
	bool_t ok = ! USHAInput(sha_context, (const uint8_t*)databits, len);
	free(databits);
	assert(ok);
	return ok;
}
/*-----------------------------------------------------------------*/
bool_t bi_sha_result(USHAContext* sha_context, bigint_t digest)
{
	uint8_t msg_digest[USHAMaxHashSize];
	bool_t ok = ! USHAResult(sha_context, msg_digest);
	if (ok) {
		bi_import_abs(digest, msg_digest,
					  (size_t)USHAHashSize(sha_context->whichSha));
	}
	assert(ok);
	return ok;
}
/*-----------------------------------------------------------------*/
bool_t bi_sha_result_len(USHAContext* sha_context, bigint_t digest,
					   unsigned digestlen)
{
	bool_t ok = TRUE;
	if (digestlen > 0) {
		uint8_t msg_digest[USHAMaxHashSize];
		if (digestlen > (unsigned)USHAHashSize(sha_context->whichSha)) {
			digestlen = (unsigned)USHAHashSize(sha_context->whichSha);
		}
		ok = ! USHAResult(sha_context, msg_digest);
		if (ok) {
			bi_import_abs(digest, msg_digest, (size_t)digestlen);
		}
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_sha_data(unsigned which_sha, bigint_t digest,
				 const void* dat, unsigned datlen)
{
	bool_t ok = TRUE;
	USHAContext sha_context;
	uint8_t msg_digest[USHAMaxHashSize];
	ok = ! USHAReset(&sha_context, select_sha(which_sha));
	ok = ok && ! USHAInput(&sha_context, (const uint8_t*)dat, datlen);
	ok = ok && ! USHAResult(&sha_context, msg_digest);
	if (ok) {
		bi_import_abs(digest, msg_digest,
					  (size_t)USHAHashSize(sha_context.whichSha));
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_sha_data_len(unsigned which_sha, bigint_t digest, unsigned digestlen,
					 const void* dat, unsigned datlen)
{
	bool_t ok = TRUE;
	if (digestlen > 0) {
		USHAContext sha_context;
		uint8_t msg_digest[USHAMaxHashSize];
		ok = ! USHAReset(&sha_context, select_sha(which_sha));
		ok = ok && ! USHAInput(&sha_context, (const uint8_t*)dat, datlen);
		if (digestlen > (unsigned)USHAHashSize(sha_context.whichSha)) {
			digestlen = (unsigned)USHAHashSize(sha_context.whichSha);
		}
		ok = ok && ! USHAResult(&sha_context, msg_digest);
		if (ok) {
			bi_import_abs(digest, msg_digest, (size_t)digestlen);
		}
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_sha(unsigned which_sha, bigint_t digest, const bigint_t dat)
{
	bool_t ok;
	USHAContext sha_context;
	uint8_t msg_digest[USHAMaxHashSize];
	unsigned buflen = 0;
	void* buf = bi_export(dat, NULL, &buflen);
	ok = ! USHAReset(&sha_context, select_sha(which_sha));
	ok = ok && ! USHAInput(&sha_context, (const uint8_t*)buf, buflen);
	free(buf);
	ok = ok && ! USHAResult(&sha_context, msg_digest);
	if (ok) {
		bi_import_abs(digest, msg_digest,
					  (size_t)USHAHashSize(sha_context.whichSha));
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
bool_t bi_sha_len(unsigned which_sha, bigint_t digest, unsigned digestlen,
				const bigint_t dat)
{
	bool_t ok = TRUE;
	if (digestlen > 0) {
		USHAContext sha_context;
		uint8_t msg_digest[USHAMaxHashSize];
		unsigned buflen = 0;
		void* buf = bi_export(dat, NULL, &buflen);
		ok = ! USHAReset(&sha_context, select_sha(which_sha));
		ok = ok && ! USHAInput(&sha_context, (const uint8_t*)buf, buflen);
		free(buf);
		if (digestlen > (unsigned)USHAHashSize(sha_context.whichSha)) {
			digestlen = (unsigned)USHAHashSize(sha_context.whichSha);
		}
		ok = ok && ! USHAResult(&sha_context, msg_digest);
		if (ok) {
			bi_import_abs(digest, msg_digest, (size_t)digestlen);
		}
	}
	assert(ok);
	return ok;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static void twos_complement(void* buf, unsigned len)
{
	int i = (int)len - 1;
	/* assert(number!=0); */
	assert(len > 0);
	while (/*(i != -1) && */ (ucp(buf)[i]==0)) {
		--i;
	}
	assert(i!=-1);
	ucp(buf)[i] ^= 0x0FFU;
	++(ucp(buf)[i--]);
	while (i != -1) {
		ucp(buf)[i--] ^= 0x0FFU;
	}
}
/*-----------------------------------------------------------------*/
void* bi_export(const bigint_t n, void* bs, unsigned* len)
{
	/* if (bs==NULL) then return value  MUST be free-ed after usage	*/
	size_t nbytes_incl_sign = bi_nbytes(n);
	size_t nbytes_excl_sign = bi_nbytes_excl_sgn(n);
	size_t inc = nbytes_incl_sign - nbytes_excl_sign;
	size_t inlen = *len;
	*len = (unsigned)nbytes_incl_sign;
	assert(inc <= 1);
	if (bs == NULL) {
		bs = malloc(*len * sizeof(char));
		if (bs == NULL) {
			*len = 0;
			return NULL;
		}
	} else if (inlen < *len) {
		*len = 0;
		return NULL;
	}
	ucp(bs)[0]=0;
	if (!bi_iszero(n)) {
		size_t cnt = nbytes_excl_sign;
		bi_export_absxx(n, ucp(bs)+inc, &cnt);
		assert(cnt == nbytes_excl_sign);
		assert((ucp(bs)[0] & 0x080U) == 0);
		if (bi_sign(n) < 0) {
			twos_complement(bs, *len);
		}
	}
	assert((*len<=1) || ! (((ucp(bs)[0] == 0)
							&& ((ucp(bs)[1] & 0x080U) == 0))
						   || ((ucp(bs)[0] == 0x0FFU)
							   && ((ucp(bs)[1] & 0x080U) != 0))));
	return bs;
	
}
/*-----------------------------------------------------------------*/
void bi_import(bigint_t n, void* bs, unsigned len)
{
/* 
 * 	assert((len<=1) || ! (((ucp(bs)[0] == 0)
 *						   && ((bs[1] & 0x080U) == 0))
 * 					      || ((ucp(bs)[0] == 0x0FFU)
 * 						      && ((ucp(bs)[1] & 0x080U) != 0))));
 */
	bool_t neg = FALSE;
	if ((ucp(bs)[0] & 0x080U) != 0) {
		twos_complement(bs, len);
		neg = TRUE;
	}
	assert((ucp(bs)[0] & 0x080U) == 0);
	bi_import_abs(n, bs, len);
	if (neg) {
		bi_negate(n, n);
	}
}
/*-----------------------------------------------------------------*/
void bi_print_bytes(FILE* stream, const void* bs, unsigned len)
{
	unsigned i;
	fprintf(stream, "{ ");
	for (i = 0; i < len; ++i) {
		fprintf(stream, "0x%02X%s ",
				(0x0FFU & cucp(bs)[i]) ,
				(i < (len-1) ? ",":""));
	}
	fprintf(stream, "}\n");
	fflush(stream);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
USHAContext* shactx_t_new()
{
	USHAContext* p = (USHAContext*)malloc(sizeof(USHAContext));
	if (p != NULL) {
		shactx_t_ctor(p);
	}
	return p;
}
/*----------------------------------------------------------------------------*/
USHAContext* shactx_t_clone(const USHAContext* o)
{
	USHAContext* p = NULL;
	if (o != NULL) {
		p = (USHAContext*)malloc(sizeof(USHAContext));
		if (p != NULL) {
			shactx_t_ctor(p);
			shactx_t_asg(p, o);
		}
	}
	return p;
}
/*----------------------------------------------------------------------------*/
void shactx_t_delete(USHAContext* p)
{
	if (p != NULL) {
		shactx_t_dtor(p);
		free(p);
	}
}
/*----------------------------------------------------------------------------*/

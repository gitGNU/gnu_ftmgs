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
#include "bigintop.h"
#include <assert.h>
/*----------------------------------------------------------------------------*/
int bi_ctor_cnt(int n)
{
	static int cnt = 0;
	return cnt += n;
}
int bi_dtor_cnt(int n)
{
	static int cnt = 0;
	return cnt += n;
}
int bi_cdtor_cntdiff()
{
	return bi_ctor_cnt(0) - bi_dtor_cnt(0);
}
/*----------------------------------------------------------------------------*/
int powmod_cnt(int n)
{
	static int cnt = 0;
	return cnt += n;
}
/*----------------------------------------------------------------------------*/
bool_t bi_iscoprime(const bigint_t val1, const bigint_t val2)
{
	bool_t ok;
	BEG_BIGINT_CHK();
	BEG_VAR_1_bigint_t(gcd);
	bi_gcd(gcd, val1, val2);
	ok = bi_equals_si(gcd, 1);
	END_VAR_1_bigint_t(gcd);
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/
extern bool_t bi_is2k(const bigint_t o1)
{
	bool_t ok = FALSE;
	BEG_BIGINT_CHK();
	if (! bi_iszero(o1)) {
		BEG_VAR_1_bigint_t(aux);
		bi_pow2k(aux, bi_nbits(o1)-1);
		ok = bi_equals(aux, o1);
		END_VAR_1_bigint_t(aux);
	}
	END_BIGINT_CHK();
	return ok;
}
/*----------------------------------------------------------------------------*/

